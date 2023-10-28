#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QProgressDialog>
#include <QDir>
#include <elzip.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	CheckForUpdateFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CheckForUpdateFile(){
    
    const std::string update_file = std::string( std::getenv( "APPDATA" ) ) + "\\ATLAS\\update.zip";

    //We need to perfop
	//check for update file
	if(std::filesystem::exists(update_file))
    {
		InstallUpdate( update_file );
        QMainWindow::hide();
	}
}

void MainWindow::InstallUpdate(std::string update_file)
{
    //The install needs to happen in this order
    //Unzip file and store in tmp folder
    //Move atlas updater to a different folder
    //Move contents of temp dir to atlas dir
        const std::string update_tmp = std::string( std::getenv( "APPDATA" ) ) + "\\ATLAS\\tmp";
	    const QString atlas_dir = QString::fromStdString( std::filesystem::current_path().string());
       //create new progress bar instance
        QProgressDialog *pd = new QProgressDialog("Update in progress. Please Wait", "Cancel", 0, 100);
		pd->show();
		pd->setValue( 10 );

        //Extract files
		elz::extractZip( update_file, update_tmp );
		pd->setValue( 60 );

        //We need to copy each file and skip the atlasupdater
		QDir dir = QString::fromStdString( update_tmp );
		QStringList fileList = dir.entryList();
		for (int i=0; i<fileList.count(); i++)
        {
            if(fileList[i] != "AtlasUpdater.exe")
            {
				QDir tdir;
				const QString curr_tmp_file = dir.filePath( fileList.at( i ) );
                const QString atlas_file = atlas_dir + "\\" +fileList[i];
				bool test = tdir.rename( curr_tmp_file, atlas_file );
				qInfo() << dir.filePath(fileList.at(i));
				//)
			}
        }
}


