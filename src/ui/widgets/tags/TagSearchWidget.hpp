//
// Created by kj16609 on 1/18/23.
//

#ifndef HYDRUS95_TAGSEARCHWIDGET_HPP
#define HYDRUS95_TAGSEARCHWIDGET_HPP

#include <QWidget>
#include <backend/Tag.hpp>

QT_BEGIN_NAMESPACE
namespace Ui
{
	class TagSearchWidget;
}
QT_END_NAMESPACE

class TagSearchWidget : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE(TagSearchWidget)

	public:
	explicit TagSearchWidget( QWidget* parent = nullptr );
	~TagSearchWidget() override;

	signals:
	void searchSimilar(const QString& text);
	void addTag(const Tag& tag);
	void tagsChanged(const std::vector<Tag>& tags);

	public slots:
	void similarTagFinished(const std::vector<Tag>& tags);

	private:
	Ui::TagSearchWidget* ui;
};


#endif	//HYDRUS95_TAGSEARCHWIDGET_HPP
