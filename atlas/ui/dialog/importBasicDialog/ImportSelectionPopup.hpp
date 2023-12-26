//
// Created by kj16609 on 12/6/23.
//

#pragma once

#include <QDialog>
#include <QWidget>

namespace atlas::ui::imports
{
	QT_BEGIN_NAMESPACE

	namespace Ui
	{
		class ImportSelectionPopup;
	}

	QT_END_NAMESPACE

	class ImportSelectionPopup final : public QDialog
	{
		enum UseID
		{
			NONE = -1,
			SINGLE = 0,
			BULK,
			TREE
		};

		Q_OBJECT

		UseID use_id { NONE };

	  public:

		explicit ImportSelectionPopup( QWidget* parent = nullptr );
		~ImportSelectionPopup() override;

		Q_DISABLE_COPY_MOVE( ImportSelectionPopup )

	  private:

		Ui::ImportSelectionPopup* ui;

	  private slots:
		//Stage 1
		void on_btnOneGame_pressed();
		void on_btnMultipleGames_pressed();

		//Stage 2 - Multiple
		void on_btnLaidRandom_pressed();
		void on_btnLaidSorted_pressed();

		//Stage 3 - Select folder
		void on_btnSelectFolder_pressed();

		//Drag to lblDragHere
		void on_lblDragHere_dragEnterEvent( QDragEnterEvent* event );
	};
} // namespace atlas::ui::imports
