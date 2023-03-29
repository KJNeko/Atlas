//
// Created by kj16609 on 3/29/23.
//

#ifndef HYDRUS95_RECORDVIEW_HPP
#define HYDRUS95_RECORDVIEW_HPP

#include <QListView>

#include "h95/Types.hpp"
#include "h95/database/Record.hpp"

enum DelegateType
{
	NO_MODE = 0,
	BANNER_VIEW = 1,
};

class RecordView : public QListView
{
	Q_OBJECT

  private:

	DelegateType current_render_mode { NO_MODE };

  public:

	RecordView( QWidget* parent = nullptr );

  public slots:
	void addRecords( const std::vector< RecordID > records );
	void setRecords(const std::vector<Record> records);
	void setRenderMode( const DelegateType type );
};

#endif //HYDRUS95_RECORDVIEW_HPP
