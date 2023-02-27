
#include "executeProc.hpp"

#include <h95/logging.hpp>
#include <fmt/chrono.h>

#ifdef __linux__

#include <unistd.h>
#include <string>

void executeProc( const std::string& path )
{
	int pid = fork();
	if ( pid == 0 )
	{  //We are child
		//execl( path.c_str(), "", nullptr );

		const auto start_time {std::chrono::steady_clock::now()};

		spdlog::debug("Executing game {}", path);

		std::system(path.c_str());

		const auto end_time {std::chrono::steady_clock::now()};

		spdlog::debug("Process killed after {}", end_time - start_time);

		std::exit(EXIT_SUCCESS);
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
