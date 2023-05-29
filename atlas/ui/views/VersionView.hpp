//
// Created by kj16609 on 4/19/23.
//

#ifndef ATLASGAMEMANAGER_VERSIONVIEW_HPP
#define ATLASGAMEMANAGER_VERSIONVIEW_HPP

#include <QWidget>

#include "core/database/GameMetadata.hpp"
#include "core/database/record/Record.hpp"
#include "core/database/record/RecordData.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class VersionView;
}

QT_END_NAMESPACE

class VersionView : public QWidget
{
	Q_DISABLE_COPY_MOVE( VersionView )
	Q_OBJECT

	std::optional< GameMetadata > m_metadata { std::nullopt };

	void reloadData();

  public:

	explicit VersionView( QWidget* parent = nullptr );
	~VersionView() override;

	void setVersion( const std::optional< GameMetadata > metadata );

  public slots:
	void on_btnChangeVersion_pressed();
	void on_btnChangeExec_pressed();

  private:

	Ui::VersionView* ui;
};

#endif //ATLASGAMEMANAGER_VERSIONVIEW_HPP
