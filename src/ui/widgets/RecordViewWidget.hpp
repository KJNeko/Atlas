//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_RECORDVIEWWIDGET_HPP
#define HYDRUS95_RECORDVIEWWIDGET_HPP

#include <QWidget>

#include <backend/Record.hpp>

QT_BEGIN_NAMESPACE
namespace Ui
{
	class RecordViewWidget;
}
QT_END_NAMESPACE

class RecordViewWidget : public QWidget
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE(RecordViewWidget)

	public:
	explicit RecordViewWidget( QWidget* parent = nullptr );
	~RecordViewWidget() override;

	void setViewPtr(const std::vector<Record>* const records);

	private:
	Ui::RecordViewWidget* ui;
};


#endif	//HYDRUS95_RECORDVIEWWIDGET_HPP
