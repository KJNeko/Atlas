//
// Created by kj16609 on 7/22/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_MAINTHREAD_HPP
#define ATLASGAMEMANAGER_MAINTHREAD_HPP

#include <QMetaObject>
#include <QObject>
#include <QThread>
#include <QTimer>

#include "core/database/FunctionDecomp.hpp"
#include "core/exceptions.hpp"

namespace utils
{
	void setMainThread( QThread* thread );
	QThread* mainThread();

	template < typename Function, typename... Args >
	FunctionReturn< Function > executeOnMain( Function&& func, Args&&... args )
	{
		QThread* main { mainThread() };
		if ( main == nullptr )
			throw AtlasException( "Tried to run executeOnMain without setting a main thread first!" );

		if ( QThread::currentThread() == mainThread() )
		{
			//Execute as normal
			return func( std::forward< Args >( args )... );
		}
		else
		{
			if constexpr ( std::same_as< FunctionReturn< Function >, void > )
			{
				//Fire and forget
				QMetaObject::invokeMethod(
					main,
					[ func = std::forward< Function >( func ), args... ]() mutable -> void
					{
						try
						{
							func( std::forward< Args >( args )... );
						}
						catch ( ... )
						{
							//Eat and silence
						}
					},
					Qt::QueuedConnection );
				return;
			}
			else
			{
				auto promise { std::make_shared< QPromise< FunctionReturn< Function > > >() };

				QMetaObject::invokeMethod(
					main,
					[ promise, func, args... ]() -> void
					{
						promise->addResult( func( std::forward< Args >( args )... ) );
						promise->finish();
					},
					Qt::QueuedConnection );

				auto future { promise->future() };
				future.waitForFinished();
				return future.result();
			}
		}
	}

} // namespace utils

#endif //ATLASGAMEMANAGER_MAINTHREAD_HPP
