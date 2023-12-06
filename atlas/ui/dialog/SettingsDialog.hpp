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

  private:

	Ui::SettingsDialog* ui;

	void prepareThemeSettings();
	void saveApplicationSettings();

	void preparePathsSettings();
	void savePathsSettings();

	void prepareThreadSettings();
	void saveThreadSettings();

	void prepareExperimentalSettings();
	void saveExperimentalSettings();

	QListView* qlv { nullptr };
	QAbstractItemModel* gridPreviewModel { nullptr };
	RecordBannerDelegate* gridPreviewDelegate { nullptr };

  private slots:
	void on_cbImageLayout_currentIndexChanged( int idx );
	void on_cbBlurType_currentIndexChanged( int idx );
	void on_btnGeneral_pressed();
	void on_btnInterface_pressed();
	void on_btnUilayout_pressed();
	void on_btnPaths_pressed();
	void on_btnExpFeatures_pressed();
	void on_btnThreading_pressed();
	void on_applySettings_pressed();
	void on_cancelSettings_pressed();
	void reject() override;
	void on_themeBox_currentTextChanged( const QString& text );
	void on_sbBlurRadius_valueChanged( int num );
	void on_sbFeatherRadius_valueChanged( int num );
	void on_sbBannerX_valueChanged( int num );
	void on_sbBannerY_valueChanged( int num );
	void reloadTheme();
	void on_cbUseSystemTheme_stateChanged( int arg1 );
	void prepareGridViewerSettings();
	void saveBannerViewerSettings();
	void on_cbTopOverlay_stateChanged( int state );
	void on_cbLockY_stateChanged( int state );
	void on_cbBottomOverlay_stateChanged( int state );
	void on_cbCapsuleBorder_stateChanged( int state );
	void on_sbOverlayHeight_valueChanged( int num );
	void on_sbOverlayOpacity_valueChanged( int num );
	void on_sbCapsuleSpace_valueChanged( int num );
	void on_cbCenterItems_stateChanged( int state );

	void on_cbFont_currentTextChanged( const QString& text );
	void on_sbFontSize_valueChanged( int num );

	void on_cbTitle_currentIndexChanged( int idx );
	void on_cbEngine_currentIndexChanged( int idx );
	void on_cbVersion_currentIndexChanged( int idx );
	void on_cbCreator_currentIndexChanged( int idx );

	void on_cbAppFont_currentIndexChanged( [[maybe_unused]] int idx );
	void on_sbAppFontSize_valueChanged( [[maybe_unused]] int num );
};

#endif //ATLAS_SETTINGSDIALOG_HPP
