#ifndef ABOUTATLAS_HPP
#define ABOUTATLAS_HPP

#include <QDialog>

namespace Ui
{
	class AboutAtlas;
}

class AboutAtlas final : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( AboutAtlas )

  public:

	explicit AboutAtlas( QWidget* parent = nullptr );

	~AboutAtlas();

  private slots:
	void on_btnGithub_pressed();
	void on_btnDiscord_pressed();
	void on_btnClose_pressed();

  private:

	Ui::AboutAtlas* ui;
};

#endif // ABOUTATLAS_HPP
