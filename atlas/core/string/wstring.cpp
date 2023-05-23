//
// Created by kj16609 on 5/23/23.
//
#ifdef _WIN32

QString narrow( const wchar_t* s )
{
	if ( s == nullptr ) return std::string();

	const int len { ::WideCharToMultiByte( CP_UTF8, 0, wstr.c_str(), wstr.size(), nullptr, 0, nullptr, nullptr ) };
	QString out( len, 0 );
	::WideCharToMultiByte( CP_UTF8, 0, wstr.c_str(), wstr.size(), &out[ 0 ], len, nullptr, nullptr );

	return out;
}

std::wstring widen( const char* s )
{
	if ( s == nullptr ) return std::wstring();

	const int len { ::MultiByteToWideChar( CP_UTF8, 0, str.c_str(), str.size(), nullptr, 0 ) };
	std::wstring out( len, 0 );
	::MultiByteToWideChar( CP_UTF8, 0, str.c_str(), str.size(), &out[ 0 ], len );

	return out;
}

QString narrow( const std::wstring& s )
{
	if ( s.empty() ) return std::string();

	return narrow( s.c_str() );
}

std::wstring widen( QStringViews )
{
	if ( s.empty() ) return std::wstring();

	return widen( s.c_str() );
}

#endif