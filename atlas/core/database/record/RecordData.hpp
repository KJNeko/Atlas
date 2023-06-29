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
#include "core/fgl/string_literal.hpp"

struct GameMetadata;
class RecordBanner;
class RecordPreviews;

struct F95Data;
struct AtlasData;

template <>
struct ColType< "title", "records" >
{
	typedef QString Type;
};

template <>
struct ColType< "creator", "records" >
{
	typedef QString Type;
};

template <>
struct ColType< "engine", "records" >
{
	typedef QString Type;
};

template <>
struct ColType< "total_playtime", "records" >
{
	typedef std::uint64_t Type;
};

template <>
struct ColType< "last_played_r", "records" >
{
	typedef std::uint64_t Type;
};

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

	template < fgl::string_literal col_name >
	ColType< col_name, "records" >::Type get()
	{
		typename ColType< col_name, "records" >::Type val;
		RapidTransaction() << atlas::database::utility::select_query< col_name, "records", "record_id" >() << m_id
			>> val;
		return val;
	}

	template < fgl::string_literal col_name >
	void set( ColType< col_name, "records" >::Type t )
	{
		RapidTransaction() << atlas::database::utility::update_query< col_name, "records", "record_id" >() << t << m_id;
	}

	std::optional< GameMetadata > getVersion( const QString );
	std::optional< GameMetadata > getLatestVersion();
	std::vector< GameMetadata > getVersions();
	QString getDesc() const;
	std::vector< QString > getUserTags() const;
	std::vector< QString > getAllTags() const;

	F95Data f95Data();
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
