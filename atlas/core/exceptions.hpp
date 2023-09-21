//
// Created by kj16609 on 9/18/23.
//

#ifndef ATLASGAMEMANAGER_EXCEPTIONS_HPP
#define ATLASGAMEMANAGER_EXCEPTIONS_HPP

#include <exception>
#include <source_location>

#include "core/logging/logging.hpp"

namespace atlas::exceptions
{
	struct AtlasException : public std::runtime_error
	{
		std::source_location sloc;
		std::string what_text;

		virtual const char* what() const noexcept override { return what_text.c_str(); }

		AtlasException( const char* const msg, const std::source_location loc = std::source_location::current() ) :
		  std::runtime_error( msg ),
		  sloc( loc ),
		  what_text( format_ns::format( "{}: {}", sloc, std::runtime_error::what() ) )
		{
			atlas::logging::errorLoc( "{}", loc, msg );
		}

		AtlasException( const std::string msg, const std::source_location loc = std::source_location::current() ) :
		  AtlasException( msg.c_str(), loc )
		{}
	};

	struct RecordException : public AtlasException
	{
		RecordException( const char* const msg, const std::source_location loc = std::source_location::current() ) :
		  AtlasException( msg, loc )
		{}
	};

	struct VersionConflict : public RecordException
	{
		RecordID m_game_id;
		QString m_version;

		VersionConflict(
			const char* const msg,
			const RecordID game_id,
			const QString version,
			const std::source_location loc = std::source_location::current() ) :
		  RecordException( msg, loc ),
		  m_game_id( game_id ),
		  m_version( version )
		{}
	};

	struct DatabaseException : public AtlasException
	{
		DatabaseException( const char* const msg, const std::source_location loc = std::source_location::current() ) :
		  AtlasException( msg, loc )
		{}

		DatabaseException( std::string msg, std::source_location loc = std::source_location::current() ) :
		  DatabaseException( msg.c_str(), loc )
		{}
	};

	struct ImportException : public AtlasException
	{
		ImportException( const char* const msg, const std::source_location loc = std::source_location::current() ) :
		  AtlasException( msg, loc )
		{}
	};

	struct NoExecutablesFound : public ImportException
	{
		NoExecutablesFound( std::filesystem::path folder, std::source_location loc = std::source_location::current() ) :
		  ImportException( format_ns::format( "No valid executables found for directory: {}", folder ).c_str(), loc )
		{}
	};

	struct TransactionInvalid : public DatabaseException
	{
		TransactionInvalid(
			const char* const m_sql_string, const std::source_location loc = std::source_location::current() ) :
		  DatabaseException(
			  format_ns::format( "Transaction accessed while invalid: Last executed: {}", m_sql_string ), loc )
		{}
	};

	struct SettingsException : public AtlasException
	{
		SettingsException( const char* const msg, const std::source_location loc = std::source_location::current() ) :
		  AtlasException( msg, loc )
		{}
	};

} // namespace atlas::exceptions

using namespace atlas::exceptions;

#endif //ATLASGAMEMANAGER_EXCEPTIONS_HPP
