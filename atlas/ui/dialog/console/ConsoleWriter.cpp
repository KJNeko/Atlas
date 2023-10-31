//
// Created by kj16609 on 10/31/23.
//

#include "ConsoleWriter.hpp"

#include <moc_ConsoleWriter.cpp>

namespace atlas::logging
{

	inline static ConsoleWriter* writer { nullptr };

	ConsoleWriter* getConsoleWriter()
	{
		if ( !writer ) writer = new ConsoleWriter();
		return writer;
	}

} // namespace atlas::logging
