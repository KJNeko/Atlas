//
// Created by kj16609 on 5/24/23.
//

#ifndef ATLASGAMEMANAGER_SIMPLEIMPORTER_HPP
#define ATLASGAMEMANAGER_SIMPLEIMPORTER_HPP

#include <QDialog>
#include <QItemSelection>
#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class SimpleImporter;
}

QT_END_NAMESPACE

struct Node;

class SimpleImporter final : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( SimpleImporter )

  public:

	explicit SimpleImporter( QWidget* parent = nullptr );
	~SimpleImporter() override;

	void setRoot( const QString path );

  private:

	Ui::SimpleImporter* ui;

	std::vector< QPersistentModelIndex > selected() const;
	void updateSidebar();

	bool no_modification { false };

	void setGameRoot( Node* node );

	void verifyGames();
	Node* root();

  private slots:
	void onCustomContextMenuRequested( const QPoint& point );
	void dirView_itemSelectionChanged( const QItemSelection& selected, const QItemSelection& deselected );

	//Dir page
	void on_cIsGameRoot_toggled( bool checked );
	void on_cIsSupporting_toggled( bool checked );
	void on_cbSupportingSelection_currentIndexChanged( int index );
	void on_leCreator_textChanged( const QString& text );
	void on_leVersion_textChanged( const QString& text );
	void on_leTitle_textChanged( const QString& text );
	void on_leEngine_textChanged( const QString& text );

	//File page
	void on_cIsBanner_toggled( bool checked );
	void on_cIsPreview_toggled( bool checked );
	void on_cbBannerType_currentIndexChanged( int index );
};

#endif //ATLASGAMEMANAGER_SIMPLEIMPORTER_HPP
