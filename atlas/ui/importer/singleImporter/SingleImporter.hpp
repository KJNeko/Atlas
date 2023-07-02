//
// Created by kj16609 on 5/26/23.
//

#ifndef ATLASGAMEMANAGER_SINGLEIMPORTER_HPP
#define ATLASGAMEMANAGER_SINGLEIMPORTER_HPP

#include <QDialog>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class SingleImporter;
}

QT_END_NAMESPACE

class SingleImporter final : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( SingleImporter )

  public:

	explicit SingleImporter( QWidget* parent = nullptr );
	~SingleImporter() override;

  private:

	Ui::SingleImporter* ui;

	void triggerImport();

	// Auto populates fields after directory has been selected
	void fillIn();

	void verifyDataEntry();

	void resizeEvent( QResizeEvent* event ) override;

  private slots:
	void on_leTitle_textChanged( const QString& text );
	void on_leCreator_textChanged( const QString& text );
	void on_leEngine_textChanged( const QString& text );
	void on_leVersion_textChanged( const QString& text );

	//File selection
	void on_leRootPath_textChanged( const QString& text );
	void on_leExecutable_textChanged( const QString& text );
	void on_btnSelectRoot_pressed();
	void on_btnSelectExec_pressed();

	void on_btnNext_clicked();
	void on_btnBack_clicked();
	void on_btnCancel_clicked();

	void on_cbUseRegex_stateChanged( int state );
	void on_leRegex_textChanged( const QString& text );

	void on_stackedWidget_currentChanged( int index );

	void on_btnSelectNormal_clicked();
	void on_btnSelectWide_clicked();
	void on_btnSelectCover_clicked();
	void on_btnSelectLogo_clicked();

	void on_leBannerNormal_textChanged( const QString& text );
	void on_leBannerWide_textChanged( const QString& text );
	void on_leBannerCover_textChanged( const QString& text );
	void on_leBannerLogo_textChanged( const QString& text );
	void on_bannerTabWidget_currentChanged( int index );
};

#endif //ATLASGAMEMANAGER_SINGLEIMPORTER_HPP
