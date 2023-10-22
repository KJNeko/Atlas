//
// Created by kj16609 on 10/6/23.
//

#include <chrono>
#include <string>
#include <cstring>
#include <unistd.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <cstdio>

int main( int argc, char** argv )
{
	printf("%d", argc);
	printf("%s", argv);

	if ( argc != 2 )
	{
		
		//TODO: Add error reporting here
		// We should expect TWO arguments. The 0th arg is the path to the OUR executable and the 1st arg is the PID of the process to watch.
	}

	//int pid = std::stoi( argv[ 1 ] );

	//Watch the pid until it dies. If it takes longer then 1 minute then abort the update process.
	const auto now { std::chrono::steady_clock::now() };
	const auto timeout { now + std::chrono::minutes( 1 ) };
	while ( std::chrono::steady_clock::now() < timeout )
	{
		//TODO: Add watching code
	}

	//One last check to ensure that we left the loop because the process died.
	if ( /* Check process aliveness here. This just gets the compiler to STFU ->*/ true )
	{ 
		STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        
        //Wide string because windows is stupid
        const std::string path = "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
        const std::string args = "1 2";
        
        char* win_buffer { new char[args.size()] } ;
        std::memcpy(win_buffer, args.c_str(), args.size());
    
        if (CreateProcessA(path.c_str(), win_buffer, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
        {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
		//If we have entered here then we are safe to begin modifying the files.
	}
	else
	{
		//Error report shit here
	}
}
