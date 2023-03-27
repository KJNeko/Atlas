//
// Created by kj16609 on 3/26/23.
//

#include "BatchImportModel.hpp"

#include <QLineEdit>
#include <QPainter>

#include "h95/logging.hpp"

int BatchImportModel::columnCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return 7;
}

int BatchImportModel::rowCount( [[maybe_unused]] const QModelIndex& parent ) const
{
	return static_cast< int >( m_data.size() );
}

QString temp( const QString temp )
{
	return "  " + temp;
}

QVariant BatchImportModel::data( const QModelIndex& index, int role ) const
{
	const auto& item { m_data.at( static_cast< std::size_t >( index.row() ) ) };
	if ( role == Qt::DisplayRole )
	{
		switch ( index.column() )
		{
			case FOLDER_PATH:
				return temp( QString::fromStdString( item.path.string() ) );
			case TITLE:
				return temp( item.title );
			case CREATOR:
				return temp( item.creator );
			case VERSION:
				return temp( item.version );
			case SIZE:
				return temp( item.size );
			case EXECUTABLES:
				return temp( QString::fromStdString( item.executable.string() ) );
			case MOVE_FLAG:
				return temp( "To Move?" );
			default:
				return QString( "wtf?" );
		}
	}
	else if ( role == Qt::CheckStateRole && index.column() == MOVE_FLAG )
		return static_cast< int >( item.move_after_import ? Qt::Checked : Qt::Unchecked );
	else if ( role == Qt::EditRole && index.column() == EXECUTABLES )
		return QVariant::fromStdVariant( std::variant< std::vector< std::filesystem::path > >( item.executables ) );
	else
		return {};
}

void BatchImportModel::addGame( GameImportData data )
{
	beginInsertRows( {}, static_cast< int >( m_data.size() ), static_cast< int >( m_data.size() ) );
	m_data.push_back( std::move( data ) );
	endInsertRows();
}

QVariant BatchImportModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
	{
		switch ( section )
		{
			case FOLDER_PATH:
				return QString( "Folder" );
			case TITLE:
				return QString( "Title" );
			case CREATOR:
				return QString( "Creator" );
			case VERSION:
				return QString( "Version" );
			case SIZE:
				return QString( "Size" );
			case EXECUTABLES:
				return QString( "Executable" );
			case MOVE_FLAG:
				return QString( "Should Move" );
			default:
				return QString( "Oh fuck?" );
		}
	}
	else
		return QAbstractItemModel::headerData( section, orientation, role );
}

Qt::ItemFlags BatchImportModel::flags( const QModelIndex& index ) const
{
	switch ( index.column() )
	{
		case TITLE:
			[[fallthrough]];
		case CREATOR:
			[[fallthrough]];
		case VERSION:
			[[fallthrough]];
		case EXECUTABLES:
			[[fallthrough]];
		case MOVE_FLAG:
			return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		case SIZE:
			[[fallthrough]];
		case FOLDER_PATH:
			[[fallthrough]];
		default:
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	}
	return QAbstractItemModel::flags( index );
}

bool BatchImportModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
	spdlog::info( "Data set" );

	switch ( index.column() )
	{
		case EXECUTABLES:
			{
				m_data.at( static_cast< std::size_t >( index.row() ) ).executable =
					value.value< QString >().toStdString();
				emit dataChanged( index, index );
				return true;
			}
		case TITLE:
			{
				m_data.at( static_cast< std::size_t >( index.row() ) ).title = value.value< QString >();
				emit dataChanged( index, index );
				return true;
			}
		case CREATOR:
			{
				m_data.at( static_cast< std::size_t >( index.row() ) ).creator = value.value< QString >();
				emit dataChanged( index, index );
				return true;
			}
		case VERSION:
			{
				m_data.at( static_cast< std::size_t >( index.row() ) ).version = value.value< QString >();
				emit dataChanged( index, index );
				return true;
			}

		default:
			return QAbstractItemModel::setData( index, value, role );
	}
}

void BatchImportModel::clearData()
{
	beginResetModel();
	m_data.clear();
	endResetModel();
}

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
			[[fallthrough]];
			//print size
		case EXECUTABLES: //print executables
			{
				const auto data { index.data().value< QString >() };
				painter->drawText( options.rect, data );
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
					box->move( options.rect.topLeft() );
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
				edit->resize(options.rect.size());
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
