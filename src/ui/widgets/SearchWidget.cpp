//
// Created by kj16609 on 3/4/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SearchWidget.h" resolved

#include "SearchWidget.hpp"
#include "ui_SearchWidget.h"

#include <h95/logging.hpp>

SearchWidget::SearchWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::SearchWidget )
{
	ui->setupUi( this );

	connect(ui->searchButton, &QPushButton::pressed, this, &SearchWidget::search);
}

SearchWidget::~SearchWidget()
{
	delete ui;
}

std::vector<Record> getAll()
{
	Transaction transaction;

	std::vector<Record> records;

	transaction.ref() << "SELECT record_id FROM records ORDER BY record_id;" >> [&](const RecordID id){
		records.emplace_back(Record::select(id, transaction));
	};

	transaction.commit();

	return records;
}

void SearchWidget::search()
{
	emit searchComplete(getAll());
}
