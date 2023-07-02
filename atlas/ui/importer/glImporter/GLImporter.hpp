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

enum FailType
{
	MissingExecutable,
	MissingData,
	InvalidThread,
	Except
};

class GLImporterRunner final : public QObject
{
	Q_OBJECT

  public:

	GLImporterRunner();

  signals:
	void processGLFolder( const std::filesystem::path root, const std::filesystem::path path );
	void message( const QString msg );
	void success();
	void failed( FailType type );

  public slots:
	void importGLGames( const std::filesystem::path root );
	void processGame( const std::filesystem::path root, const std::filesystem::path path );
};

class GLImporter final : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( GLImporter )

	QThread m_thread {};
	GLImporterRunner runner {};

	std::uint64_t success { 0 };
	std::uint64_t failed_exception { 0 };
	std::uint64_t failed_data { 0 };
	std::uint64_t failed_exec { 0 };

  public:

	explicit GLImporter( QWidget* parent = nullptr );
	~GLImporter() override;

	void setImportDir( const std::filesystem::path path );

  private:

	void updateText();

	Ui::GLImporter* ui;

  signals:
	void startImport( const std::filesystem::path );

  public slots:
	void addMessage( const QString );
	void addSuccess();
	void addFailed( const FailType type );
};

#endif //ATLASGAMEMANAGER_GLIMPORTER_HPP
