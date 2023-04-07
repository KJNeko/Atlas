//
// Created by kj16609 on 4/5/23.
//

#include "QueryBuilder.hpp"

#include <tracy/Tracy.hpp>

enum TokenOperators
{
	NOT,
	OR,
	AND,
	BEGIN_GROUP,
	END_GROUP,
	OPERATOR_END,
	INVALID_OPERATOR
};

inline static constexpr std::array< std::pair< std::string_view, TokenOperators >, OPERATOR_END > operators {
	{ { "!", NOT }, { "|", OR }, { "&", AND }, { "(", BEGIN_GROUP }, { ")", END_GROUP } }
};

enum Namespaces
{
	SYSTEM,
	CREATOR,
	ENGINE,
	TITLE,
	TAG,
	NAMESPACE_END,
	INVALID_NAMESPACE
};

inline static constexpr std::array< std::pair< std::string_view, Namespaces >, NAMESPACE_END > namespaces {
	{ { "system:", SYSTEM }, { "creator:", CREATOR }, { "engine:", ENGINE }, { "title:", TITLE }, { "tag", TAG } }
};

enum SystemTokens
{
	FILESIZE,
	SYSTEM_END,
	INVALID_SYSTEM
};

inline static constexpr std::array< std::pair< std::string_view, SystemTokens >, SYSTEM_END > systems {
	{ { "system:size", FILESIZE } }
};

std::string_view trimSpaces( std::string_view str )
{
	ZoneScoped;
	const auto start_nonspace { str.find_first_not_of( ' ' ) };
	const auto end_nonspace { str.find_last_not_of( ' ' ) };

	if ( start_nonspace == 0 && end_nonspace == str.size() - 1 ) return str;

	if ( start_nonspace == str.npos || end_nonspace == str.npos ) return {};

	if ( start_nonspace == end_nonspace ) return {};

	return str.substr( start_nonspace, end_nonspace - start_nonspace + 1 );
}

std::string parseBytesize( std::string_view str )
{
	ZoneScoped;
	str = trimSpaces( str );
	std::size_t num { std::stoull( std::string( str.substr( 0, str.size() - 1 ) ) ) };
	const char last { str.at( str.size() - 1 ) };

	constexpr std::size_t multiplier { 1000 };

	switch ( last )
	{
		case 'T':
			num = num * multiplier;
			[[fallthrough]];
		case 'G':
			num = num * multiplier;
			[[fallthrough]];
		case 'M':
			num = num * multiplier;
			[[fallthrough]];
		case 'K':
			num = num * multiplier;
			[[fallthrough]];
		case 'B':
			[[fallthrough]];
		default:
			return std::to_string( num );
	}
}

bool isNamespace( const std::string_view str )
{
	ZoneScoped;
	for ( const auto& [ text, type ] : namespaces )
		if ( str.starts_with( text ) ) return true;
	return false;
}

bool isOperator( const std::string_view str )
{
	ZoneScoped;
	for ( const auto& [ text, type ] : operators )
		if ( str.starts_with( text ) ) return true;
	return false;
}

std::string parseOperator( const std::string_view str )
{
	ZoneScoped;
	for ( const auto& [ text, type ] : operators )
	{
		if ( str.starts_with( text ) )
		{
			switch ( type )
			{
				case NOT:
					return " NOT";
				case OR:
					return " OR";
				case AND:
					return " AND";
				case BEGIN_GROUP:
					{
						std::string_view temp_view { trimSpaces( str ) };

						return " (" + processString( temp_view.substr( 1, temp_view.size() - 2 ) ) + " )";
					}
				case END_GROUP:
					[[fallthrough]];
				case OPERATOR_END:
					[[fallthrough]];
				case INVALID_OPERATOR:
					[[fallthrough]];
				default:
					throw std::runtime_error(
						fmt::format( "{}: Failed to process \"{}\" in switch statement", __func__, str ) );
			}
		}
	}

	throw std::runtime_error( fmt::format( "{}: Failed to process \"{}\"", __func__, str ) );
}

std::string parseSystem( const std::string_view str )
{
	ZoneScoped;
	for ( const auto& [ text, type ] : systems )
	{
		if ( str.starts_with( text ) )
		{
			switch ( type )
			{
				case FILESIZE:
					{
						//Extract up to first `<` or `>`
						const auto pos { std::min( str.find( '<' ), str.find( '>' ) ) };
						if ( pos == str.npos )
							throw std::runtime_error( fmt::format(
								"Failed to process \"{}\" in switch statement for {}: Expected \'<\' or \'>\'",
								str,
								__func__ ) );

						const auto substr { str.substr( pos + 1 ) };

						const auto bytes { parseBytesize( substr ) };

						return " record_id IN (SELECT record_id FROM game_metadata GROUP BY record_id HAVING sum(folder_size) > "
						     + bytes + ")";
					}
				case SYSTEM_END:
					[[fallthrough]];
				case INVALID_SYSTEM:
					[[fallthrough]];
				default:
					throw std::runtime_error(
						fmt::format( "{}: Failed to process \"{}\" in switch statement", __func__, str ) );
			}
		}
	}

	throw std::runtime_error( fmt::format( "{}: Failed to process \"{}\"", __func__, str ) );
}

std::pair< std::string_view, std::string_view > seperateNamespace( const std::string_view str )
{
	ZoneScoped;
	const auto pos { str.find( ':' ) };
	return { str.substr( 0, pos ), str.substr( pos + 1, str.size() - pos ) };
}

//! Parses a given \ref NamespaceParsing from a string
/**
 * @param str
 * @return
 */
std::string parseNamespace( const std::string_view str )
{
	ZoneScoped;
	for ( const auto& [ text, type ] : namespaces )
	{
		if ( str.starts_with( text ) )
		{
			if ( type == SYSTEM )
				return parseSystem( str );
			else
			{
				const auto [ namespace_text, sub ] = seperateNamespace( str );
				switch ( type )
				{
					case CREATOR:
						return fmt::format( " creator LIKE \'{}\'", escape( sub ) );
					case ENGINE:
						return fmt::format( " engine LIKE \'{}\'", escape( sub ) );
					case TITLE:
						return fmt::format( " title LIKE \'{}\'", escape( sub ) );
					case TAG:
						//TODO: Do the tag system
						return "";
					case SYSTEM:
						[[fallthrough]];
					case NAMESPACE_END:
						[[fallthrough]];
					case INVALID_NAMESPACE:
						[[fallthrough]];
					default:
						throw std::runtime_error(
							fmt::format( "{}: Failed to process \"{}\" in switch statement", __func__, str ) );
				}
			}
		}
	}

	throw std::runtime_error( fmt::format( "{}: Failed to process \"{}\"", __func__, str ) );
}

//! Parses the string via `parseNamespace(const std::string_view)` or `parseOperator(const std::string_view)`.
/**
 * @warning Calling this with a single operator is considered undefined behaviour
 * @throws ParseError if `isNamespace(const std::string_view)` and `isOperator(const std::string_view)` are both false
 * @param str
 */
std::string parse( const std::string_view str )
{
	ZoneScoped;
	if ( isNamespace( str ) )
		return parseNamespace( str );
	else if ( isOperator( str ) )
		return parseOperator( str );
	else
		throw std::runtime_error( fmt::format( "Failed to parse! {}", str ) );
}

//! Extracts characters until reaching a grouping operator or namespace or system tag
std::string_view extractUntilNext( std::string_view& str )
{
	ZoneScoped;
	constexpr auto start { 0 };

	auto next { str.npos };
	//Extract the entire group if we find one
	if ( str.starts_with( '(' ) )
	{
		//Start of a group. Find the next one
		next = str.find( ')' ) + 1;
	}
	else
	{
		for ( const auto& [ text, op ] : operators )
			if ( const auto pos = str.find( text, 1 ); pos != str.npos && pos < next && str.at( pos - 1 ) != '\\' )
				next = pos;

		for ( const auto& [ text, op ] : namespaces )
			if ( const auto pos = str.find( text, 1 ); pos != str.npos && pos < next && str.at( pos - 1 ) != '\\' )
				next = pos;
	}

	if ( next == str.npos )
	{
		const auto substr { str };
		str = "";
		return substr;
	}

	const auto substr { str.substr( start, next ) };
	str = str.substr( next, str.size() - next );

	//Remove any leading/ending zeros
	const bool leading { substr.starts_with( ' ' ) };
	const bool ending { substr.ends_with( ' ' ) };

	return substr.substr( leading ? 1 : 0, substr.size() - ( ending ? 1 : 0 ) );
}

std::string processString( std::string_view str_view )
{
	ZoneScoped;
	str_view = trimSpaces( str_view );
	spdlog::info( "Processing \"{}\"", str_view );
	std::string query;

	//Prevents infinite loops if we somehow reach one.
	std::uint64_t cycles { 0 };

	while ( str_view.size() > 0 && cycles < 512 )
	{
		std::string_view to_parse { extractUntilNext( str_view ) };

		query += parse( to_parse );

		++cycles;
	}

	return query;
}

std::string escape( std::string_view view )
{
	std::string str { view };
	ZoneScoped;

	auto pos { str.find( '\'' ) };

	while ( true )
	{
		pos = str.find( '\'', pos + 1 );

		if ( pos == str.npos )
			break;
		else
			str.replace( pos, 1, "\\\'" );
	}

	return str;
}

std::string generateQuery( const std::string str )
{
	ZoneScoped;
	std::string query { "SELECT record_id FROM records WHERE" };
	std::string_view str_view { str };

	return query + processString( str_view );
}
