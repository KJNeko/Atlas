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

	std::vector< RecordID > completed_records {};

	std::atomic< bool > abort_task { false };

  public:

	ImportProcessor();

	~ImportProcessor() = default;

	std::vector< RecordID > getCompleted() { return completed_records; }

  public slots:

	void importGames(
		const std::vector< GameImportData > data, const std::filesystem::path source, const bool move_after_import );
	void abort();

  signals:
	void importComplete();
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
