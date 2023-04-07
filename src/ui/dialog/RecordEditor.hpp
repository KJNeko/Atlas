//
// Created by kj16609 on 4/4/23.
//

#ifndef HYDRUS95_RECORDEDITOR_HPP
#define HYDRUS95_RECORDEDITOR_HPP

#include <QDialog>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class RecordEditor;
}

QT_END_NAMESPACE

class RecordEditor : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE(RecordEditor)

  public:

	explicit RecordEditor( QWidget *parent = nullptr );
	~RecordEditor() override;

  private:

	Ui::RecordEditor *ui;
};

#endif //HYDRUS95_RECORDEDITOR_HPP
