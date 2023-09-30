#ifndef __BLURHASH_COMMON_H__
#define __BLURHASH_COMMON_H__

#include <algorithm>
#include <array>
#include <math.h>

#ifndef M_PI
#define M_PI std::numbers::pi_v
#endif

enum blurhashColor
{
	RED = 0,
	GREEN,
	BLUE
};

inline int linearTosRGB( float value )
{
	const float v { std::clamp( value, 0.0f, 1.0f ) };
	if ( v <= 0.0031308f )
		return static_cast< int >( std::round( static_cast< float >( v ) * 12.92f * 255.0f ) );
	else
		return static_cast< int >( std::round( ( 1.055f * std::pow( v, 1.0f / 2.4f ) - 0.055f ) * 255.0f ) );
}

inline static std::uint64_t max { 0 };

namespace internal
{
	constexpr inline float sRGBToLinear( int value )
	{
		const float v { static_cast< float >( value ) / 255.0f };
		if ( v <= 0.04045f )
			return v / 12.92f;
		else
			return std::pow( ( v + 0.055f ) / 1.055f, 2.4f );
	}
} // namespace internal

inline static constexpr std::array< float, 255 > sRGBLookupTable { []() -> std::array< float, 255 >
	                                                               {
																	   std::array< float, 255 > buffer;

																	   for ( size_t i = 0; i < buffer.size(); ++i )
																	   {
																		   buffer[ i ] = internal::sRGBToLinear( i );
																	   }

																	   return buffer;
																   }() };
//#define USE_LUT 1

#ifdef USE_LUT
inline float sRGBToLinear( int value )
{
	assert( value < sRGBLookupTable.size() );
	return sRGBLookupTable[ value ];
}
#else
constexpr inline float sRGBToLinear( int value )
{
	const float v { static_cast< float >( value ) / 255.0f };
	if ( v <= 0.04045f )
		return v / 12.92f;
	else
		return std::pow( ( v + 0.055f ) / 1.055f, 2.4f );
}
#endif

inline float signPow( float value, float exp )
{
	return std::copysign( std::pow( std::abs( value ), exp ), value );
}

constexpr std::array< char, 83 > base_83_set {
	[]() -> std::array< char, 83 >
	{
		const auto str { "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz#$%*+,-.:;=?@[]^_{|}~" };

		std::array< char, 83 > buffer;

		for ( size_t i = 0; i < buffer.size(); ++i ) buffer[ i ] = str[ i ];

		return buffer;
	}()
};

constexpr auto chars = base_83_set;

#endif
