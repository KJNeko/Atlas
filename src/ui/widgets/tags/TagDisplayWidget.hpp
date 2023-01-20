//
// Created by kj16609 on 1/18/23.
//

#ifndef HYDRUS95_TAGDISPLAYWIDGET_HPP
#define HYDRUS95_TAGDISPLAYWIDGET_HPP

#include <QWidget>
#include "h95/database/Tag.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class TagDisplayWidget;
}
QT_END_NAMESPACE

class TagDisplayWidget : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( TagDisplayWidget )

	QString active_text {};
	std::vector< Tag > m_tags {};

	public:
	explicit TagDisplayWidget( QWidget* parent = nullptr );
	~TagDisplayWidget() override;

	public slots:
	void tagsChanged( const std::vector< Tag >& tags ) { m_tags = tags; }

	signals:
	//! Emitted on T_EMIT
	void selected( const Tag& tag );

	private:
	Ui::TagDisplayWidget* ui;

	void keyPressEvent( QKeyEvent* event ) override;
};


#endif	//HYDRUS95_TAGDISPLAYWIDGET_HPP
