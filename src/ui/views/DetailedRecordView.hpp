//
// Created by kj16609 on 3/29/23.
//

#ifndef HYDRUS95_DETAILEDRECORDVIEW_HPP
#define HYDRUS95_DETAILEDRECORDVIEW_HPP

#include <QWidget>

#include "h95/database/Record.hpp"

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

	GameMetadata selectedVersion() const;

	void reloadRecord();

  public:

	explicit DetailedRecordView( QWidget *parent = nullptr );
	~DetailedRecordView() override;

  public slots:
	void setRecord( const Record record );
	void clearRecord();
	void paintEvent( QPaintEvent *event ) override;

	//Button handling
	void on_btnPlay_pressed();
	void on_tbSelectVersion_pressed();


  private:
	Ui::DetailedRecordView *ui;
};

#endif //HYDRUS95_DETAILEDRECORDVIEW_HPP
