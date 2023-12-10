//#include "mainwindow.hpp"
//#include <QApplication>
#include <algorithm>
#include <cstring>
#include <elzip.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <unistd.h>
#include <vector>
#include <windows.h>
#include <winuser.h>

int main( int argc, char** argv )
{
	if ( argc <= 1 )
	{
		int msgboxID = MessageBox(
			NULL,
			"The Updater can not be ran directly at the moment. Please run from Atlas",
			"Atlas Updater v1.0",
			MB_ICONERROR | MB_OK );

		switch ( msgboxID )
		{
			case IDOK:
				// TODO: add code
				exit( 0 );
		}
	}
	else
	{
		const std::string update_file = std::string( std::getenv( "APPDATA" ) ) + "\\ATLAS\\update.zip";
		const std::string update_tmp_dir = std::string( std::getenv( "APPDATA" ) ) + "\\ATLAS\\tmp";
		const std::string atlas_dir = std::filesystem::current_path().string();
		const std::string atlas_exe = atlas_dir + "\\Atlas.exe";
		bool errors = false;
		std::vector< std::string > folders { "data", "updater" };

		//check that the update file exist
		if ( std::filesystem::exists( update_file ) )
		{
			//extract update.zip
			elz::extractZip( update_file, update_tmp_dir );
		}
		else
		{
			std::cout << "error" << '\n';
			errors = true;
		}

		//Atlas updater version should increase if changes are made.
		//Check if there were errors
		if ( errors == true )
		{
			int msgboxID = MessageBox(
				NULL,
				"There was an error while trying to run the updater. You will need to download the latest update from github",
				"Atlas Updater v1.0",
				MB_ICONERROR | MB_OK );

			switch ( msgboxID )
			{
				case IDOK:
					// TODO: add code
					break;
			}
		}
		else
		{
			const std::string ps1_exe = "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
			STARTUPINFOA si;
			PROCESS_INFORMATION pi;
			ZeroMemory( &si, sizeof( si ) );
			si.cb = sizeof( si );
			ZeroMemory( &pi, sizeof( pi ) );

			const std::string args = "-noexit Copy-item \'" + update_tmp_dir + "\\*\' -Destination \'" + atlas_dir
			                       + "\' -Recurse -force ; start \'" + atlas_exe + "\'";

			char* win_buffer { new char[ args.size() ] };
			std::memcpy( win_buffer, args.c_str(), args.size() );

			if ( CreateProcessA(
					 ps1_exe.c_str(), win_buffer, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi ) )
			{
				exit( 0 ); //Kill Program
				WaitForSingleObject( pi.hProcess, INFINITE );
				CloseHandle( pi.hProcess );
				CloseHandle( pi.hThread );
			}
		}
	}
}
