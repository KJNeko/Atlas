//
// Created by kj16609 on 1/13/23.
//

#ifndef HYDRUS95_IMPORTDIALOG_HPP
#define HYDRUS95_IMPORTDIALOG_HPP

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui
{
	class ImportDialog;
}
QT_END_NAMESPACE

class ImportDialog : public QDialog
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE( ImportDialog )

	public:
	explicit ImportDialog( QWidget* parent = nullptr );
	~ImportDialog() override;

	private:
	Ui::ImportDialog* ui;
};


#endif	//HYDRUS95_IMPORTDIALOG_HPP
