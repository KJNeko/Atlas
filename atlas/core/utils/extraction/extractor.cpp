//
// Created by towerwatchman on 01/24/24.
//

#include "extractor.hpp"

#include <moc_extractor.cpp>

#include <bit7z/bitarchivereader.hpp>
#include <bit7z/bitfileextractor.hpp>

namespace atlas::utils
{

	bool Extractor::extractArchive( GameImportData game )
	{
		m_signaler = std::make_unique< ProgressSignaler >();
		m_signaler->setMessage( "Extracting: " + game.title );
		m_signaler->setSubMessage( "" );
		m_signaler->setProgress( 0 );
		m_signaler->setMax( 0 );

		try
		{ // bit7z classes can throw BitException objects
			using namespace bit7z;

			Bit7zLibrary lib {};
			BitFileExtractor extractor { lib, BitFormat::Auto };
			uint64_t data_size {};

			//Get the total size
			extractor.setTotalCallback( [ &data_size ]( const uint64_t total_size ) { data_size = total_size; } );
			qInfo() << data_size;

			extractor.setProgressCallback(
				[ &data_size, this ]( uint64_t processed_size )
				{
					auto progress = static_cast< int >( ( 100.0 * processed_size ) / data_size );
					extractionProgress( progress, 100 );
					return true;
				} );
			extractor.setFileCallback( [ this ]( std::string file_path ) { fileProgress( file_path ); } );

			// Extracting a simple archive
			qInfo() << "Archive Path: " << game.archive_path
					<< "\nRelative Path: " << QString::fromStdString( game.relative_path.string() );
			//CREATE DIR FIRST
			if ( !std::filesystem::create_directories( game.relative_path ) )
			{
				qInfo() << "Path Already Exist ";
			}
			//Extract files
			extractor.extract( game.archive_path.toStdString(), game.relative_path.string() );
			//Check if there is a single folder in dir. If there is, move all files one folder up
			qInfo() << "Checking folder: " << QString::fromStdString( game.relative_path.string() );
			int file_count { 0 };
			std::string folder_path { "" };

			for ( const auto& file : std::filesystem::directory_iterator( game.relative_path.string() ) )
			{
				file_count++;
				folder_path = file.path().string();
			}
			//if there is one dir then we need to move the files one dir up
			if ( file_count == 1 )
			{
				for ( std::filesystem::path p : std::filesystem::directory_iterator( folder_path ) )
				{
					std::filesystem::rename( p, game.relative_path / p.filename() );
				}
				//Delete the old folder
				std::filesystem::remove( folder_path );
			}
			this->deleteLater();
			return true;
		}
		catch ( const bit7z::BitException& ex )
		{
			std::cout << ex.what();
			this->deleteLater();
			return false;
		}
	}

	void Extractor::extractionProgress( qint64 bytesReceived, qint64 bytesTotal )
	{
		m_signaler->setProgress( bytesReceived );
		m_signaler->setMax( static_cast< int >( bytesTotal ) );
	}

	void Extractor::fileProgress( std::string file )
	{
		m_signaler->setSubMessage( "Current file: " + QString::fromStdString( file ) );
	}
} // namespace atlas::utils
