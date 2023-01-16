//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_RECORDVIEW_HPP
#define HYDRUS95_RECORDVIEW_HPP

#include <QWidget>
#include <backend/Record.hpp>

QT_BEGIN_NAMESPACE
namespace Ui
{
	class RecordView;
}
QT_END_NAMESPACE

class RecordView : public QWidget
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE(RecordView)

	public:
	explicit RecordView( QWidget* parent = nullptr );
	~RecordView() override;

	private:
	Ui::RecordView* ui;
};


#endif	//HYDRUS95_RECORDVIEW_HPP
