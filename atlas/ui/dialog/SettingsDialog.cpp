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

	loadSettings();

	//Any special loaders should *likely* be placed below loadSettings
	loadFontSettings();

	loadThemeSettings();
}

void SettingsDialog::loadThemeSettings()
{
	atlas::logging::debug( "Preparing theme settings" );

	// Set via loader
	//ui->cbUseSystemTheme->setChecked( config::ui::use_system_theme::get() );

	if ( !std::filesystem::exists( config::app::theme::getPath() ) )
		std::filesystem::create_directories( config::app::theme::getPath() );
	//Load all valid options.
	for ( auto& qss_option : std::filesystem::directory_iterator( config::app::theme::getPath() ) )
	{
		if ( qss_option.is_regular_file() && qss_option.path().extension() == ".qss" )
			ui->theme_box->addItem( QString::fromStdString( qss_option.path().filename().string() ) );
	}

	//Select current option
	for ( int i = 0; i < ui->theme_box->count(); ++i )
	{
		if ( ui->theme_box->itemText( i ).toStdString() == config::paths::theme::getPath().filename().string() )
		{
			//Found
			ui->theme_box->setCurrentIndex( i );
			break;
		}
	}

	ui->theme_box->setEnabled( !ui->settings_app_use_system_theme->isChecked() );
	atlas::logging::debug( "Finished preparing Theme settings" );
}

void SettingsDialog::loadFontSettings()
{
	//Add full list of fonts to comboBox.
	ui->settings_ui_font->addItems( QFontDatabase::families() );
	ui->settings_app_font->addItems( QFontDatabase::families() );

	//Set Fonts
	ui->settings_app_fontsize->setValue( config::app::fontsize::get() );

	ui->settings_app_font->setCurrentText(
		config::app::font::get() == "" ? QString::fromStdString( QApplication::font().defaultFamily().toStdString() ) :
										 config::app::font::get() );

	ui->settings_ui_font_size->setValue( config::ui::font_size::get() );
	ui->settings_ui_font->setCurrentText(
		config::ui::font::get() == "" ? QString::fromStdString( QApplication::font().defaultFamily().toStdString() ) :
										config::app::font::get() );
}

SettingsDialog::~SettingsDialog()
{
	delete gridPreviewDelegate;
	delete gridPreviewModel;
	delete ui;
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

/*
void SettingsDialog::on_themeBox_currentTextChanged( const QString& text )
{
	reloadTheme();

	if ( ui->settings_app_default_theme->isChecked() )
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
}*/

void SettingsDialog::reloadTheme()
{
	if ( config::app::use_system_theme::get() )
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

/*
void SettingsDialog::on_cbUseSystemTheme_stateChanged( [[maybe_unused]] int arg1 )
{
	ui->settings_app_theme->setEnabled( !ui->settings_app_default_theme->isChecked() );
	on_themeBox_currentTextChanged( ui->settings_app_theme->currentText() );
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
	ui->settings_ui_blur_type->setEnabled( idx == FIT_BLUR_EXPANDING || idx == FIT_BLUR_STRETCH );
	ui->settings_ui_blur_strength->setEnabled( idx == FIT_BLUR_EXPANDING || idx == FIT_BLUR_STRETCH );
	ui->settings_ui_feather_radius->setEnabled( idx == FIT_BLUR_EXPANDING || idx == FIT_BLUR_STRETCH );
	qlv->repaint();
	gridPreviewDelegate->m_scale_type = static_cast< SCALE_TYPE >( idx );
	///config::ui::gridImageLayout::set(idx);
	qlv->repaint();
	qlv->setFocus(); //force view to repaint widget
}

void SettingsDialog::on_sbBlurRadius_valueChanged( int num )
{
	//config::ui::gridBlurRadius::set(num);
	gridPreviewDelegate->m_blur_radius = num;
	qlv->repaint();
}

void SettingsDialog::on_cbBlurType_currentIndexChanged( int idx )
{
	//config::ui::gridBlurType::set(idx);
	gridPreviewDelegate->m_blur_type = static_cast< BLUR_TYPE >( idx );
	qlv->repaint();
}

void SettingsDialog::on_sbFeatherRadius_valueChanged( int num )
{
	//config::ui::gridFeatherRadius::set(num);
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
	if ( ui->settings_banner_locksize->checkState() )
	{
		//gridPreviewDelegate->m_grid_size.setHeight( static_cast< int >( w_scaled ) );
		//ui->sbBannerY->setValue( static_cast< int >( w_scaled ) );
	}
	gridPreviewDelegate->m_banner_size.setWidth( num );

	qlv->repaint();
}

void SettingsDialog::on_sbBannerY_valueChanged( int num )
{
	//config::ui::banner_y::set(num);
	gridPreviewDelegate->m_banner_size.setHeight( num );
	qlv->repaint();
}

void SettingsDialog::on_cbCapsuleBorder_stateChanged( int state )
{
	//config::ui::gridEnableBorder::set(state);
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
	ui->settings_banner_locksize->setEnabled( !state );
}

void SettingsDialog::on_cbCenterItems_stateChanged( int state )
{
	gridPreviewDelegate->m_center_widgets = state;
}

void SettingsDialog::on_cbAppFont_currentIndexChanged( [[maybe_unused]] int idx )
{
	//QFont font { ui->cbAppFont->currentText(), ui->sbAppFontSize->value() };
	//ui->lbSampleText->setFont( font );
	//lbSampleText->
	//dynamic_cast< QApplication* >( QApplication::instance() )->setFont( font );
}

void SettingsDialog::on_sbAppFontSize_valueChanged( [[maybe_unused]] int num )
{
	//QFont font { ui->cbAppFont->currentText(), num };
	//ui->lbSampleText->setFont( font );
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
*/
std::pair< QString, QString > splitSettingName( QString str )
{
	if ( !str.startsWith( "settings_", Qt::CaseInsensitive ) )
	{
		throw AtlasException( "Invalid setting name (Missing `settings_`)" );
	}

	//Remove starting `settings_` from string
	str.remove( 0, QString( "settings_" ).size() );

	auto next_underscore { str.indexOf( "_" ) };

	//Everything up to the next `_` is for the namespace
	const auto namespace_name { str.left( next_underscore ) };
	//Everything after is the setting name
	const auto setting_name { str.right( str.size() - next_underscore - 1 ) };

	return { namespace_name, setting_name };
}

void SettingsDialog::populateSettings( std::vector< QWidget* > widgets )
{
	//All widgets passed into this function must follow the format of `settings_namespacename_settingsname` naming

	for ( QWidget* widget : widgets )
	{
		auto [ namespace_name, setting_name ] = splitSettingName( widget->objectName() );

		atlas::logging::debug( "Filling settings for object: {}", widget->objectName() );

		if ( auto* ptr = qobject_cast< QCheckBox* >( widget ) )
		{
			auto ret { config::get< bool >( namespace_name, setting_name ) };
			if ( ret.has_value() ) ptr->setChecked( ret.value() );
			continue;
		}

		if ( auto* ptr = qobject_cast< QSpinBox* >( widget ) )
		{
			auto ret { config::get< int >( namespace_name, setting_name ) };
			if ( ret.has_value() ) ptr->setValue( ret.value() );
			continue;
		}

		if ( auto* ptr = qobject_cast< QLabel* >( widget ) )
		{
			auto ret { config::get< QString >( namespace_name, setting_name ) };
			if ( ret.has_value() ) ptr->setText( ret.value() );
			continue;
		}

		if ( auto* ptr = qobject_cast< QComboBox* >( widget ) )
		{
			auto ret { config::get< int >( namespace_name, setting_name ) };
			if ( ret.has_value() ) ptr->setCurrentIndex( ret.value() );
			continue;
		}

		if ( auto* ptr = qobject_cast< QLineEdit* >( widget ) )
		{
			auto ret { config::get< QString >( namespace_name, setting_name ) };
			if ( ret.has_value() ) ptr->setText( ret.value() );
			continue;
		}

		throw AtlasException( "Invalid widget type. Type: " + std::string( widget->metaObject()->className() ) );
	}
}

void SettingsDialog::loadSettings()
{
	std::vector< QWidget* > widgets;

	for ( QWidget* child : ui->stackedWidget->findChildren< QWidget* >() )
	{
		if ( child->objectName().startsWith( "settings_", Qt::CaseInsensitive ) )
		{
			widgets.push_back( child );
		}
	}

	populateSettings( widgets );
}
