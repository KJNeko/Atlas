//
// Created by kj16609 on 2/26/23.
//

#ifndef HYDRUS95_GAMEEDITIMAGEMODEL_HPP
#define HYDRUS95_GAMEEDITIMAGEMODEL_HPP

#include <filesystem>
#include <vector>

#include <QAbstractItemModel>

class GameEditImageModel : public QAbstractItemModel
{
	Q_DISABLE_COPY_MOVE( GameEditImageModel )

	std::filesystem::path& m_banner;
	std::vector< std::filesystem::path >& m_previews;

	public:
	GameEditImageModel(
		std::filesystem::path& banner,
		std::vector< std::filesystem::path >& previews,
		QObject* parent = nullptr ) :
	  QAbstractItemModel( parent ),
	  m_banner( banner ),
	  m_previews( previews )
	{
	}

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex() ) const override;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;

	void removeItems(const std::vector<std::size_t>& indexes);
	void addPreview(std::string path);
};


#endif	//HYDRUS95_GAMEEDITIMAGEMODEL_HPP
