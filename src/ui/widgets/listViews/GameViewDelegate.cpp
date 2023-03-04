//
// Created by kj16609 on 3/4/23.
//

#include "GameViewDelegate.hpp"

#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QMenu>

#include <ui/dialog/GameEditDialog.hpp>

#include <h95/database/Record.hpp>
#include <h95/config.hpp>

void GameViewDelegate::paint( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex& index ) const
{
	const auto& data { *( index.data().value< const Record* >() ) };

	painter->save();

	const auto banner { data.getBanner(
		getSettings< int >( "main_view/banner_width", 400 ),
		getSettings< int >( "main_view/banner_height", 180 ) ) };

	const auto rect { options.rect.center() - banner.rect().center() };

	painter->drawPixmap( rect, std::move( banner ) );

	painter->drawRect( options.rect );
	painter->drawText( options.rect, Qt::AlignHCenter | Qt::AlignTop, data.m_title );

	painter->restore();
}

QSize GameViewDelegate::sizeHint(
	[[maybe_unused]] const QStyleOptionViewItem& option,
	[[maybe_unused]] const QModelIndex& index ) const
{
	return { getSettings< int >( "main_view/item_width", 400 ), getSettings< int >( "main_view/item_height", 200 ) };
}


bool GameViewDelegate::editorEvent(
	QEvent* event,
	[[maybe_unused]] QAbstractItemModel* model,
	[[maybe_unused]] const QStyleOptionViewItem& option,
	[[maybe_unused]] const QModelIndex& index )
{
	if ( event->type() == QEvent::MouseButtonPress )
	{
		QMouseEvent* m_event { dynamic_cast< QMouseEvent* >( event ) };

		if ( m_event->button() == Qt::RightButton )
		{
			QMenu menu;

			const Record* data_ptr { index.data().value< const Record* >() };

			const auto game_id { data_ptr->m_id };

			menu.addAction(
				"Manage record",
				[this, game_id]()
				{
					GameEditDialog* dialog = new GameEditDialog( game_id );
					dialog->exec();
					dialog->deleteLater();
				} );

			menu.exec( m_event->globalPosition().toPoint() );

			return true;
		}
		else
			return false;
	}
	return false;
}

