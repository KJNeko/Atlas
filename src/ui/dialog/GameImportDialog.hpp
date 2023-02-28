//
// Created by kj16609 on 1/17/23.
//

#ifndef HYDRUS95_GAMEIMPORTDIALOG_HPP
#define HYDRUS95_GAMEIMPORTDIALOG_HPP

#include <QDialog>

#include <h95/PathManager.hpp>

QT_BEGIN_NAMESPACE
namespace Ui
{
	class GameImportDialog;
}
QT_END_NAMESPACE

class GameImportDialog final : public QDialog
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE( GameImportDialog )

	PathManager path_manager {};
	bool good_import { false };

	public:
	explicit GameImportDialog( QWidget* parent = nullptr );
	explicit GameImportDialog( const QUrl& url, QWidget* parent = nullptr );
	~GameImportDialog() override;

	private:
	Ui::GameImportDialog* ui;

	void verifySettings();
	void parseInfo();

	public:
	void setInformation( const QString& creator, const QString& title, const QString& engine );

	private:
	signals:
	void importComplete();

	private slots:
	void on_cancelButton_pressed();
	void on_importButton_pressed();

	void on_selectPath_pressed();
	void on_selectExec_pressed();
	void on_selectBanner_pressed();
	void on_selectPreviews_pressed();

	void on_title_textChanged( const QString text );
	void on_creator_textChanged( const QString text );
	void on_version_textChanged( const QString text );
	void on_engine_textChanged( const QString text );
	void on_folderPath_textChanged( const QString text );
	void on_execPath_textChanged( const QString text );
	void on_dest_textChanged( const QString text );
};


#endif	//HYDRUS95_GAMEIMPORTDIALOG_HPP
