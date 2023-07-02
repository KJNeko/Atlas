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

enum class RecordColumns
{
	Title,
	Creator,
	Engine,
	LastPlayed,
	TotalPlaytime
};

template < RecordColumns col >
struct ColInfo;

template < RecordColumns col >
using RecordColType = ColInfo< col >::Type;

template <>
struct ColInfo< RecordColumns::Title >
{
	using Type = QString;
	static constexpr fgl::string_literal col_name { "title" };
};

template <>
struct ColInfo< RecordColumns::Creator >
{
	using Type = QString;
	static constexpr fgl::string_literal col_name { "creator" };
};

template <>
struct ColInfo< RecordColumns::Engine >
{
	using Type = QString;
	static constexpr fgl::string_literal col_name { "engine" };
};

template <>
struct ColInfo< RecordColumns::LastPlayed >
{
	using Type = std::uint64_t;
	static constexpr fgl::string_literal col_name { "last_played_r" };
};

template <>
struct ColInfo< RecordColumns::TotalPlaytime >
{
	using Type = std::uint64_t;
	static constexpr fgl::string_literal col_name { "total_playtime" };
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

	template < RecordColumns col >
	ColInfo< col >::Type get()
	{
		typename ColInfo< col >::Type val {};
		RapidTransaction()
				<< atlas::database::utility::select_query< ColInfo< col >::col_name, "records", "record_id" >() << m_id
			>> val;
		return val;
	}

	template < RecordColumns... cols >
		requires( sizeof...( cols ) > 1 )
	std::tuple< RecordColType< cols >... > get()
	{
		std::tuple< RecordColType< cols >... > tpl {};
		RapidTransaction()
				<< atlas::database::utility::select_query_t< "records", "record_id", ColInfo< cols >::col_name... >()
				<< m_id
			>> tpl;
		return tpl;
	}

	template < RecordColumns col >
	void set( ColInfo< col >::Type t )
	{
		RapidTransaction()
			<< atlas::database::utility::update_query< ColInfo< col >::col_name, "records", "record_id" >() << t
			<< m_id;
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
