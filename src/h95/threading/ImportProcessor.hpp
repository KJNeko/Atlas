//
// Created by kj16609 on 3/28/23.
//

#ifndef HYDRUS95_IMPORTPROCESSOR_HPP
#define HYDRUS95_IMPORTPROCESSOR_HPP

#include <QtCore>

#include "ImportPreProcessor.hpp"
#include "h95/Types.hpp"
#include "ui/dialog/ProgressBarDialog.hpp"

class ImportProcessor : public QObject
{
	Q_OBJECT

	QThread thread;

	std::vector< RecordID > completed_records;

  public:

	ImportProcessor();

	~ImportProcessor();

	std::vector< RecordID > getCompleted() { return completed_records; }

  public slots:

	void importGames( const std::vector< GameImportData > data, const std::filesystem::path source );

  signals:
	void importComplete();
	void importCount( const int count );
	void startProgressBar();
	void updateMax( const int max );
	void updateSubMax( const int max );
	void updateText( const QString );
	void updateSubText( const QString );
	void updateSubValue( const int value );
	void updateValue( const int value );
	void closeProgressBar();
};

#endif //HYDRUS95_IMPORTPROCESSOR_HPP
