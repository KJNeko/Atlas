//
// Created by kj16609 on 10/6/23.
//

#include "updater.hpp"

namespace atlas::updater
{

	void startUpdater()
	{
#ifdef _WIN32
		//TODO: Add windows code here for starting the updater and terminating ourselves.

		// Ask threads doing work to stop and wait for them to finish.

		// Start updater. Feed it out PID so it can watch us die.

		// Terminate ourselves
#else
		//noop
		return;
#endif
	}

} // namespace atlas::updater
