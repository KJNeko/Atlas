//
// Created by kj16609 on 1/13/23.
//

#ifndef ATLAS_RECORD_HPP
#define ATLAS_RECORD_HPP

#include <QPixmap>

#include "core/Types.hpp"
#include "core/config.hpp"
#include "core/database/Column.hpp"
#include "core/database/Database.hpp"
#include "core/remote/concept.hpp"

struct GameMetadata;
class RecordBanner;
class RecordPreviews;

#define COL RecordDataColumn
#define KEY_TYPE RecordID

DEFINE_COL_BASE( "records", "record_id" )

DEFINE_COL_STRUCT( QString, "title", RecordDataTitle )
DEFINE_COL_STRUCT( QString, "engine", RecordDataEngine )
DEFINE_COL_STRUCT( QString, "creator", RecordDataCreator )
DEFINE_COL_STRUCT( std::uint64_t, "last_played_r", RecordDataLastPlayed )
DEFINE_COL_STRUCT( std::uint32_t, "total_playtime", RecordDataTotalPlaytime )

#undef COL
#undef KEY_TYPE

class F95Data;
class AtlasData;

struct RecordData
{
	RecordData() = delete;
	RecordData( RecordData&& other ) = default;

  private:

	RecordID m_id { 0 };

  public:

	RecordBanner banners();
	RecordPreviews previews();

	RecordID getID() const { return m_id; }

	RecordDataTitle title { m_id };
	RecordDataCreator creator { m_id };
	RecordDataEngine engine { m_id };
	RecordDataLastPlayed last_played { m_id };
	RecordDataTotalPlaytime total_playtime { m_id };

	std::optional< GameMetadata > getVersion( const QString );
	std::optional< GameMetadata > getLatestVersion();
	std::vector< GameMetadata > getVersions();
	QString getDesc() const;
	std::vector< QString > getUserTags() const;
	std::vector< QString > getAllTags() const;

	F95Data f95Data();
	void linkF95Data( const F95ID id );
	AtlasData atlasData();
	void linkAtlasData( const AtlasID id );

	//Setters
	void addVersion(
		QString version,
		std::filesystem::path game_path,
		std::filesystem::path exec_path,
		const uint64_t folder_size,
		bool in_place );
	void removeVersion( const GameMetadata& );
	//void setBannerOverride( const std::filesystem::path&, Transaction = Transaction( Autocommit ) );

	void setDesc( const QString& str );
	void addUserTag( const QString str );
	void removeUserTag( const QString str );

	//! Fetches the RecordData for the given ID
	/**
	 * @param id
	 * @param transaction. Defaults to autocommit on return
	 */
	RecordData( const RecordID id );

	/**
	 * @warning Constructing will create a new record in the database. Pass in Transaction as last parameter in order to not commit on return
	 * @param title
	 * @param creator
	 * @param engine
	 * @param transaction
	 */
	RecordData( QString title, QString creator, QString engine );

	friend struct RecordAlreadyExists;
	friend class Record;
};

//! Checks if a record exists with the given parameters
bool recordExists( const QString& title, const QString& creator, const QString& engine );

//! Returns 0 if there is not record with this data
RecordID recordID( const QString& title, const QString& creator, const QString& engine );

#endif //ATLAS_RECORD_HPP
