//
// Created by kj16609 on 7/22/23.
//

#include "mainThread.hpp"

namespace utils
{
	namespace internal
	{
		inline static QThread* main_thread { nullptr };

	}

	void setMainThread( QThread* ptr )
	{
		internal::main_thread = ptr;
	}

	QThread* mainThread()
	{
		return internal::main_thread;
	}

} // namespace utils
