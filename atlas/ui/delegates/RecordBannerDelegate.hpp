//
// Created by kj16609 on 3/29/23.
//

#ifndef ATLAS_RECORDBANNERDELEGATE_HPP
#define ATLAS_RECORDBANNERDELEGATE_HPP

#include <QAbstractItemDelegate>

#include "core/config.hpp"
#include "core/database/record/Game.hpp"
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
	bool m_enable_bottom_overlay;
	int m_feather_radius;
	int m_blur_radius;
	BLUR_TYPE m_blur_type;
	bool m_enable_capsule_border;
	int m_font_size;
	QString m_font_family;
	LOCATION m_title_location;
	LOCATION m_engine_location;
	LOCATION m_version_location;
	LOCATION m_creator_location;
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
	QString toCamelCase( const QString& s );
	QSize sizeHint( const QStyleOptionViewItem& item, const QModelIndex& index ) const override;
	void drawText(
		QPainter* painter, const QRect& rect, const int strip_size, const LOCATION location, const QString& str ) const;

  public slots:
	void reloadConfig();
};

#endif //ATLAS_RECORDBANNERDELEGATE_HPP
