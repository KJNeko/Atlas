//
// Created by kj16609 on 3/29/23.
//

#ifndef ATLAS_DETAILEDRECORDVIEW_HPP
#define ATLAS_DETAILEDRECORDVIEW_HPP

#include <QScrollArea>
#include <QWidget>

#include "core/database/Record.hpp"
#include "core/database/RecordData.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class DetailedRecordView;
}

QT_END_NAMESPACE

class DetailedRecordView final : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( DetailedRecordView )

  public:

	explicit DetailedRecordView( QWidget* parent = nullptr );
	~DetailedRecordView() override;

  public slots:
	void setRecord( const Record record );

  private:

	Ui::DetailedRecordView* ui;
};

#endif //ATLAS_DETAILEDRECORDVIEW_HPP
