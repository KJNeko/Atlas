//
// Created by kj16609 on 10/31/23.
//

#pragma once

#include <QObject>

#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#endif

#include <spdlog/sinks/base_sink.h>

#include "spdlog/spdlog.h"

#pragma GCC diagnostic pop

#include <QDebug>

#include <queue>

namespace atlas::logging
{

	class ConsoleWriter : public QObject
	{
		Q_OBJECT
		std::vector< QString > messages {};

	  public:

		void write( const QString& msg )
		{
			messages.emplace_back( msg );
			emit dataWritten( msg );
		}

		void reprintMessages()
		{
			for ( const auto& msg : messages ) emit dataWritten( msg );
		}

	  signals:
		void dataWritten( const QString& data );
	};

	ConsoleWriter* getConsoleWriter();

	template < typename Mutex >
	class ConsoleSink : public spdlog::sinks::base_sink< Mutex >
	{
	  public:

		ConsoleSink() : spdlog::sinks::base_sink< Mutex >() {}

	  protected:

		void sink_it_( const spdlog::details::log_msg& msg ) override
		{
			spdlog::memory_buf_t formatted;
			spdlog::sinks::base_sink< Mutex >::formatter_->format( msg, formatted );
			getConsoleWriter()->write( QString::fromStdString( fmt::to_string( formatted ) ) );
		}

		void flush_() override { return; }
	};

	using ConsoleSinkMT = ConsoleSink< std::mutex >;
	using ConsoleSinkST = ConsoleSink< spdlog::details::null_mutex >;

} // namespace atlas::logging