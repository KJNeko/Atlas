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
	ui->cbFont->addItems( QFontDatabase::families() );
	ui->cbAppFont->addItems( QFontDatabase::families() );

	//Set Fonts
	ui->sbAppFontSize->setValue( config::application::fontSize::get() );

	ui->cbAppFont->setCurrentText(
		config::application::font::get() == "" ?
			QString::fromStdString( QApplication::font().defaultFamily().toStdString() ) :
			config::application::font::get() );

	ui->sbFontSize->setValue( config::grid_ui::fontSize::get() );
	ui->cbFont->setCurrentText(
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

	ui->cbTopOverlay->setChecked( config::grid_ui::enableTopOverlay::get() );
	ui->cbBottomOverlay->setChecked( config::grid_ui::enableBottomOverlay::get() );
	ui->sb_top_overlay_h->setValue( config::grid_ui::top_overlay_height::get() );
	ui->sb_bottom_overlay_h->setValue( config::grid_ui::bottom_overlay_height::get() );
	ui->sbOverlayOpacity->setValue( config::grid_ui::overlayOpacity::get() );
	ui->pbOverlayColor->setText( config::grid_ui::overlayColor::get() );
	ui->sbFontSize->setValue( config::grid_ui::fontSize::get() );
	ui->cbFont->setCurrentText( config::grid_ui::font::get() );
	ui->cbOverlayLayout->setCurrentIndex( config::grid_ui::overlayLayout::get() );

	//START BANNER SETTINGS
	//Title
	ui->cb_title_enable->setChecked( config::grid_ui::title_enable::get() );
	ui->sp_title_x->setValue( config::grid_ui::title_x::get() );
	ui->sp_title_y->setValue( config::grid_ui::title_y::get() );
	ui->cb_title_align->setCurrentIndex( config::grid_ui::title_align::get() );
	gridPreviewDelegate->m_title_bcolor = colorFromString( config::grid_ui::title_bcolor::get() );
	gridPreviewDelegate->m_title_fcolor = colorFromString( config::grid_ui::title_fcolor::get() );
	ui->sp_title_fontsize->setValue( config::grid_ui::title_font_size::get() );
	gridPreviewDelegate->m_title_bold = config::grid_ui::title_bold_enable::get();
	gridPreviewDelegate->m_title_italics = config::grid_ui::title_italics_enable::get();

	//Engine
	ui->cb_engine_enable->setChecked( config::grid_ui::engine_enable::get() );
	ui->sp_engine_x->setValue( config::grid_ui::engine_x::get() );
	ui->sp_engine_y->setValue( config::grid_ui::engine_y::get() );
	ui->cb_engine_align->setCurrentIndex( config::grid_ui::engine_align::get() );
	ui->cb_engine_default_colors->setChecked( config::grid_ui::engine_default_color::get() );
	gridPreviewDelegate->m_engine_bcolor = colorFromString( config::grid_ui::engine_bcolor::get() );
	gridPreviewDelegate->m_engine_fcolor = colorFromString( config::grid_ui::engine_fcolor::get() );
	gridPreviewDelegate->m_engine_bold = config::grid_ui::engine_bold_enable::get();
	gridPreviewDelegate->m_engine_italics = config::grid_ui::engine_italics_enable::get();
	//Version
	ui->cb_version_enable->setChecked( config::grid_ui::version_enable::get() );
	ui->sp_version_x->setValue( config::grid_ui::version_x::get() );
	ui->sp_version_y->setValue( config::grid_ui::version_y::get() );
	ui->cb_version_align->setCurrentIndex( config::grid_ui::version_align::get() );
	gridPreviewDelegate->m_version_bcolor = colorFromString( config::grid_ui::version_bcolor::get() );
	gridPreviewDelegate->m_version_fcolor = colorFromString( config::grid_ui::version_fcolor::get() );
	gridPreviewDelegate->m_version_bold = config::grid_ui::version_bold_enable::get();
	gridPreviewDelegate->m_version_italics = config::grid_ui::version_italics_enable::get();
	//CREATOR
	ui->cb_creator_enable->setChecked( config::grid_ui::creator_enable::get() );
	ui->sp_creator_x->setValue( config::grid_ui::creator_x::get() );
	ui->sp_creator_y->setValue( config::grid_ui::creator_y::get() );
	ui->cb_creator_align->setCurrentIndex( config::grid_ui::creator_align::get() );
	gridPreviewDelegate->m_creator_bcolor = colorFromString( config::grid_ui::creator_bcolor::get() );
	gridPreviewDelegate->m_creator_fcolor = colorFromString( config::grid_ui::creator_fcolor::get() );
	gridPreviewDelegate->m_creator_bold = config::grid_ui::creator_bold_enable::get();
	gridPreviewDelegate->m_creator_italics = config::grid_ui::creator_italics_enable::get();
	//STATUS
	ui->cb_status_enable->setChecked( config::grid_ui::status_enable::get() );
	ui->sp_status_x->setValue( config::grid_ui::status_x::get() );
	ui->sp_status_y->setValue( config::grid_ui::status_y::get() );
	ui->cb_status_align->setCurrentIndex( config::grid_ui::status_align::get() );
	ui->cb_status_link->setCurrentIndex( config::grid_ui::status_link::get() );
	//gridPreviewDelegate->m_status_bcolor = colorFromString( config::grid_ui::status_bcolor::get() );
	//gridPreviewDelegate->m_status_fcolor = colorFromString( config::grid_ui::status_fcolor::get() );
	//gridPreviewDelegate->m_status_bold = config::grid_ui::status_bold_enable::get();
	//gridPreviewDelegate->m_status_italics = config::grid_ui::status_italics_enable::get();
	//END BANNER SETTINGS
	//GemeType
	ui->cb_gametype_enable->setChecked( config::grid_ui::gametype_enable::get() );
	ui->sp_gametype_x->setValue( config::grid_ui::gametype_x::get() );
	ui->sp_gametype_y->setValue( config::grid_ui::gametype_y::get() );
	ui->cb_gametype_align->setCurrentIndex( config::grid_ui::gametype_align::get() );
	ui->cb_gametype_link->setCurrentIndex( config::grid_ui::gametype_link::get() );

	ui->cbCenterItems->setChecked( config::grid_ui::centerWidgets::get() );

	//Disable ui elements for future implementations
	//ui->sbCapsuleSpace->setEnabled( false ); //Feature Not Enabled
	ui->sbSelectedOpacity->setEnabled( false );
	ui->cbLockY->setEnabled( false );

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

	config::grid_ui::enableTopOverlay::set( ui->cbTopOverlay->checkState() );
	config::grid_ui::enableBottomOverlay::set( ui->cbBottomOverlay->checkState() );
	config::grid_ui::top_overlay_height::set( ui->sb_top_overlay_h->value() );
	config::grid_ui::bottom_overlay_height::set( ui->sb_bottom_overlay_h->value() );
	config::grid_ui::overlayOpacity::set( ui->sbOverlayOpacity->value() );
	config::grid_ui::overlayColor::set( ui->pbOverlayColor->text() );
	config::grid_ui::fontSize::set( ui->sbFontSize->value() );
	config::grid_ui::font::set( ui->cbFont->currentText() );
	config::grid_ui::centerWidgets::set( ui->cbCenterItems->checkState() );
	config::grid_ui::overlayLayout::set( ui->cbOverlayLayout->currentIndex() );
	config::grid_ui::overlayColor::
		set( gridPreviewDelegate->m_overlay_color.alpha() == 0 ?
	             "transparent" :
	             gridPreviewDelegate->m_overlay_color.name( QColor::HexArgb ).toLower() );

	//Overlay settings
	//Title
	config::grid_ui::title_enable::set( ui->cb_title_enable->checkState() );
	config::grid_ui::title_x::set( ui->sp_title_x->value() );
	config::grid_ui::title_y::set( ui->sp_title_y->value() );
	config::grid_ui::title_align::set( ui->cb_title_align->currentIndex() );
	config::grid_ui::title_font_size::set( ui->sp_title_fontsize->value() );
	config::grid_ui::title_bcolor::
		set( gridPreviewDelegate->m_title_bcolor.alpha() == 0 ?
	             "transparent" :
	             gridPreviewDelegate->m_title_bcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::title_fcolor::
		set( gridPreviewDelegate->m_title_fcolor.alpha() == 0 ?
	             "transparent" :
	             gridPreviewDelegate->m_title_fcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::title_bold_enable::set( gridPreviewDelegate->m_title_bold );
	config::grid_ui::title_italics_enable::set( gridPreviewDelegate->m_title_italics );
	//Engine
	config::grid_ui::engine_enable::set( ui->cb_engine_enable->checkState() );
	config::grid_ui::engine_x::set( ui->sp_engine_x->value() );
	config::grid_ui::engine_y::set( ui->sp_engine_y->value() );
	config::grid_ui::engine_default_color::set( ui->cb_engine_default_colors->checkState() );
	config::grid_ui::engine_align::set( ui->cb_engine_align->currentIndex() );
	config::grid_ui::title_bcolor::
		set( gridPreviewDelegate->m_engine_bcolor.alpha() == 0 ?
	             "transparent" :
	             gridPreviewDelegate->m_engine_bcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::title_fcolor::
		set( gridPreviewDelegate->m_engine_fcolor.alpha() == 0 ?
	             "transparent" :
	             gridPreviewDelegate->m_engine_fcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::title_bold_enable::set( gridPreviewDelegate->m_engine_bold );
	config::grid_ui::title_italics_enable::set( gridPreviewDelegate->m_engine_italics );
	//Version
	config::grid_ui::version_enable::set( ui->cb_version_enable->checkState() );
	config::grid_ui::version_x::set( ui->sp_version_x->value() );
	config::grid_ui::version_y::set( ui->sp_version_y->value() );
	config::grid_ui::engine_align::set( ui->cb_engine_align->currentIndex() );
	config::grid_ui::version_bcolor::
		set( gridPreviewDelegate->m_version_bcolor.alpha() == 0 ?
	             "transparent" :
	             gridPreviewDelegate->m_version_bcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::version_fcolor::
		set( gridPreviewDelegate->m_version_fcolor.alpha() == 0 ?
	             "transparent" :
	             gridPreviewDelegate->m_version_fcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::version_bold_enable::set( gridPreviewDelegate->m_version_bold );
	config::grid_ui::version_italics_enable::set( gridPreviewDelegate->m_version_italics );
	//Creator
	config::grid_ui::creator_enable::set( ui->cb_creator_enable->checkState() );
	config::grid_ui::creator_x::set( ui->sp_creator_x->value() );
	config::grid_ui::creator_y::set( ui->sp_creator_y->value() );
	config::grid_ui::engine_align::set( ui->cb_engine_align->currentIndex() );
	config::grid_ui::creator_bcolor::
		set( gridPreviewDelegate->m_creator_bcolor.alpha() == 0 ?
	             "transparent" :
	             gridPreviewDelegate->m_creator_bcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::creator_fcolor::
		set( gridPreviewDelegate->m_creator_fcolor.alpha() == 0 ?
	             "transparent" :
	             gridPreviewDelegate->m_creator_fcolor.name( QColor::HexArgb ).toLower() );
	config::grid_ui::creator_bold_enable::set( gridPreviewDelegate->m_creator_bold );
	config::grid_ui::creator_italics_enable::set( gridPreviewDelegate->m_creator_italics );
	//Status
	config::grid_ui::status_enable::set( ui->cb_status_enable->checkState() );
	config::grid_ui::status_x::set( ui->sp_status_x->value() );
	config::grid_ui::status_y::set( ui->sp_status_y->value() );
	config::grid_ui::status_align::set( ui->cb_status_align->currentIndex() );
	config::grid_ui::status_link::set( ui->cb_status_link->currentIndex() );
	//gametype
	config::grid_ui::gametype_enable::set( ui->cb_gametype_enable->checkState() );
	config::grid_ui::gametype_x::set( ui->sp_gametype_x->value() );
	config::grid_ui::gametype_y::set( ui->sp_gametype_y->value() );
	config::grid_ui::gametype_align::set( ui->cb_gametype_align->currentIndex() );
	config::grid_ui::gametype_link::set( ui->cb_gametype_link->currentIndex() );

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
	return color.setNamedColor( str );
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
void SettingsDialog::on_cbTopOverlay_stateChanged( [[maybe_unused]] int state )
{
	ui->cbTopOverlay->setChecked( static_cast< bool >( state ) );
	gridPreviewDelegate->m_enable_top_overlay = static_cast< bool >( state );
	qlv->repaint();
}

void SettingsDialog::on_cbBottomOverlay_stateChanged( [[maybe_unused]] int state )
{
	ui->cbBottomOverlay->setChecked( static_cast< bool >( state ) );
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

void SettingsDialog::on_sbOverlayOpacity_valueChanged( int num )
{
	gridPreviewDelegate->m_overlay_opacity = num;
	qlv->repaint();
}

void SettingsDialog::on_cbFont_currentTextChanged( const QString& text )
{
	gridPreviewDelegate->m_font_family = text;
	qlv->repaint();
}

void SettingsDialog::on_sbFontSize_valueChanged( int num )
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

//Overlay Settings
//Overlay
void SettingsDialog::on_pbOverlayColor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_overlay_color, nullptr, "Overlay Background Color", QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_overlay_color = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_cbOverlayLayout_currentIndexChanged( int index )
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
		gridPreviewDelegate->m_title_bcolor, nullptr, "Title Background Color", QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_title_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_title_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_title_fcolor, nullptr, "Title Foreground Color", QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_title_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_sp_title_fontsize_valueChanged( int num )
{
	gridPreviewDelegate->m_title_fontsize = num;
	qlv->repaint();
}

void SettingsDialog::on_pb_title_bold_pressed()
{
	gridPreviewDelegate->m_title_bold = !gridPreviewDelegate->m_title_bold;
}

void SettingsDialog::on_pb_title_italics_pressed()
{
	gridPreviewDelegate->m_title_italics = !gridPreviewDelegate->m_title_italics;
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
		gridPreviewDelegate->m_engine_bcolor, nullptr, "Engine Background Color", QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_engine_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_engine_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_engine_fcolor, nullptr, "Engine Foreground Color", QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_engine_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_cb_engine_default_colors_stateChanged( int state )
{
	gridPreviewDelegate->m_engine_default_colors = state;
	qlv->repaint();
}

void SettingsDialog::on_pb_engine_bold_pressed()
{
	gridPreviewDelegate->m_engine_bold = !gridPreviewDelegate->m_engine_bold;
}

void SettingsDialog::on_pb_engine_italics_pressed()
{
	gridPreviewDelegate->m_engine_italics = !gridPreviewDelegate->m_engine_italics;
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
		gridPreviewDelegate->m_version_bcolor, nullptr, "Version Background Color", QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_version_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_version_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_version_fcolor, nullptr, "Version Foreground Color", QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_version_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_version_bold_pressed()
{
	gridPreviewDelegate->m_version_bold = !gridPreviewDelegate->m_version_bold;
}

void SettingsDialog::on_pb_version_italics_pressed()
{
	gridPreviewDelegate->m_version_italics = !gridPreviewDelegate->m_version_italics;
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
		gridPreviewDelegate->m_creator_bcolor, nullptr, "Creator Background Color", QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_creator_bcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_creator_fcolor_pressed()
{
	QColorDialog colorDialog( this );
	QColor backgroundColor = colorDialog.getColor(
		gridPreviewDelegate->m_creator_fcolor, nullptr, "Creator Foreground Color", QColorDialog::ShowAlphaChannel );
	gridPreviewDelegate->m_creator_fcolor = backgroundColor;
	qlv->repaint();
}

void SettingsDialog::on_pb_creator_bold_pressed()
{
	gridPreviewDelegate->m_creator_bold = !gridPreviewDelegate->m_creator_bold;
}

void SettingsDialog::on_pb_creator_italics_pressed()
{
	gridPreviewDelegate->m_creator_italics = !gridPreviewDelegate->m_creator_italics;
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

void SettingsDialog::on_cb_status_link_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_status_link = state;
}

/*
void SettingsDialog::on_pb_status_bold_pressed()
{}

void SettingsDialog::on_pb_status_italics_pressed()
{}
*/
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

void SettingsDialog::on_cb_gametype_link_currentIndexChanged( int state )
{
	gridPreviewDelegate->m_gametype_link = state;
}

/*
void SettingsDialog::on_pb_gametype_bold_pressed()
{}

void SettingsDialog::on_pb_gametype_italics_pressed()
{}

//Rating
void SettingsDialog::on_cb_rating_enable_stateChanged( int state )
{}

void SettingsDialog::on_sp_rating_x_valueChanged( int num )
{}

void SettingsDialog::on_sp_rating_y_valueChanged( int num )
{}

void SettingsDialog::on_cb_rating_align_currentIndexChanged( int state )
{}

void SettingsDialog::on_cb_rating_link_currentIndexChanged( int state )
{}

//Views
void SettingsDialog::on_cb_views_enable_stateChanged( int state )
{}

void SettingsDialog::on_sp_views_x_valueChanged( int num )
{}

void SettingsDialog::on_sp_views_y_valueChanged( int num )
{}

void SettingsDialog::on_cb_views_align_currentIndexChanged( int state )
{}

void SettingsDialog::on_cb_views_link_currentIndexChanged( int state )
{}

//Likes
void SettingsDialog::on_cb_likes_enable_stateChanged( int state )
{}

void SettingsDialog::on_sp_likes_x_valueChanged( int num )
{}

void SettingsDialog::on_sp_likes_y_valueChanged( int num )
{}

void SettingsDialog::on_cb_likes_align_currentIndexChanged( int state )
{}

void SettingsDialog::on_cb_likes_link_currentIndexChanged( int state )
{}

//Favorite
void SettingsDialog::on_cb_favorite_enable_stateChanged( int state )
{}

void SettingsDialog::on_sp_favorite_x_valueChanged( int num )
{}

void SettingsDialog::on_sp_favorite_y_valueChanged( int num )
{}

void SettingsDialog::on_cb_favorite_align_currentIndexChanged( int state )
{}

void SettingsDialog::on_cb_favorite_link_currentIndexChanged( int state )
{}

//UpdateIcon
void SettingsDialog::on_cb_updateicon_enable_stateChanged( int state )
{}

void SettingsDialog::on_sp_updateicon_x_valueChanged( int num )
{}

void SettingsDialog::on_sp_updateicon_y_valueChanged( int num )
{}

void SettingsDialog::on_cb_updateicon_align_currentIndexChanged( int state )
{}
*/
