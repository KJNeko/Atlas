//
// Created by kj16609 on 1/18/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SelectedViewWidget.h" resolved


#include <QResizeEvent>

#include <tracy/Tracy.hpp>

#include <h95/config.hpp>
#include <h95/executeProc.hpp>

#include "SelectedViewWidget.hpp"
#include "ui_SelectedViewWidget.h"
#include "h95/KeyReplacer.hpp"

SelectedViewWidget::SelectedViewWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::SelectedViewWidget )
{
	ui->setupUi( this );
}

SelectedViewWidget::~SelectedViewWidget()
{
	delete ui;
}

void SelectedViewWidget::recordSelected( const QPersistentModelIndex& record )
{
	ZoneScoped;

	if ( record.isValid() )
	{
		if ( this->isHidden() ) this->show();

		const auto& record_data { *record.data().value< const Record* >() };

		selected = record;

		if ( !selected.has_value() )
		{
			spdlog::warn( "Somehow selected didn't have a value!" );
			return;
		}

		ui->banner->setPixmap(
			record_data.getBanner( ui->bannerFrame->size().width() - 20, ui->bannerFrame->minimumHeight() - 20 ) );

		// If there is a creator do {title} by {creator} else just do {title}
		if ( record_data.m_creator.isEmpty() )
			ui->title->setText( record_data.m_title );
		else
			ui->title->setText( QString( "%1 by %2" ).arg( record_data.m_title, record_data.m_creator ) );

		ui->versionSelection->clear();

		QStringList versions;

		//Populate version selection
		for ( const auto& version : record_data.m_versions ) { versions.emplace_back( version.m_version ); }

		ui->versionSelection->addItems( versions );
	}
	else
	{
		this->hide();
		selected = std::nullopt;
	}
}

void SelectedViewWidget::resizeEvent( QResizeEvent* event )
{
	ZoneScoped;
	if ( selected.has_value() )
	{
		const auto record_data { selected.value().data().value< const Record* >() };

		ui->banner->setPixmap(
			record_data->getBanner( ui->bannerFrame->size().width() - 20, ui->bannerFrame->minimumHeight() - 20 ) );
	}
	QWidget::resizeEvent( event );
}

void SelectedViewWidget::on_closeButton_pressed()
{
	emit hiding();
	selected = std::nullopt;
	this->hide();
}

void SelectedViewWidget::on_playButton_pressed()
{
	const auto selected_text { ui->versionSelection->currentText() };

	const auto& record_data { *selected.value().data().value< const Record* >() };

	for ( const auto& version : record_data.m_versions )
	{
		if ( selected_text == version.m_version )
		{
			QString launch_str { QString::fromStdString( ( version.m_game_path / version.m_exec_path ).string() ) };

			//Check if we should fill in the launch config replacements
			if ( version.m_use_custom_launch_config )
			{
				//todo
				launch_str = version.m_custom_launch_str;
			}

			if ( !version.m_ignore_global_launch_config )
			{
				KeyReplacer replacer;
				replacer.registerKey( "{command}", std::move( launch_str ) );
				replacer.registerKey( "{version}", version.m_version );
				replacer.registerKey( "{title}", record_data.m_title );
				replacer.registerKey( "{creator}", record_data.m_creator );

				launch_str = getSettings< QString >( "execution/global_command", "{command}" );

				replacer.replaceKeys( launch_str );
			}

			spdlog::info( "Launching {}", launch_str.toStdString() );

			executeProc( launch_str.toStdString() );
			return;
		}
	}
}

void SelectedViewWidget::keyPressEvent( QKeyEvent* event )
{
	//Hide on escape
	if ( event->key() == Qt::Key_Escape )
	{
		event->accept();

		emit hiding();
		selected = std::nullopt;
		this->hide();
	}
	QWidget::keyPressEvent( event );
}
