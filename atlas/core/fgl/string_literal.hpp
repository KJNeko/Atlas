#pragma once
#ifndef FGL_TYPES_STRING_LITERAL_HPP_INCLUDED
#define FGL_TYPES_STRING_LITERAL_HPP_INCLUDED

#include <algorithm> // copy
#include <cstddef> // size_t
#include <cstdint>
#include <string_view>

namespace fgl
{

	template < typename T, std::uint64_t N >
	using carray = T[ N ];

	/**
@file

@example example/fgl/types/string_literal.cpp
	An example for @ref group-types-string_literal

@defgroup group-types-string_literal String Literal

@brief A wrapper for string literals

@see the example program @ref example/fgl/types/string_literal.cpp
@{
*/

	///@todo i should rethink this https://www.youtube.com/watch?v=ABg4_EV5L3w

	/**
@brief A wrapper type for string literals
@details Due to how C++ works, string literals cannot be passed as non-type
	template arguments. This wrapper type helps to work around that limitation.
@tparam N the number of characters in the string literal including
	null-terminator, which is used as the C array extent. It must be greater
	than zero. This extent should be automatically deduced by the constructor.
*/
	template < std::size_t N >
		requires( N > 0 )
	struct string_literal
	{
		using char_array_t = fgl::carray< char, N >;
		char_array_t array {};

		/// Constructs a string_literal from a C-style <tt>const char[]</tt>
		[[nodiscard]] consteval string_literal( const char_array_t& cstring_literal ) noexcept
		{
			std::ranges::copy( cstring_literal, array );
		}

		constexpr operator const char *() const noexcept { return &array[ 0 ]; }

		constexpr operator std::string_view() const noexcept { return std::string_view { &array[ 0 ], N - 1 }; }

		const constexpr char& operator[]( std::size_t index ) const noexcept { return array[ index ]; }

		constexpr std::size_t size() const noexcept { return N; }

		const constexpr char* data() const noexcept { return &array[ 0 ]; }

		const constexpr char* begin() const noexcept { return &array[ 0 ]; }

		const constexpr char* end() const noexcept { return &array[ N ]; }

		const constexpr char* cbegin() const noexcept { return &array[ 0 ]; }

		const constexpr char* cend() const noexcept { return &array[ N ]; }

		template < std::size_t BN >
		consteval fgl::string_literal< ( N - 1 ) + BN > operator+( const fgl::string_literal< BN > b ) const
		{
			char tmp_array[ ( N - 1 ) + BN ] {};

			std::copy( begin(), end() - 1, &tmp_array[ 0 ] );
			std::copy( b.begin(), b.end(), &tmp_array[ N - 1 ] );

			return { tmp_array };
		}
	};

	///@} group-types-string_literal
} // namespace fgl

#endif // FGL_TYPES_STRING_LITERAL_HPP_INCLUDED
