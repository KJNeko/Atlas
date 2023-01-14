//
// Created by kj16609 on 1/13/23.
//

#ifndef HYDRUS95_FILETAB_HPP
#define HYDRUS95_FILETAB_HPP

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui
{
	class FileTab;
}
QT_END_NAMESPACE

class FileTab : public QWidget
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE(FileTab)

	public:
	explicit FileTab( QWidget* parent = nullptr );
	~FileTab() override;

	//! Adds a blank record to the tab.
	void addBlank();

	private:
	Ui::FileTab* ui;
};


#endif	//HYDRUS95_FILETAB_HPP
