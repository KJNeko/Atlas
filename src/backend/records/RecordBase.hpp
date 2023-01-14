//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_RECORDBASE_HPP
#define HYDRUS95_RECORDBASE_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <exception>
#include <filesystem>

#include <QImage>
#include <QFileInfo>

#include "GameRecord.hpp"

using RecordID = uint32_t;

enum class RecordType
{
	NORMAL_RECORD = 0,
	GAME_RECORD = 1,
	ASSERT_RECORD = 2
};

struct NormalRecord
{
};

class RecordBase
{
	public:
	//! Internal record id
	RecordID record_id { 0 };

	private:

	std::optional< QString > m_name { std::nullopt };
	std::optional< QString > m_desc { std::nullopt };
	std::optional< QString > m_notes { std::nullopt };
	std::optional< std::filesystem::path > m_path { std::nullopt };
	std::optional< std::filesystem::path > m_banner_path { std::nullopt };

	RecordType type { RecordType::NORMAL_RECORD };

	std::variant< NormalRecord, GameRecord > subrecord { NormalRecord() };

	public:
	RecordBase( const RecordID id ) noexcept;

	void setName( const QString& name );
	QString getName() const;

	void setDescription( const QString& desc );
	QString getDesc() const;

	void setNotes( const QString& notes );
	QString getNotes() const;

	void setAsGameRecord( const GameRecord& record );
	void setAsNormalRecord();

	template< typename T > T getSubRecord() const { return std::get< T >( subrecord ); }

	void setPath( const std::filesystem::path& path );
	std::filesystem::path getPath() const;

	void setBanner( const std::filesystem::path& path );
	std::filesystem::path getBannerPath() const;
	inline QImage getBanner() const { return { QImage( QFileInfo( getBannerPath() ).filePath() ) }; }

	RecordType getType() const { return RecordType::NORMAL_RECORD; }

	bool operator==( const RecordBase& other ) { return other.record_id == this->record_id; }
};


struct DuplicateNameRecord : public std::runtime_error
{
	DuplicateNameRecord( const std::string str ) : std::runtime_error( str ) {}
};

#endif	//HYDRUS95_RECORDBASE_HPP
