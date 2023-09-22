//
// Created by kj16609 on 9/22/23.
//

#include <QCryptographicHash>

#include <array>
#include <filesystem>
#include <fstream>
#include <string>

#include "core/exceptions.hpp"
#include "core/logging/logging.hpp"

namespace atlas::crypto::sha256
{

	std::string hashFile( const std::filesystem::path path )
	{
		if ( std::ifstream ifs( path ); ifs )
		{
			std::array< char, 2048 > buffer {};
			QCryptographicHash hash { QCryptographicHash::Sha256 };

			while ( ifs.good() && !ifs.eof() )
			{
				ifs.read( buffer.data(), buffer.size() );
				const auto read_bytes { ifs.gcount() };
				hash.addData( QByteArrayView( buffer.data(), read_bytes ) );
			}

			return hash.result().toHex().toStdString();
		}
		else
			throw AtlasException( format_ns::format( "Failed to open file: {}", path ).c_str() );
	}

	std::string hash( const QByteArrayView view )
	{
		QCryptographicHash hash { QCryptographicHash::Sha256 };
		hash.addData( view );

		return hash.result().toHex().toStdString();
	}
} // namespace atlas::crypto::sha256