//
// Created by kj16609 on 3/26/23.
//

#ifndef HYDRUS95_REGEX_HPP
#define HYDRUS95_REGEX_HPP

#include <QString>

/**
 * @page RegexGuide
 * @section RegexGuideGettingStarted Getting Started
 * TODO
 *
 * @page AdvRegexGuide
 * @section AdvRegexGuideParsing Advanced Parsing
 * The filepath parser allows for passing in raw regex in the middle of pre-existing fill-ins.
 *
 *
 *
 * When `skip regexify` is unchecked the following is done
 * - All `{}` groups (`{title}` for example) are stripped down and replaced liked so `{title}` -> `(?P<title>[^\\\/]+)`
 * - The exception to this is `{path}` which is
 * - The start of the regex has `^` appended to it
 * - The end of the regex has `$` appended to it
 * - This means the following `{title}/{creator}` gets converted to `(?P<title>[^\\\/]+)\\(?P<creator>[^\\\/]+)`
 *
 * The following are advanced examples that can be used\n
 * Example 1: `{creator}/.*?-{title}-{version}`
 * | path | title | creator | version |
 * |------------|--------|---|---|
 * | ~/Games/CreatorName1/Collection1-MyTitle-v1.0 | MyTitle | CreatorName1 | v1.0 |
 * | ~/Games/CreatorName1/Collection1-MyOtherGame-V2.0 | MyOtherGame | CreatorName1 | v2.0 |
 *
 * Example 2: `{creator}-{title}-{version}`
 * | path | title | creator | version |
 * |------------|--------|---|---|
 * | ~/Games/CreatorName1-MyTitle-v1.0 | MyTitle | CreatorName1 | v1.0 |
 * | ~/Games/CreatorName1-MyOtherGame-V2.0 | MyOtherGame | CreatorName1 | v2.0 |
 *
 * @note While we do allow completely raw regex you must mind a few things.
 *
 * - In order to generate the regex we append the base path to the front of the regex. Example: The 'regex' `{creator}/{title}/{version}` with the base path `/my/games/` becomes `/my/games/{creator}/{title}/{version}`. This is always done and is not influenced by `skip regexify`
 * - You MUST fill in the capture group `title`.
 * - Capture groups `creator` `version` and `engine` exist and are searched for. But are ultimately optional
 */

QString groupify( const QString group_name );

//! SHOULD NOT BE USED ANYWHERE EXCEPT FOR PATHS
QString escapeStr( QString pattern );

QString processRegexify( QString pattern );

QString regexify( QString pattern );

bool valid( QString pattern, QString text );

struct GroupsOutput
{
	QString title;
	QString creator;
	QString version;
	QString engine;
};

//! Extracts all groups from a given string with a given pattern.
GroupsOutput extractGroups( QString pattern, QString text );

#endif //HYDRUS95_REGEX_HPP
