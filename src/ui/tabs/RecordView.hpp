//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_RECORDVIEW_HPP
#define HYDRUS95_RECORDVIEW_HPP

#include <QWidget>
#include "h95/database/Record.hpp"
#include "h95/backend/RecordViewBackend.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class RecordView;
}
QT_END_NAMESPACE

class RecordView : public QWidget
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE(RecordView)

	std::vector<Tag> active_tags;

	public:
	explicit RecordView( QWidget* parent = nullptr );
	~RecordView() override;

	public slots:
	void searchSimilar(QString text);
	void addTag(const Tag& tag);
	void removeTag(const Tag& tag);

	signals:
	void similarTagFinished(const std::vector<Tag>& tags);
	void tagsChanged(const std::vector<Tag>& tags);
	void recordsUpdated(const std::vector<Record>& records);

	private:
	Ui::RecordView* ui;
};


#endif	//HYDRUS95_RECORDVIEW_HPP
