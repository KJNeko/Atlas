//
// Created by kj16609 on 1/18/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SelectedViewWidget.h" resolved

#include <tracy/Tracy.hpp>
#include <spdlog/spdlog.h>
#include "SelectedViewWidget.hpp"
#include "ui_SelectedViewWidget.h"


SelectedViewWidget::SelectedViewWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::SelectedViewWidget )
{
	ui->setupUi( this );
}

SelectedViewWidget::~SelectedViewWidget()
{
	delete ui;
}

void SelectedViewWidget::recordSelected(const QPersistentModelIndex& record )
{
	ZoneScoped;

	if(record.isValid())
	{
		if(this->isHidden())
			this->show();

		const auto record_data {record.data().value<const Record*>()};

		selected = *record_data;

		auto banner {selected->getBanner(ui->banner->size().width() - 20, 400)};
		ui->banner->setPixmap(std::move(banner));
	}
	else
		this->hide();


	//TODO: Make file selection view
}
