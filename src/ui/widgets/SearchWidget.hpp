//
// Created by kj16609 on 3/4/23.
//

#ifndef HYDRUS95_SEARCHWIDGET_HPP
#define HYDRUS95_SEARCHWIDGET_HPP

#include <QWidget>

#include "h95/database/Record.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class SearchWidget;
}
QT_END_NAMESPACE

class SearchWidget final : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE(SearchWidget)

	public:
	explicit SearchWidget( QWidget* parent = nullptr );
	~SearchWidget() override;

	signals:
	void searchComplete(std::vector<Record> records);

	public slots:
	void search();

	private:
	Ui::SearchWidget* ui;
};

#endif	//HYDRUS95_SEARCHWIDGET_HPP
