//
// Created by kj16609 on 5/19/23.
//

#ifndef ATLASGAMEMANAGER_PROGRESSMESSAGE_HPP
#define ATLASGAMEMANAGER_PROGRESSMESSAGE_HPP

#include <QWidget>

#include "NotificationPopup.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class ProgressMessage;
}

QT_END_NAMESPACE

class ProgressMessageSignaler final : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( ProgressMessageSignaler )

  public:

	ProgressMessageSignaler( QObject* parent = nullptr ) : QObject( parent ) {}

  public slots:

	void setMax( int max );
	void setProgress( int progress );
	void setMessage( QString message );
	void setFinished();
	void setRange( int min, int max );

  signals:
	void maxChanged( int max );
	void progressChanged( int progress );
	void messageChanged( QString message );
	void selfClose();
};

class ProgressMessage final : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( ProgressMessage )

	explicit ProgressMessage( const QString name, QWidget* parent = nullptr );
	~ProgressMessage() override;

  private:

	Ui::ProgressMessage* ui;

	friend class NotificationPopup;
	template < typename T >
	friend T::Signaler createNotification( const QString, const bool );

  public:

	using Signaler = ProgressMessageSignaler;
	std::unique_ptr< ProgressMessageSignaler > getSignaler();

  private slots:
	void changeMax( int max );
	void changeProgress( int progress );
	void changeMessage( QString message );

  public slots:
	void closeSelf();
};

#endif //ATLASGAMEMANAGER_PROGRESSMESSAGE_HPP
