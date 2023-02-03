//
// Created by kj16609 on 1/31/23.
//

#ifndef HYDRUS95_KEYREPLACER_HPP
#define HYDRUS95_KEYREPLACER_HPP

#include <unordered_map>
#include <QString>

class KeyReplacer
{
	std::unordered_map<QString, QString> key_map {};
	public:
	void registerKey(const QString& key, QString value);
	//void parseKeys(const QString& mask, const QString& str);

	QString value(const QString& value) const;

	QString& replaceKeys(QString& str) const;

	void clear();
};

#endif	//HYDRUS95_KEYREPLACER_HPP
