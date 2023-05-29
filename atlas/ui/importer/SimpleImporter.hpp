//
// Created by kj16609 on 5/24/23.
//

#ifndef ATLASGAMEMANAGER_SIMPLEIMPORTER_HPP
#define ATLASGAMEMANAGER_SIMPLEIMPORTER_HPP

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class SimpleImporter;
}

QT_END_NAMESPACE

class SimpleImporter final : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( SimpleImporter )

  public:

	explicit SimpleImporter( QWidget* parent = nullptr );
	~SimpleImporter() override;

  private:

	Ui::SimpleImporter* ui;
};

#endif //ATLASGAMEMANAGER_SIMPLEIMPORTER_HPP
