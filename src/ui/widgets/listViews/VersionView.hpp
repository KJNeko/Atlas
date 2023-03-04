//
// Created by kj16609 on 3/1/23.
//

#ifndef HYDRUS95_VERSIONVIEW_HPP
#define HYDRUS95_VERSIONVIEW_HPP

#include <QListView>
#include "h95/database/GameMetadata.hpp"

class VersionViewModel : public QAbstractItemModel
{
	Q_DISABLE_COPY_MOVE(VersionViewModel)

	std::vector<GameMetadata> m_versions {};

	public:
	VersionViewModel(QObject* parent = nullptr);

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	void setVersions(std::vector<GameMetadata> metadata);
	void addVersion(GameMetadata metadata);
	void removeVersion(const GameMetadata& metadata);
	std::vector<GameMetadata> versions() const;
};

class VersionView : public QListView
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE( VersionView )

	public:
	explicit VersionView( QWidget* parent = nullptr );
	~VersionView() override;


	void setVersions(std::vector<GameMetadata> metadata);
	void addVersion(GameMetadata metadata);
	void removeVersion(const GameMetadata& metadata);
	std::vector<GameMetadata> versions() const;

};


#endif	//HYDRUS95_VERSIONVIEW_HPP
