//
// Created by kj16609 on 3/4/23.
//

#ifndef HYDRUS95_GAMETAB_HPP
#define HYDRUS95_GAMETAB_HPP

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui
{
	class GameTab;
}
QT_END_NAMESPACE

class GameTab final : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE(GameTab)

	public:
	explicit GameTab( QWidget* parent = nullptr );
	~GameTab() override;

	void update();

	private:
	Ui::GameTab* ui;
};


#endif	//HYDRUS95_GAMETAB_HPP
