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

		for ( auto itter = std::filesystem::directory_iterator( dir ); itter != std::filesystem::directory_iterator(); )
		{
			FileInfo info { itter->path(), path, itter->is_regular_file() ? itter->file_size() : 0, std::uint8_t(depth + 1) };

			if ( itter->is_directory() ) dirs.emplace_back( *itter, depth + 1 );

			++itter;
			if ( itter == std::filesystem::directory_iterator() && dirs.size() == 0 )
				co_return std::move( info );
			else
				co_yield std::move( info );
		}
	}
}

FileScanner::FileScanner( const std::filesystem::path &path ) : m_path( path ), file_scanner( scan_files( path ) )
{}

FileInfo &FileScanner::at( std::size_t index )
{
	ZoneScoped;
	if ( index >= files.size() && !file_scanner.m_h.done() )
	{
		ZoneScopedN("Fetch file info");
		files.emplace_back( file_scanner() );

		return files.at( files.size() - 1 );
	}
	else
		return files.at( index );
}
