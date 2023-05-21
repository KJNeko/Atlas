//
// Created by kj16609 on 5/19/23.
//

#ifndef ATLASGAMEMANAGER_TASKPOPUP_HPP
#define ATLASGAMEMANAGER_TASKPOPUP_HPP

#include <QDialog>
#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class TaskPopup;
}

QT_END_NAMESPACE

class TaskPopup : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( TaskPopup )

  public:

	explicit TaskPopup( QWidget* parent = nullptr );
	~TaskPopup() override;

	void resizeEvent( QResizeEvent* event ) override;

	void addMessage( QWidget* message );
	void removeMessage( QWidget* message );
	void expand();

  signals:
	void popupResized();

  private:

	Ui::TaskPopup* ui;

  private slots:
	void on_btnHideShow_clicked();
};

void initTaskPopup( QWidget* parent );
TaskPopup* getTaskPopup();

#endif //ATLASGAMEMANAGER_TASKPOPUP_HPP
