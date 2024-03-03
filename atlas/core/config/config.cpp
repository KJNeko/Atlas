//
// Created by kj16609 on 3/3/24.
//

#include "config.hpp"

namespace atlas::config
{

	void init()
	{
		//TODO: This
	}

	QSettings getConfigObject()
	{
		return { "./data/config.ini", QSettings::IniFormat };
	}

} // namespace atlas::config
