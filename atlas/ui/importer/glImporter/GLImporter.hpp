//
// Created by kj16609 on 6/5/23.
//

#ifndef ATLASGAMEMANAGER_GLIMPORTER_HPP
#define ATLASGAMEMANAGER_GLIMPORTER_HPP

#include <QDialog>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class GLImporter;
}

QT_END_NAMESPACE

class GLImporter : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( GLImporter )

  public:

	explicit GLImporter( QWidget* parent = nullptr );
	~GLImporter() override;

  private:

	Ui::GLImporter* ui;
};

#endif //ATLASGAMEMANAGER_GLIMPORTER_HPP