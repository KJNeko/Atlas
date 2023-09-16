//
// Created by kj16609 on 9/12/23.
//

#ifndef ATLASGAMEMANAGER_IMPORTNOTIFIER_HPP
#define ATLASGAMEMANAGER_IMPORTNOTIFIER_HPP

#include <QObject>

namespace atlas::import
{

	class ImportNotifier final : public QObject
	{
		Q_OBJECT

	  public:

		ImportNotifier( QObject* parent = nullptr );

		virtual ~ImportNotifier() {}

		void notify();
	  signals:
		void notification();
	};

	void notifyImportComplete();

	namespace internal
	{
		ImportNotifier& getNotifier();
	}

} // namespace atlas::import

#endif //ATLASGAMEMANAGER_IMPORTNOTIFIER_HPP
