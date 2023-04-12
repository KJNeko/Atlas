//
// Created by kj16609 on 4/9/23.
//

#include "FileScanner.hpp"

#include <tracy/Tracy.hpp>

#include "atlas/logging.hpp"

FileInfo FileScannerGenerator::operator()()
{
	ZoneScoped;
	m_h();
	if ( m_h.promise().exception ) std::rethrow_exception( m_h.promise().exception );
	return std::move( m_h.promise().value );
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"

FileScannerGenerator scan_files( const std::filesystem::path path )
{
	ZoneScoped;
	spdlog::debug( "Scanning path {}", path );
	std::vector< std::pair< std::filesystem::path, std::uint8_t > > dirs {};

	dirs.emplace_back( path, 0 );

	while ( dirs.size() > 0 )
	{
		const auto [ dir, depth ] { std::move( dirs.at( dirs.size() - 1 ) ) };
		dirs.pop_back();
		std::vector< std::filesystem::path > scanback;

		for ( auto itter = std::filesystem::directory_iterator( dir ); itter != std::filesystem::directory_iterator(); )
		{
			if ( itter->is_directory() )
			{
				scanback.emplace_back( *itter );
				++itter;
				continue;
			}

			FileInfo info {
				itter->path(), path, itter->is_regular_file() ? itter->file_size() : 0, std::uint8_t( depth + 1 )
			};

			++itter;

			//If we are at the last file and there are no more directories to scan then return.
			if ( itter == std::filesystem::directory_iterator() && dirs.size() == 0 && scanback.size() == 0 )
				co_return std::move( info );
			else
				co_yield std::move( info );
		}

		//The 'scanback' section is to ensure that we don't accidently over-read when we reach a directory with no files.
		for ( const auto &scan : scanback )
		{
			if ( std::filesystem::directory_iterator( scan ) != std::filesystem::directory_iterator() )
				dirs.emplace_back( scan, depth + 1 );
		}

		if ( dirs.size() == 0 )
		{
			if ( scanback.size() > 1 )
			{
				for ( std::size_t i = 0; i < scanback.size() - 1; ++i )
				{
					FileInfo info { scanback.at( i ), path, 0, std::uint8_t( depth + 1 ) };
					co_yield std::move( info );
				}
			}
			co_return FileInfo { scanback.at( scanback.size() - 1 ), path, 0, std::uint8_t( depth + 1 ) };
		}
		else
		{
			for ( const auto &dir_path : scanback ) co_yield FileInfo { dir_path, path, 0, std::uint8_t( depth + 1 ) };
		}
	}

	throw std::runtime_error("Managed to escape loop in coroutine scan_files");
}

#pragma GCC diagnostic pop

FileScanner::FileScanner( const std::filesystem::path &path ) : m_path( path ), file_scanner( scan_files( path ) )
{}

FileInfo &FileScanner::at( std::size_t index )
{
	ZoneScoped;
	if ( index >= files.size() && !file_scanner.m_h.done() )
	{
		ZoneScopedN( "Fetch file info" );
		files.emplace_back( file_scanner() );

		return files.at( files.size() - 1 );
	}
	else
		return files.at( index );
}

bool FileScanner::iterator::operator==( [[maybe_unused]] const iterator &end ) const
{
	ZoneScoped;

	return m_scanner.file_scanner.m_h.done() && ( m_idx == m_scanner.files.size() );
}
