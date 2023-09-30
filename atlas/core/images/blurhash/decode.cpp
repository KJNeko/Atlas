#include "decode.h"

#include <QDebug>

#include <tracy/Tracy.hpp>

#include <stdexcept>
#include <string_view>
#include <vector>

#include "common.h"

namespace blurhash
{
	inline uint8_t clampToUByte( const int val )
	{
		ZoneScoped;
		if ( val <= 0 )
			return 0;
		else if ( val >= 255 )
			return 255;
		else
			return static_cast< uint8_t >( val );
	}

	int decodeToInt( const std::string_view string, int start, int end )
	{
		ZoneScoped;
		constexpr int chars_size { chars.size() };
		int value { 0 };
		for ( int iter1 = start; iter1 < end; ++iter1 )
		{
			int index { -1 };
			for ( int iter2 = 0; iter2 < chars_size; ++iter2 )
			{
				if ( chars[ iter2 ] == string[ iter1 ] )
				{
					index = iter2;
					break;
				}
			}
			if ( index == -1 ) throw std::runtime_error( "Failed to decode value to int" );
			value = value * chars_size + index;
		}
		return value;
	}

	bool isValidBlurhash( const std::string_view blurhash )
	{
		ZoneScoped;
		const int hash_length { static_cast< int >( blurhash.size() ) };

		if ( hash_length < 6 ) return false;

		const int sizeFlag { decodeToInt( blurhash, 0, 1 ) }; //Get size from first character
		const int numY { static_cast< int >( std::floor( static_cast< float >( sizeFlag ) / 9.0f ) + 1.0f ) };
		const int numX { ( sizeFlag % 9 ) + 1 };

		if ( hash_length != 4 + 2 * numX * numY ) return false;
		return true;
	}

	std::tuple< float, float, float > decodeDC( int value )
	{
		ZoneScoped;
		const float r { sRGBToLinear( value >> 16 ) }; // R-component
		const float g { sRGBToLinear( ( value >> 8 ) & 255 ) }; // G-Component
		const float b { sRGBToLinear( value & 255 ) }; // B-Component

		return { r, g, b };
	}

	std::tuple< float, float, float > decodeAC( int value, const float maximumValue )
	{
		ZoneScoped;
		const int quantR { static_cast< int >( std::floor( value / ( 19 * 19 ) ) ) };
		const int quantG { static_cast< int >( std::floor( value / 19 ) ) % 19 };
		const int quantB { static_cast< int >( value % 19 ) };

		const float r { signPow( ( static_cast< float >( quantR ) - 9.0f ) / 9.0f, 2.0f ) * maximumValue };
		const float g { signPow( ( static_cast< float >( quantG ) - 9.0f ) / 9.0f, 2.0f ) * maximumValue };
		const float b { signPow( ( static_cast< float >( quantB ) - 9.0f ) / 9.0f, 2.0f ) * maximumValue };

		return { r, g, b };
	}

	std::vector< std::uint8_t > decode( const std::string_view hash, int width, int height, int punch, int channels )
	{
		ZoneScoped;
		const int bytes_per_row { channels * width };

		std::vector< std::uint8_t > buffer;
		buffer.resize( height * bytes_per_row );

		if ( !isValidBlurhash( hash.data() ) ) throw std::runtime_error( "Invalid blurhash" );
		if ( punch < 1 ) punch = 1;

		const int size_flag { decodeToInt( hash, 0, 1 ) };
		const int components_y { static_cast< int >( std::floor( size_flag / 9 ) ) + 1 };
		const int components_x { ( size_flag % 9 ) + 1 };

		const int quantized_max_value { decodeToInt( hash.data(), 1, 2 ) };

		const float maxValue { static_cast< float >( quantized_max_value + 1 ) / 166.0f };

		const int colors_size { components_x * components_y };
		float colors[ colors_size ][ 3 ];

		const auto dc { decodeDC( decodeToInt( hash.data(), 2, 6 ) ) };
		colors[ 0 ][ RED ] = std::get< RED >( dc );
		colors[ 0 ][ GREEN ] = std::get< GREEN >( dc );
		colors[ 0 ][ BLUE ] = std::get< BLUE >( dc );

		for ( int itter = 1; itter < colors_size; ++itter )
		{
			const int value { decodeToInt( hash.data(), 4 + itter * 2, 6 + itter * 2 ) };
			const auto ac { decodeAC( value, maxValue * punch ) };
			colors[ itter ][ RED ] = std::get< RED >( ac );
			colors[ itter ][ GREEN ] = std::get< GREEN >( ac );
			colors[ itter ][ BLUE ] = std::get< BLUE >( ac );
		}

		for ( int y = 0; y < height; ++y )
		{
			const int y_idx { y * bytes_per_row };
			for ( int x = 0; x < width; ++x )
			{
				const int x_idx { channels * x };
				float r { 0.0f };
				float g { 0.0f };
				float b { 0.0f };

				for ( int j = 0; j < components_y; ++j )
				{
					const float y_basics { static_cast< float >( std::cos( ( M_PI * y * j ) / height ) ) };
					for ( int i = 0; i < components_x; ++i )
					{
						const float basics { static_cast< float >( std::cos( ( M_PI * x * i ) / width ) ) * y_basics };

						const int idx { i + j * components_x };
						r += colors[ idx ][ RED ] * basics;
						g += colors[ idx ][ GREEN ] * basics;
						b += colors[ idx ][ BLUE ] * basics;
					}
				}

				buffer[ x_idx + y_idx + RED ] = clampToUByte( linearTosRGB( r ) );
				buffer[ x_idx + y_idx + GREEN ] = clampToUByte( linearTosRGB( g ) );
				buffer[ x_idx + y_idx + BLUE ] = clampToUByte( linearTosRGB( b ) );

				if ( channels == 4 )
					buffer[ x_idx + y_idx + 3 ] = 255; // If nChannels=4, treat each pixel as RGBA instead of RGB
			}
		}

		return buffer;
	}
} // namespace blurhash
