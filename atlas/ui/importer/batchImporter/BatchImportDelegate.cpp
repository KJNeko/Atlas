//
// Created by kj16609 on 3/27/23.
//

#include "BatchImportDelegate.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QEvent>
#include <QLineEdit>
#include <QPainter>
#include <QString>

#include "BatchImportModel.hpp"

void BatchImportDelegate::paint( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex& index )
	const
{
	painter->save();

	using enum BatchImportModel::ImportColumns;

	switch ( static_cast< BatchImportModel::ImportColumns >( index.column() ) )
	{
		case VERSION:
			[[fallthrough]];
		case TITLE:
			{
				//Print out icons for title first.
				const auto data { index.data().value< QString >() };
				painter->drawText( options.rect, data );

				//Shift rect over by size of text
				auto top_right { options.rect.bottomRight() };

				//Text info
				const auto text_height { painter->fontMetrics().height() };

				//Drop top_right down to match the height of text.
				const auto diff { options.rect.height() - text_height };

				top_right -= QPoint( 0, options.rect.height() - ( diff / 2 ) );

				const auto icons { index.data( BatchImportModel::TitleIcons ).value< std::vector< QPixmap > >() };
				for ( const auto& ico : icons )
				{
					auto img { ico.scaledToHeight( text_height, Qt::FastTransformation ) };
					top_right -= QPoint( img.width(), 0 );

					const QRect rect { top_right, img.size() };
					painter->drawPixmap( rect, img );
				}

				break;
			}
		case FOLDER_PATH:
			[[fallthrough]]; //print path
		case CREATOR:
			[[fallthrough]]; //print creator

		case ENGINE:
			[[fallthrough]];
		case SIZE:
			{
				const auto data { index.data().value< QString >() };
				painter->drawText( options.rect, data );
				break;
			}
			//print size
		case EXECUTABLE: //print executables
			{
				const auto data { index.data().value< QString >() };

				const auto file_options {
					index.data( BatchImportModel::ExecutablesEditRole ).value< std::vector< std::filesystem::path > >()
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
		default:
			break;
	}

	painter->restore();
}

QSize BatchImportDelegate::
	sizeHint( [[maybe_unused]] const QStyleOptionViewItem& item, [[maybe_unused]] const QModelIndex& index ) const
{
	using enum BatchImportModel::ImportColumns;

	const auto font_info { item.fontMetrics };

	switch ( static_cast< BatchImportModel::ImportColumns >( index.column() ) )
	{
		case EXECUTABLE:
			{
				const auto text { index.data().value< QString >() };
				const auto file_options {
					index.data( BatchImportModel::ExecutablesEditRole ).value< std::vector< std::filesystem::path > >()
				};
				constexpr int black_medium_down_pointing_triangle_unicode { 0x23F7 }; //U+23F7 (⏷)

				const QString text_modified { file_options.size() > 1 ?
					                              text + ' ' + QChar( black_medium_down_pointing_triangle_unicode ) :
					                              text };

				return font_info.size( Qt::TextSingleLine, text_modified ) + QSize( 15, 0 );
			}
		case TITLE:
			{
				const auto icons { index.data( BatchImportModel::TitleIcons ).value< std::vector< QPixmap > >() };
				const auto text_height { font_info.height() };
				int img_accum { 0 };
				for ( const auto& ico : icons )
				{
					auto img { ico.scaledToHeight( text_height, Qt::FastTransformation ) };
					img_accum += img.width();
				}

				const auto text { index.data().value< QString >() };
				return font_info.size( Qt::TextSingleLine, text ) + QSize( 15, 0 ) + QSize( img_accum, 0 );
			}
		case VERSION:
			{
				const auto icons { index.data( BatchImportModel::TitleIcons ).value< std::vector< QPixmap > >() };
				const auto text_height { font_info.height() };
				int img_accum { 0 };
				for ( const auto& ico : icons )
				{
					auto img { ico.scaledToHeight( text_height, Qt::FastTransformation ) };
					img_accum += img.width();
				}

				const auto text { index.data().value< QString >() };
				return font_info.size( Qt::TextSingleLine, text ) + QSize( 15, 0 ) + QSize( img_accum, 0 );
			}
		case CREATOR:
			[[fallthrough]];
		case ENGINE:
			[[fallthrough]];
		case SIZE:
			[[fallthrough]];
		case FOLDER_PATH:
			[[fallthrough]];
		case COLUMNS_MAX:
			[[fallthrough]];
		case IS_CONFLICTING:
			[[fallthrough]];
		default:
			{
				const auto text { index.data().value< QString >() };
				return font_info.size( Qt::TextSingleLine, text ) + QSize( 15, 0 );
			}
	}
}

QWidget* BatchImportDelegate::
	createEditor( QWidget* parent, const QStyleOptionViewItem& options, const QModelIndex& index ) const
{
	using enum BatchImportModel::ImportColumns;

	switch ( static_cast< BatchImportModel::ImportColumns >( index.column() ) )
	{
		case EXECUTABLE:
			{
				const auto data {
					index.data( BatchImportModel::ExecutablesEditRole ).value< std::vector< std::filesystem::path > >()
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
				edit->setObjectName( "QTableItemLineEdit" );
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
	using enum BatchImportModel::ImportColumns;

	switch ( static_cast< BatchImportModel::ImportColumns >( index.column() ) )
	{
		case EXECUTABLE:
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
		default:
			return QAbstractItemDelegate::setModelData( editor, model, index );
	}
}

void BatchImportDelegate::setEditorData( QWidget* editor, const QModelIndex& index ) const
{
	using enum BatchImportModel::ImportColumns;

	switch ( static_cast< BatchImportModel::ImportColumns >( index.column() ) )
	{
		case EXECUTABLE:
			{
				auto box { dynamic_cast< QComboBox* >( editor ) };

				const auto data {
					index.data( BatchImportModel::ExecutablesEditRole ).value< std::vector< std::filesystem::path > >()
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
		default:
			return QAbstractItemDelegate::setEditorData( editor, index );
	}
}
