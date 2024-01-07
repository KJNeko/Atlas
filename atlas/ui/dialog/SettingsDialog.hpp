//
// Created by kj16609 on 3/24/23.
//

#pragma once
#ifndef ATLAS_SETTINGSDIALOG_HPP
#define ATLAS_SETTINGSDIALOG_HPP

#include <QDialog>
#include <QtWidgets>

#include "ui/delegates/RecordBannerDelegate.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class SettingsDialog;
}

QT_END_NAMESPACE

class SettingsDialog final : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( SettingsDialog )

  public:

	explicit SettingsDialog( QWidget* parent = nullptr );
	~SettingsDialog() override;

	void loadSettings();

  private:

	Ui::SettingsDialog* ui;

	QListView* qlv { nullptr };
	QAbstractItemModel* gridPreviewModel { nullptr };
	RecordBannerDelegate* gridPreviewDelegate { nullptr };

	void populateSettings( std::vector< QWidget* > widgets );

  private slots:
	void on_btnGeneral_pressed();
	void on_btnInterface_pressed();
	void on_btnUilayout_pressed();
	void on_btnPaths_pressed();
	void on_btnExpFeatures_pressed();
	void on_btnThreading_pressed();
	void on_applySettings_pressed();
	void on_cancelSettings_pressed();
	void reject() override;
	void reloadTheme();

	QColor colorFromString( QString str );
};

#endif //ATLAS_SETTINGSDIALOG_HPP
