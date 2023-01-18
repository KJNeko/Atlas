//
// Created by kj16609 on 1/15/23.
//

#include "RecordViewDelegate.hpp"
#include "RecordViewModel.hpp"

#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QMenu>

void RecordViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	painter->save();

	const auto* record {index.data().value<Record*>()};

	painter->drawText(option.rect, "Testing.");
	painter->drawText(option.rect, "\n" + QString::number(record->m_id));

	painter->restore();
}


QSize RecordViewDelegate::sizeHint([[maybe_unused]] const QStyleOptionViewItem& option, [[maybe_unused]] const QModelIndex& index) const
{
	return {300,200};
}


bool RecordViewDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, [[maybe_unused]] const QStyleOptionViewItem& option, const QModelIndex& index)
{
	if(event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent* m_event {dynamic_cast<QMouseEvent*>(event)};

		if(m_event->button() == Qt::RightButton)
		{
			QMenu menu;

			menu.addAction("Manage record", [=](){
				auto* r_model{dynamic_cast<RecordViewModel*>(model)};
				[[maybe_unused]] auto& record {r_model->at(index.row())};
			});

			menu.exec();

			return true;
		}
		else
			return false;

	}
		return false;

}
