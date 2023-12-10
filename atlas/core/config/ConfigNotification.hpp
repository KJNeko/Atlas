//
// Created by kj16609 on 4/26/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_CONFIGNOTIFICATION_HPP
#define ATLASGAMEMANAGER_CONFIGNOTIFICATION_HPP

#include <QObject>

namespace config
{

	class ConfigNotification final : public QObject
	{
		Q_OBJECT

	  public:

		ConfigNotification( QObject* parent = nullptr );

		virtual ~ConfigNotification() {}

		void notify();
	  signals:
		void notification();
	};

	void notify();

	namespace internal
	{
		ConfigNotification& getNotifier();
	}
} // namespace config

#endif //ATLASGAMEMANAGER_CONFIGNOTIFICATION_HPP
