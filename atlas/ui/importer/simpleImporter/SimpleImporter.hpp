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

  private slots:
	//void onCustomContextMenuRequested( const QPoint& point );
	void dirView_itemSelectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
};

#endif //ATLASGAMEMANAGER_SIMPLEIMPORTER_HPP
