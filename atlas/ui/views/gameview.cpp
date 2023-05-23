#include "GameView.hpp"

#include <QDateTime>
#include <QGraphicsBlurEffect>
#include <QPaintEvent>
#include <QPainter>

#include "core/database/GameMetadata.hpp"
#include "core/foldersize.hpp"
#include "core/utils/QImageBlur.hpp"
#include "ui/delegates/ImageDelegate.hpp"
#include "ui/dialog/RecordEditor.hpp"
#include "ui/models/FilepathModel.hpp"
#include "ui_GameView.h"

GameView::GameView( QWidget* parent ) : QWidget( parent ), ui( new Ui::GameView )
{
	ui->setupUi( this );
	ui->previewList->setItemDelegate( new ImageDelegate() );
	ui->previewList->setModel( new FilepathModel() );
}

GameView::~GameView()
{
	delete ui;
}

void GameView::setRecord( const Record record )
{
	m_record = record;
	reloadRecord();
}

void GameView::reloadRecord()
{
	if ( m_record == nullptr ) return;
	const auto& record { *m_record };

	//PLACEHOLDERS FOR DATA UNTIL WE ADD TO DB
	QString description = "Test Data";
	QString developer = record->getCreator();
	QString publisher = "Test Data";
	QString original_name = "Test Data";
	QString censored = "Test Data";
	QString language = "Test Data";
	QString translations = "Test Data";
	QString voice = "Test Data";
	QString platform = "Test Data";
	QString release_date = "Test Data";
	QString genre = "Test Data";
	QString tags = "Test Data";
	QString current_version = "v1.0";

	//END PLACEHOLDERS

	//Set cover Image
	QPixmap cover {
		record->getBanner( ui->coverImage->width(), ui->coverImage->height(), FIT_BLUR_EXPANDING, BannerType::Cover )
	};
	if ( cover.isNull() )
	{
		ui->coverWidget->hide();
	}
	else
	{
		ui->coverWidget->show();
		ui->coverImage->setPixmap( cover );
	}

	if ( record->getLastPlayed() == 0 )
	{
		ui->lbLastPlayed->setText( "Never" );
	}
	else
	{
		//Convert UNIX timestamp to QDateTime
		const QDateTime date {
			QDateTime::fromSecsSinceEpoch( static_cast< qint64 >( record->getLastPlayed() ), Qt::LocalTime )
		};
		ui->lbLastPlayed->setText( QString( "%1" ).arg( date.toString() ) );
	}

	//Sum up all the file sizes in the game's folder across multiple versions
	const auto versions { record->getVersions() };

	std::size_t total_size { 0 };
	for ( const auto& version : versions ) total_size += version.getFolderSize();

	std::size_t latest_size { 0 };
	if ( const auto latest = record->getLatestVersion(); latest.has_value() )
	{
		latest_size = latest.value().getFolderSize();
	}

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
	playtimes.reserve( record->getVersions().size() );
	for ( Index i = 0; i < versions.size(); ++i )
		if ( versions[ i ].getLastPlayed() > 0 ) playtimes.emplace_back( versions[ i ].getLastPlayed(), i );

	std::sort(
		playtimes.begin(), playtimes.end(), []( const auto& lhs, const auto& rhs ) { return lhs.first > rhs.first; } );

	const auto latest_playtime { playtimes.empty() ? record->getLastPlayed() : playtimes[ 0 ].first };

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

	ui->lbTotalPlaytime->setText( QString( "%1" ).arg(
		QDateTime::fromSecsSinceEpoch( static_cast< qint64 >( record->getTotalPlaytime() ), Qt::LocalTime )
			.toUTC()
			.toString( "hh:mm:ss" ) ) );

	dynamic_cast< FilepathModel* >( ui->previewList->model() )->setFilepaths( m_record.value()->getPreviewPaths() );

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
		"<html><b>Description: </b>" + description + "<br><b>Developer: </b>" + developer + "<br><b>Publisher: </b>"
		+ publisher + "<br><b>Original Name: </b>" + original_name );
}

void GameView::clearRecord()
{
	m_record = std::nullopt;
}

void GameView::paintEvent( [[maybe_unused]] QPaintEvent* event )
{
	spdlog::info( "Painting Detail UI" );

	//spdlog::info( record->getTitle() );
	if ( *m_record != nullptr )
	{
		QPainter painter { this };

		painter.save();

		const Record& record { *m_record };
		const int image_height = 360;
		const int image_feather = 45;
		const int image_blur = 45;
		const int font_size = image_height * .1;
		const int logo_size = static_cast< int >( image_height * .75 );

		//Paint the banner
		const QSize banner_size { ui->bannerFrame->size() };
		QPixmap banner {
			record->getBanner( banner_size.width(), image_height, SCALE_TYPE::FIT_BLUR_EXPANDING, BannerType::Wide )
		};

		if ( banner.isNull() ) //return normal banner
		{
			spdlog::info( "banner is null" );
			banner = record->getBanner(
				banner_size.width(), image_height, SCALE_TYPE::FIT_BLUR_EXPANDING, BannerType::Normal );
		}
		//blur banner
		banner = blurToSize( banner, banner_size.width(), image_height, image_feather, image_blur, FEATHER_IMAGE );

		//Get Logo
		QPixmap logo { record->getBanner( logo_size, logo_size, SCALE_TYPE::KEEP_ASPECT_RATIO, BannerType::Logo ) };
		//Used if logo does not work
		QFont font { painter.font().toString(), font_size };
		QString str( record->getTitle() );
		QFontMetrics fm( font );
		painter.setFont( font );
		int font_width = fm.horizontalAdvance( str );
		int font_height = fm.height();

		//spdlog::info( "current width: {} current height: {}", banner_size.width(), banner_size.height() );
		const QRect pixmap_rect { 0, 0, banner.width(), banner.height() };
		const QRect pixmap_logo { static_cast< int >( ui->bannerFrame->width() * .1 ),
			                      ( image_height / 2 ) - ( logo.height() / 2 ) - 40,
			                      logo.width(),
			                      logo.height() };

		QRect boundingRect;
		const QRect font_rectangle = QRect(
			static_cast< int >( ui->bannerFrame->width() * .1 ),
			( image_height / 2 ) - ( font_height / 2 ),
			font_width,
			font_height );

		painter.drawPixmap( pixmap_rect, banner );

		//check if logo is null, if it is then draw text instead

		logo.isNull() ? painter.drawText( font_rectangle, 0, record->getTitle(), &boundingRect ) :
						painter.drawPixmap( pixmap_logo, logo );
		painter.restore();
	}
}

GameMetadata GameView::selectedVersion()
{
	if ( !m_record.has_value() ) throw std::runtime_error( "selectedVersion: Record invalid" );

	if ( m_record.value()->getVersions().size() == 0 )
		throw std::runtime_error( "selectedVersion: No versions available" );

	return m_record.value()->getVersions()[ selected_version_idx ];
}

void GameView::on_btnPlay_pressed()
{
	selectedVersion().playGame();
}

void GameView::on_tbSelectVersion_pressed()
{}

void GameView::on_btnManageRecord_pressed()
{
	if ( !m_record.has_value() ) return;

	RecordEditor editor { m_record.value()->getID(), this };
	editor.exec();
}

void GameView::resizeEvent( [[maybe_unused]] QResizeEvent* event )
{
	if ( ui->previewList->model()->rowCount() > 0 )
	{
		//ui->previewList->setFixedHeight(
		//		( ui->previewList->model()->rowCount() / ui->previewList->model()->columnCount() )
		//		* ui->previewList->sizeHintForRow( 1 ) );
	}
}
