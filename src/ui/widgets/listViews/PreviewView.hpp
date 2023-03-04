//
// Created by kj16609 on 3/1/23.
//

#ifndef HYDRUS95_PREVIEWVIEW_HPP
#define HYDRUS95_PREVIEWVIEW_HPP

#include <QListView>
#include <QAbstractListModel>
#include <filesystem>

class PreviewViewModel final : public QAbstractListModel
{
	std::vector< std::filesystem::path > m_previews {};

	public:
	QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const override;
	QModelIndex parent( const QModelIndex& index ) const override;

	int rowCount( const QModelIndex& parent = QModelIndex() ) const override;

	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;

	void addPreview( std::filesystem::path path );
	void setPreviews( std::vector< std::filesystem::path > previews );
	void removePreview( std::size_t index );

	std::vector< std::filesystem::path > previews();
};

class PreviewView final : public QListView
{
	Q_OBJECT

	public:
	explicit PreviewView( QWidget* parent = nullptr );
	~PreviewView() override;

	std::vector<std::filesystem::path> previews() const;
	void setPreviews(std::vector<std::filesystem::path> paths);
};


#endif	//HYDRUS95_PREVIEWVIEW_HPP
