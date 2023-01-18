//
// Created by kj16609 on 1/13/23.
//

#ifndef HYDRUS95_RECORD_HPP
#define HYDRUS95_RECORD_HPP

#include "Types.hpp"
#include "Metadata.hpp"
#include <QVector>


struct Record
{
	RecordID m_id { 0 };
	RecordType m_type { INVALID_RECORD };
	Metadata m_metadata { EmptyMetadata() };

	std::filesystem::path m_banner { ":/banner/invalid_banner.png" };
	std::vector< std::filesystem::path > m_previews {};

	private:
	Record() = delete;
	Record(
		const RecordID id,
		const RecordType type,
		const Metadata& metadata,
		const std::filesystem::path& banner = ":/banner/invalid_banner.png",
		const std::vector< std::filesystem::path >& previews = {} ) :
	  m_id( id ),
	  m_type( type ),
	  m_metadata( metadata ),
	  m_banner( banner ),
	  m_previews( previews )
	{
	}

	public:
	static QString tableQuery();
	void bindTo( QSqlQuery& query ) const;

	static Record select( const RecordID id );
	static Record create(
		const RecordType type,
		const Metadata& metadata,
		const std::filesystem::path& path = ":/banner_invalid.png",
		const std::vector< std::filesystem::path >& previews = {} );

	bool operator==( const Record& other ) const;
};

#endif	//HYDRUS95_RECORD_HPP
