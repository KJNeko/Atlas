//
// Created by kj16609 on 3/1/23.
//

#ifndef HYDRUS95_EDITLABEL_HPP
#define HYDRUS95_EDITLABEL_HPP

#include <QWidget>
#include <QLineEdit>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui
{
	class EditLabel;
}
QT_END_NAMESPACE

class EditLabel : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( EditLabel )

	public:
	explicit EditLabel( QWidget* parent = nullptr );
	~EditLabel() override;

	void setText(QString text);
	QString text() const;

	void setLabel(QString label);

	private:
	Ui::EditLabel* ui;
};

#endif	//HYDRUS95_EDITLABEL_HPP
