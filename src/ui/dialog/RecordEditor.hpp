//
// Created by kj16609 on 1/13/23.
//

#ifndef HYDRUS95_RECORDEDITOR_HPP
#define HYDRUS95_RECORDEDITOR_HPP

#include <QDialog>
#include "backend/records/RecordBase.hpp"
#include "backend/records/Record.hpp"


QT_BEGIN_NAMESPACE
namespace Ui
{
	class RecordEditor;
}
QT_END_NAMESPACE

class RecordEditor : public QDialog
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE(RecordEditor)

	Record m_record;

	public:
	explicit RecordEditor(Record record, QWidget* parent = nullptr );
	~RecordEditor() override;

	private:
	Ui::RecordEditor* ui;

	private slots:
	void on_recordType_currentIndexChanged();
	void on_selectBanner_clicked( );
	void on_selectExec_clicked( );
	void on_apply_clicked();
	void on_cancel_clicked();
};


#endif	//HYDRUS95_RECORDEDITOR_HPP
