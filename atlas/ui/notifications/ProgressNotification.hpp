//
// Created by kj16609 on 7/21/23.
//

#ifndef ATLASGAMEMANAGER_PROGRESSNOTIFICATION_HPP
#define ATLASGAMEMANAGER_PROGRESSNOTIFICATION_HPP

#include "Notification.hpp"
#include "NotificationSignalers.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class ProgressNotification;
}

QT_END_NAMESPACE

class ProgressSignaler final : public atlas::notifications::NotificationSignaler
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( ProgressSignaler )

  public:

	ProgressSignaler() = default;

	void setMax( int i );
	void setProgress( int i );
	void setSubMessage( const QString str );
	void setMessage( const QString str );

  private:

  signals:
	void progressChanged( int i );
	void maxChanged( int i );
	void messageChanged( const QString str );
	void subMessageChanged( const QString str );
};

class ProgressNotification : public Notification
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( ProgressNotification )

  public:

	explicit ProgressNotification( QWidget* parent = nullptr );
	~ProgressNotification() override;

	using Signaler = ProgressSignaler;

	std::unique_ptr< ProgressSignaler > getSignaler();

  private:

	Ui::ProgressNotification* ui;
};

#endif //ATLASGAMEMANAGER_PROGRESSNOTIFICATION_HPP
