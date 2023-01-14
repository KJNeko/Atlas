//
// Created by kj16609 on 1/13/23.
//

#ifndef HYDRUS95_RECORDDIALOG_HPP
#define HYDRUS95_RECORDDIALOG_HPP

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui
{
	class RecordDialog;
}
QT_END_NAMESPACE

class RecordDialog : public QWidget
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE(RecordDialog)

	public:
	explicit RecordDialog( QWidget* parent = nullptr );
	~RecordDialog() override;

	private:
	Ui::RecordDialog* ui;
};


#endif	//HYDRUS95_RECORDDIALOG_HPP
