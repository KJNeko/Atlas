//
// Created by kj16609 on 2/22/23.
//

#ifndef HYDRUS95_GAMEEDITDIALOG_HPP
#define HYDRUS95_GAMEEDITDIALOG_HPP

#include <QDialog>

#include <h95/Types.hpp>
#include <h95/database/Record.hpp>
#include <QItemSelection>


QT_BEGIN_NAMESPACE
namespace Ui
{
	class GameEditDialog;
}
QT_END_NAMESPACE

class GameEditDialog final : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( GameEditDialog )

	RecordID m_id;
	std::optional< Record > m_record { std::nullopt };

	public:
	explicit GameEditDialog( const RecordID game_id, QWidget* parent = nullptr );
	~GameEditDialog() override;

	private:
	Ui::GameEditDialog* ui;

	private slots:
	void on_nameEdit_textChanged( const QString& text );
	void on_creatorEdit_textChanged( const QString& text );
	void imageSelectionChanged( const QItemSelection& selected, const QItemSelection& deselected );

	//Image editing
	void on_replaceBanner_pressed();
	void on_addPreview_pressed();
	void on_removePreview_pressed();

	//Extra buttons
	void on_applyChanges_pressed();
	void on_cancelChanges_pressed();
	void on_applyChanges_2_pressed() { on_applyChanges_pressed(); }
	void on_cancelChanges_2_pressed() { on_cancelChanges_pressed(); }

	//Version stuff
	void on_addVersion_pressed();
	void on_removeVersion_pressed();

	public:
	Record getRecord() const;
	bool recordDeleted() const { return m_record.has_value(); }
};


#endif	//HYDRUS95_GAMEEDITDIALOG_HPP
