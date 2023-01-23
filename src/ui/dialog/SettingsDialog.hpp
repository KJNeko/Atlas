//
// Created by kj16609 on 1/23/23.
//

#ifndef HYDRUS95_SETTINGSDIALOG_HPP
#define HYDRUS95_SETTINGSDIALOG_HPP

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui
{
	class SettingsDialog;
}
QT_END_NAMESPACE

class SettingsDialog : public QDialog
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE(SettingsDialog)

	public:
	explicit SettingsDialog( QWidget* parent = nullptr );
	~SettingsDialog() override;

	private:
	Ui::SettingsDialog* ui;
};


#endif	//HYDRUS95_SETTINGSDIALOG_HPP
