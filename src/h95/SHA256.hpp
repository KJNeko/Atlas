//
// Created by kj16609 on 3/12/23.
//

#ifndef HYDRUS95_SHA256_HPP
#define HYDRUS95_SHA256_HPP

#include <array>
#include <string>
#include <vector>

#include <QByteArrayView>

class SHA256
{
	std::array< char, 32 > m_data;

	SHA256() = delete;

	public:
	bool operator==( const SHA256& other ) const = default;

	std::string toHex() const;
	QByteArrayView toByteView() const;

	const char* data() const { return m_data.data(); }
	const std::size_t size() const { return m_data.size(); }

	SHA256( std::istream& istream );

	SHA256( const std::vector< char >& data );
};


#endif	//HYDRUS95_SHA256_HPP
