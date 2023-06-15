//
// Created by kj16609 on 6/14/23.
//

#include <filesystem>
#include <fstream>
#include <zlib.h>

#include <QDir>

#include "core/logging.hpp"

namespace atlas
{
	std::filesystem::path extract( const std::filesystem::path path )
	{
		if ( std::ifstream ifs( path, std::ios::binary ); ifs )
		{
			//Open out location
			const std::filesystem::path out_path { QDir::tempPath().toStdString() + "/atlas/" + path.stem().string() };
			std::filesystem::create_directories( out_path.parent_path() );
			std::ofstream ofs { out_path, std::ios::binary };

			int ret;
			unsigned have;
			z_stream strm;
			constexpr int CHUNK { 1 << 18 }; // 256kb
			unsigned char in[ CHUNK ];
			unsigned char out[ CHUNK ];

			strm.zalloc = nullptr;
			strm.zfree = nullptr;
			strm.opaque = nullptr;
			strm.avail_in = 0;
			strm.next_in = nullptr;
			ret = inflateInit( &strm );
			strm.next_in = in;
			strm.next_out = out;
			if ( ret != Z_OK ) throw std::runtime_error( fmt::format( "Failed to initialize zlib: {}", ret ) );

			do {
				strm.avail_in = static_cast< unsigned int >( ifs.readsome( reinterpret_cast< char* >( in ), CHUNK ) );
				if ( ifs.fail() || ifs.bad() )
				{
					(void)inflateEnd( &strm );
					throw std::runtime_error( "Failed to read file" );
				}

				if ( strm.avail_in == 0 ) break;

				do {
					strm.avail_out = CHUNK;

					ret = inflate( &strm, Z_NO_FLUSH );
					assert( ret != Z_STREAM_ERROR );
					switch ( ret )
					{
						case Z_NEED_DICT:
							[[fallthrough]];
						case Z_DATA_ERROR:
							[[fallthrough]];
						case Z_MEM_ERROR:
							(void)inflateEnd( &strm );
							throw std::runtime_error( fmt::format( "Failed to inflate file: {}", ret ) );
						default:
							break;
					}

					have = CHUNK - strm.avail_out;

					ofs << std::string_view( reinterpret_cast< char* >( out ), have );
				}
				while ( strm.avail_out == 0 );
			}
			while ( ret != Z_STREAM_END );
			(void)inflateEnd( &strm );
			return out_path;
		}
		else
			throw std::runtime_error( fmt::format( "Failed to open file {}", path ) );
	}
} // namespace atlas
