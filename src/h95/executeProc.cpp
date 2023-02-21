
#include "executeProc.hpp"

#include <h95/logging.hpp>

#ifdef __linux__

#include <stdio.h>
#include <unistd.h>
#include <string>

void executeProc( const std::string& path )
{
	int pid = fork();
	if ( pid == 0 )
	{  //We are child
		execl( path.c_str(), new char( '\n' ) );
		spdlog::info( "I still exist?" );
		throw std::runtime_error( "AAAAAAAAA" );
	}
}

#elif _WIN64

#include <Windows.h>
#include <cstring>

void executeProc( const std::string& path_str )
{
	LPSTR path;
	path = new char[4096 * 4];
	std::strcpy( path, path_str.c_str() );

	//Fork here
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof( si ) );
	si.cb = sizeof( si );
	ZeroMemory( &pi, sizeof( pi ) );

	if ( !CreateProcessA( path, nullptr, nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi ) )
		spdlog::error( "Failed to execute process {}", path );
}


#else
#error "No define for handling this OS"
#endif
