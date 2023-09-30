
#include <QDebug>
#include <QtLogging>

#include <tracy/Tracy.hpp>

#include <cstdint>
#include <cstring>
#include <memory>
#include <string.h>

#include "common.h"

constexpr int number_of_components_size { 1 };
constexpr int max_ac_components_size { 1 };
constexpr int dc_component_size { 4 };
constexpr int characters_per_pixel { 2 };
constexpr int sRGB_size { 24 };
constexpr int color_size { 24 / 8 };

namespace blurhash
{
	inline char* encode_int( int value, int length, char* buffer )
	{
		ZoneScoped;
		int divisor = 1;
		for ( int i = 0; i < length - 1; ++i ) divisor *= 83;

		for ( int i = 0; i < length; ++i )
		{
			const int digit { ( value / divisor ) % 83 };
			divisor /= 83;
			const auto character { chars[ digit ] };
			*buffer++ = character;
		}
		return buffer;
	}

	//! Constexpr encodeInt used for encoding an integer during compile time
	template < std::uint64_t length >
	inline constexpr std::array< char, length > constEncodeInt( int value )
	{
		std::array< char, length > buffer;

		encode_int( value, length, buffer.data() );

		return buffer;
	}

	inline std::tuple< float, float, float > multiplyBasisFunction(
		int xComponent, int yComponent, int width, int height, const uint8_t* rgb, size_t bytesPerRow )
	{
		ZoneScoped;
		float r = 0, g = 0, b = 0;
		const float normalisation = ( xComponent == 0 && yComponent == 0 ) ?: 2;

		for ( int y = 0; y < height; y++ )
		{
			const auto y_idx { y * bytesPerRow };
			for ( int x = 0; x < width; x++ )
			{
				const auto x_idx { 3 * x };
				const auto idx { x_idx + y_idx };

				const double basis { std::cos( M_PI * xComponent * x / width )
					                 * std::cos( M_PI * yComponent * y / height ) };

				r += static_cast< float >( basis ) * sRGBToLinear( rgb[ idx + RED ] );
				g += static_cast< float >( basis ) * sRGBToLinear( rgb[ idx + GREEN ] );
				b += static_cast< float >( basis ) * sRGBToLinear( rgb[ idx + 2 ] );
			}
		}

		const float scale { normalisation / ( width * height ) };

		return std::make_tuple( r * scale, g * scale, b * scale );
	}

	inline int encodeDC( float r, float g, float b )
	{
		ZoneScoped;
		const int roundedR { linearTosRGB( r ) };
		const int roundedG { linearTosRGB( g ) };
		const int roundedB { linearTosRGB( b ) };
		return ( roundedR << 16 ) + ( roundedG << 8 ) + roundedB;
	}

	inline int encodeAC( float r, float g, float b, float maximumValue )
	{
		ZoneScoped;
		const int quantR { static_cast<
			int >( std::
			           max( 0.0f,
			                std::min( 18.0f, std::floor( signPow( r / maximumValue, 0.5f ) * 9.0f + 9.5f ) ) ) ) };
		const int quantG { static_cast<
			int >( std::
			           max( 0.0f,
			                std::min( 18.0f, std::floor( signPow( g / maximumValue, 0.5f ) * 9.0f + 9.5f ) ) ) ) };
		const int quantB { static_cast<
			int >( std::
			           max( 0.0f,
			                std::min( 18.0f, std::floor( signPow( b / maximumValue, 0.5f ) * 9.0f + 9.5f ) ) ) ) };

		return ( quantR * 19 * 19 ) + ( quantG * 19 ) + quantB;
	}

	template < int x_comp = 8, int y_comp = 8 >
	std::string encode( int width, int height, const std::uint8_t* rgb, const int channels )
	{
		ZoneScoped;
		constexpr int component_count { x_comp * y_comp - 1 };
		static_assert( x_comp > 1 );
		static_assert( y_comp > 1 );
		const int bytes_per_row { channels * width };

		std::array< std::array< std::array< float, 3 >, x_comp >, y_comp > factors;

		for ( std::size_t y = 0; y < y_comp; ++y )
		{
			for ( std::size_t x = 0; x < x_comp; ++x )
			{
				const auto factor { multiplyBasisFunction( x, y, width, height, rgb, bytes_per_row ) };
				factors[ y ][ x ][ RED ] = std::get< RED >( factor );
				factors[ y ][ x ][ GREEN ] = std::get< GREEN >( factor );
				factors[ y ][ x ][ BLUE ] = std::get< BLUE >( factor );
			}
		}

		float* const dc { factors[ 0 ][ 0 ].data() };
		float* const ac { dc + 3 };
		constexpr int acCount { x_comp * y_comp - 1 };

		constexpr int sizeFlag { ( x_comp - 1 ) + ( y_comp - 1 ) * 9 };

		std::array<
			char,
			number_of_components_size + max_ac_components_size + ( component_count * characters_per_pixel )
				+ dc_component_size >
			buffer;

		char* ptr { buffer.data() };

		const auto size_encoding { ::blurhash::constEncodeInt< number_of_components_size >( sizeFlag ) };
		std::memcpy( ptr, size_encoding.data(), size_encoding.size() );
		ptr += size_encoding.size();

		//Diff here
		float max_value { 0.0f };
		if ( acCount > 0 )
		{
			float actual_max_value { 0.0f };
			for ( int i = 0; i < acCount * 3; ++i )
			{
				actual_max_value = std::max( std::abs( ac[ i ] ), actual_max_value );
			}

			const int quantized_max_value {
				std::clamp( 0, 82, static_cast< int >( std::floor( actual_max_value * 166.0f - 0.5f ) ) )
			};

			max_value = static_cast< float >( quantized_max_value + 1 ) / 166.0f;

			ptr = ::blurhash::encode_int( quantized_max_value, max_ac_components_size, ptr );
		}
		else
		{
			max_value = 1.0f;
			ptr = ::blurhash::encode_int( 0, max_ac_components_size, ptr );
		}

		ptr = encode_int( encodeDC( dc[ RED ], dc[ GREEN ], dc[ BLUE ] ), dc_component_size, ptr );

		for ( int i = 0; i < acCount; ++i )
		{
			const auto idx { i * 3 };
			const auto ac_val { encodeAC( ac[ idx + RED ], ac[ idx + 1 ], ac[ idx + 2 ], max_value ) };

			ptr = ::blurhash::encode_int( ac_val, characters_per_pixel, ptr );
		}

		return std::string( buffer.data(), buffer.size() );
	}

} // namespace blurhash