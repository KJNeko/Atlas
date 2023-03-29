//
// Created by kj16609 on 3/12/23.
//

#include "SHA256.hpp"

#include <fstream>
#include <vector>

#include <QCryptographicHash>

#include <tracy/Tracy.hpp>

std::array< char, 32 > sha256FromIstream( std::istream& istream )
{
	ZoneScoped;
	const auto idx { istream.tellg() };
	istream.seekg( 0, std::istream::beg );

	constexpr std::size_t block_size { 4096 };

	QCryptographicHash hasher { QCryptographicHash::Algorithm::Sha256 };

	if ( istream.good() )
	{
		std::array< char, block_size > data;

		while ( true )
		{
			istream.read( data.data(), data.size() );

			const auto read_count { istream.gcount() };
			if ( read_count == 0 || istream.eof() || !istream.good() ) break;

			hasher.addData( { data.data(), read_count } );
		}
	}
	else
		throw std::runtime_error( "Stream not good" );

	//Seek to where it was left off.
	istream.seekg( idx );

	std::array< char, 32 > m_data;
	memcpy( m_data.data(), hasher.result().data(), m_data.size() );

	return m_data;
}

SHA256::SHA256( std::istream& istream ) : m_data( sha256FromIstream( istream ) )
{}

std::array< char, 32 > sha256FromVec( const std::vector< char >& data )
{
	ZoneScoped;
	std::array< char, 32 > arry;

	if ( data.size() != 32 )
		throw std::runtime_error( "Improper size" );
	else
		memcpy( arry.data(), data.data(), arry.size() );

	return arry;
}

SHA256::SHA256( const std::vector< char >& data ) : m_data { sha256FromVec( data ) }
{}

std::string SHA256::toHex() const
{
	return toByteView().toByteArray().toHex().toStdString();
}

QByteArrayView SHA256::toByteView() const
{
	return QByteArrayView( m_data.data(), static_cast< qsizetype >( m_data.size() ) );
}
