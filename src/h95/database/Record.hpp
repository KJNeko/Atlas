//
// Created by kj16609 on 1/13/23.
//

#ifndef HYDRUS95_RECORD_HPP
#define HYDRUS95_RECORD_HPP

#include "h95/Types.hpp"
#include "GameMetadata.hpp"

struct Record
{
	RecordID m_id;
	GameMetadata m_metadata;
	QString m_title;
	QString m_creator;
	QString m_version;
	QString m_engine;

	std::filesystem::path m_banner;
	std::vector< std::filesystem::path > m_previews;

	private:
	Record() = delete;
	Record(
		const RecordID id,
		const QString title,
		const QString creator,
		const QString version,
		const QString engine,
		const GameMetadata& metadata,
		const std::filesystem::path& banner,
		const std::vector< std::filesystem::path >& previews ) :
	  m_id( id ),
	  m_metadata( metadata ),
	  m_title( title ),
	  m_creator( creator ),
	  m_version( version ),
	  m_engine(engine),
	  m_banner( banner ),
	  m_previews( previews )
	{
	}

	public:
	static Record select( const RecordID id );
	static Record create(
		const QString& title,
		const QString& creator,
		const QString& version,
		const QString& engine,
		const GameMetadata& metadata,
		const std::filesystem::path& banner,
		const std::vector< std::filesystem::path >& previews );


#ifndef NDEBUG
	bool operator==( const Record& other ) const = default;
#else
	bool operator==(const Record& other) const;
#endif
};

#endif	//HYDRUS95_RECORD_HPP
