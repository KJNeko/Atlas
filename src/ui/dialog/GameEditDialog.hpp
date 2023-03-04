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

	std::optional<Record> m_record {std::nullopt};

	public:
	explicit GameEditDialog( const RecordID game_id, QWidget* parent = nullptr );
	~GameEditDialog() override;

	private slots:
	void on_applyChanges_pressed();
	void on_cancelChanges_pressed();
	void on_addVersion_pressed();
	void on_removeVersion_pressed();

	void resizeEvent(QResizeEvent* event) override;

	private:
	Ui::GameEditDialog* ui;
};


#endif	//HYDRUS95_GAMEEDITDIALOG_HPP
