#include "AboutAtlas.hpp"

#include <moc_AboutAtlas.cpp>

#include <QDesktopServices>
#include <QUrl>
#include <QtGlobal>

#ifdef __GNUC__
#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#endif

#include <spdlog/spdlog.h>

#pragma GCC diagnostic pop
#else

#include <spdlog/spdlog.h>

#endif

#include <lz4.h>
#include <sqlite3.h>

#include "core/version.hpp"
#include "ui_AboutAtlas.h"

AboutAtlas::AboutAtlas( QWidget* parent ) : QDialog( parent ), ui( new Ui::AboutAtlas )
{
	ui->setupUi( this );
	//ui->versionLabel->setText( ATLAS_VERSION_STRING );
	ui->lblGitBranch->setText( QString::fromLocal8Bit( utils::git_branch ) );
	ui->lblGitVersion->setText( QString::fromLocal8Bit( utils::git_tag ) );
	ui->lblGitRev->setText( QString::fromLocal8Bit( utils::git_revision ) );

	const auto qt6_ver { QString( "Qt6 Version: %1" ).arg( QT_VERSION_STR ) };
	const auto sqlite3_ver { QString( "SQLite3 Version: %1" ).arg( SQLITE_VERSION ) };
	const auto lz4_ver { QString( "LZ4: %1" ).arg( LZ4_VERSION_STRING ) };
	const auto spdlog_ver {
		QString( "spdlog: %1.%2.%3" ).arg( SPDLOG_VER_MAJOR ).arg( SPDLOG_VER_MINOR ).arg( SPDLOG_VER_PATCH )
	};

#ifdef HAVE_STD_FORMAT
	const auto fmt_ver { "fmt: No (libc++20)" };
#else
	const auto fmt_ver {
		QString( "fmt: %1.%2.%3" ).arg( FMT_VERSION / 10000 ).arg( FMT_VERSION % 10000 / 100 ).arg( FMT_VERSION % 100 )
	};
#endif

	ui->lblCompiler->setText( QString( "%1 (%2) - %3" )
	                              .arg( ATLAS_COMPILER_ID )
	                              .arg( ATLAS_COMPILER_VER )
	                              .arg( ATLAS_PLATFORM_ID ) );

	ui->lbUsingVers->setText( QString( "Using: %1 | %2 | %3 | %4 | %5" )
	                              .arg( qt6_ver )
	                              .arg( sqlite3_ver )
	                              .arg( lz4_ver )
	                              .arg( spdlog_ver )
	                              .arg( fmt_ver ) );
}

AboutAtlas::~AboutAtlas()
{
	delete ui;
}

void AboutAtlas::on_btnGithub_pressed()
{
	QDesktopServices::openUrl( QUrl( "https://github.com/KJNeko/Atlas" ) );
}

void AboutAtlas::on_btnDiscord_pressed()
{
	QDesktopServices::openUrl( QUrl( "https://discord.gg/f3ee3Mzm" ) );
}

void AboutAtlas::on_btnClose_pressed()
{
	this->close();
}