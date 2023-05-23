
#include "executeProc.hpp"

#include "core/logging.hpp"

#ifdef __linux__

#include <string>

void executeProc( const std::string& path )
{
	spdlog::debug( "Executing game {}", path );

	std::system( ( '\"' + path + '\"' ).c_str() );
}

#elif _WIN64

#include <Windows.h>
#include <cstring>

void executeProc( const std::string& path_str )
{
	spdlog::debug( "Executing game {}", path_str );

	LPWSTR path;
	path = new char[ 4096 * 4 ];
	//std::strcpy( path, path_str.c_str() );
	int len { MultiByteToWideChar( CP_ACP, 0, path_str.c_str(), path_str.size() + 1, 0, 0 ) };
	wchar_t* buff = new wchar_t[ len ];
	MultiByteToWideChar( CP_ACP, 0, path_str.c_str(), path_str.size() + 1, buff, len );
	path = buff;

	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof( si ) );
	si.cb = sizeof( si );
	ZeroMemory( &pi, sizeof( pi ) );

	if ( !CreateProcessW( nullptr, path, nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi ) )
		spdlog::error( "Failed to execute process {}", path );
}

#else
#error "No define for handling this OS"
#endif
