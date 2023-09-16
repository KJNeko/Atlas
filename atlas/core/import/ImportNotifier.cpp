//
// Created by kj16609 on 9/12/23.
//

#include "ImportNotifier.hpp"

#include <moc_ImportNotifier.cpp>

namespace atlas::import
{

	namespace internal
	{
		inline static ImportNotifier notif;

		ImportNotifier& getNotifier()
		{
			return notif;
		}
	} // namespace internal

	void notifyImportComplete()
	{
		internal::notif.notify();
	}

	ImportNotifier::ImportNotifier( QObject* parent ) : QObject( parent )
	{}

	void ImportNotifier::notify()
	{
		emit notification();
	}

} // namespace atlas::import
