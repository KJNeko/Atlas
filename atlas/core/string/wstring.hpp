//
// Created by kj16609 on 5/23/23.
//

#ifndef ATLASGAMEMANAGER_WSTRING_HPP
#define ATLASGAMEMANAGER_WSTRING_HPP

#ifdef _WIN32

std::string narrow( const wchar_t* s );
std::wstring widen( const char* s );
std::string narrow( const std::wstring& s );
std::wstring widen( QStringViews );

#else

//Noop
#define narrow( s ) s
#define widen( s ) s

#endif

#endif //ATLASGAMEMANAGER_WSTRING_HPP
