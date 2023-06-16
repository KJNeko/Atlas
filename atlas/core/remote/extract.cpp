//
// Created by kj16609 on 6/14/23.
//

#include <filesystem>
#include <fstream>
#include <lz4frame.h>
#include <stdio.h>

#include <tracy/Tracy.hpp>

#include "core/logging.hpp"

namespace atlas
{

	std::size_t get_block_size( const LZ4F_frameInfo_t* info )
	{
		switch ( info->blockSizeID )
		{
			default:
				throw std::runtime_error( "Invalid block size" );
			case LZ4F_default:
				[[fallthrough]];
			case LZ4F_max64KB:
				return 1 << 16;
			case LZ4F_max256KB:
				return 1 << 18;
			case LZ4F_max1MB:
				return 1 << 20;
			case LZ4F_max4MB:
				return 1 << 22;
		}
	}

	std::filesystem::path extract( const std::filesystem::path path )
	{
		ZoneScoped;
		spdlog::info( "Extracting {}", path );
		LZ4F_dctx* dctx { nullptr };
		if ( const auto status = LZ4F_createDecompressionContext( &dctx, LZ4F_VERSION ); LZ4F_isError( status ) )
		{
			throw std::runtime_error(
				fmt::format( "Failed to create decompression context: {}", LZ4F_getErrorName( status ) ) );
		}

		if ( std::ifstream ifs( path ); ifs )
		{
			std::ofstream ofs { path.parent_path() / path.stem() };

			//Read header
			std::array< char, LZ4F_HEADER_SIZE_MAX > header_buffer;
			std::size_t header_size {
				static_cast< size_t >( ifs.readsome( header_buffer.data(), header_buffer.size() ) )
			};
			std::size_t processed_bytes { header_size };
			assert( header_size >= LZ4F_MIN_SIZE_TO_KNOW_HEADER_LENGTH );
			LZ4F_frameInfo_t info;
			if ( const auto fires = LZ4F_getFrameInfo( dctx, &info, header_buffer.data(), &processed_bytes );
			     LZ4F_isError( fires ) )
			{
				throw std::runtime_error( fmt::format( "Failed to get frame info: {}", LZ4F_getErrorName( fires ) ) );
			}

			const auto block_size { get_block_size( &info ) };
			std::vector< char > decompression_buffer;
			decompression_buffer.resize( block_size );

			//Rewind file
			ifs.seekg( static_cast< long >( processed_bytes ), std::ios::beg );

			std::size_t ret { 1 };

			std::vector< char > buffer;

			while ( ret != 0 && ifs.good() )
			{
				const std::size_t left_over { buffer.size() };
				buffer.resize( block_size );

				//Read in_buffer
				std::size_t read_bytes { buffer.size() };
				read_bytes += static_cast< size_t >(
					ifs.readsome( buffer.data() + left_over, static_cast< long >( buffer.size() - left_over ) ) );
				std::size_t dst_size { block_size };
				if ( read_bytes == 0 ) break;
				ret = LZ4F_decompress(
					dctx, decompression_buffer.data(), &dst_size, buffer.data(), &read_bytes, nullptr );

				//Lop off data processed from buffer
				buffer.erase( buffer.begin(), buffer.begin() + static_cast< long >( read_bytes ) );

				//write output
				ofs << std::string_view( decompression_buffer.data(), dst_size );
			}
		}
		else
			throw std::runtime_error( fmt::format( "Failed to open file: {}", path.string() ) );

		LZ4F_freeDecompressionContext( dctx );
		spdlog::info( "Finished extracting {} to {:ce}", path, path.parent_path() / path.stem() );
		return path.parent_path() / path.stem();
	}

} // namespace atlas
