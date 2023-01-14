//
// Created by kj16609 on 1/13/23.
//

#include "RecordBase.hpp"
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QSqlQuery>
#include <QDir>
#include <QDirIterator>

RecordBase::RecordBase( const RecordID id ) noexcept : record_id(id)
{
	//Find out what type of record are we
	QSqlQuery query;
	query.exec("SELECT record_type FROM records WHERE uid = :id");
	query.bindValue(":id", record_id);
	query.exec();
	query.first();

	type = static_cast<RecordType>(query.value(0).value<int>());

	switch(type)
	{
		default: [[fallthrough]];
		case RecordType::NORMAL_RECORD:
			break;
		case RecordType::GAME_RECORD:
			subrecord = GameRecord(id);
			break;
		case RecordType::ASSERT_RECORD:
//			subrecord = AssetRecord(id);
			break;
	}
}


QString RecordBase::getName() const
{
	QSqlQuery query;
	query.prepare("SELECT record_name FROM records WHERE uid = :id");
	query.bindValue(":id", record_id);
	query.exec();
	query.first();
	return query.value(0).toString();
}


QString RecordBase::getDesc() const
{
	QSqlQuery query;
	query.prepare("SELECT record_desc FROM records WHERE uid = :id");
	query.bindValue(":id", record_id);
	query.exec();
	query.first();
	return query.value(0).toString();
}


QString RecordBase::getNotes() const
{
	QSqlQuery query;
	query.prepare("SELECT record_notes FROM records WHERE uid = :id");
	query.bindValue(":id", record_id);
	query.exec();
	query.first();
	return query.value(0).toString();
}

std::filesystem::path RecordBase::getPath() const
{
	QSqlQuery query;
	query.prepare("SELECT record_path FROM records WHERE uid = :id");
	query.bindValue(":id", record_id);
	query.exec();
	query.first();
	return query.value(0).toString().toStdString();
}

std::filesystem::path RecordBase::getBannerPath() const
{
	QDirIterator itter {QDir(getPath() / "h95-data")};
	while(itter.hasNext())
	{
		const auto info {itter.nextFileInfo()};
		if(info.fileName() == "banner")
			return info.filesystemPath();
	}

	return {":/invalid_banner.png"};
}

void RecordBase::setBanner( const std::filesystem::path& path )
{
	QImage img{path.c_str()};
	if(img.isNull())
		return;
	else
		img.save(QString::fromStdString(getBannerPath().string() + path.extension().string()));
}

void RecordBase::setPath( const std::filesystem::path& path )
{
	QSqlQuery query;
	query.prepare("UPDATE records SET record_path = :path");
	query.bindValue(":path", QString::fromStdString(path.string()));
	query.exec();
	std::filesystem::create_directories(path / "h95-data");
	return;
}

void RecordBase::setName( const QString& name )
{
	QSqlQuery query;
	query.prepare("UPDATE records SET record_name = :name");
	query.bindValue(":path", name);
	query.exec();
	return;
}

void RecordBase::setDescription( const QString& desc )
{
	QSqlQuery query;
	query.prepare("UPDATE records SET record_description = :text");
	query.bindValue(":text", desc);
	query.exec();
	return;
}

void RecordBase::setNotes( const QString& notes )
{
	QSqlQuery query;
	query.prepare("UPDATE records SET record_notes = :text");
	query.bindValue(":text", notes);
	query.exec();
	return;
}

QString GameRecord::getVersion() const
{
	return "VERSION NOT IMPLEMENTED";
}

