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
	void on_cbLockY_stateChanged( int state );
	void on_cbCapsuleBorder_stateChanged( int state );

	void on_sbCapsuleSpace_valueChanged( int num );
	void on_cbCenterItems_stateChanged( int state );

	QColor colorFromString( QString str );
	//Overlay Details
	//Overlay
	void on_sb_top_overlay_h_valueChanged( int num );
	void on_sb_bottom_overlay_h_valueChanged( int num );
	void on_pb_top_overlay_bcolor_pressed();
	void on_pb_bottom_overlay_bcolor_pressed();
	void on_cb_overlay_layout_currentIndexChanged( int index );
	void cb_top_overlay_stateChanged( int state );
	void cb_bottom_overlay_stateChanged( int state );
	//font
	void on_cb_font_currentTextChanged( const QString& text );
	void on_sb_font_size_valueChanged( int num );
	void on_cbAppFont_currentIndexChanged( [[maybe_unused]] int idx );
	void on_sbAppFontSize_valueChanged( [[maybe_unused]] int num );
	void on_cb_font_bold_stateChanged( int state );
	void on_cb_font_italic_stateChanged( int state );
	void on_cb_text_shadow_stateChanged( int state );
	void on_sp_bounding_box_valueChanged( int num );
	void on_sp_corner_radius_valueChanged( int num );
	//Title
	void on_cb_title_enable_stateChanged( int state );
	void on_sp_title_x_valueChanged( int num );
	void on_sp_title_y_valueChanged( int num );
	void on_cb_title_align_currentIndexChanged( int state );
	void on_pb_title_bcolor_pressed();
	void on_pb_title_fcolor_pressed();
	void on_sb_title_fontsize_valueChanged( int num );
	void on_cb_title_default_stateChanged( int state );
	void on_cb_title_link_currentIndexChanged( int state );
	//Engine
	void on_cb_engine_enable_stateChanged( int state );
	void on_sp_engine_x_valueChanged( int num );
	void on_sp_engine_y_valueChanged( int num );
	void on_cb_engine_align_currentIndexChanged( int state );
	void on_pb_engine_bcolor_pressed();
	void on_pb_engine_fcolor_pressed();
	void on_cb_engine_default_stateChanged( int state );
	void on_cb_engine_link_currentIndexChanged( int state );
	//Version
	void on_cb_version_enable_stateChanged( int state );
	void on_sp_version_x_valueChanged( int num );
	void on_sp_version_y_valueChanged( int num );
	void on_cb_version_align_currentIndexChanged( int state );
	void on_pb_version_bcolor_pressed();
	void on_pb_version_fcolor_pressed();
	void on_cb_version_default_stateChanged( int state );
	void on_cb_version_link_currentIndexChanged( int state );
	//Creator
	void on_cb_creator_enable_stateChanged( int state );
	void on_sp_creator_x_valueChanged( int num );
	void on_sp_creator_y_valueChanged( int num );
	void on_cb_creator_align_currentIndexChanged( int state );
	void on_pb_creator_bcolor_pressed();
	void on_pb_creator_fcolor_pressed();
	void on_cb_creator_default_stateChanged( int state );
	void on_cb_creator_link_currentIndexChanged( int state );
	//Status
	void on_cb_status_enable_stateChanged( int state );
	void on_sp_status_x_valueChanged( int num );
	void on_sp_status_y_valueChanged( int num );
	void on_cb_status_align_currentIndexChanged( int state );
	void on_pb_status_bcolor_pressed();
	void on_pb_status_fcolor_pressed();
	void on_cb_status_default_stateChanged( int state );
	void on_cb_status_link_currentIndexChanged( int state );
	//GameType
	void on_cb_gametype_enable_stateChanged( int state );
	void on_sp_gametype_x_valueChanged( int num );
	void on_sp_gametype_y_valueChanged( int num );
	void on_cb_gametype_align_currentIndexChanged( int state );
	void on_pb_gametype_bcolor_pressed();
	void on_pb_gametype_fcolor_pressed();
	void on_cb_gametype_default_stateChanged( int state );
	void on_cb_gametype_link_currentIndexChanged( int state );
	//Rating
	void on_cb_rating_enable_stateChanged( int state );
	void on_sp_rating_x_valueChanged( int num );
	void on_sp_rating_y_valueChanged( int num );
	void on_cb_rating_align_currentIndexChanged( int state );
	void on_pb_rating_bcolor_pressed();
	void on_pb_rating_fcolor_pressed();
	void on_cb_rating_default_stateChanged( int state );
	void on_cb_rating_link_currentIndexChanged( int state );
	//Views
	void on_cb_views_enable_stateChanged( int state );
	void on_sp_views_x_valueChanged( int num );
	void on_sp_views_y_valueChanged( int num );
	void on_cb_views_align_currentIndexChanged( int state );
	void on_pb_views_bcolor_pressed();
	void on_pb_views_fcolor_pressed();
	void on_cb_views_default_stateChanged( int state );
	void on_cb_views_link_currentIndexChanged( int state );
	//Likes
	void on_cb_likes_enable_stateChanged( int state );
	void on_sp_likes_x_valueChanged( int num );
	void on_sp_likes_y_valueChanged( int num );
	void on_cb_likes_align_currentIndexChanged( int state );
	void on_pb_likes_bcolor_pressed();
	void on_pb_likes_fcolor_pressed();
	void on_cb_likes_default_stateChanged( int state );
	void on_cb_likes_link_currentIndexChanged( int state );
	//Favorite
	void on_cb_favorite_enable_stateChanged( int state );
	void on_sp_favorite_x_valueChanged( int num );
	void on_sp_favorite_y_valueChanged( int num );
	void on_cb_favorite_align_currentIndexChanged( int state );
	void on_pb_favorite_bcolor_pressed();
	void on_pb_favorite_fcolor_pressed();
	void on_cb_favorite_default_stateChanged( int state );
	void on_cb_favorite_link_currentIndexChanged( int state );
	//UpdateIcon
	void on_cb_updateicon_enable_stateChanged( int state );
	void on_sp_updateicon_x_valueChanged( int num );
	void on_sp_updateicon_y_valueChanged( int num );
	void on_cb_updateicon_align_currentIndexChanged( int state );
	void on_pb_updateicon_bcolor_pressed();
	void on_pb_updateicon_fcolor_pressed();
	void on_cb_updateicon_default_stateChanged( int state );
	void on_cb_updateicon_link_currentIndexChanged( int state );
};

#endif //ATLAS_SETTINGSDIALOG_HPP
