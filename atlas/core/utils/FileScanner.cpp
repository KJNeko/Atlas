//
// Created by kj16609 on 4/9/23.
//

#include "FileScanner.hpp"

#include <assert.h>
#include <queue>

#include "atlas/core/logging.hpp"

FileInfo FileScannerGenerator::operator()()
{
	m_h();
	if ( m_h.promise().exception ) std::rethrow_exception( m_h.promise().exception );
	return std::move( m_h.promise().value );
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"

FileScannerGenerator scan_files( const std::filesystem::path path )
{
	if ( !std::filesystem::exists( path ) )
	{
		spdlog::error( "scan_files: Path {} does not exist.", path.string() );
		throw std::runtime_error( "Path does not exist." );
	}

	spdlog::debug( "Scanning path {}", path );
	std::queue< std::pair< std::filesystem::path, std::uint8_t > > dirs {};

	dirs.push( { path, 0 } );

	auto dir_empty = []( const std::filesystem::path& dir_path ) -> bool
	{ return std::filesystem::directory_iterator( dir_path ) == std::filesystem::directory_iterator(); };

	if ( dir_empty( path ) ) co_return FileInfo { path, path, 0, 0 };

	while ( dirs.size() > 0 )
	{
		const auto [ dir, depth ] { std::move( dirs.front() ) };
		dirs.pop();
		std::vector< std::filesystem::path > nested_dirs;

		//Recurse through the directory.
		for ( auto itter = std::filesystem::directory_iterator( dir ); itter != std::filesystem::directory_iterator(); )
		{
			if ( itter->is_directory() )
			{
				//Add it to the scanlist
				nested_dirs.emplace_back( *itter );
				++itter;
				continue;
			}

			FileInfo info {
				*itter, path, itter->is_regular_file() ? itter->file_size() : 0, std::uint8_t( depth + 1 )
			};

			++itter;

			//If we are at the last file and there are no more directories to scan then return.
			if ( itter == std::filesystem::directory_iterator() && dirs.size() == 0 && nested_dirs.size() == 0 )
				co_return std::move( info );
			else
				co_yield std::move( info );
		}

		//Add the nested dirs to the scanlist and yield them
		for ( std::size_t i = 0; i < nested_dirs.size(); ++i )
		{
			// Check if the directory is empty and if it's not then add it to the scan queue.
			if ( !dir_empty( nested_dirs.at( i ) ) )
			{
				dirs.push( { nested_dirs.at( i ), depth + 1 } );
				co_yield FileInfo { nested_dirs.at( i ), path, 0, std::uint8_t( depth + 1 ) };
			}
			else
			{
				if ( i == nested_dirs.size() - 1 && dirs.size() == 0 )
					co_return FileInfo { nested_dirs.at( i ), path, 0, std::uint8_t( depth + 1 ) };
				else
					co_yield FileInfo { nested_dirs.at( i ), path, 0, std::uint8_t( depth + 1 ) };
			}
		}
	}

	spdlog::error( "Managed to escape loop in coroutine scan_files" );
	throw std::runtime_error( "Managed to escape loop in coroutine scan_files" );
}

#pragma GCC diagnostic pop

FileScanner::FileScanner( const std::filesystem::path& path ) : m_path( path ), file_scanner( scan_files( path ) )
{
	files.emplace_back( file_scanner() );
}

FileInfo& FileScanner::at( std::size_t index )
{
	if ( index >= files.size() && !file_scanner.m_h.done() )
	{
		const auto size { files.size() };

		files.emplace_back( file_scanner() );

		assert( files.size() > 0 );
		assert( files.size() == size + 1 );

		return files.at( files.size() - 1 );
	}
	else
		return files.at( index );
}

bool FileScanner::iterator::operator==( [[maybe_unused]] const iterator& end ) const
{
	return m_scanner.file_scanner.m_h.done() && ( m_idx == m_scanner.files.size() );
}
