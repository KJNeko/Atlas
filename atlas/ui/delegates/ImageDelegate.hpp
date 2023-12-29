//
// Created by kj16609 on 4/17/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_IMAGEDELEGATE_HPP
#define ATLASGAMEMANAGER_IMAGEDELEGATE_HPP

#include <QAbstractItemDelegate>

#include "ui/models/FilepathModel.hpp"

class ImageDelegate final : public QAbstractItemDelegate
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( ImageDelegate )

	FilepathModel* m_model;

	std::map< QPersistentModelIndex, std::unique_ptr< atlas::images::ImageLoader > > m_image_futures;

  public:

	void resetFutures() { m_image_futures.clear(); }

	bool use_thumbnils { true };

	ImageDelegate( FilepathModel* model, QObject* parent = nullptr ) : QAbstractItemDelegate( parent ), m_model( model )
	{}

	void paint( QPainter* painter, const QStyleOptionViewItem& item, const QModelIndex& index ) const override;
	QSize sizeHint( const QStyleOptionViewItem& item, const QModelIndex& index ) const override;
};

#endif //ATLASGAMEMANAGER_IMAGEDELEGATE_HPP
