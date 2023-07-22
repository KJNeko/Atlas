#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>

#include "core/database/record/Game.hpp"
#include "core/database/record/GameData.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class GameWidget;
}

QT_END_NAMESPACE

/**
 * GameWidget allows for the displaying of a set game (set via setRecord). The active version can be
 */
class GameWidget final : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( GameWidget )

	std::optional< Game > m_record { std::nullopt };

	std::size_t selected_version_idx { 0 };

	//! Returns the user select version. Returns nullopt if no version was selected.
	std::optional< Version > selectedVersion();

	//! Reloads the widget with all the record information
	void reloadRecord();

  public:

	explicit GameWidget( QWidget* parent = nullptr );
	~GameWidget();

  public slots:
	void setRecord( const Game record );
	void clearRecord();
	void paintEvent( QPaintEvent* event ) override;

	//Button handling
	void on_btnPlay_pressed();
	void on_tbSelectVersion_pressed();
	void on_btnManageRecord_pressed();
	void on_copyRecordToClip_pressed();

  private:

	Ui::GameWidget* ui;
	void resizeEvent( QResizeEvent* event ) override;
};

#endif // GAMEVIEW_H