//
// Created by kj16609 on 1/2/24.
//

#pragma once

namespace atlas::literals::size_literals
{
	inline unsigned long long int operator""_B( unsigned long long int val )
	{
		return val;
	}

	inline unsigned long long int operator""_KiB( unsigned long long int val )
	{
		return val * 1024;
	}

	inline unsigned long long int operator""_MiB( unsigned long long int val )
	{
		return val * 1024_KiB;
	}

	inline unsigned long long int operator""_GiB( unsigned long long int val )
	{
		return val * 1024_MiB;
	}

} // namespace atlas::literals::size_literals