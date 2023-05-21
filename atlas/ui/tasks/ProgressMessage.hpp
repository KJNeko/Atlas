//
// Created by kj16609 on 5/19/23.
//

#ifndef ATLASGAMEMANAGER_PROGRESSMESSAGE_HPP
#define ATLASGAMEMANAGER_PROGRESSMESSAGE_HPP

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class ProgressMessage;
}

QT_END_NAMESPACE

class ProgressMessageSignaler : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( ProgressMessageSignaler )

  public:

	void setMax( int max );
	void setProgress( int progress );
	void setMessage( QString message );
	void setFinished();

	ProgressMessageSignaler( QObject* parent = nullptr ) : QObject( parent ) {}

  private:

  signals:
	void maxChanged( int max );
	void progressChanged( int progress );
	void messageChanged( QString message );
	void selfClose();
};

class ProgressMessage : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( ProgressMessage )

  public:

	explicit ProgressMessage( const QString name, QWidget* parent = nullptr );
	~ProgressMessage() override;

	std::unique_ptr< ProgressMessageSignaler > getSignaler();

  private:

	Ui::ProgressMessage* ui;

  private slots:
	void changeMax( int max );
	void changeProgress( int progress );
	void changeMessage( QString message );
	void closeSelf();
};

namespace atlas::notifications
{
	std::unique_ptr< ProgressMessageSignaler > createProgressNotification( QString name, const bool reveal = false );
}

#endif //ATLASGAMEMANAGER_PROGRESSMESSAGE_HPP
