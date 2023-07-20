//
// Created by kj16609 on 4/19/23.
//

#ifndef ATLASGAMEMANAGER_VERSIONVIEW_HPP
#define ATLASGAMEMANAGER_VERSIONVIEW_HPP

#include <QWidget>

#include "core/database/Version.hpp"
#include "core/database/record/Game.hpp"
#include "core/database/record/GameData.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class VersionView;
}

QT_END_NAMESPACE

class VersionView final : public QWidget
{
	Q_DISABLE_COPY_MOVE( VersionView )
	Q_OBJECT

	std::optional< Version > m_metadata { std::nullopt };

	void reloadData();

  public:

	explicit VersionView( QWidget* parent = nullptr );
	~VersionView() override;

	void setVersion( const std::optional< Version > metadata );

  public slots:
	void on_btnChangeVersion_pressed();
	void on_btnChangeExec_pressed();

  private:

	Ui::VersionView* ui;
};

#endif //ATLASGAMEMANAGER_VERSIONVIEW_HPP
