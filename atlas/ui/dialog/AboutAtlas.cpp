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
	ui->versionLabel->setText( ATLASVERSION_STR );

	ui->lbQt6Ver->setText( QString( "Qt6 Version: %1" ).arg( QT_VERSION_STR ) );
	ui->lbSqlite3Ver->setText( QString( "SQLite3 Version: %1" ).arg( SQLITE_VERSION ) );
	ui->lbLZ4Ver->setText( QString( "LZ4: %1" ).arg( LZ4_VERSION_STRING ) );
	ui->lbSPDLogVer->setText( QString( "spdlog: %1.%2.%3" )
	                              .arg( SPDLOG_VER_MAJOR )
	                              .arg( SPDLOG_VER_MINOR )
	                              .arg( SPDLOG_VER_PATCH ) );
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