//
// Created by kj16609 on 3/24/23.
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
	Q_DISABLE_COPY_MOVE( SettingsDialog )

  public:

	explicit SettingsDialog( QWidget *parent = nullptr );
	~SettingsDialog() override;

  private:

	Ui::SettingsDialog *ui;

	void prepareThemeSettings();
	void saveThemeSettings();

	void preparePathsSettings();
	void savePathsSettings();

  private slots:
	void on_settingsList_currentRowChanged( int idx );
	void on_applySettings_pressed();
	void on_cancelSettings_pressed();
	void reject() override;
	void on_themeBox_currentTextChanged( const QString &text );
	void reloadTheme();
};

#endif //HYDRUS95_SETTINGSDIALOG_HPP
