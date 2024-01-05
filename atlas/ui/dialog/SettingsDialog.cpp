//
// Created by kj16609 on 3/24/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SettingsDialog.h" resolved

#include "SettingsDialog.hpp"

#include <moc_SettingsDialog.cpp>

#include <QFile>
#include <QMessageBox>
#include <QStandardItemModel>

#include "ProgressBarDialog.hpp"
#include "core/config/config.hpp"
#include "core/database/RapidTransaction.hpp"
#include "core/logging/logging.hpp"
#include "core/utils/foldersize.hpp"
#include "ui_SettingsDialog.h"

class DummyRecordModel : public QAbstractListModel
{
  public:

	DummyRecordModel( QObject* parent = nullptr ) : QAbstractListModel( parent ) {}

	int rowCount( [[maybe_unused]] const QModelIndex& parent ) const override { return 1; }

	QVariant data( [[maybe_unused]] const QModelIndex& index, [[maybe_unused]] int role ) const override
	{
		if ( role == Qt::DisplayRole )
			return QVariant::fromStdVariant( std::variant< atlas::records::Game >( atlas::records::Game( 1 ) ) );
		return {};
	}
};

SettingsDialog::SettingsDialog( QWidget* parent ) :
  QDialog( parent ),
  ui( new Ui::SettingsDialog ),
  gridPreviewModel( new DummyRecordModel() ),
  gridPreviewDelegate( new RecordBannerDelegate( nullptr ) )
{
	ui->setupUi( this );
	qlv = ui->grid_preview;

	qlv->setItemDelegate( gridPreviewDelegate );
	qlv->setModel( gridPreviewModel );

	//Add full list of fonts to comboBox.
	ui->cb_font->addItems( QFontDatabase::families() );
	ui->cbAppFont->addItems( QFontDatabase::families() );

	//Set Fonts
	ui->sbAppFontSize->setValue( config::application::fontSize::get() );

	ui->cbAppFont->setCurrentText(
		config::application::font::get() == "" ?
			QString::fromStdString( QApplication::font().defaultFamily().toStdString() ) :
			config::application::font::get() );

	ui->sb_font_size->setValue( config::grid_ui::font_size::get() );
	ui->cb_font->setCurrentText(
		config::grid_ui::font::get() == "" ?
			QString::fromStdString( QApplication::font().defaultFamily().toStdString() ) :
			config::application::font::get() );

	//Make sure menu is at index 0
	ui->stackedWidget->setCurrentIndex( 0 );

	//Set general as highlighted
	ui->btnGeneral->setChecked( true );

	//Set Update channel
	ui->cbUpdateChannel->setCurrentText( config::application::update_channel::get() );

	prepareThemeSettings();
	preparePathsSettings();
	prepareGridViewerSettings();
	prepareThreadSettings();
	prepareExperimentalSettings();
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::prepareThemeSettings()
{
	atlas::logging::debug( "Preparing theme settings" );
	ui->cbUseSystemTheme->setChecked( config::ui::use_system_theme::get() );

	if ( !std::filesystem::exists( config::application::theme_folder::getPath() ) )
		std::filesystem::create_directories( config::application::theme_folder::getPath() );
	//Load all valid options.
	for ( auto& qss_option : std::filesystem::directory_iterator( config::application::theme_folder::getPath() ) )
	{
		if ( qss_option.is_regular_file() && qss_option.path().extension() == ".qss" )
			ui->themeBox->addItem( QString::fromStdString( qss_option.path().filename().string() ) );
	}

	//Select current option
	for ( int i = 0; i < ui->themeBox->count(); ++i )
	{
		if ( ui->themeBox->itemText( i ).toStdString() == config::paths::theme::getPath().filename().string() )
		{
			//Found
			ui->themeBox->setCurrentIndex( i );
			break;
		}
	}

	ui->themeBox->setEnabled( !ui->cbUseSystemTheme->isChecked() );
	atlas::logging::debug( "Finished preparing Theme settings" );
}

void SettingsDialog::saveApplicationSettings()
{
	atlas::logging::debug( "Saving application settings" );
	config::ui::use_system_theme::set( ui->cbUseSystemTheme->isChecked() );
	config::paths::theme::set( config::application::theme_folder::get() + "/" + ui->themeBox->currentText() );
	config::application::font::set( ui->cbAppFont->currentText() );
	config::application::fontSize::set( ui->sbAppFontSize->value() );
	//Set font for application
	QFont font { ui->cbAppFont->currentText(), ui->sbAppFontSize->value() };
	dynamic_cast< QApplication* >( QApplication::instance() )->setFont( font );
	config::application::update_channel::set( ui->cbUpdateChannel->currentText() );
	//Set exp features
	//config::experimental::local_match::set( ui->cbExpFindAtlData->checkState() );

	reloadTheme();
	atlas::logging::debug( "Saved application settings" );
}

void SettingsDialog::prepareGridViewerSettings()
{
	atlas::logging::debug( "Preparing grid viewer settings" );
	//Init Grid previewSettings
	//Grid Capsule Settings

	//QT TEST TO SIMPLIFY CODE
	QList< QObject* > widgets = ui->groupBox_2->children();

	for ( auto const & widget : widgets )
	{
		//if ( dynamic_cast< const QCheckBox* >( widget ) != 0 ) return;

		qInfo() << "-- Found object -- " << widget->objectName();
	}

	//Set after delegate
	ui->cbImageLayout->setCurrentIndex( config::grid_ui::imageLayout::get() );
	ui->cbBlurType->setCurrentIndex( config::grid_ui::blurType::get() );
	ui->sbBlurRadius->setValue( config::grid_ui::blurRadius::get() );
	ui->sbFeatherRadius->setValue( config::grid_ui::featherRadius::get() );
	ui->sbBannerX->setValue( config::grid_ui::bannerSizeX::get() );
	ui->sbBannerY->setValue( config::grid_ui::bannerSizeY::get() );
	ui->sbCapsuleSpace->setValue( config::grid_ui::bannerSpacing::get() );
	ui->sbSelectedOpacity->setValue( config::grid_ui::selectedOpacity::get() );
	ui->cbCapsuleBorder->setChecked( config::grid_ui::enableCapsuleBorder::get() );

	ui->cb_top_overlay->setChecked( config::grid_ui::enable_top_overlay::get() );
	ui->cb_bottom_overlay->setChecked( config::grid_ui::enable_bottom_overlay::get() );
	ui->sb_top_overlay_h->setValue( config::grid_ui::top_overlay_height::get() );
	ui->sb_bottom_overlay_h->setValue( config::grid_ui::bottom_overlay_height::get() );
	ui->sb_font_size->setValue( config::grid_ui::font_size::get() );
	ui->cb_font->setCurrentText( config::grid_ui::font::get() );
	ui->cb_overlay_layout->setCurrentIndex( config::grid_ui::overlay_layout::get() );
	ui->cb_font_bold->setChecked( config::grid_ui::font_bold::get() );
	ui->cb_font_italic->setChecked( config::grid_ui::font_italic::get() );
	ui->cb_text_shadow->setChecked( config::grid_ui::font_shadow::get() );
	ui->sp_bounding_box->setValue( config::grid_ui::padding::get() );
	ui->sp_corner_radius->setValue( config::grid_ui::corner_radius::get() );

	//START BANNER SETTINGS
	//Title
	ui->cb_title_enable->setChecked( config::grid_ui::title_enable::get() );
	ui->sp_title_x->setValue( config::grid_ui::title_x::get() );
	ui->sp_title_y->setValue( config::grid_ui::title_y::get() );
	ui->cb_title_align->setCurrentIndex( config::grid_ui::title_align::get() );
	gridPreviewDelegate->m_title_bcolor = colorFromString( config::grid_ui::title_bcolor::get() );
	gridPreviewDelegate->m_title_fcolor = colorFromString( config::grid_ui::title_fcolor::get() );
	ui->sb_title_font_size->setValue( config::grid_ui::title_font_size::get() );
	ui->cb_title_default->setChecked( config::grid_ui::title_default::get() );
	ui->cb_title_link->setCurrentIndex( config::grid_ui::title_link::get() );

	//Engine
	ui->cb_engine_enable->setChecked( config::grid_ui::engine_enable::get() );
	ui->sp_engine_x->setValue( config::grid_ui::engine_x::get() );
	ui->sp_engine_y->setValue( config::grid_ui::engine_y::get() );
	ui->cb_engine_align->setCurrentIndex( config::grid_ui::engine_align::get() );
	ui->cb_engine_default->setChecked( config::grid_ui::engine_default::get() );
	gridPreviewDelegate->m_engine_bcolor = colorFromString( config::grid_ui::engine_bcolor::get() );
	gridPreviewDelegate->m_engine_fcolor = colorFromString( config::grid_ui::engine_fcolor::get() );
	ui->cb_engine_link->setCurrentIndex( config::grid_ui::engine_link::get() );
	//Version
	ui->cb_version_enable->setChecked( config::grid_ui::version_enable::get() );
	ui->sp_version_x->setValue( config::grid_ui::version_x::get() );
	ui->sp_version_y->setValue( config::grid_ui::version_y::get() );
	ui->cb_version_align->setCurrentIndex( config::grid_ui::version_align::get() );
	ui->cb_version_default->setChecked( config::grid_ui::version_default::get() );
	gridPreviewDelegate->m_version_bcolor = colorFromString( config::grid_ui::version_bcolor::get() );
	gridPreviewDelegate->m_version_fcolor = colorFromString( config::grid_ui::version_fcolor::get() );
	ui->cb_version_link->setCurrentIndex( config::grid_ui::version_link::get() );
	//CREATOR
	ui->cb_creator_enable->setChecked( config::grid_ui::creator_enable::get() );
	ui->sp_creator_x->setValue( config::grid_ui::creator_x::get() );
	ui->sp_creator_y->setValue( config::grid_ui::creator_y::get() );
	ui->cb_creator_align->setCurrentIndex( config::grid_ui::creator_align::get() );
	ui->cb_creator_default->setChecked( config::grid_ui::creator_default::get() );
	gridPreviewDelegate->m_creator_bcolor = colorFromString( config::grid_ui::creator_bcolor::get() );
	gridPreviewDelegate->m_creator_fcolor = colorFromString( config::grid_ui::creator_fcolor::get() );
	ui->cb_creator_link->setCurrentIndex( config::grid_ui::creator_link::get() );
	//STATUS
	ui->cb_status_enable->setChecked( config::grid_ui::status_enable::get() );
	ui->sp_status_x->setValue( config::grid_ui::status_x::get() );
	ui->sp_status_y->setValue( config::grid_ui::status_y::get() );
	ui->cb_status_align->setCurrentIndex( config::grid_ui::status_align::get() );
	ui->cb_status_default->setChecked( config::grid_ui::status_default::get() );
	gridPreviewDelegate->m_status_bcolor = colorFromString( config::grid_ui::status_bcolor::get() );
	gridPreviewDelegate->m_status_fcolor = colorFromString( config::grid_ui::status_fcolor::get() );
	ui->cb_status_link->setCurrentIndex( config::grid_ui::status_link::get() );
	//GameType
	ui->cb_gametype_enable->setChecked( config::grid_ui::gametype_enable::get() );
	ui->sp_gametype_x->setValue( config::grid_ui::gametype_x::get() );
	ui->sp_gametype_y->setValue( config::grid_ui::gametype_y::get() );
	ui->cb_gametype_align->setCurrentIndex( config::grid_ui::gametype_align::get() );
	ui->cb_gametype_default->setChecked( config::grid_ui::gametype_default::get() );
	gridPreviewDelegate->m_gametype_bcolor = colorFromString( config::grid_ui::gametype_bcolor::get() );
	gridPreviewDelegate->m_gametype_fcolor = colorFromString( config::grid_ui::gametype_fcolor::get() );
	ui->cb_gametype_link->setCurrentIndex( config::grid_ui::gametype_link::get() );
	//rating
	ui->cb_rating_enable->setChecked( config::grid_ui::rating_enable::get() );
	ui->sp_rating_x->setValue( config::grid_ui::rating_x::get() );
	ui->sp_rating_y->setValue( config::grid_ui::rating_y::get() );
	ui->cb_rating_align->setCurrentIndex( config::grid_ui::rating_align::get() );
	ui->cb_rating_default->setChecked( config::grid_ui::rating_default::get() );
	gridPreviewDelegate->m_rating_bcolor = colorFromString( config::grid_ui::rating_bcolor::get() );
	gridPreviewDelegate->m_rating_fcolor = colorFromString( config::grid_ui::rating_fcolor::get() );
	ui->cb_rating_link->setCurrentIndex( config::grid_ui::rating_link::get() );
	//views
	ui->cb_views_enable->setChecked( config::grid_ui::views_enable::get() );
	ui->sp_views_x->setValue( config::grid_ui::views_x::get() );
	ui->sp_views_y->setValue( config::grid_ui::views_y::get() );
	ui->cb_views_align->setCurrentIndex( config::grid_ui::views_align::get() );
	ui->cb_views_default->setChecked( config::grid_ui::views_default::get() );
	gridPreviewDelegate->m_views_bcolor = colorFromString( config::grid_ui::views_bcolor::get() );
	gridPreviewDelegate->m_views_fcolor = colorFromString( config::grid_ui::views_fcolor::get() );
	ui->cb_views_link->setCurrentIndex( config::grid_ui::views_link::get() );
	//likes
	ui->cb_likes_enable->setChecked( config::grid_ui::likes_enable::get() );
	ui->sp_likes_x->setValue( config::grid_ui::likes_x::get() );
	ui->sp_likes_y->setValue( config::grid_ui::likes_y::get() );
	ui->cb_likes_align->setCurrentIndex( config::grid_ui::likes_align::get() );
	ui->cb_likes_default->setChecked( config::grid_ui::likes_default::get() );
	gridPreviewDelegate->m_likes_bcolor = colorFromString( config::grid_ui::likes_bcolor::get() );
	gridPreviewDelegate->m_likes_fcolor = colorFromString( config::grid_ui::likes_fcolor::get() );
	ui->cb_likes_link->setCurrentIndex( config::grid_ui::likes_link::get() );
	//favorite
	ui->cb_favorite_enable->setChecked( config::grid_ui::favorite_enable::get() );
	ui->sp_favorite_x->setValue( config::grid_ui::favorite_x::get() );
	ui->sp_favorite_y->setValue( config::grid_ui::favorite_y::get() );
	ui->cb_favorite_align->setCurrentIndex( config::grid_ui::favorite_align::get() );
	ui->cb_favorite_default->setChecked( config::grid_ui::favorite_default::get() );
	gridPreviewDelegate->m_favorite_bcolor = colorFromString( config::grid_ui::favorite_bcolor::get() );
	gridPreviewDelegate->m_favorite_fcolor = colorFromString( config::grid_ui::favorite_fcolor::get() );
	ui->cb_favorite_link->setCurrentIndex( config::grid_ui::favorite_link::get() );
	//updateicon
	ui->cb_updateicon_enable->setChecked( config::grid_ui::updateicon_enable::get() );
	ui->sp_updateicon_x->setValue( config::grid_ui::updateicon_x::get() );
	ui->sp_updateicon_y->setValue( config::grid_ui::updateicon_y::get() );
	ui->cb_updateicon_align->setCurrentIndex( config::grid_ui::updateicon_align::get() );
	ui->cb_updateicon_default->setChecked( config::grid_ui::updateicon_default::get() );
	gridPreviewDelegate->m_updateicon_bcolor = colorFromString( config::grid_ui::updateicon_bcolor::get() );
	gridPreviewDelegate->m_updateicon_fcolor = colorFromString( config::grid_ui::updateicon_fcolor::get() );
	ui->cb_updateicon_link->setCurrentIndex( config::grid_ui::updateicon_link::get() );
	//END BANNER SETTINGS
	ui->cbCenterItems->setChecked( config::grid_ui::centerWidgets::get() );

	gridPreviewDelegate->m_grid_size.setHeight( ui->grid_preview->height() );
	gridPreviewDelegate->m_grid_size.setWidth( ui->grid_preview->width() );

	//Experimental Settings
	ui->cbExpFindAtlData->setChecked( config::experimental::local_match::get() );
	ui->cbExpThumb->setChecked( config::experimental::loading_preview::get() );
	ui->spExpThumbRadius->setValue( config::experimental::loading_preview_blur::get() );

	//ui->cbCenterItems->setEnabled( false );
	//ui->leFont->setEnabled( false );
	atlas::logging::debug( "Finished preparing Grid settings" );
}

void SettingsDialog::saveBannerViewerSettings()
{
	atlas::logging::debug( "Saving banner view settings" );
	config::grid_ui::imageLayout::set( static_cast< SCALE_TYPE >( ui->cbImageLayout->currentIndex() ) );
	config::grid_ui::blurType::set( static_cast< BLUR_TYPE >( ui->cbBlurType->currentIndex() ) );
	config::grid_ui::blurRadius::set( ui->sbBlurRadius->value() );
	config::grid_ui::featherRadius::set( ui->sbFeatherRadius->value() );
	config::grid_ui::bannerSizeX::set( ui->sbBannerX->value() );
	config::grid_ui::bannerSizeY::set( ui->sbBannerY->value() );
	config::grid_ui::bannerSpacing::set( ui->sbCapsuleSpace->value() );
	config::grid_ui::selectedOpacity::set( ui->sbSelectedOpacity->value() );
	config::grid_ui::enableCapsuleBorder::set( ui->cbCapsuleBorder->checkState() );

	config::grid_ui::enable_top_overlay::set( ui->cb_top_overlay->checkState() );
	config::grid_ui::enable_bottom_overlay::set( ui->cb_bottom_overlay->checkState() );
	config::grid_ui::top_overlay_height::set( ui->sb_top_overlay_h->value() );
	config::grid_ui::bottom_overlay_height::set( ui->sb_bottom_overlay_h->value() );
	config::grid_ui::font_size::set( ui->sb_font_size->value() );
	config::grid_ui::font::set( ui->cb_font->currentText() );
	config::grid_ui::centerWidgets::set( ui->cbCenterItems->checkState() );
	config::grid_ui::overlay_layout::set( ui->cb_overlay_layout->currentIndex() );
	config::grid_ui::top_overlay_bcolor::set( gridPreviewDelegate->m_top_overlay_color.name( QColor::HexArgb )
	                                              .toLower() );
	config::grid_ui::bottom_overlay_bcolor::set( gridPreviewDelegate->m_bottom_overlay_color.name( QColor::HexArgb )
	                                                 .toLower() );

	config::grid_ui::font_bold::set( ui->cb_font_bold->checkState() );
	config::grid_ui::font_italic::set( ui->cb_font_italic->checkState() );
	config::grid_ui::font_shadow::set( ui->cb_text_shadow->checkState() );
	config::grid_ui::padding::set( ui->sp_bounding_box->value() );
	config::grid_ui::corner_radius::set( ui->sp_corner_radius->value() );

	//Overlay settings
	//Title
	config::grid_ui::title_enable::set( ui->cb_title_enable->checkState() );
	config::grid_ui::title_x::set( ui->sp_title_x->value() );
	config::grid_ui::title_y::set( ui->sp_title_y->value() );
	config::grid_ui::title_align::set( ui->cb_title_align->currentIndex() );
	config::grid_ui::title_font_size::set( ui->sb_title_font_size->value() );
	config::grid_ui::title_bcolor::set( gridPreviewDelegate->m_title_bcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::title_fcolor::set( gridPreviewDelegate->m_title_fcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::title_default::set( ui->cb_title_default->checkState() );
	config::grid_ui::title_link::set( ui->cb_title_link->currentIndex() );

	//Engine
	config::grid_ui::engine_enable::set( ui->cb_engine_enable->checkState() );
	config::grid_ui::engine_x::set( ui->sp_engine_x->value() );
	config::grid_ui::engine_y::set( ui->sp_engine_y->value() );
	config::grid_ui::engine_align::set( ui->cb_engine_align->currentIndex() );
	config::grid_ui::engine_bcolor::set( gridPreviewDelegate->m_engine_bcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::engine_fcolor::set( gridPreviewDelegate->m_engine_fcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::engine_default::set( ui->cb_engine_default->checkState() );
	config::grid_ui::engine_link::set( ui->cb_engine_link->currentIndex() );
	//Version
	config::grid_ui::version_enable::set( ui->cb_version_enable->checkState() );
	config::grid_ui::version_x::set( ui->sp_version_x->value() );
	config::grid_ui::version_y::set( ui->sp_version_y->value() );
	config::grid_ui::version_align::set( ui->cb_version_align->currentIndex() );
	config::grid_ui::version_bcolor::set( gridPreviewDelegate->m_version_bcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::version_fcolor::set( gridPreviewDelegate->m_version_fcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::version_default::set( ui->cb_version_default->checkState() );
	config::grid_ui::version_link::set( ui->cb_version_link->currentIndex() );
	//Creator
	config::grid_ui::creator_enable::set( ui->cb_creator_enable->checkState() );
	config::grid_ui::creator_x::set( ui->sp_creator_x->value() );
	config::grid_ui::creator_y::set( ui->sp_creator_y->value() );
	config::grid_ui::creator_align::set( ui->cb_creator_align->currentIndex() );
	config::grid_ui::creator_bcolor::set( gridPreviewDelegate->m_creator_bcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::creator_fcolor::set( gridPreviewDelegate->m_creator_fcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::creator_default::set( ui->cb_creator_default->checkState() );
	config::grid_ui::creator_link::set( ui->cb_creator_link->currentIndex() );
	//Status
	config::grid_ui::status_enable::set( ui->cb_status_enable->checkState() );
	config::grid_ui::status_x::set( ui->sp_status_x->value() );
	config::grid_ui::status_y::set( ui->sp_status_y->value() );
	config::grid_ui::status_align::set( ui->cb_status_align->currentIndex() );
	config::grid_ui::status_bcolor::set( gridPreviewDelegate->m_status_bcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::status_fcolor::set( gridPreviewDelegate->m_status_fcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::status_default::set( ui->cb_status_default->checkState() );
	config::grid_ui::status_link::set( ui->cb_status_link->currentIndex() );
	//gametype
	config::grid_ui::gametype_enable::set( ui->cb_gametype_enable->checkState() );
	config::grid_ui::gametype_x::set( ui->sp_gametype_x->value() );
	config::grid_ui::gametype_y::set( ui->sp_gametype_y->value() );
	config::grid_ui::gametype_align::set( ui->cb_gametype_align->currentIndex() );
	config::grid_ui::gametype_bcolor::set( gridPreviewDelegate->m_gametype_bcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::gametype_fcolor::set( gridPreviewDelegate->m_gametype_fcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::gametype_default::set( ui->cb_gametype_default->checkState() );
	config::grid_ui::gametype_link::set( ui->cb_gametype_link->currentIndex() );
	//rating
	config::grid_ui::rating_enable::set( ui->cb_rating_enable->checkState() );
	config::grid_ui::rating_x::set( ui->sp_rating_x->value() );
	config::grid_ui::rating_y::set( ui->sp_rating_y->value() );
	config::grid_ui::rating_align::set( ui->cb_rating_align->currentIndex() );
	config::grid_ui::rating_bcolor::set( gridPreviewDelegate->m_rating_bcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::rating_fcolor::set( gridPreviewDelegate->m_rating_fcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::rating_default::set( ui->cb_rating_default->checkState() );
	config::grid_ui::rating_link::set( ui->cb_rating_link->currentIndex() );
	//views
	config::grid_ui::views_enable::set( ui->cb_views_enable->checkState() );
	config::grid_ui::views_x::set( ui->sp_views_x->value() );
	config::grid_ui::views_y::set( ui->sp_views_y->value() );
	config::grid_ui::views_align::set( ui->cb_views_align->currentIndex() );
	config::grid_ui::views_bcolor::set( gridPreviewDelegate->m_views_bcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::views_fcolor::set( gridPreviewDelegate->m_views_fcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::views_default::set( ui->cb_views_default->checkState() );
	config::grid_ui::views_link::set( ui->cb_views_link->currentIndex() );
	//likes
	config::grid_ui::likes_enable::set( ui->cb_likes_enable->checkState() );
	config::grid_ui::likes_x::set( ui->sp_likes_x->value() );
	config::grid_ui::likes_y::set( ui->sp_likes_y->value() );
	config::grid_ui::likes_align::set( ui->cb_likes_align->currentIndex() );
	config::grid_ui::likes_bcolor::set( gridPreviewDelegate->m_likes_bcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::likes_fcolor::set( gridPreviewDelegate->m_likes_fcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::likes_default::set( ui->cb_likes_default->checkState() );
	config::grid_ui::likes_link::set( ui->cb_likes_link->currentIndex() );
	//favorite
	config::grid_ui::favorite_enable::set( ui->cb_favorite_enable->checkState() );
	config::grid_ui::favorite_x::set( ui->sp_favorite_x->value() );
	config::grid_ui::favorite_y::set( ui->sp_favorite_y->value() );
	config::grid_ui::favorite_align::set( ui->cb_favorite_align->currentIndex() );
	config::grid_ui::favorite_bcolor::set( gridPreviewDelegate->m_favorite_bcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::favorite_fcolor::set( gridPreviewDelegate->m_favorite_fcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::favorite_default::set( ui->cb_favorite_default->checkState() );
	config::grid_ui::favorite_link::set( ui->cb_favorite_link->currentIndex() );
	//updateicon
	config::grid_ui::updateicon_enable::set( ui->cb_updateicon_enable->checkState() );
	config::grid_ui::updateicon_x::set( ui->sp_updateicon_x->value() );
	config::grid_ui::updateicon_y::set( ui->sp_updateicon_y->value() );
	config::grid_ui::updateicon_align::set( ui->cb_updateicon_align->currentIndex() );
	config::grid_ui::updateicon_bcolor::set( gridPreviewDelegate->m_updateicon_bcolor.name( QColor::HexArgb )
	                                             .toLower() );
	config::grid_ui::updateicon_fcolor::set( gridPreviewDelegate->m_updateicon_fcolor.name( QColor::HexArgb )
	                                             .toLower() );
	config::grid_ui::updateicon_default::set( ui->cb_updateicon_default->checkState() );
	config::grid_ui::updateicon_link::set( ui->cb_updateicon_link->currentIndex() );

	//Save experimental settings
	config::experimental::local_match::set( ui->cbExpFindAtlData->checkState() );
	config::experimental::loading_preview::set( ui->cbExpThumb->checkState() );
	config::experimental::loading_preview_blur::set( ui->spExpThumbRadius->value() );

	config::notify();
	atlas::logging::debug( "Finished saving banner view settings" );
}

void SettingsDialog::preparePathsSettings()
{
	atlas::logging::debug( "Preparing paths settings" );
	//Set 'root' canomical path
	ui->canonicalPath->setText( QString::fromStdString( std::filesystem::canonical( "./" ).string() ) );

	//Set paths
	ui->imagesPath->setText( config::paths::images::get() );
	ui->gamePath->setText( config::paths::games::get() );
	ui->databasePath->setText( config::paths::database::get() );

	QLocale locale { this->locale() };

	//Set filesizes
	ui->imagesSizeLabel
		->setText( locale.formattedDataSize( static_cast< qint64 >( atlas::utils::folderSize( config::paths::images::
	                                                                                              getPath() ) ) ) );

	std::uint64_t total_filesize { 0 };

	//TODO: Set a seperate in_place and not in_place size
	RapidTransaction() << "SELECT SUM(folder_size) FROM versions" >> total_filesize;

	ui->gamesSizeLabel->setText( locale.formattedDataSize( static_cast< qint64 >( total_filesize ) ) );

	ui->databaseSizeLabel->setText( locale.formattedDataSize(
		static_cast< qint64 >( std::filesystem::file_size( config::paths::database::getPath() / "atlas.db" ) ) ) );
	atlas::logging::debug( "Finished preparing paths settings" );
}

void SettingsDialog::savePathsSettings()
{
	atlas::logging::debug( "Saving path settings" );
	//Handle pathSettings
	if ( ui->gamePath->text() != config::paths::games::get() )
	{
		const std::filesystem::path new_path { ui->gamePath->text().toStdString() };

		if (
			QMessageBox::question(
				this,
				"Folder movement required",
				"We need to move the folder in order to set it to it's new location. This could take awhile, Are you sure? This CAN NOT be aborted once started." )
			== QMessageBox::No )
			return;

		if ( !std::filesystem::exists( new_path ) )
		{
			if ( QMessageBox::question(
					 this,
					 "Directory does not exist",
					 "The Game directory given does not exist. Should we create it? 'No' will abort." )
			     == QMessageBox::No )
				return;
			else
				std::filesystem::create_directories( new_path );
		}

		ProgressBarDialog progress_dialog { this };
		progress_dialog.show();

		std::vector< std::filesystem::path > folders;
		for ( const auto& group : std::filesystem::directory_iterator( config::paths::games::getPath() ) )
			if ( group.is_directory() ) folders.emplace_back( group.path() );

		progress_dialog.setMax( static_cast< int >( folders.size() ) );
		progress_dialog.showSubProgress( true );

		for ( std::size_t i = 0; i < folders.size(); ++i )
		{
			progress_dialog.setValue( static_cast< int >( i ) );
			progress_dialog
				.setText( QString::fromStdString( "Top level folder: " + folders.at( i ).filename().string() ) );

			std::vector< std::filesystem::path > files;

			const std::filesystem::path games_path { config::paths::games::getPath() };

			//Find all files that we need to copy.
			for ( const auto& file : std::filesystem::recursive_directory_iterator( folders.at( i ) ) )
				if ( file.is_regular_file() ) files.emplace_back( std::filesystem::relative( file, games_path ) );

			progress_dialog.setSubMax( static_cast< int >( files.size() ) );

			for ( std::size_t fidx = 0; fidx < files.size(); ++fidx )
			{
				progress_dialog.setSubValue( static_cast< int >( fidx ) );

				const auto source_path { games_path / files.at( fidx ) };
				const auto dest_path { new_path / files.at( fidx ) };

				if ( !std::filesystem::exists( dest_path.parent_path() ) )
					std::filesystem::create_directories( dest_path.parent_path() );

				progress_dialog.setSubText( QString::fromStdString( files.at( fidx ).string() ) );
				adjustSize();

				std::filesystem::copy( source_path, dest_path );

				QApplication::processEvents();
			}
		}

		progress_dialog.hide();

		//Delete old game folder
		std::filesystem::remove_all( config::paths::games::getPath() );

		//Set new config option
		config::paths::games::setPath( new_path );
	}

	if ( ui->imagesPath->text() != config::paths::images::get() )
	{
		const std::filesystem::path new_image_path { ui->imagesPath->text().toStdString() };

		//TODO: Make progress bar for copying images
		for ( const auto& image : std::filesystem::directory_iterator( config::paths::images::getPath() ) )
			std::filesystem::copy( image, new_image_path / image.path().filename() );

		//Delete old image folder
		std::filesystem::remove_all( config::paths::images::getPath() );

		config::paths::images::setPath( new_image_path );
	}
	atlas::logging::debug( "Finished saving path settings" );
}

void SettingsDialog::prepareThreadSettings()
{
	atlas::logging::debug( "Preparing thread settings" );
	using namespace config::threads;

	ui->sbImageImportThreads->setValue( image_import_threads::get() );
	ui->sbImageLoadingThreads->setValue( image_loader_threads::get() );
	ui->sbImportPreProcessorThreads->setValue( import_pre_loader_threads::get() );
	ui->sbImportThreads->setValue( import_threads::get() );
	atlas::logging::debug( "Finished preparing thread settings" );
}

void SettingsDialog::saveThreadSettings()
{
	atlas::logging::debug( "Saving thread settings" );
	using namespace config::threads;

	image_import_threads::set( ui->sbImageImportThreads->value() );
	image_loader_threads::set( ui->sbImageLoadingThreads->value() );
	import_pre_loader_threads::set( ui->sbImportPreProcessorThreads->value() );
	import_threads::set( ui->sbImportThreads->value() );
	atlas::logging::debug( "Finished saving thread settings" );
}

void SettingsDialog::prepareExperimentalSettings()
{
	atlas::logging::debug( "Preparing experimental settings" );
	ui->cbExpFindAtlData->setChecked( config::experimental::local_match::get() );
	atlas::logging::debug( "Finished preparing experimental settings" );
}

void SettingsDialog::saveExperimentalSettings()
{
	atlas::logging::debug( "Saving experimental settings" );
	config::experimental::local_match::set( ui->cbExpFindAtlData->isChecked() );
	atlas::logging::debug( "Finished saving experimental settings" );
}

//Fix for Linux QT ISSUE
QColor SettingsDialog::colorFromString( QString str )
{
	QColor color;
#if ( QT_VERSION >= QT_VERSION_CHECK( 6, 4, 0 ) )
	return QColor::fromString( str );
#else
	color.setNamedColor( str );
	return color;
#endif
}

//USED TO CHANGE STACKED WIDGET INDEX

void SettingsDialog::on_btnGeneral_pressed()
{
	ui->btnGeneral->setChecked( true );
	ui->btnExpFeatures->setChecked( false );
	ui->btnInterface->setChecked( false );
	ui->btnPaths->setChecked( false );
	ui->btnThreading->setChecked( false );
	ui->btnUilayout->setChecked( false );

	ui->stackedWidget->setCurrentIndex( 0 );
	ui->lblSettingsHeader->setText( "GENERAL" );
}

void SettingsDialog::on_btnInterface_pressed()
{
	ui->btnGeneral->setChecked( false );
	ui->btnExpFeatures->setChecked( false );
	ui->btnInterface->setChecked( true );
	ui->btnPaths->setChecked( false );
	ui->btnThreading->setChecked( false );
	ui->btnUilayout->setChecked( false );

	ui->stackedWidget->setCurrentIndex( 1 );
	ui->lblSettingsHeader->setText( "INTERFACE" );
}

void SettingsDialog::on_btnUilayout_pressed()
{
	ui->btnGeneral->setChecked( false );
	ui->btnExpFeatures->setChecked( false );
	ui->btnInterface->setChecked( false );
	ui->btnPaths->setChecked( false );
	ui->btnThreading->setChecked( false );
	ui->btnUilayout->setChecked( true );

	ui->stackedWidget->setCurrentIndex( 2 );
	ui->lblSettingsHeader->setText( "UI LAYOUT" );

	//repaint banner view Do not remove
	qlv->repaint();
}

void SettingsDialog::on_btnPaths_pressed()
{
	ui->btnGeneral->setChecked( false );
	ui->btnExpFeatures->setChecked( false );
	ui->btnInterface->setChecked( false );
	ui->btnPaths->setChecked( true );
	ui->btnThreading->setChecked( false );
	ui->btnUilayout->setChecked( false );

	ui->stackedWidget->setCurrentIndex( 3 );
	ui->lblSettingsHeader->setText( "PATHS" );
}

void SettingsDialog::on_btnThreading_pressed()
{
	ui->btnGeneral->setChecked( false );
	ui->btnExpFeatures->setChecked( false );
	ui->btnInterface->setChecked( false );
	ui->btnPaths->setChecked( false );
	ui->btnThreading->setChecked( true );
	ui->btnUilayout->setChecked( false );

	ui->stackedWidget->setCurrentIndex( 4 );
	ui->lblSettingsHeader->setText( "THREADING" );
}

void SettingsDialog::on_btnExpFeatures_pressed()
{
	ui->btnGeneral->setChecked( false );
	ui->btnExpFeatures->setChecked( true );
	ui->btnInterface->setChecked( false );
	ui->btnPaths->setChecked( false );
	ui->btnThreading->setChecked( false );
	ui->btnUilayout->setChecked( false );

	ui->stackedWidget->setCurrentIndex( 5 );
	ui->lblSettingsHeader->setText( "EXPERIMENTAL FEATURES" );
}

void SettingsDialog::on_applySettings_pressed()
{
	savePathsSettings();
	saveBannerViewerSettings();
	saveApplicationSettings();
	saveThreadSettings();
	saveExperimentalSettings();
	this->accept();
}

void SettingsDialog::on_cancelSettings_pressed()
{
	this->reject();
}

void SettingsDialog::reject()
{
	if ( QMessageBox::
	         question( this, "Are you sure?", "Are you sure you want to exit? Any unapplied changes will be lost" )
	     == QMessageBox::Yes )
	{
		QDialog::reject();
		reloadTheme();
	}
}

void SettingsDialog::on_themeBox_currentTextChanged( const QString& text )
{
	reloadTheme();

	if ( ui->cbUseSystemTheme->isChecked() )
	{
		atlas::logging::debug( "Using system theme" );
		dynamic_cast< QApplication* >( QApplication::instance() )->setStyleSheet( "" );
		ensurePolished();
		return;
	}
	else
	{
		atlas::logging::debug( "Theme changed to {}", text );
		QFile file { config::application::theme_folder::get() + "/" + text };
		file.open( QFile::ReadOnly );
		QString style { file.readAll() };

		dynamic_cast< QApplication* >( QApplication::instance() )->setStyleSheet( std::move( style ) );

		ensurePolished();
		return;
	}
}

void SettingsDialog::reloadTheme()
{
	if ( config::ui::use_system_theme::get() )
	{
		dynamic_cast< QApplication* >( QApplication::instance() )->setStyleSheet( "" );
		ensurePolished();
		return;
	}
	else
	{
		QFile file { config::paths::theme::get() };

		if ( !file.exists() )
		{
			config::paths::theme::setDefault();

			file.open( QFile::ReadOnly );

			QString style { file.readAll() };

			dynamic_cast< QApplication* >( QApplication::instance() )->setStyleSheet( style );

			ensurePolished();

			return;
		}

		file.open( QFile::ReadOnly );

		QString style { file.readAll() };

		dynamic_cast< QApplication* >( QApplication::instance() )->setStyleSheet( style );

		ensurePolished();
	}
}

void SettingsDialog::on_cbUseSystemTheme_stateChanged( [[maybe_unused]] int arg1 )
{
	ui->themeBox->setEnabled( !ui->cbUseSystemTheme->isChecked() );
	on_themeBox_currentTextChanged( ui->themeBox->currentText() );
}

//Grid Layout UI
void SettingsDialog::on_cb_top_overlay_stateChanged( int state )
{
	//ui->cb_top_overlay->setChecked( static_cast< bool >( state ) );
	gridPreviewDelegate->m_enable_top_overlay = static_cast< bool >( state );
	qlv->repaint();
}

void SettingsDialog::on_cb_bottom_overlay_stateChanged( int state )
{
	//ui->cb_bottom_overlay->setChecked( static_cast< bool >( state ) );
	gridPreviewDelegate->m_enable_bottom_overlay = static_cast< bool >( state );
	qlv->repaint();
}

void SettingsDialog::on_cbImageLayout_currentIndexChanged( int idx )
{
	//only show these options when SCALE_TYPE is set to Blur
	ui->cbBlurType->setEnabled( idx == FIT_BLUR_EXPANDING || idx == FIT_BLUR_STRETCH );
	ui->sbBlurRadius->setEnabled( idx == FIT_BLUR_EXPANDING || idx == FIT_BLUR_STRETCH );
	ui->sbFeatherRadius->setEnabled( idx == FIT_BLUR_EXPANDING || idx == FIT_BLUR_STRETCH );
	qlv->repaint();
	gridPreviewDelegate->m_scale_type = static_cast< SCALE_TYPE >( idx );
	///config::grid_ui::gridImageLayout::set(idx);
	qlv->repaint();
	qlv->setFocus(); //force view to repaint widget
}

void SettingsDialog::on_sbBlurRadius_valueChanged( int num )
{
	//config::grid_ui::gridBlurRadius::set(num);
	gridPreviewDelegate->m_blur_radius = num;
	qlv->repaint();
}

void SettingsDialog::on_cbBlurType_currentIndexChanged( int idx )
{
	//config::grid_ui::gridBlurType::set(idx);
	gridPreviewDelegate->m_blur_type = static_cast< BLUR_TYPE >( idx );
	qlv->repaint();
}

void SettingsDialog::on_sbFeatherRadius_valueChanged( int num )
{
	//config::grid_ui::gridFeatherRadius::set(num);
	gridPreviewDelegate->m_feather_radius = num;
	qlv->repaint();
}

void SettingsDialog::on_sbBannerX_valueChanged( int num )
{
	//const int pixels = { num * ui->sbBannerY->value() };
	//const double h_ratio { static_cast< double >( ( num + 1 ) ) / static_cast< double >( ui->sbBannerY->value() ) };
	//const double w_scaled { static_cast< double >( num ) / h_ratio };
	//printf( "num:%d ratio:%f scaled:%f\n", num, h_ratio, w_scaled );
	//TODO: Will be implented in V2
	if ( ui->cbLockY->checkState() )
	{
		//gridPreviewDelegate->m_grid_size.setHeight( static_cast< int >( w_scaled ) );
		//ui->sbBannerY->setValue( static_cast< int >( w_scaled ) );
	}
	gridPreviewDelegate->m_banner_size.setWidth( num );

	qlv->repaint();
}

void SettingsDialog::on_sbBannerY_valueChanged( int num )
{
	//config::grid_ui::banner_y::set(num);
	gridPreviewDelegate->m_banner_size.setHeight( num );
	qlv->repaint();
}

void SettingsDialog::on_cbCapsuleBorder_stateChanged( int state )
{
	//config::grid_ui::gridEnableBorder::set(state);
	gridPreviewDelegate->m_enable_capsule_border = static_cast< bool >( state );
	qlv->repaint();
}

void SettingsDialog::on_sb_top_overlay_h_valueChanged( int num )
{
	gridPreviewDelegate->m_top_overlay_height = num;
	qlv->repaint();
}

void SettingsDialog::on_sb_bottom_overlay_h_valueChanged( int num )
{
	gridPreviewDelegate->m_bottom_overlay_height = num;
	qlv->repaint();
}

void SettingsDialog::on_cb_font_currentTextChanged( const QString& text )
{
	gridPreviewDelegate->m_font_family = text;
	qlv->repaint();
}

void SettingsDialog::on_sb_font_size_valueChanged( int num )
{
	gridPreviewDelegate->m_font_size = num;
	qlv->repaint();
}

void SettingsDialog::on_sbCapsuleSpace_valueChanged( int num )
{
	gridPreviewDelegate->m_grid_spacing = num;
	//qlv->setSpacing( num );
	//qlv->repaint();
}

void SettingsDialog::on_cbLockY_stateChanged( int state )
{
	ui->sbBannerY->setEnabled( !state );
}

void SettingsDialog::on_cbCenterItems_stateChanged( int state )
{
	gridPreviewDelegate->m_center_widgets = state;
}

void SettingsDialog::on_cbAppFont_currentIndexChanged( [[maybe_unused]] int idx )
{
	QFont font { ui->cbAppFont->currentText(), ui->sbAppFontSize->value() };
	ui->lbSampleText->setFont( font );
	//lbSampleText->
	//dynamic_cast< QApplication* >( QApplication::instance() )->setFont( font );
}

void SettingsDialog::on_sbAppFontSize_valueChanged( [[maybe_unused]] int num )
{
	QFont font { ui->cbAppFont->currentText(), num };
	ui->lbSampleText->setFont( font );
}

void SettingsDialog::on_cb_font_bold_stateChanged( int state )
{
	gridPreviewDelegate->m_font_bold = state;
	qlv->repaint();
}

void SettingsDialog::on_cb_font_italic_stateChanged( int state )
{
	gridPreviewDelegate->m_font_italic = state;
	qlv->repaint();
}

void SettingsDialog::on_cb_text_shadow_stateChanged( int state )
{
	gridPreviewDelegate->m_font_shadow = state;
	qlv->repaint();
}

void SettingsDialog::on_sp_bounding_box_valueChanged( int value )
{
	gridPreviewDelegate->m_padding = value;
	qlv->repaint();
}

void SettingsDialog::on_sp_corner_radius_valueChanged( int value )
{
	gridPreviewDelegate->m_corner_radius = value;
	qlv->repaint();
}

//Overlay Settings
//Overlay
void SettingsDialog::on_pb_top_overlay_bcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_top_overlay_color,
		nullptr,
		"Overlay Top Background Color",
		QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_top_overlay_color = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_bottom_overlay_bcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_bottom_overlay_color,
		nullptr,
		"Overlay Bottom Background Color",
		QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_bottom_overlay_color = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_cb_overlay_layout_currentIndexChanged( int index )
{
	gridPreviewDelegate->m_overlay_layout = index;
	qlv->repaint();
}

//TITLE
void SettingsDialog::on_cb_title_enable_stateChanged( int state )
{
	gridPreviewDelegate->m_title_enable = state;
	qlv->repaint();
}

void SettingsDialog::on_sp_title_x_valueChanged( int num )
{
	gridPreviewDelegate->m_title_x = num;
	qlv->repaint();
}

void SettingsDialog::on_sp_title_y_valueChanged( int num )
{
	gridPreviewDelegate->m_title_y = num;
	qlv->repaint();
}

void SettingsDialog::on_cb_title_align_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_title_align = state;
	qlv->repaint();
}

void SettingsDialog::on_pb_title_bcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_title_bcolor, nullptr, "Title Background Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_title_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_title_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_title_fcolor, nullptr, "Title Foreground Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_title_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_sb_title_font_size_valueChanged( int num )
{
	gridPreviewDelegate->m_title_fontsize = num;
	qlv->repaint();
}

void SettingsDialog::on_cb_title_default_stateChanged( int state )
{
	gridPreviewDelegate->m_title_default = state;
	qlv->repaint();
}

void SettingsDialog::on_cb_title_link_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_title_link = state;
}

//ENGINE
void SettingsDialog::on_cb_engine_enable_stateChanged( int state )
{
	gridPreviewDelegate->m_engine_enable = state;
	qlv->repaint();
}

void SettingsDialog::on_sp_engine_x_valueChanged( int num )
{
	gridPreviewDelegate->m_engine_x = num;
	qlv->repaint();
}

void SettingsDialog::on_sp_engine_y_valueChanged( int num )
{
	gridPreviewDelegate->m_engine_y = num;
	qlv->repaint();
}

void SettingsDialog::on_cb_engine_align_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_engine_align = state;
	qlv->repaint();
}

void SettingsDialog::on_pb_engine_bcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_engine_bcolor,
		nullptr,
		"Engine Background Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_engine_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_engine_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_engine_fcolor,
		nullptr,
		"Engine Foreground Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_engine_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_cb_engine_default_stateChanged( int state )
{
	gridPreviewDelegate->m_engine_default = state;
	qlv->repaint();
}

void SettingsDialog::on_cb_engine_link_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_engine_link = state;
}

//VERSION
void SettingsDialog::on_cb_version_enable_stateChanged( int state )
{
	gridPreviewDelegate->m_version_enable = state;
	qlv->repaint();
}

void SettingsDialog::on_sp_version_x_valueChanged( int num )
{
	gridPreviewDelegate->m_version_x = num;
	qlv->repaint();
}

void SettingsDialog::on_sp_version_y_valueChanged( int num )
{
	gridPreviewDelegate->m_version_y = num;
	qlv->repaint();
}

void SettingsDialog::on_cb_version_align_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_version_align = state;
	qlv->repaint();
}

void SettingsDialog::on_pb_version_bcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_version_bcolor,
		nullptr,
		"Version Background Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_version_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_version_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_version_fcolor,
		nullptr,
		"Version Foreground Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_version_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_cb_version_default_stateChanged( int state )
{
	gridPreviewDelegate->m_version_default = state;
	qlv->repaint();
}

void SettingsDialog::on_cb_version_link_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_version_link = state;
}

//Creator
void SettingsDialog::on_cb_creator_enable_stateChanged( int state )
{
	gridPreviewDelegate->m_creator_enable = state;
	qlv->repaint();
}

void SettingsDialog::on_sp_creator_x_valueChanged( int num )
{
	gridPreviewDelegate->m_creator_x = num;
	qlv->repaint();
}

void SettingsDialog::on_sp_creator_y_valueChanged( int num )
{
	gridPreviewDelegate->m_creator_y = num;
	qlv->repaint();
}

void SettingsDialog::on_cb_creator_align_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_creator_align = state;
	qlv->repaint();
}

void SettingsDialog::on_pb_creator_bcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_creator_bcolor,
		nullptr,
		"Creator Background Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_creator_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_creator_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_creator_fcolor,
		nullptr,
		"Creator Foreground Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_creator_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_cb_creator_default_stateChanged( int state )
{
	gridPreviewDelegate->m_creator_default = state;
	qlv->repaint();
}

void SettingsDialog::on_cb_creator_link_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_creator_link = state;
}

//Status

void SettingsDialog::on_cb_status_enable_stateChanged( int state )
{
	gridPreviewDelegate->m_status_enable = state;
	qlv->repaint();
}

void SettingsDialog::on_sp_status_x_valueChanged( int num )
{
	gridPreviewDelegate->m_status_x = num;
	qlv->repaint();
}

void SettingsDialog::on_sp_status_y_valueChanged( int num )
{
	gridPreviewDelegate->m_status_y = num;
	qlv->repaint();
}

void SettingsDialog::on_cb_status_align_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_status_align = state;
	qlv->repaint();
}

void SettingsDialog::on_pb_status_bcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_status_bcolor,
		nullptr,
		"status Background Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_status_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_status_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_status_fcolor,
		nullptr,
		"status Foreground Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_status_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_cb_status_default_stateChanged( int state )
{
	gridPreviewDelegate->m_status_default = state;
	qlv->repaint();
}

void SettingsDialog::on_cb_status_link_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_status_link = state;
	qlv->repaint();
}

//GameType
void SettingsDialog::on_cb_gametype_enable_stateChanged( int state )
{
	gridPreviewDelegate->m_gametype_enable = state;
	qlv->repaint();
}

void SettingsDialog::on_sp_gametype_x_valueChanged( int num )
{
	gridPreviewDelegate->m_gametype_x = num;
	qlv->repaint();
}

void SettingsDialog::on_sp_gametype_y_valueChanged( int num )
{
	gridPreviewDelegate->m_gametype_y = num;
	qlv->repaint();
}

void SettingsDialog::on_cb_gametype_align_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_gametype_align = state;
	qlv->repaint();
}

void SettingsDialog::on_pb_gametype_bcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_gametype_bcolor,
		nullptr,
		"Gametype Background Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_gametype_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_gametype_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_gametype_fcolor,
		nullptr,
		"Gametype Foreground Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_gametype_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_cb_gametype_default_stateChanged( int state )
{
	gridPreviewDelegate->m_gametype_default = state;
	qlv->repaint();
}

void SettingsDialog::on_cb_gametype_link_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_gametype_link = state;
}

//rating
void SettingsDialog::on_cb_rating_enable_stateChanged( int state )
{
	gridPreviewDelegate->m_rating_enable = state;
	qlv->repaint();
}

void SettingsDialog::on_sp_rating_x_valueChanged( int num )
{
	gridPreviewDelegate->m_rating_x = num;
	qlv->repaint();
}

void SettingsDialog::on_sp_rating_y_valueChanged( int num )
{
	gridPreviewDelegate->m_rating_y = num;
	qlv->repaint();
}

void SettingsDialog::on_cb_rating_align_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_rating_align = state;
	qlv->repaint();
}

void SettingsDialog::on_pb_rating_bcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_rating_bcolor,
		nullptr,
		"Rating Background Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_rating_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_rating_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_rating_fcolor,
		nullptr,
		"Rating Foreground Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_rating_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_cb_rating_default_stateChanged( int state )
{
	gridPreviewDelegate->m_rating_default = state;
	qlv->repaint();
}

void SettingsDialog::on_cb_rating_link_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_rating_link = state;
}

//views
void SettingsDialog::on_cb_views_enable_stateChanged( int state )
{
	gridPreviewDelegate->m_views_enable = state;
	qlv->repaint();
}

void SettingsDialog::on_sp_views_x_valueChanged( int num )
{
	gridPreviewDelegate->m_views_x = num;
	qlv->repaint();
}

void SettingsDialog::on_sp_views_y_valueChanged( int num )
{
	gridPreviewDelegate->m_views_y = num;
	qlv->repaint();
}

void SettingsDialog::on_cb_views_align_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_views_align = state;
	qlv->repaint();
}

void SettingsDialog::on_pb_views_bcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_views_bcolor, nullptr, "Views Background Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_views_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_views_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_views_fcolor, nullptr, "Views Foreground Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_views_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_cb_views_default_stateChanged( int state )
{
	gridPreviewDelegate->m_views_default = state;
	qlv->repaint();
}

void SettingsDialog::on_cb_views_link_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_views_link = state;
}

//likes
void SettingsDialog::on_cb_likes_enable_stateChanged( int state )
{
	gridPreviewDelegate->m_likes_enable = state;
	qlv->repaint();
}

void SettingsDialog::on_sp_likes_x_valueChanged( int num )
{
	gridPreviewDelegate->m_likes_x = num;
	qlv->repaint();
}

void SettingsDialog::on_sp_likes_y_valueChanged( int num )
{
	gridPreviewDelegate->m_likes_y = num;
	qlv->repaint();
}

void SettingsDialog::on_cb_likes_align_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_likes_align = state;
	qlv->repaint();
}

void SettingsDialog::on_pb_likes_bcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_likes_bcolor, nullptr, "Likes Background Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_likes_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_likes_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_likes_fcolor, nullptr, "Likes Foreground Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_likes_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_cb_likes_default_stateChanged( int state )
{
	gridPreviewDelegate->m_likes_default = state;
	qlv->repaint();
}

void SettingsDialog::on_cb_likes_link_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_likes_link = state;
}

//favorite
void SettingsDialog::on_cb_favorite_enable_stateChanged( int state )
{
	gridPreviewDelegate->m_favorite_enable = state;
	qlv->repaint();
}

void SettingsDialog::on_sp_favorite_x_valueChanged( int num )
{
	gridPreviewDelegate->m_favorite_x = num;
	qlv->repaint();
}

void SettingsDialog::on_sp_favorite_y_valueChanged( int num )
{
	gridPreviewDelegate->m_favorite_y = num;
	qlv->repaint();
}

void SettingsDialog::on_cb_favorite_align_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_favorite_align = state;
	qlv->repaint();
}

void SettingsDialog::on_pb_favorite_bcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_favorite_bcolor,
		nullptr,
		"Favorite Background Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_favorite_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_favorite_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_favorite_fcolor,
		nullptr,
		"Favorite Foreground Color" ); //, QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_favorite_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_cb_favorite_default_stateChanged( int state )
{
	gridPreviewDelegate->m_favorite_default = state;
	qlv->repaint();
}

void SettingsDialog::on_cb_favorite_link_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_favorite_link = state;
}

//updateicon
void SettingsDialog::on_cb_updateicon_enable_stateChanged( int state )
{
	gridPreviewDelegate->m_updateicon_enable = state;
	qlv->repaint();
}

void SettingsDialog::on_sp_updateicon_x_valueChanged( int num )
{
	gridPreviewDelegate->m_updateicon_x = num;
	qlv->repaint();
}

void SettingsDialog::on_sp_updateicon_y_valueChanged( int num )
{
	gridPreviewDelegate->m_updateicon_y = num;
	qlv->repaint();
}

void SettingsDialog::on_cb_updateicon_align_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_updateicon_align = state;
	qlv->repaint();
}

void SettingsDialog::on_pb_updateicon_bcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_updateicon_bcolor,
		nullptr,
		"updateicon Background Color" ); //,QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_updateicon_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_updateicon_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_updateicon_fcolor,
		nullptr,
		"updateicon Foreground Color" ); //,QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_updateicon_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_cb_updateicon_default_stateChanged( int state )
{
	gridPreviewDelegate->m_updateicon_default = state;
	qlv->repaint();
}

void SettingsDialog::on_cb_updateicon_link_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_updateicon_link = state;
}