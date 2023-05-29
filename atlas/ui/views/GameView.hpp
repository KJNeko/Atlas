#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>

#include "core/database/GameMetadata.hpp"
#include "core/database/record/Record.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class GameView;
}

QT_END_NAMESPACE

class GameView final : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( GameView )

	std::optional< Record > m_record { std::nullopt };

	std::size_t selected_version_idx { 0 };

	GameMetadata selectedVersion();

	void reloadRecord();

  public:

	explicit GameView( QWidget* parent = nullptr );
	~GameView();

  public slots:
	void setRecord( const Record record );
	void clearRecord();
	void paintEvent( QPaintEvent* event ) override;

	//Button handling
	void on_btnPlay_pressed();
	void on_tbSelectVersion_pressed();
	void on_btnManageRecord_pressed();

  private:

	Ui::GameView* ui;
	void resizeEvent( QResizeEvent* event ) override;
};

#endif // GAMEVIEW_H
