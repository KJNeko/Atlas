//#include "mainwindow.hpp"
//#include <QApplication>
#include <elzip.hpp>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <tchar.h>
#include <cstring>


int main()// int argc, char** argv )
{
    const std::string update_file = std::string( std::getenv( "APPDATA" ) ) + "\\ATLAS\\update.zip";
    const std::string update_tmp_dir = std::string( std::getenv( "APPDATA" ) ) + "\\ATLAS\\tmp";
    const std::string atlas_dir =std::filesystem::current_path().string();
    const std::string atlas_exe = atlas_dir + "\\Atlas.exe";
	bool errors = false;
    std::vector<std::string> folders { "data", "updater" };


	//check that the update file exist
	if ( std::filesystem::exists( update_file ) )
	{
        //extract update.zip
        elz::extractZip( update_file, update_tmp_dir );

        for (auto const& dir_entry : std::filesystem::directory_iterator{update_tmp_dir}){
			std::string atlas_dir_path = atlas_dir + "\\" + dir_entry.path().filename().string();
			std::filesystem::path dst( atlas_dir_path );


            if(!(std::find(folders.begin(), folders.end(), dir_entry.path().filename().string()) != folders.end()))
            {
                //Check for updater file
                
                if(dir_entry.path().filename() != "AtlasUpdater.exe" && dir_entry.path().filename() != "updater") 
                {
                    std::cout << dir_entry.path().string() << " -> " << dst.string() << '\n';  
                    try{
                        if(std::filesystem::remove_all(dst))
                        {
                            std::cout << "file removed" << "\n";
                        }
                        if(!dir_entry.is_directory())
                        {
                            std::filesystem::copy( dir_entry.path(),
                                                dst,
                                                std::filesystem::copy_options::overwrite_existing );
                        }
                        else{
                            std::filesystem::copy( dir_entry.path(),
                                                dst,
                                                std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive );
                            std::cout << "file Added" << "\n";
                        }
                    }
                    catch (const std::exception &e)
                    {
                        std::cout << e.what() << "\n";       
                        std::cout << dir_entry.path().filename().string() << "  " << dst.string() << '\n';            
                    }
                }
            }
        }
		//errors = false;
	}
    else{
        std::cout << "error" << '\n';
		errors = true;
	}

    //Atlas updater version should increase if changes are made. 
    //Check if there were errors
    if( errors == true){
         int msgboxID = MessageBox(
            NULL,
            "There was an error while trying to run the updater. You will need to download the latest update from github",
            "Atlas Updater v1.0",
            MB_ICONERROR | MB_OK
        );

        switch (msgboxID)
        {
        case IDOK:
            // TODO: add code
            break;
        }
    }
    else{
        int msgboxID = MessageBox(
            NULL,
            "Update Complete. Press OK to continue",
            "Atlas Updater v1.0",
            MB_ICONASTERISK  | MB_OK
        );

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        
        //Wide string because windows is stupid
        
        const std::string args = "0"; //Default, DO NOT SHOW GUI

        char* win_buffer { new char[args.size()] } ;
        std::memcpy(win_buffer, args.c_str(), args.size());
    
        if (CreateProcessA(atlas_exe.c_str(), win_buffer, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
        {
            exit(0); //Kill Program
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        

        switch (msgboxID)
        {
        case IDOK:
            // TODO: add code
            break;
        }
    }
    
}
