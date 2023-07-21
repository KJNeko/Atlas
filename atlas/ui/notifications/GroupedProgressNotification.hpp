//
// Created by kj16609 on 7/21/23.
//

#ifndef ATLASGAMEMANAGER_GROUPEDPROGRESSNOTIFICATION_HPP
#define ATLASGAMEMANAGER_GROUPEDPROGRESSNOTIFICATION_HPP

#include "Notification.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class GroupedProgressNotification;
}

QT_END_NAMESPACE

class GroupedProgressNotification : public Notification
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( GroupedProgressNotification )

  public:

	explicit GroupedProgressNotification( QWidget* parent = nullptr );
	~GroupedProgressNotification() override;

  private:

	Ui::GroupedProgressNotification* ui;
};

#endif //ATLASGAMEMANAGER_GROUPEDPROGRESSNOTIFICATION_HPP
