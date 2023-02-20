//
// Created by kj16609 on 1/18/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SelectedViewWidget.h" resolved

#include <tracy/Tracy.hpp>
#include "SelectedViewWidget.hpp"
#include "ui_SelectedViewWidget.h"
#include "h95/config.hpp"

#include <QResizeEvent>

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

		const auto record_data { record.data().value< const Record* >() };

		selected = record;

		if(!selected.has_value())
		{
			spdlog::warn("Somehow selected didn't have a value!");
			return;
		}

		ui->banner->setPixmap(
			record_data->getBanner( ui->bannerFrame->size().width() - 20, ui->bannerFrame->minimumHeight() - 20 ) );

		// If there is a creator do {title} by {creator} else just do {title}
		if ( record_data->m_creator.isEmpty() )
			ui->title->setText( record_data->m_title );
		else
			ui->title->setText( QString( "%1 by %2" ).arg( record_data->m_title, record_data->m_creator ) );

		ui->versionSelection->clear();

		QStringList versions;

		//Populate version selection
		for(const auto& version : record_data->m_versions)
		{
			versions.emplace_back(version.version);
		}

		ui->versionSelection->addItems(versions);
	}
	else
		this->hide();
}

void SelectedViewWidget::resizeEvent( QResizeEvent* event )
{
	ZoneScoped;
	if ( selected.has_value())
	{
		const auto record_data {selected.value().data().value<const Record*>()};

		ui->banner->setPixmap(
			record_data->getBanner( ui->bannerFrame->size().width() - 20, ui->bannerFrame->minimumHeight() - 20 ) );
	}
	QWidget::resizeEvent( event );
}

void SelectedViewWidget::on_closeButton_pressed()
{
	emit hiding();
	this->hide();
}

void SelectedViewWidget::keyPressEvent( QKeyEvent* event )
{
	//Hide on escape
	if ( event->key() == Qt::Key_Escape )
	{
		event->accept();

		emit hiding();
		this->hide();
	}
	QWidget::keyPressEvent( event );
}
