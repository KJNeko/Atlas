#include "AboutAtlas.hpp"
#include "ui_AboutAtlas.h"

#include <QDesktopServices>
#include <QUrl>

AboutAtlas::AboutAtlas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutAtlas)
{
    ui->setupUi(this);
}

AboutAtlas::~AboutAtlas()
{
    delete ui;
}

void AboutAtlas::on_btnGithub_pressed(){    
	QDesktopServices::openUrl(QUrl("https://github.com/KJNeko/Atlas"));
}

void AboutAtlas::on_btnDiscord_pressed(){
    QDesktopServices::openUrl(QUrl("https://discord.gg/f3ee3Mzm"));
}