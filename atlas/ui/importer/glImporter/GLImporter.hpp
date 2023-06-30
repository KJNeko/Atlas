//
// Created by kj16609 on 6/5/23.
//

#ifndef ATLASGAMEMANAGER_GLIMPORTER_HPP
#define ATLASGAMEMANAGER_GLIMPORTER_HPP

#include <filesystem>

#include <QDialog>
#include <QFuture>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class GLImporter;
}

QT_END_NAMESPACE

class GLImporterRunner : public QObject
{
	Q_OBJECT

  public:

	GLImporterRunner();

  signals:
	void processGLFolder( const std::filesystem::path path );
	void message( const QString msg );

  public slots:
	void importGLGames( const std::filesystem::path root );
	void processGame( const std::filesystem::path path );
};

class GLImporter : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( GLImporter )

	QThread m_thread {};
	GLImporterRunner runner {};

  public:

	explicit GLImporter( QWidget* parent = nullptr );
	~GLImporter() override;

	void setImportDir( const std::filesystem::path path );

  private:

	Ui::GLImporter* ui;

  signals:
	void startImport( const std::filesystem::path );

  public slots:
	void addMessage( const QString );
};

#endif //ATLASGAMEMANAGER_GLIMPORTER_HPP
