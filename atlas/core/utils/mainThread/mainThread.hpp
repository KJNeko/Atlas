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
	FunctionReturn< Function > executeOnMain( Function&& func, Args... args )
	{
		auto main { mainThread() };
		if ( main == nullptr )
			throw AtlasException( "Tried to run executeOnMain without setting a main thread first!" );

		if ( QThread::currentThread() == mainThread() )
		{
			//Execute as normal
			func( args... );
		}
		else
		{
			QTimer timer;
			timer.moveToThread( main );
			//As long as we move the timer to the main thread it'll execute it's timeout on the main thread.
			timer.setSingleShot( true );

			using RetType = FunctionDecomp< Function >::ResultType;

			std::promise< RetType > promise {};
			std::future< RetType > future { promise.get_future() };

			std::tuple< Args... > args_tpl { args... };

			QObject::connect(
				&timer,
				&QTimer::timeout,
				//Safe to ref args here since we copy them into our function AND block on the future for completion
				[ &promise, func, &args_tpl ]()
				{
					if constexpr ( std::same_as< RetType, void > )
					{
						std::apply( func, args_tpl );
						promise.set_value();
					}
					else
					{
						promise.set_value( std::apply( func, args_tpl ) );
					}
				} );

			QMetaObject::invokeMethod( &timer, "start", Qt::QueuedConnection, Q_ARG( int, 0 ) );

			//Wait until the main thread executes what we asked.
			future.wait();
		}
	}

} // namespace utils

#endif //ATLASGAMEMANAGER_MAINTHREAD_HPP
