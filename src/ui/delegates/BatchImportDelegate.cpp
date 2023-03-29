//
// Created by kj16609 on 3/27/23.
//

#include "BatchImportDelegate.hpp"

#include <QLineEdit>
#include <QPainter>

#include "ui/models/BatchImportModel.hpp"

void BatchImportDelegate::paint( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex& index )
	const
{
	painter->save();

	switch ( index.column() )
	{
		case FOLDER_PATH:
			[[fallthrough]]; //print path
		case TITLE:
			[[fallthrough]]; //print title
		case CREATOR:
			[[fallthrough]]; //print creator
		case VERSION:
			[[fallthrough]]; // print version
		case SIZE:
			{
				const auto data { index.data().value< QString >() };
				painter->drawText( options.rect, Qt::AlignLeft | Qt::AlignVCenter | Qt::ElideRight, data );
				break;
			}
			//print size
		case EXECUTABLES: //print executables
			{
				const auto data { index.data().value< QString >() };
				const auto file_options {
					index.data( Qt::ItemDataRole::EditRole ).value< std::vector< std::filesystem::path > >()
				};
				if ( file_options.size() > 1 )
				{
					painter->drawText( options.rect, Qt::AlignLeft | Qt::AlignVCenter | Qt::ElideRight, data );
					painter->drawText(
						options.rect,
						Qt::AlignRight | Qt::AlignVCenter,
						QString( "%1 options" ).arg( file_options.size() ) );
				}
				else
					painter->drawText( options.rect, Qt::AlignLeft | Qt::AlignVCenter | Qt::ElideRight, data );

				break;
			}
		case MOVE_FLAG:
			{
				painter->drawText( options.rect, "Move after import" );
				break;
			} // print flag
		default:
			break;
	}

	painter->restore();
}

QSize BatchImportDelegate::
	sizeHint( [[maybe_unused]] const QStyleOptionViewItem& item, [[maybe_unused]] const QModelIndex& index ) const
{
	const auto info { item.fontMetrics };
	const auto text { index.data().value< QString >() };

	if ( index.column() == EXECUTABLES )
	{
		const auto size { info.size( Qt::TextSingleLine, text ) };
		return { size.width() + 50, size.height() };
	}
	else
		return info.size( Qt::TextSingleLine, text );
}

QWidget* BatchImportDelegate::
	createEditor( QWidget* parent, const QStyleOptionViewItem& options, const QModelIndex& index ) const
{
	switch ( index.column() )
	{
		case EXECUTABLES:
			{
				const auto data {
					index.data( Qt::ItemDataRole::EditRole ).value< std::vector< std::filesystem::path > >()
				};

				if ( data.size() > 1 )
				{
					QComboBox* box { new QComboBox( parent ) };

					box->show();

					const QPoint point { options.rect.topLeft().x(),
						                 options.rect.center().y() - ( box->height() / 2 ) };

					box->move( point );

					return box;
				}
				break;
			}

		case TITLE:
			[[fallthrough]];
		case CREATOR:
			[[fallthrough]];
		case VERSION:
			{
				QLineEdit* edit { new QLineEdit( parent ) };
				edit->move( options.rect.topLeft() );
				edit->resize( options.rect.size() );
				return edit;
			}
		default:
			return QAbstractItemDelegate::createEditor( parent, options, index );
	}

	return QAbstractItemDelegate::createEditor( parent, options, index );
}

void BatchImportDelegate::setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const
{
	switch ( index.column() )
	{
		case EXECUTABLES:
			{
				auto box { dynamic_cast< QComboBox* >( editor ) };

				model->setData( index, box->currentText() );

				return;
			}
		case TITLE:
			[[fallthrough]];
		case CREATOR:
			[[fallthrough]];
		case VERSION:
			{
				auto edit { dynamic_cast< QLineEdit* >( editor ) };
				model->setData( index, edit->text() );
				break;
			}
		default:
			return QAbstractItemDelegate::setModelData( editor, model, index );
	}
}

void BatchImportDelegate::setEditorData( QWidget* editor, const QModelIndex& index ) const
{
	switch ( index.column() )
	{
		case EXECUTABLES:
			{
				auto box { dynamic_cast< QComboBox* >( editor ) };

				const auto data {
					index.data( Qt::ItemDataRole::EditRole ).value< std::vector< std::filesystem::path > >()
				};
				QStringList executables;
				for ( const auto& path : data ) executables << QString::fromStdString( path.string() );

				box->addItems( executables );
				box->adjustSize();
				return;
			}
		case TITLE:
			[[fallthrough]];
		case CREATOR:
			[[fallthrough]];
		case VERSION:
			{
				auto edit { dynamic_cast< QLineEdit* >( editor ) };
				edit->setText( index.data( Qt::DisplayRole ).value< QString >() );
				break;
			}
		default:
			return QAbstractItemDelegate::setEditorData( editor, index );
	}
}
