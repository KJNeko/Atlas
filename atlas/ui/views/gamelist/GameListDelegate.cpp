//
// Created by kj16609 on 5/25/23.
//

#include "GameListDelegate.hpp"

#include <QPainter>

#include "ui/models/RecordListModel.hpp"

void GameListDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	const Record record { index.data().value< Record >() };
	const QString title { record->title.get() };

	painter->save();

	if ( option.state & QStyle::State_Selected ) painter->fillRect( option.rect, option.palette.highlight() );

	painter->drawText( option.rect, title );

	painter->restore();
}

QSize GameListDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	//Font height
	QFontMetrics fm { option.font };

	const Record record { index.data().value< Record >() };
	const QString title { record->title.get() };

	return fm.size( Qt::TextSingleLine, title );
}
