//
// Created by kj16609 on 4/4/23.
//

#ifndef ATLAS_RECORDEDITOR_HPP
#define ATLAS_RECORDEDITOR_HPP

#include <QDialog>

#include "core/database/record/GameData.hpp"
#include "core/database/record/game/Game.hpp"

QT_BEGIN_NAMESPACE

namespace Ui
{
	class RecordEditor;
}

QT_END_NAMESPACE

class RecordEditor final : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( RecordEditor )

	atlas::records::Game m_record;
	std::filesystem::path m_banner_path;
	std::vector< std::filesystem::path > m_preview_paths;
	std::vector< atlas::records::Version > m_versions;

	void loadRecordInfo();
	void loadBanners();
	void loadPreviews();
	void loadTags();
	void loadVersions();

  public:

	explicit RecordEditor( const RecordID record, QWidget* parent = nullptr );
	~RecordEditor() override;

	void switchTabs( const int index );

  private:

	Ui::RecordEditor* ui;

	void resizeEvent( QResizeEvent* event ) override;

	//On show
	void showEvent( QShowEvent* event ) override;

  private slots:
	void on_splitter_splitterMoved( int pos, int index );

	//Banner
	void on_btnSetBanner_pressed();

	//Previews
	void on_btnAddPreviews_pressed();
	void on_btnRemovePreviews_pressed();
	void on_previewList_reordered();

	//Versions
	void on_btnDeleteVersion_pressed();
	void on_btnAddVersion_pressed();
	void on_versionList_currentRowChanged( int idx );

	//Record info
	void on_btnChangeTitle_pressed();
	void on_btnChangeCreator_pressed();
	void on_btnChangeEngine_pressed();

	//Record info cont.
	void on_btnApplyDesc_pressed();

	//Tags
	void on_tagEdit_returnPressed();

	//Close
	void on_btnClose_pressed();
};

#endif //ATLAS_RECORDEDITOR_HPP
