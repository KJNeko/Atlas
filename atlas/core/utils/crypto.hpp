//
// Created by kj16609 on 9/22/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_CRYPTO_HPP
#define ATLASGAMEMANAGER_CRYPTO_HPP

#include <filesystem>
#include <string>

namespace atlas::crypto::sha256
{

	std::string hashFile( const std::filesystem::path path );

	std::string hash( const QByteArrayView view );

} // namespace atlas::crypto::sha256

#endif //ATLASGAMEMANAGER_CRYPTO_HPP
