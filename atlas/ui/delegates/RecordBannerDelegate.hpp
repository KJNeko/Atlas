//
// Created by kj16609 on 3/29/23.
//

#pragma once
#ifndef ATLAS_RECORDBANNERDELEGATE_HPP
#define ATLAS_RECORDBANNERDELEGATE_HPP

#include <QAbstractItemDelegate>
#include <QColor>
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
	//overlay
	int m_top_overlay_height;
	int m_bottom_overlay_height;
	int m_overlay_opacity;
	bool m_enable_top_overlay;
	bool m_enable_bottom_overlay;
	int m_overlay_layout;
	QColor m_overlay_color;
	int m_feather_radius;
	int m_blur_radius;
	BLUR_TYPE m_blur_type;
	bool m_enable_capsule_border;
	int m_font_size;
	QString m_font_family;

	//title
	bool m_title_enable;
	int m_title_x;
	int m_title_y;
	int m_title_align;
	QColor m_title_bcolor;
	QColor m_title_fcolor;
	int m_title_fontsize;
	bool m_title_bold;
	bool m_title_italics;
	//engine
	bool m_engine_enable;
	int m_engine_x;
	int m_engine_y;
	int m_engine_align;
	QColor m_engine_bcolor;
	QColor m_engine_fcolor;
	bool m_engine_default_colors;
	bool m_engine_bold;
	bool m_engine_italics;
	//version
	bool m_version_enable;
	int m_version_x;
	int m_version_y;
	int m_version_align;
	QColor m_version_bcolor;
	QColor m_version_fcolor;
	bool m_version_bold;
	bool m_version_italics;
	//creator
	bool m_creator_enable;
	int m_creator_x;
	int m_creator_y;
	int m_creator_align;
	QColor m_creator_bcolor;
	QColor m_creator_fcolor;
	bool m_creator_bold;
	bool m_creator_italics;
	//status
	bool m_status_enable;
	int m_status_x;
	int m_status_y;
	int m_status_align;
	int m_status_link;
	//QColor m_status_bcolor;
	//QColor m_status_fcolor;
	//bool m_status_bold;
	//bool m_status_italics;
	//gametype
	bool m_gametype_enable;
	int m_gametype_x;
	int m_gametype_y;
	int m_gametype_align;
	int m_gametype_link;
	//QColor m_gametype_bcolor;
	//QColor m_gametype_fcolor;
	//bool m_gametype_bold;
	//bool m_gametype_italics;

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
	QColor colorFromString( QString str );
	QRect drawText(
		QPainter* painter,
		const int x,
		const int y,
		const QRect rect,
		const QString& str,
		const int font_size,
		const QString font_family,
		const int padding,
		QColor backgroundColor ) const;
	QColor getEngineColor( QString engine, bool isEnabled ) const;
	QColor getStatusColor( QString status, bool isEnabled ) const;
	QColor getGameTypeColor( QString status, bool isEnabled ) const;

  public slots:
	void reloadConfig();
};

#endif //ATLAS_RECORDBANNERDELEGATE_HPP
