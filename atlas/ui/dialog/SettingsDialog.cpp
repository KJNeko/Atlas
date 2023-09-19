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
#include "core/config.hpp"
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
	ui->cbUseSystemTheme->setChecked( config::ui::use_system_theme::get() );

	if ( !std::filesystem::exists( "./data/themes" ) ) std::filesystem::create_directories( "./data/themes" );
	//Load all valid options.
	for ( auto& qss_option : std::filesystem::directory_iterator( "./data/themes" ) )
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
}

void SettingsDialog::saveApplicationSettings()
{
	config::ui::use_system_theme::set( ui->cbUseSystemTheme->isChecked() );
	config::paths::theme::set( "./data/themes/" + ui->themeBox->currentText() );
	config::application::font::set( ui->cbAppFont->currentText() );
	config::application::fontSize::set( ui->sbAppFontSize->value() );
	//Set font for application
	QFont font { ui->cbAppFont->currentText(), ui->sbAppFontSize->value() };
	dynamic_cast< QApplication* >( QApplication::instance() )->setFont( font );

	//Set exp features
	//config::experimental::local_match::set( ui->cbExpFindAtlData->checkState() );

	reloadTheme();
}

void SettingsDialog::prepareGridViewerSettings()
{
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
	ui->leSelectedColor->setText( config::grid_ui::selectedColor::get() );
	ui->sbSelectedOpacity->setValue( config::grid_ui::selectedOpacity::get() );
	ui->cbCapsuleBorder->setChecked( config::grid_ui::enableCapsuleBorder::get() );
	ui->leBorderColor->setText( config::grid_ui::borderColor::get() );

	ui->cbTopOverlay->setChecked( config::grid_ui::enableTopOverlay::get() );
	ui->cbBottomOverlay->setChecked( config::grid_ui::enableBottomOverlay::get() );
	ui->sbOverlayHeight->setValue( config::grid_ui::overlayHeight::get() );
	ui->sbOverlayOpacity->setValue( config::grid_ui::overlayOpacity::get() );
	ui->leOverlayColor->setText( config::grid_ui::overlayColor::get() );
	ui->sbFontSize->setValue( config::grid_ui::fontSize::get() );
	ui->cbFont->setCurrentText( config::grid_ui::font::get() );

	//Text & Icon Locations
	ui->cbTitle->setCurrentIndex( config::grid_ui::titleLocation::get() );
	ui->cbEngine->setCurrentIndex( config::grid_ui::engineLocation::get() );
	ui->cbVersion->setCurrentIndex( config::grid_ui::versionLocation::get() );
	ui->cbCreator->setCurrentIndex( config::grid_ui::creatorLocation::get() );
	ui->cbCenterItems->setChecked( config::grid_ui::centerWidgets::get() );

	//Disable ui elements for future implementations
	//ui->sbCapsuleSpace->setEnabled( false ); //Feature Not Enabled
	ui->leSelectedColor->setEnabled( false );
	ui->sbSelectedOpacity->setEnabled( false );
	ui->leBorderColor->setEnabled( false );
	ui->leOverlayColor->setEnabled( false );
	ui->cbRatingIcon->setEnabled( false );
	ui->cbGameIcon->setEnabled( false );
	ui->cbFavIcon->setEnabled( false );
	ui->cbEngineIcon->setEnabled( false );
	ui->cbDownloadIcon->setEnabled( false );
	ui->cbLockY->setEnabled( false );

	gridPreviewDelegate->m_grid_size.setHeight( ui->grid_preview->height() );
	gridPreviewDelegate->m_grid_size.setWidth( ui->grid_preview->width() );

	//Experimental Settings
	ui->cbExpFindAtlData->setChecked( config::experimental::local_match::get() );
	ui->cbExpThumb->setChecked( config::experimental::loading_preview::get() );
	ui->spExpThumbRadius->setValue( config::experimental::loading_preview_blur::get() );

	//ui->cbCenterItems->setEnabled( false );
	//ui->leFont->setEnabled( false );
}

void SettingsDialog::saveBannerViewerSettings()
{
	config::grid_ui::imageLayout::set( static_cast< SCALE_TYPE >( ui->cbImageLayout->currentIndex() ) );
	config::grid_ui::blurType::set( static_cast< BLUR_TYPE >( ui->cbBlurType->currentIndex() ) );
	config::grid_ui::blurRadius::set( ui->sbBlurRadius->value() );
	config::grid_ui::featherRadius::set( ui->sbFeatherRadius->value() );
	config::grid_ui::bannerSizeX::set( ui->sbBannerX->value() );
	config::grid_ui::bannerSizeY::set( ui->sbBannerY->value() );
	config::grid_ui::bannerSpacing::set( ui->sbCapsuleSpace->value() );
	config::grid_ui::selectedColor::set( ui->leSelectedColor->text() );
	config::grid_ui::selectedOpacity::set( ui->sbSelectedOpacity->value() );
	config::grid_ui::enableCapsuleBorder::set( ui->cbCapsuleBorder->checkState() );
	config::grid_ui::borderColor::set( ui->leBorderColor->text() );

	config::grid_ui::enableTopOverlay::set( ui->cbTopOverlay->checkState() );
	config::grid_ui::enableBottomOverlay::set( ui->cbBottomOverlay->checkState() );
	config::grid_ui::overlayHeight::set( ui->sbOverlayHeight->value() );
	config::grid_ui::overlayOpacity::set( ui->sbOverlayOpacity->value() );
	config::grid_ui::overlayColor::set( ui->leOverlayColor->text() );
	config::grid_ui::fontSize::set( ui->sbFontSize->value() );
	config::grid_ui::font::set( ui->cbFont->currentText() );
	config::grid_ui::centerWidgets::set( ui->cbCenterItems->checkState() );

	config::grid_ui::titleLocation::set( static_cast< LOCATION >( ui->cbTitle->currentIndex() ) );
	config::grid_ui::engineLocation::set( static_cast< LOCATION >( ui->cbEngine->currentIndex() ) );
	config::grid_ui::versionLocation::set( static_cast< LOCATION >( ui->cbVersion->currentIndex() ) );
	config::grid_ui::creatorLocation::set( static_cast< LOCATION >( ui->cbCreator->currentIndex() ) );

	//Save experimental settings
	config::experimental::local_match::set( ui->cbExpFindAtlData->checkState() );
	config::experimental::loading_preview::set( ui->cbExpThumb->checkState() );
	config::experimental::loading_preview_blur::set( ui->spExpThumbRadius->value() );

	config::notify();
}

void SettingsDialog::preparePathsSettings()
{
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

	ui->gamesSizeLabel
		->setText( locale.formattedDataSize( static_cast< qint64 >( atlas::utils::folderSize( config::paths::games::
	                                                                                              getPath() ) ) ) );

	ui->databaseSizeLabel->setText( locale.formattedDataSize(
		static_cast< qint64 >( std::filesystem::file_size( config::paths::database::getPath() / "atlas.db" ) ) ) );
}

void SettingsDialog::savePathsSettings()
{
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
}

void SettingsDialog::prepareThreadSettings()
{
	using namespace config::threads;

	ui->sbImageImportThreads->setValue( image_import_threads::get() );
	ui->sbImageLoadingThreads->setValue( image_loader_threads::get() );
	ui->sbImportPreProcessorThreads->setValue( import_pre_loader_threads::get() );
	ui->sbImportThreads->setValue( import_threads::get() );
}

void SettingsDialog::saveThreadSettings()
{
	using namespace config::threads;

	image_import_threads::set( ui->sbImageImportThreads->value() );
	image_loader_threads::set( ui->sbImageLoadingThreads->value() );
	import_pre_loader_threads::set( ui->sbImportPreProcessorThreads->value() );
	import_threads::set( ui->sbImportThreads->value() );
}

void SettingsDialog::prepareExperimentalSettings()
{
	ui->cbExpFindAtlData->setChecked( config::experimental::local_match::get() );
}

void SettingsDialog::saveExperimentalSettings()
{
	config::experimental::local_match::set( ui->cbExpFindAtlData->isChecked() );
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
		QFile file { "./data/themes/" + text };
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
	atlas::logging::info( "{}", static_cast< int >( idx ) );
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

void SettingsDialog::on_sbOverlayHeight_valueChanged( int num )
{
	gridPreviewDelegate->m_strip_height = num;
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

void SettingsDialog::on_cbTitle_currentIndexChanged( int idx )
{
	gridPreviewDelegate->m_title_location = static_cast< LOCATION >( idx );
	qlv->repaint();
}

void SettingsDialog::on_cbEngine_currentIndexChanged( int idx )
{
	gridPreviewDelegate->m_engine_location = static_cast< LOCATION >( idx );
	qlv->repaint();
}

void SettingsDialog::on_cbVersion_currentIndexChanged( int idx )
{
	gridPreviewDelegate->m_version_location = static_cast< LOCATION >( idx );
	qlv->repaint();
}

void SettingsDialog::on_cbCreator_currentIndexChanged( int idx )
{
	gridPreviewDelegate->m_creator_location = static_cast< LOCATION >( idx );
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
