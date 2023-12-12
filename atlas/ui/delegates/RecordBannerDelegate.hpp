//
// Created by kj16609 on 3/29/23.
//

#pragma once
#ifndef ATLAS_RECORDBANNERDELEGATE_HPP
#define ATLAS_RECORDBANNERDELEGATE_HPP

#include <QAbstractItemDelegate>
#include <QtCore/QModelIndex>

#include "core/config/config.hpp"
#include "core/database/record/game/Game.hpp"
#include "core/utils/QImageBlur.hpp"
class RecordListModel;

class RecordBannerDelegate final : public QAbstractItemDelegate
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( RecordBannerDelegate )

  public:

	QSize m_grid_size;
	SCALE_TYPE m_scale_type;
	int m_strip_height;
	int m_overlay_opacity;
	bool m_enable_top_overlay;
	int m_overlay_layout;
	bool m_enable_bottom_overlay;
	int m_feather_radius;
	int m_blur_radius;
	BLUR_TYPE m_blur_type;
	bool m_enable_capsule_border;
	int m_font_size;
	QString m_font_family;
	/*LOCATION m_title_location;
	LOCATION m_engine_location;
	LOCATION m_version_location;
	LOCATION m_creator_location;*/
	//title
	bool m_title_enable;
	int m_title_x;
	int m_title_y;
	QColor m_title_bcolor;
	//engine
	bool m_engine_enable;
	int m_engine_x;
	int m_engine_y;
	bool m_enable_engine_color;
	//version
	bool m_version_enable;
	int m_version_x;
	int m_version_y;
	QColor m_version_bcolor;
	//creator
	bool m_creator_enable;
	int m_creator_x;
	int m_creator_y;
	QColor m_creator_bcolor;

	int m_grid_spacing;
	QSize m_banner_size;
	int m_window_height;
	int m_window_width;
	bool m_center_widgets;
	RecordListModel* m_model { nullptr };

	RecordBannerDelegate( RecordListModel* model, QWidget* parent = nullptr );

  signals:
	void sizeHintChanged( const QModelIndex& index ) const;

  private:

	void paint( QPainter* painter, const QStyleOptionViewItem& item, const QModelIndex& index ) const override;
	QSize calculateSize( const int w_width, const int b_width, const int b_height, const int spacing );
	QSize sizeHint( const QStyleOptionViewItem& item, const QModelIndex& index ) const override;
	void drawText( QPainter* painter, const int x, const int y, const QRect rect, const QString& str ) const;

  public slots:
	void reloadConfig();
};

#endif //ATLAS_RECORDBANNERDELEGATE_HPP
