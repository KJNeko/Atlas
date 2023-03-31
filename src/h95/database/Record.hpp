//
// Created by kj16609 on 1/13/23.
//

#ifndef HYDRUS95_RECORD_HPP
#define HYDRUS95_RECORD_HPP

#include <QPixmap>

#include "FlyWeight.hpp"
#include "h95/Types.hpp"
#include "h95/database/GameMetadata.hpp"

enum IMAGE_TYPE
{
	IMAGE_BANNER = 1,
	IMAGE_PREVIEW = 2,
	IMAGE_UNKNOWN = 999
};

struct RecordData : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(RecordData)

	RecordData() = default;

  private:

	RecordID m_id { 0 };
	QString m_title {};
	QString m_creator {};
	QString m_engine {};

	//! Unix timestamp (Jan 01 1970 Epoch)
	std::uint64_t m_last_played { 0 };

	//! Seconds counter
	std::uint32_t m_total_playtime { 0 };

	std::vector< GameMetadata > m_versions {};

	std::filesystem::path m_banner {};
	std::vector< std::filesystem::path > m_previews {};

  public:

	//Getters
	RecordID getID() const;
	const QString& getTitle() const;
	const QString& getCreator() const;
	const QString& getEngine() const;
	std::uint64_t getLastPlayed() const;
	std::uint32_t getTotalPlaytime() const;
	GameMetadata& getVersion( const QString );
	std::vector< GameMetadata >& getVersions();
	const std::filesystem::path& getBannerPath() const;
	QPixmap getBanner() const;
	QPixmap getBanner( int width, int height, bool expanding = false ) const;
	const std::vector< std::filesystem::path >& getPreviewPaths() const;
	std::vector< QPixmap > getPreviews() const;
	GameMetadata& getLatestVersion();

	//Setters
	void setTitle( QString, Transaction = Transaction( true ) );
	void setCreator( QString, Transaction = Transaction( true ) );
	void setEngine( QString, Transaction = Transaction( true ) );
	void setLastPlayed( const std::uint64_t, Transaction = Transaction( true ) );
	void addPlaytime( const std::uint32_t, Transaction = Transaction( true ) );
	void setTotalPlaytime( const std::uint32_t, Transaction = Transaction( true ) );
	void addVersion( GameMetadata, Transaction = Transaction( true ) );
	void removeVersion( const GameMetadata&, Transaction = Transaction( true ) );

	void setBanner( const std::filesystem::path&, Transaction = Transaction( true ) );
	void addPreview( const std::filesystem::path&, Transaction = Transaction( true ) );

	//! Syncs the data from the database
	void sync( Transaction transaction = Transaction( true ) );

  signals:
	//! Emitted when ANY data is changed
	void dataChanged();
	void titleChanged( QString );
	void creatorChanged( QString );
	void engineChanged( QString );
	void lastPlayedChanged( std::uint64_t );
	void totalPlaytimeChanged( std::uint32_t );
	void versionsChanged( const std::vector< GameMetadata >& );
	void bannerPathChanged( std::filesystem::path );
	void bannerChanged( QPixmap );
	void previewPathChanged( std::vector< std::filesystem::path > );
	void previewsChanged( std::vector< QPixmap > );

  public:

	RecordData(
		RecordID id,
		QString title,
		QString creator,
		QString engine,
		const std::uint64_t last_played,
		const std::uint32_t total_playtime,
		std::vector< GameMetadata > versions,
		std::filesystem::path banner,
		std::vector< std::filesystem::path > previews ) :
	  m_id( id ),
	  m_title( std::move( title ) ),
	  m_creator( std::move( creator ) ),
	  m_engine( std::move( engine ) ),
	  m_last_played( last_played ),
	  m_total_playtime( total_playtime ),
	  m_versions( std::move( versions ) ),
	  m_banner( std::move( banner ) ),
	  m_previews( std::move( previews ) )
	{}

	/**
	 * @param id
	 * @param transaction
	 */
	RecordData( const RecordID id, Transaction transaction = Transaction( true ) );

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
	RecordData(
		QString title,
		QString creator,
		QString engine,
		const std::uint64_t last_played,
		const std::uint32_t total_playtime,
		std::vector< GameMetadata > versions,
		std::filesystem::path banner,
		std::vector< std::filesystem::path > previews,
		Transaction = Transaction( true ) );

	//! Defined to comply with FlyWeight HasStaticKeyFunc constraint
	inline static RecordID key( const RecordID id, [[maybe_unused]] Transaction transaction = Transaction( true ) )
	{
		return id;
	}

	//! Defined to comply with FlyWeight HasKeyFunc constraint
	RecordID key() const { return m_id; }

	friend struct RecordAlreadyExists;
};

using Record = FlyWeight< RecordData, RecordID >;

Record importRecord( QString title, QString creator, QString engine, Transaction transaction = Transaction(true) );

struct RecordException : public std::runtime_error
{
	RecordException( const char* const msg ) : std::runtime_error( msg ) {}
};

struct RecordAlreadyExists : public RecordException
{
	Record record;

	RecordAlreadyExists( Record record_in ) :
	  RecordException( ( "Record already exists with id " + std::to_string( record_in->m_id ) ).c_str() ),
	  record( std::move( record_in ) )
	{}
};

struct InvalidRecordID : public RecordException
{
	RecordID id;

	InvalidRecordID( const RecordID in_id ) :
	  RecordException( ( "Invalid record id = " + std::to_string( in_id ) ).c_str() ),
	  id( in_id )
	{}
};

#endif //HYDRUS95_RECORD_HPP
