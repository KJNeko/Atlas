//
// Created by kj16609 on 4/9/23.
//

#include "FileScanner.hpp"

#include <queue>

#include <tracy/Tracy.hpp>

#include "core/logging.hpp"

FileInfo FileScannerGenerator::operator()()
{
	ZoneScoped;
	if ( m_h.done() ) throw std::runtime_error( "FileScannerGenerator is done." );
	m_h();

	if ( m_h.promise().exception ) std::rethrow_exception( m_h.promise().exception );
	if ( m_h.promise().value.has_value() )
		return m_h.promise().value.value();
	else
		throw std::runtime_error( "Failed to get value from FileScannerGenerator." );
}

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default" // Added due to GCC bug 109867
#pragma GCC diagnostic ignored                                                                                         \
	"-Wzero-as-null-pointer-constant" // Added due to FileScannerGenerator returns being weird at the end of scan_files and thinking that it's somehow returning zero as a nullptr. Probably UB but we throw at the end anyways so *shrug* Should be defined enough.
#endif

FileScannerGenerator scan_files( const std::filesystem::path path )
{
	if ( !std::filesystem::exists( path ) )
	{
		spdlog::error( "scan_files: Path {} does not exist.", path.string() );
		throw std::runtime_error( "Path does not exist." );
	}

	auto dir_empty = []( const std::filesystem::path& dir_path ) -> bool
	{ return std::filesystem::directory_iterator( dir_path ) == std::filesystem::directory_iterator(); };

	if ( dir_empty( path ) ) co_return FileInfo { path, path, 0, 0 };

	std::queue< std::pair< std::filesystem::path, std::uint8_t > > dirs {};

	dirs.push( { path, 0 } );

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
				//Add directory to scan list.
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
				//Dir is empty. If we don't have anything else to process then return. Else yield.
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

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

FileScanner::FileScanner( const std::filesystem::path& path ) :
  m_path( path ),
  file_scanner( scan_files( path ) ),
  files( { file_scanner() } )
{}

const FileInfo& FileScanner::at( std::size_t index )
{
	ZoneScoped;
	if ( index >= files.size() && !file_scanner.m_h.done() )
	{
		// Index is higher then what we have.
		// Scanner is also NOT done.
		// We use the coroutine to fetch what the next file should be.

		std::size_t diff { index - ( files.size() - 1 ) };

		while ( diff > 0 && !file_scanner.m_h.done() )
		{
			files.emplace_back( file_scanner() );
			--diff;
		}
	}

	if ( files.size() < index )
		throw std::
			runtime_error( fmt::format( "FileScanner::at({}): size < index : {} < {}", index, files.size(), index ) );

	return files.at( index );
}

bool FileScanner::iterator::operator==( const std::unreachable_sentinel_t ) const
{
	ZoneScoped;
	return m_scanner.file_scanner.m_h.done() && ( m_idx == m_scanner.files.size() );
}
