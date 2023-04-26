//
// Created by kj16609 on 4/9/23.
//

#ifndef ATLASGAMEMANAGER_FILESCANNER_HPP
#define ATLASGAMEMANAGER_FILESCANNER_HPP

#include <coroutine>
#include <filesystem>
#include <string>
#include <vector>

#include <tracy/Tracy.hpp>

#include "atlas/logging.hpp"

struct FileInfo
{
	std::string filename {};
	std::string ext {};
	std::filesystem::path path {};
	std::size_t size { 0 };
	std::uint8_t depth { 0 };
	std::filesystem::path relative {};

	FileInfo() = default;

	FileInfo(
		std::filesystem::path path_in,
		const std::filesystem::path& source,
		const std::size_t filesize,
		const std::uint8_t file_depth ) :
	  filename( path_in.filename().string() ),
	  ext( path_in.extension().string() ),
	  path( std::move( path_in ) ),
	  size( filesize ),
	  depth( file_depth ),
	  relative( std::filesystem::relative( path, source ) )
	{}
};

struct FileScannerGenerator
{
	struct promise_type;
	using handle_type = std::coroutine_handle< promise_type >;

	struct promise_type
	{
		FileInfo value {};
		std::exception_ptr exception { nullptr };

		FileScannerGenerator get_return_object() { return FileScannerGenerator( handle_type::from_promise( *this ) ); }

		std::suspend_always initial_suspend() noexcept { return {}; }

		std::suspend_always final_suspend() noexcept { return {}; }

		void unhandled_exception()
		{
			spdlog::critical( "An exception was thrown in the file scanner coroutine." );
			std::terminate();
		}

		void return_value( FileInfo&& from )
		{
			assert( from.filename != "" );
			value = std::move( from );
		}

		std::suspend_always yield_value( FileInfo from )
		{
			ZoneScoped;
			assert( from.filename != "" );
			value = std::move( from );
			return {};
		}
	};

	handle_type m_h;

	FileScannerGenerator( handle_type h ) : m_h( h ) {}

	~FileScannerGenerator() { m_h.destroy(); }

	FileInfo operator()();
};

struct FileScanner
{
	std::filesystem::path m_path;
	std::vector< FileInfo > files {};

	friend class iterator;

	FileScannerGenerator file_scanner;

  private:

	class iterator
	{
		std::size_t m_idx { 0 };
		FileScanner& m_scanner;

	  public:

		iterator( const std::size_t idx, FileScanner& scanner ) : m_idx( idx ), m_scanner( scanner ) {}

		FileScanner::iterator& operator++()
		{
			ZoneScoped;
			++m_idx;
			return *this;
		}

		// Operator != required to check for end I assume. Where if the this returns true then we are good to continue
		// So instead we can just return the state of the scanner. And if the scanner is complete then we'll return false here.
		//bool operator !=
		bool operator==( [[maybe_unused]] const iterator& end ) const;

		// Required for the for loop
		const FileInfo& operator*() { return m_scanner.at( m_idx ); }
	};

  public:

	FileScanner( const std::filesystem::path& path );

  private:

	FileInfo& at( std::size_t index );

  public:

	iterator begin() { return iterator( 0, *this ); }

	//This *probably* isn't required(?) but the for loop will want it anyways. So we can just return literaly anything here since it's not used anyways.
	iterator end() { return iterator( 0, *this ); }

	std::filesystem::path path() const { return m_path; }
};

#endif //ATLASGAMEMANAGER_FILESCANNER_HPP
