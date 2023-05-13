//
// Created by kj16609 on 3/29/23.
//

#ifndef ATLAS_DETAILEDRECORDVIEW_HPP
#define ATLAS_DETAILEDRECORDVIEW_HPP

#include <QWidget>

#include "atlas/core/database/Record.hpp"
#include "atlas/database/RecordData.hpp"

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

	std::optional< Record > m_record { std::nullopt };

	std::size_t selected_version_idx { 0 };

	GameMetadata selectedVersion();

	void reloadRecord();

  public:

	explicit DetailedRecordView( QWidget* parent = nullptr );
	~DetailedRecordView() override;

  public slots:
	void setRecord( const Record record );
	void clearRecord();
	void paintEvent( QPaintEvent* event ) override;

	//Button handling
	void on_btnPlay_pressed();
	void on_tbSelectVersion_pressed();
	void on_btnManageRecord_pressed();

  private:

	Ui::DetailedRecordView* ui;
};

#endif //ATLAS_DETAILEDRECORDVIEW_HPP
