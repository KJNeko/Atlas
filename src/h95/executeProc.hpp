//
// Created by kj16609 on 2/20/23.
//

#ifndef HYDRUS95_EXECUTEPROC_HPP
#define HYDRUS95_EXECUTEPROC_HPP


#ifdef __linux__

#include <stdio.h>
#include <unistd.h>
#include <string>

void executeProc(const std::string path)
{
	int pid = fork();
	if(pid == 0)
	{//We are child
		execl(path.c_str(), new char('\n'));
		spdlog::info("I still exist?");
		throw std::runtime_error("AAAAAAAAA");
	}
}

#elif __WIN64
#include <Windows.h>

void executeProc(LPCSTR lpApplicationName)
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	CreateProcessA(
		lpApplicationName,
		argv[1],
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi
		);
}


#else
#error "No define for handling this OS"
#endif




#endif	//HYDRUS95_EXECUTEPROC_HPP
