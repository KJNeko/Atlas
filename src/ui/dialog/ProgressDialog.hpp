//
// Created by kj16609 on 3/5/23.
//

#ifndef HYDRUS95_PROGRESSDIALOG_HPP
#define HYDRUS95_PROGRESSDIALOG_HPP

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui
{
	class ProgressDialog;
}
QT_END_NAMESPACE

class ProgressDialog final : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE(ProgressDialog)

	public:
	explicit ProgressDialog( QWidget* parent = nullptr );
	~ProgressDialog() override;

	void updateMax(const int sub_progress, const int total_progress);
	void updateProgress(const QString& status_str, const int sub_progress, const int total_progress);

	private:
	Ui::ProgressDialog* ui;
};


#endif	//HYDRUS95_PROGRESSDIALOG_HPP
