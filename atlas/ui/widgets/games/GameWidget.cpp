#include "GameWidget.hpp"

#include <QClipboard>
#include <QDateTime>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPaintEvent>
#include <QPainter>

#include "core/database/record/Version.hpp"
#include "core/utils/QImageBlur.hpp"
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
	const QString& description = record->m_description;
	const QString& developer = record->m_creator;
	[[maybe_unused]] const QString& engine = record->m_engine;
	QString publisher = "";
	QString original_name = "";
	QString censored = "";
	QString language = "";
	QString translations = "";
	QString voice = "";
	QString platform = "";
	QString release_date = "";
	QString genre = "";
	QString tags = "";
	QString current_version = "";
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
	spdlog::info( "versions: {}", record->m_versions.size() );
	if ( record->m_versions.size() == 1 )
	{
		ui->tbSelectVersion->hide();
	}

	//Sum up all the file sizes in the game's folder across multiple versions
	const auto& versions { record->m_versions };

	std::size_t total_size { 0 };
	for ( const auto& version : versions ) total_size += version.getFolderSize();

	const std::size_t latest_size { versions.front().getFolderSize() };

	spdlog::info( "Latest size: {}, Total size: {}", latest_size, total_size );

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
		//need to fix. row count is not updating
		ui->previewList->setFixedHeight( ui->previewList->model()->rowCount() * ui->previewList->sizeHintForRow( 1 ) );
	}
	else
	{
		ui->previewList->hide();
	}

	//Set Description
	ui->teDescription->setText( description );
	ui->teDetails->setText(
		"<html><b>Developer: </b>" + developer + "<br><b>Engine: </b>" + engine + "<br><b>Version: </b>"
		+ versions[ 0 ].getVersionName() + "<br><b>Release Date: </b>" + release_date );

	const QPixmap cover { image_future.result() };

	cover.isNull() ? ui->coverWidget->hide() : ui->coverWidget->show(); //Hide or show based on if image is avail

	ui->coverImage->setPixmap( cover ); //Set cover. If empty then it will do nothing.
}

void GameWidget::clearRecord()
{
	m_record = std::nullopt;
}

void GameWidget::paintEvent( [[maybe_unused]] QPaintEvent* event )
{
	ZoneScoped;
	spdlog::info( "Painting Detail ui" );

	if ( m_record->valid() )
	{
		QPainter painter { this };

		painter.save();

		atlas::records::Game& record { *m_record };
		const int image_height { 360 };
		const int image_feather { 60 };
		const int image_blur { 75 };
		const int font_size { static_cast< int >( static_cast< float >( image_height ) * 0.1f ) };

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
		QFont font { painter.font().toString(), font_size };
		const QString& title { record->m_title };
		QFontMetrics fm( font );
		painter.setFont( font );
		int font_width = fm.horizontalAdvance( title );
		int font_height = fm.height();

		//We need to do some magic for logo sizes
		//634 is min size banner width can be
		double logo_offset = 1.0 - ( 634.0 / ui->bannerFrame->width() );
		spdlog::info( logo_offset );
		logo_offset = logo_offset <= .01 ? .01 : logo_offset >= .1 ? .1 : logo_offset;
		const auto banner { banner_future.result() };
		const QRect pixmap_rect { 0, 0, banner.width(), banner.height() };
		const QRect pixmap_logo { static_cast< int >( ui->bannerFrame->width() * logo_offset ),
			                      ( image_height / 2 ) - ( logo.height() / 2 ) - 30,
			                      logo.width(),
			                      logo.height() };

		QRect boundingRect;
		const QRect font_rectangle = QRect(
			static_cast< int >( ui->bannerFrame->width() * logo_offset ),
			( image_height / 2 ) - ( font_height / 2 ),
			font_width,
			font_height );

		painter.drawPixmap( pixmap_rect, banner );

		//check if logo is null, if it is then draw text instead

		logo.isNull() ? painter.drawText( font_rectangle, 0, title, &boundingRect ) :
						painter.drawPixmap( pixmap_logo, logo );
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
		QIcon icon { ":/images/assets/stop_selected.svg" };
		ui->btnPlay->setIcon( icon );
		ui->btnPlay->setStyleSheet(
			"background-color: qlineargradient(spread:pad, x1:1, y1:0.46, x2:0, y2:0.539636, stop:0 rgba(65, 159, 238, 255), stop:1 rgba(41, 99, 210, 255));" );
		version.value().playGame();
	}
	else
	{
		atlas::logging::userwarn( "No version select. Or game has no versions" );
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

void GameWidget::on_copyRecordToClip_pressed()
{
	const auto record_data { atlas::logging::dev::serialize( this->m_record.value() ) };

	QJsonDocument doc;
	doc.setObject( record_data );

	QGuiApplication::clipboard()->setText( doc.toJson() );
}

void GameWidget::resizeEvent( [[maybe_unused]] QResizeEvent* event )
{
	if ( ui->previewList->model()->rowCount() > 0 )
	{
		//ui->previewList->setFixedHeight(
		//		( ui->previewList->model()->rowCount() / ui->previewList->model()->columnCount() )
		//		* ui->previewList->sizeHintForRow( 1 ) );
	}
}
