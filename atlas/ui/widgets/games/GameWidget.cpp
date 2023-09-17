#include "GameWidget.hpp"

#include <QClipboard>
#include <QDateTime>
#include <QFontMetrics>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QPaintEvent>
#include <QPainter>
#include <QTimeZone>
#include <QTimer>

#include "core/database/record/Version.hpp"
#include "core/database/remote/AtlasData.hpp"
#include "core/utils/QImageBlur.hpp"
#include "core/utils/execute/executeProc.hpp"
#include "core/utils/foldersize.hpp"
#include "moc_GameWidget.cpp"
#include "ui/delegates/ImageDelegate.hpp"
#include "ui/dialog/RecordEditor.hpp"
#include "ui/models/FilepathModel.hpp"
#include "ui_GameWidget.h"

GameWidget::GameWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::GameWidget )
{
	ui->setupUi( this );
	ui->previewList->setItemDelegate( new ImageDelegate() );
	ui->previewList->setModel( new FilepathModel() );
	//Used to detect when game has exited. Check every 2 seconds
	QTimer* timer = new QTimer( this );
	connect( timer, SIGNAL( timeout() ), this, SLOT( updateGameState() ) );
	timer->start( 2000 );

	//Check how many versions there are
}

GameWidget::~GameWidget()
{
	delete ui;
}

void GameWidget::setRecord( const atlas::records::Game record )
{
	m_record = record;

	reloadRecord();
}

void GameWidget::reloadRecord()
{
	ZoneScoped;
	auto& record { *m_record };

	//PLACEHOLDERS FOR DATA UNTIL WE ADD TO DB
	QString title = record->m_title;
	QString developer = record->m_creator;
	QString engine = record->m_engine;
	QString overview { "" };
	QString category { "" };
	QString censored { "" };
	QString status { "" };
	QString language { "" };
	QString release_date { "" };
	QString os { "" };
	QString genre { "" };
	QString tags { "" };
	QString current_version { "" };
	//END PLACEHOLDERS

	//Get cover image
	const int cover_offset = 0;
	QFuture< QPixmap > image_future { record.requestBanner(
		ui->coverImage->width() - cover_offset,
		ui->coverImage->height() - cover_offset,
		SCALE_TYPE::KEEP_ASPECT_RATIO,
		BannerType::Cover ) };

	if ( record->m_last_played == 0 )
	{
		ui->lbLastPlayed->setText( "Never" );
	}

	else
	{
		//Convert UNIX timestamp to QDateTime
		const QDateTime date {
			QDateTime::fromSecsSinceEpoch( static_cast< qint64 >( record->m_last_played ), Qt::LocalTime )
		};
		ui->lbLastPlayed->setText( QString( "%1" ).arg( date.toString() ) );
	}

	//Hide versions icon if there is only 1
	//spdlog::info( "versions: {}", record->m_versions.size() );
	if ( record->m_versions.size() == 1 )
	{
		ui->tbSelectVersion->hide();
	}

	//Sum up all the file sizes in the game's folder across multiple versions
	const auto& versions { record->m_versions };

	std::size_t total_size { 0 };
	for ( const auto& version : versions ) total_size += version.getFolderSize();

	const std::size_t latest_size { versions.front().getFolderSize() };

	//spdlog::info( "Latest size: {}, Total size: {}", latest_size, total_size );

	const auto& locale { this->locale() };

	if ( latest_size == total_size )
		ui->lbStorageUsed
			->setText( QString( "%1" ).arg( locale.formattedDataSize( static_cast< qint64 >( total_size ) ) ) );
	else
		ui->lbStorageUsed->setText( QString( "%1 (%2 total)" )
		                                .arg(
											locale.formattedDataSize( static_cast< qint64 >( latest_size ) ),
											locale.formattedDataSize( static_cast< qint64 >( total_size ) ) ) );

	//If the record has a date/time that is larger then any of the versions then use that
	using Index = std::uint64_t;
	std::vector< std::pair< std::uint64_t, Index > > playtimes;

	for ( Index i = 0; i < versions.size(); ++i )
		if ( versions[ i ].getLastPlayed() > 0 ) playtimes.emplace_back( versions[ i ].getLastPlayed(), i );

	std::sort(
		playtimes.begin(), playtimes.end(), []( const auto& lhs, const auto& rhs ) { return lhs.first > rhs.first; } );

	const auto latest_playtime { playtimes.empty() ? record->m_last_played : playtimes[ 0 ].first };

	if ( latest_playtime == 0 )
	{
		ui->lbLastPlayed->setText( "Never" );
	}
	else
	{
		//Convert UNIX timestamp to QDateTime
		const QDateTime date {
			QDateTime::fromSecsSinceEpoch( static_cast< qint64 >( latest_playtime ), Qt::LocalTime )
		};
		ui->lbLastPlayed->setText( QString( "%1" ).arg( date.toString() ) );
	}

	ui->lbTotalPlaytime
		->setText( QString( "%1" )
	                   .arg( QDateTime::
	                             fromSecsSinceEpoch( static_cast< qint64 >( record->m_total_playtime ), Qt::LocalTime )
	                                 .toUTC()
	                                 .toString( "hh:mm:ss" ) ) );

	//PREVIEWS
	dynamic_cast< FilepathModel* >( ui->previewList->model() )->setFilepaths( record->m_preview_paths );

	//Set height of PreviewList
	if ( ui->previewList->model()->rowCount() > 0 )
	{
		ui->previewList->show();
		//spdlog::info( "items{}", ui->previewList->model()->rowCount() );
		//spdlog::info( "item size{}", ui->previewList->sizeHint().width() );
		//spdlog::info( "preview width{}", ui->previewList->width() );
		//spdlog::info( "cols{}", ui->previewList->width() / ui->previewList->sizeHint().width());

		//ui->previewList->setFixedSize(ui->bannerDetailFrame->width(), ui->previewList->sizeHintForRow(0) * ui->previewList->model()->rowCount() + 2 * ui->previewList->frameWidth());

		//need to fix. row count is not updating
		ui->previewList->setFixedHeight( ui->previewList->model()->rowCount() * ui->previewList->sizeHintForRow( 1 ) );
	}
	else
	{
		//ui->previewList->hide();
	}

	std::optional< atlas::remote::AtlasRemoteData > atlas_data = record.findAtlasData( title, developer );

	//Fill vars with data if available, Check if cb is enabled from settings menu
	if ( atlas_data.has_value() && config::experimental::local_match::get() )
	{
		//QDateTime::fromMSecsSinceEpoch( &atlas_data.value()->release_date, QTimeZone::utc );
		overview = atlas_data.value()->overview;
		status = "<b>Status: </b>" + atlas_data.value()->status + "<br>";
		current_version = " (Remote" + atlas_data.value()->version + ")";
		censored = "<b>Censored: </b>" + atlas_data.value()->censored + "<br>";
		language = "<b>Language: </b>" + atlas_data.value()->language + "<br>";
		os = "<b>OS: </b>" + atlas_data.value()->os + "<br>";
		category = "<b>Category: </b>" + atlas_data.value()->category + "<br>";
		release_date = "<b>Release Date: </b>" + release_date + "<br>";
	}
	//Set Description

	ui->teDescription->setText( overview );
	title = "<b>Title: </b>" + title + "<br>";
	developer = "<b>Developer: </b>" + developer + "<br>";
	engine = "<b>Engine: </b>" + engine + "<br>";
	QString version { "<b> Version : </b> " + versions[ 0 ].getVersionName() +  current_version
		              + "<br>" };

	ui->teDetails->setText(
		"<html>" + title + developer + engine + version + status + censored + language + os + category + release_date
		+ "</html>" );

	const QPixmap cover { image_future.result() };

	cover.isNull() ? ui->coverWidget->hide() : ui->coverWidget->show(); //Hide or show based on if image is avail

	ui->coverImage->setPixmap( cover ); //Set cover. If empty then it will do nothing.

	//Experimental Functions

	//spdlog::info( "{}", record.findAtlasData( title.toStdString(), developer.toStdString() ) );
}

void GameWidget::clearRecord()
{
	m_record = std::nullopt;
}

void GameWidget::paintEvent( [[maybe_unused]] QPaintEvent* event )
{
	ZoneScoped;
	//spdlog::info( "Painting Detail ui" );
	QRect bannerFrame {
		ui->bannerFrame->x(), ui->bannerFrame->y(), ui->bannerFrame->width(), ui->bannerFrame->height()
	};

	if ( m_record->valid() )
	{
		QPainter painter { this };

		painter.save();

		atlas::records::Game& record { *m_record };
		const int image_height { 360 };
		const int image_feather { 60 };
		const int image_blur { 75 };
		//const int font_size { static_cast< int >( static_cast< float >( image_height ) * 0.1f ) };

		//Math for showing logo
		//150 is min width for lofo heigh and 280 is max height
		const double scale_factor = ui->bannerFrame->width() > 2300 ? .85 : ui->bannerFrame->width() / 2300.0;
		const int logo_height =
			( image_height * scale_factor ) < 150 ? 150 :
			( image_height * scale_factor ) > 280 ? 280 :
													( static_cast< int >( image_height * scale_factor ) );
		const int logo_width = 600;

		//Get Logo
		auto logo_future {
			record.requestBanner( logo_width, logo_height, SCALE_TYPE::KEEP_ASPECT_RATIO, BannerType::Logo )
		};

		//spdlog::info( "height:{} width:{}", logo_height, logo_width );
		//spdlog::info( ui->bannerFrame->width() );

		//Paint the banner
		const QSize banner_size { ui->bannerFrame->size() };
		auto banner_future {
			record.requestBanner( banner_size.width(), image_height, SCALE_TYPE::FIT_BLUR_EXPANDING, BannerType::Wide )
				.then(
					QtFuture::Launch::Async,
					[ &banner_size, &record ]( QPixmap banner )
					{
						// if the banner is null then we probably don't have a wide banner and should try the normal banner instead
						if ( banner.isNull() )
						{
							ZoneScopedN( "Load alternative image" );
							banner = record
				                         .requestBanner(
											 banner_size.width(),
											 image_height,
											 SCALE_TYPE::FIT_BLUR_EXPANDING,
											 BannerType::Normal )
				                         .result();
							if ( banner.isNull() ) return QPixmap();
						}

						{
							ZoneScopedN( "Blur image" );
							return blurToSize(
								std::move( banner ),
								banner_size.width(),
								image_height,
								image_feather,
								image_blur,
								FEATHER_IMAGE );
						}
					} )
		};

		/*
		QPixmap banner {
			record->banners()
				.getBanner( banner_size.width(), image_height, SCALE_TYPE::FIT_BLUR_EXPANDING, BannerType::Wide )
		};*/

		//Check if there is a wide banner, if not use normal banner

		QPixmap logo { logo_future.result() };
		//Used if logo does not work
		QFont font { config::grid_ui::font::get(), 10 };
		//const QString& title { record->m_title };
		QFontMetrics fm( font );
		painter.setFont( font );
		//int font_width = fm.horizontalAdvance( title );
		//int font_height = fm.height();

		//We need to do some magic for logo sizes
		//634 is min size banner width can be
		double logo_offset = 1.0 - ( 634.0 / ui->bannerFrame->width() );
		//NEEDS TO BE REFACTORED
		logo_offset = logo_offset <= .01 ? .01 : logo_offset >= .1 ? .1 : logo_offset;
		const auto banner { banner_future.result() };
		const QRect pixmap_rect { 0, 0, banner.width(), banner.height() };
		const QRect pixmap_logo { static_cast< int >( ui->bannerFrame->width() * logo_offset ),
			                      ( image_height / 2 ) - ( logo.height() / 2 ) - 30,
			                      logo.width(),
			                      logo.height() };

		const QRect pixmap_engine_rect { pixmap_rect.width() - 100, pixmap_rect.height() - 100, 30, 30 };

		//Engine Stuff
		/*const QPixmap engine_logo { QString::fromStdString(getEngineLogo(record->m_engine.toStdString())) };
		const QString engine_text { record->m_engine };
		QFontMetrics font_metrics(font);
		const int engine_text_width { font_metrics.horizontalAdvance( engine_text ) };*/

		QRect boundingRect;

		painter.drawPixmap( pixmap_rect, banner );
		painter.drawPixmap( pixmap_logo, logo );

		//painter.drawPixmap(pixmap_engine_rect, engine_logo.scaled(30,30,Qt::KeepAspectRatio, Qt::SmoothTransformation) );
		painter.restore();
	}
}

std::optional< atlas::records::Version > GameWidget::selectedVersion()
{
	ZoneScoped;
	if ( !m_record.has_value() ) throw std::runtime_error( "selectedVersion: Record invalid" );

	const auto& versions { m_record.value()->m_versions };

	if ( versions.size() == 0 )
		return std::nullopt;
	else
		return versions.at( selected_version_idx );
}

void GameWidget::on_btnPlay_pressed()
{
	if ( auto version = selectedVersion(); version.has_value() )
	{
		if ( processIsRunning() )
		{
			softTerminateProcess();
		}
		else
		{
			version.value().playGame();
		}

		reloadRecord();
	}
	else
	{
		atlas::logging::warn( "No version select. Or game has no versions" );
	}
}

void GameWidget::on_tbSelectVersion_pressed()
{}

void GameWidget::on_btnManageRecord_pressed()
{
	if ( !m_record.has_value() ) return;

	RecordEditor editor { m_record.value()->m_game_id, this };
	editor.exec();
}

/*void GameWidget::on_copyRecordToClip_pressed()
{
	const auto record_data { atlas::logging::dev::serialize( this->m_record.value() ) };

	QJsonDocument doc;
	doc.setObject( record_data );

	QGuiApplication::clipboard()->setText( doc.toJson() );
}*/

void GameWidget::resizeEvent( [[maybe_unused]] QResizeEvent* event )
{
	//spdlog::info( "resize event" );
	reloadRecord();

	if ( ui->previewList->model()->rowCount() > 0 )
	{
		//ui->previewList->setFixedHeight(
		//		( ui->previewList->model()->rowCount() / ui->previewList->model()->columnCount() )
		//		* ui->previewList->sizeHintForRow( 1 ) );
	}
}

void GameWidget::updateGameState()
{
	if ( lastState != processIsRunning() )
	{
		spdlog::info( "Reload Record" );
		reloadRecord();
	}
	//Check if game is already running. Update Status
	if ( processIsRunning() == true )
	{
		QIcon icon { ":/images/assets/stop_selected.svg" };
		ui->btnPlay->setIcon( icon );
		ui->btnPlay->setStyleSheet(
			"background-color: qlineargradient(spread:pad, x1:1, y1:0.46, x2:0, y2:0.539636, stop:0 rgba(65, 159, 238, 255), stop:1 rgba(41, 99, 210, 255));" );
	}
	else
	{
		QIcon icon { ":/images/assets/play_selected.svg" };
		ui->btnPlay->setIcon( icon );
		ui->btnPlay->setStyleSheet(
			"background-color: qlineargradient(spread:pad, x1:1, y1:0.46, x2:0, y2:0.539636, stop:0 rgba(43, 185, 67, 255), stop:1 rgba(111, 204, 0, 255));" );
	}

	lastState = processIsRunning();
	//Check last state. If changed, update playtime.
}

std::string GameWidget::getEngineLogo( std::string engine )
{
	//":/images/assets/Atlas_logo_v2.svg"
	//:/engines/assets/engine/3dsmax_icon.svg
	if ( engine == "Ren'Py" )
	{
		return ":/engines/assets/engine/renpy_icon.svg";
	}
	else if ( engine == "Unity" )
	{
		return ":/engines/assets/engine/unity_icon.svg";
	}
	else
	{
		return "";
	}
}