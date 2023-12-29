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
	QColor m_top_overlay_color;
	QColor m_bottom_overlay_color;
	int m_feather_radius;
	int m_blur_radius;
	BLUR_TYPE m_blur_type;
	bool m_enable_capsule_border;
	//font
	int m_font_size;
	QString m_font_family;
	bool m_font_bold;
	bool m_font_italic;
	bool m_font_shadow;
	int m_padding;
	int m_corner_radius;

	//title
	bool m_title_enable;
	int m_title_x;
	int m_title_y;
	int m_title_align;
	QColor m_title_bcolor;
	QColor m_title_fcolor;
	int m_title_fontsize;
	bool m_title_default;
	int m_title_link;
	//engine
	bool m_engine_enable;
	int m_engine_x;
	int m_engine_y;
	int m_engine_align;
	QColor m_engine_bcolor;
	QColor m_engine_fcolor;
	bool m_engine_default;
	int m_engine_link;
	//version
	bool m_version_enable;
	int m_version_x;
	int m_version_y;
	int m_version_align;
	QColor m_version_bcolor;
	QColor m_version_fcolor;
	bool m_version_default;
	int m_version_link;
	//creator
	bool m_creator_enable;
	int m_creator_x;
	int m_creator_y;
	int m_creator_align;
	QColor m_creator_bcolor;
	QColor m_creator_fcolor;
	bool m_creator_default;
	int m_creator_link;
	//status
	bool m_status_enable;
	int m_status_x;
	int m_status_y;
	int m_status_align;
	QColor m_status_bcolor;
	QColor m_status_fcolor;
	bool m_status_default;
	int m_status_link;
	//gametype
	bool m_gametype_enable;
	int m_gametype_x;
	int m_gametype_y;
	int m_gametype_align;
	QColor m_gametype_bcolor;
	QColor m_gametype_fcolor;
	bool m_gametype_default;
	int m_gametype_link;
	//rating
	bool m_rating_enable;
	int m_rating_x;
	int m_rating_y;
	int m_rating_align;
	QColor m_rating_bcolor;
	QColor m_rating_fcolor;
	bool m_rating_default;
	int m_rating_link;
	//views
	bool m_views_enable;
	int m_views_x;
	int m_views_y;
	int m_views_align;
	QColor m_views_bcolor;
	QColor m_views_fcolor;
	bool m_views_default;
	int m_views_link;
	//likes
	bool m_likes_enable;
	int m_likes_x;
	int m_likes_y;
	int m_likes_align;
	QColor m_likes_bcolor;
	QColor m_likes_fcolor;
	bool m_likes_default;
	int m_likes_link;
	//favorite
	bool m_favorite_enable;
	int m_favorite_x;
	int m_favorite_y;
	int m_favorite_align;
	QColor m_favorite_bcolor;
	QColor m_favorite_fcolor;
	bool m_favorite_default;
	int m_favorite_link;
	//updateicon
	bool m_updateicon_enable;
	int m_updateicon_x;
	int m_updateicon_y;
	int m_updateicon_align;
	QColor m_updateicon_bcolor;
	QColor m_updateicon_fcolor;
	bool m_updateicon_default;
	int m_updateicon_link;

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
		const QFont::Weight font_weight,
		const QFont::Style font_style,
		const bool text_shadow,
		const int padding,
		const int corner_radius,
		const int Alignment,
		const int link,
		QColor backgroundColor,
		QColor foregroundColor,
		QString pixmap_str ) const;

	void drawIcon(
		QPainter* painter,
		const int x,
		const int y,
		const QRect rect,
		const int size,
		const int align,
		const int link,
		QString pixmap_str ) const;

	QColor getEngineColor( QString engine ) const;
	QColor getStatusColor( QString status ) const;
	QColor getGameTypeColor( QString status ) const;

  public slots:
	void reloadConfig();
};

namespace atlas::banner
{
	struct BannerData
	{
		int x {};
		int y {};
		QRect rect {};
		QString str {};
		int font_size {};
		QString font_family {};
		int padding {};
		int Alignment {};
		QColor backgroundColor {};
	};
} // namespace atlas::banner

#endif //ATLAS_RECORDBANNERDELEGATE_HPP
