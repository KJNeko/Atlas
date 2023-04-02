//
// Created by kj16609 on 3/27/23.
//

#include "BatchImportDelegate.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QPainter>
#include <QString>

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
		case ENGINE:
			[[fallthrough]];
		case SIZE:
			{
				const auto data { index.data().value< QString >() };
				painter->drawText( options.rect, Qt::AlignLeft | Qt::AlignVCenter, data );
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
					constexpr int black_medium_down_pointing_triangle_unicode { 0x23F7 }; //U+23F7 (⏷)
					painter->drawText(
						options.rect,
						Qt::AlignLeft | Qt::AlignVCenter,
						data + ' ' + QChar( black_medium_down_pointing_triangle_unicode ) );
				}
				else
					painter->drawText( options.rect, Qt::AlignLeft | Qt::AlignVCenter, data );

				break;
			}
		case MOVE_FLAG:
			{
				//Draw checkmark if checked
				const auto data { index.data().value< bool >() };
				constexpr int heavy_check_mark_unicode { 0x2714 }; //U+2714 (✔)
				constexpr int heavy_multiplication_x_unicode { 0x2716 }; //U+2716 (✖)
				painter->drawText(
					options.rect,
					Qt::AlignLeft | Qt::AlignVCenter,
					QChar( data ? heavy_check_mark_unicode : heavy_multiplication_x_unicode ) );

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
		const auto file_options {
			index.data( Qt::ItemDataRole::EditRole ).value< std::vector< std::filesystem::path > >()
		};
		constexpr int black_medium_down_pointing_triangle_unicode { 0x23F7 }; //U+23F7 (⏷)

		const QString text_modified { file_options.size() > 1 ?
			                              text + ' ' + QChar( black_medium_down_pointing_triangle_unicode ) :
			                              text };

		return info.size( Qt::TextSingleLine, text_modified );
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
			[[fallthrough]];
		case ENGINE:
			{
				QLineEdit* edit { new QLineEdit( parent ) };
				edit->move( options.rect.topLeft() );
				edit->resize( options.rect.size() );
				return edit;
			}
		case MOVE_FLAG:
			{
				QCheckBox* check { new QCheckBox( parent ) };
				check->setText( "Move after import?" );
				check->move( options.rect.topLeft() );
				check->resize( options.rect.size() );
				return check;
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
		case ENGINE:
			[[fallthrough]];
		case VERSION:
			{
				auto edit { dynamic_cast< QLineEdit* >( editor ) };
				model->setData( index, edit->text() );
				break;
			}
		case MOVE_FLAG:
			{
				auto check { dynamic_cast< QCheckBox* >( editor ) };
				model->setData( index, check->isChecked() );
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
		case ENGINE:
			[[fallthrough]];
		case VERSION:
			{
				auto edit { dynamic_cast< QLineEdit* >( editor ) };
				edit->setText( index.data( Qt::DisplayRole ).value< QString >() );
				break;
			}
		case MOVE_FLAG:
			{
				auto check { dynamic_cast< QCheckBox* >( editor ) };
				check->setChecked( index.data( Qt::DisplayRole ).value< bool >() );
				break;
			}
		default:
			return QAbstractItemDelegate::setEditorData( editor, index );
	}
}
