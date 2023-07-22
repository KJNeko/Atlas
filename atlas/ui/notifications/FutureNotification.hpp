//
// Created by kj16609 on 7/22/23.
//

#ifndef ATLASGAMEMANAGER_FUTURENOTIFICATION_HPP
#define ATLASGAMEMANAGER_FUTURENOTIFICATION_HPP

#include <QFuture>

#include "Notification.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class FutureNotification;
}

QT_END_NAMESPACE

class FutureNotification final : public Notification
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( FutureNotification )

  public:

	explicit FutureNotification( QWidget* parent = nullptr );
	~FutureNotification() override;

  private:

	Ui::FutureNotification* ui;
};

#endif //ATLASGAMEMANAGER_FUTURENOTIFICATION_HPP
