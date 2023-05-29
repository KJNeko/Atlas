//
// Created by kj16609 on 1/13/23.
//

#ifndef ATLAS_RECORD_HPP
#define ATLAS_RECORD_HPP

#include <QPixmap>

#include "core/Types.hpp"
#include "core/config.hpp"
#include "core/database/Database.hpp"

struct GameMetadata;
class RecordBanner;
class RecordPreviews;

#include "core/database/helpers.hpp"

struct RecordData
{
	RecordData() = delete;
	RecordData( RecordData&& other ) = default;

  private:

	RecordID m_id { 0 };

	/*QString m_title {};
	QString m_creator {};
	QString m_engine {};

	//! Unix timestamp (Jan 01 1970 Epoch)
	std::uint64_t m_last_played { 0 };

	//! Seconds counter
	std::uint32_t m_total_playtime { 0 };

	std::vector< GameMetadata > m_versions {};

	std::filesystem::path m_banner {};
	std::filesystem::path m_override_banner {};
	std::vector< std::filesystem::path > m_previews {};*/

  public:

	RecordBanner banners();
	RecordPreviews previews();

	//Getters
	RecordID getID() const;

	Column< QString, RecordID > title { "title", "records", "record_id", m_id };
	Column< QString, RecordID > creator { "creator", "records", "record_id", m_id };
	Column< QString, RecordID > engine { "engine", "records", "record_id", m_id };
	Column< std::uint64_t, RecordID > last_played { "last_played_r", "records", "record_id", m_id };
	Column< std::uint32_t, RecordID > total_playtime { "total_playtime", "records", "record_id", m_id };

	std::optional< GameMetadata > getVersion( const QString, Transaction transaction = Transaction( Autocommit ) );
	std::optional< GameMetadata > getLatestVersion( Transaction transaction = Transaction( Autocommit ) );
	std::vector< GameMetadata > getVersions( Transaction transaction = Transaction( Autocommit ) );
	QString getDesc( Transaction transaction = Transaction( Autocommit ) ) const;
	std::vector< QString > getUserTags( Transaction transaction = Transaction( Autocommit ) ) const;
	std::vector< QString > getAllTags( Transaction transaction = Transaction( Autocommit ) ) const;

	//Setters
	void addVersion(
		QString version,
		std::filesystem::path game_path,
		std::filesystem::path exec_path,
		const uint64_t folder_size,
		bool in_place,
		Transaction transaction = Transaction( Autocommit ) );
	void removeVersion( const GameMetadata&, Transaction = Transaction( Autocommit ) );
	//void setBannerOverride( const std::filesystem::path&, Transaction = Transaction( Autocommit ) );

	void setDesc( const QString& str, Transaction transaction = Transaction( Autocommit ) );
	void addUserTag( const QString str, Transaction transaction = Transaction( Autocommit ) );
	void removeUserTag( const QString str, Transaction transaction = Transaction( Autocommit ) );

	//! Fetches the RecordData for the given ID
	/**
	 * @param id
	 * @param transaction. Defaults to autocommit on return
	 */
	RecordData( const RecordID id, Transaction transaction = Transaction( Transaction::Autocommit ) );

	/**
	 * @warning Constructing will create a new record in the database. Pass in Transaction as last parameter in order to not commit on return
	 * @param title
	 * @param creator
	 * @param engine
	 * @param last_played
	 * @param total_playtime
	 * @param versions
	 * @param banner
	 * @param previews
	 * @param transaction
	 */
	RecordData( QString title, QString creator, QString engine, Transaction = Transaction( Transaction::Autocommit ) );

	friend struct RecordAlreadyExists;
	friend class Record;
};

//! Checks if a record exists with the given parameters
bool recordExists(
	const QString& title,
	const QString& creator,
	const QString& engine,
	Transaction transaction = Transaction( Transaction::Autocommit ) );

//! Returns 0 if there is not record with this data
RecordID recordID( const QString& title, const QString& creator, const QString& engine, Transaction transaction );

#endif //ATLAS_RECORD_HPP
