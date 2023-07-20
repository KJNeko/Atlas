//
// Created by kj16609 on 5/25/23.
//

#include "GameListDelegate.hpp"

#include <moc_GameListDelegate.cpp>

#include <QPainter>

#include <tracy/Tracy.hpp>

#include "core/database/record/GameData.hpp"
#include "ui/models/RecordListModel.hpp"

void GameListDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	ZoneScoped;
	const Game record { index.data().value< Game >() };
	const QString& title { record->m_title };

	painter->save();

	if ( option.state & QStyle::State_Selected ) painter->fillRect( option.rect, option.palette.highlight() );

	painter->drawText( option.rect, title );

	painter->restore();
}

QSize GameListDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	ZoneScoped;
	//Font height
	QFontMetrics fm { option.font };

	const Game record { index.data().value< Game >() };
	const QString& title { record->m_title };

	return fm.size( Qt::TextSingleLine, title );
}
