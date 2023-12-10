//
// Created by kj16609 on 7/21/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_MESSAGENOTIFICATION_HPP
#define ATLASGAMEMANAGER_MESSAGENOTIFICATION_HPP

#include "Notification.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class MessageNotification;
}

QT_END_NAMESPACE

enum class MessageLevel
{
	ATLAS_DEBUG,
	ATLAS_INFO_SELFCLOSE,
	ATLAS_INFO,
	ATLAS_WARNING,
	ATLAS_ERROR,
	ATLAS_CRITICAL
};

class MessageNotification final : public Notification
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( MessageNotification )

	//! Full message that was sent to the log
	QString m_full_message;

  public:

	explicit MessageNotification(
		const MessageLevel level, QString user_message, QString full_message, QWidget* parent = nullptr );
	~MessageNotification() override;

  private:

	Ui::MessageNotification* ui;
};

#endif //ATLASGAMEMANAGER_MESSAGENOTIFICATION_HPP
