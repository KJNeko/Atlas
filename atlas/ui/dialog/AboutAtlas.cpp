#include "AboutAtlas.hpp"

#include <QDesktopServices>
#include <QUrl>

#include "core/version.hpp"
#include "ui_AboutAtlas.h"

AboutAtlas::AboutAtlas( QWidget* parent ) : QDialog( parent ), ui( new Ui::AboutAtlas )
{
	ui->setupUi( this );
	ui->versionLabel->setText( ATLASVERSION_STR );
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