//
// Created by kj16609 on 3/2/23.
//

#include <h95/KeyReplacer.hpp>

#include "GTestBox.hpp"

class TestKeyReplacer : public ::testing::Test
{
	public:
	inline static std::unordered_map< QString, QString >& exposeKeyList( KeyReplacer& replacer )
	{
		return replacer.key_list;
	}
};

TEST_F( TestKeyReplacer, setKey )
{
	KeyReplacer replacer;

	replacer.setKey( "{title}", "My Game Name" );

	GTEST_ASSERT_NE(
		TestKeyReplacer::exposeKeyList( replacer ).find( "{title}" ),
		TestKeyReplacer::exposeKeyList( replacer ).end() );
	GTEST_ASSERT_EQ( TestKeyReplacer::exposeKeyList( replacer ).find( "{title}" )->second, "My Game Name" );
}

TEST_F( TestKeyReplacer, setKeyExisting )
{
	KeyReplacer replacer;
	replacer.setKey( "{title}", "My Game Name" );
	replacer.setKey( "{title}", "My Other Name" );

	GTEST_ASSERT_NE(
		TestKeyReplacer::exposeKeyList( replacer ).find( "{title}" ),
		TestKeyReplacer::exposeKeyList( replacer ).end() );
	GTEST_ASSERT_EQ( TestKeyReplacer::exposeKeyList( replacer ).find( "{title}" )->second, "My Other Name" );
}

TEST_F( TestKeyReplacer, value )
{
	KeyReplacer replacer;
	replacer.setKey( "{title}", "My Game Name" );

	GTEST_ASSERT_EQ( replacer.value( "{title}" ), "My Game Name" );
}

TEST_F( TestKeyReplacer, valueInvalid )
{
	KeyReplacer replacer;
	replacer.setKey( "{title}", "My Game Name" );

	GTEST_ASSERT_EQ( replacer.value( "{creator}" ), "" );
}

TEST_F( TestKeyReplacer, extractKeysEndBackslash )
{
	KeyReplacer replacer;
	std::filesystem::path path { "/run/media/kj16609/HDDWIN/NSFWSorted/Acerola/Brave Alchemist Colette/1.04/" };
	std::filesystem::path pattern { "{path}/{creator}/{title}/{version}" };

	replacer.extractKeys( path, pattern );

	GTEST_ASSERT_EQ( replacer.value( "{path}" ), "/run/media/kj16609/HDDWIN/NSFWSorted" );
	GTEST_ASSERT_EQ( replacer.value( "{creator}" ), "Acerola" );
	GTEST_ASSERT_EQ( replacer.value( "{title}" ), "Brave Alchemist Colette" );
	GTEST_ASSERT_EQ( replacer.value( "{version}" ), "1.04" );
}

TEST_F( TestKeyReplacer, extractKeys )
{
	KeyReplacer replacer;
	std::filesystem::path path { "/run/media/kj16609/HDDWIN/NSFWSorted/Acerola/Brave Alchemist Colette/1.04" };
	std::filesystem::path pattern { "{path}/{creator}/{title}/{version}" };

	replacer.extractKeys( path, pattern );

	GTEST_ASSERT_EQ( replacer.value( "{path}" ), "/run/media/kj16609/HDDWIN/NSFWSorted" );
	GTEST_ASSERT_EQ( replacer.value( "{creator}" ), "Acerola" );
	GTEST_ASSERT_EQ( replacer.value( "{title}" ), "Brave Alchemist Colette" );
	GTEST_ASSERT_EQ( replacer.value( "{version}" ), "1.04" );
}
