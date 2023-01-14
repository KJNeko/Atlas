//
// Created by kj16609 on 1/14/23.
//

#ifndef HYDRUS95_PARSERS_HPP
#define HYDRUS95_PARSERS_HPP

/**


 @page ParserPage Hydrus95 Parsers

@section DefaultParsers Build-in/Pre-packaged parsers

@subsection ParserInfo
H95's internals use various 'parsers' in order to accumulate information for records. Without any parsers it won't know how to handle any records.

Parsers can be set to override eachother based on a number priority (1 beats 2, 2 beats 3, ect) where when parsers have a identical key the parser with the higher number will have it's information displayed.
Parsers can also be set on a key by key basis to hide keys or force them to always display regardless of parser priority.
There is also a `master keylist` which shows all possible keys that can populated (see @refitem MasterKeylist) which can also allow you to change the order in which keys are displayed and how they are displayed (See @refitem KeyDisplayTypes).

Parsers are also able to manage their own keylist in the @refitem ParserKeylist menu. Parsers are able to change things like

A parser will be given the table `parsername_parserinfo` So the F95Parser information exists in the table `f95parser_parserinfo` and the local parser info is in the table `localparser_parserinfo`

During when the parser is processing input at the end when the parser is finally complete and has collected all the information it can. It will check if a key is required or not. These can be set by the user (Only recomended for advanced users and only on the local parser) which will make the parser 'fail' and abort any action it was trying to perform. This can be helpful to prevent a parser from collecting the wrong amount of information after a site's page has been updated and the parser has not been.


@subsection LocalParserDefault Local Parser
H95 considers local metadata as a 'local parser' which can be populated by feeding the parser folders on your local machine. This information is meant to be overriden and populated by the user.
The user can also add any number of keys and change them to display however they see fit.
<table>
<caption id="local_table_keys">Local keys</caption>
<tr>		<th>Key		<th>Type		<th>Required		<th>Desc
<tr>		<td>`title`		<td>String		<td>Yes		<td>Title of the record
<tr>		<td>`creator`		<td>String		<td>No		<td>Creator of the game/asset/creation the record represents.


@subsection F95ParserDefault F95 Parser
The F95 parser is build to support searching for updated games by examining the rss feed (and main page if the rss feed moves too quickly between updates)

 The F95 parser provides the following information
 <table>
 <caption id="f95_table_keys">F95 Keys</caption>
 <tr><th>Key<th>Type<th>Desc
 <tr><td>`title_thread`<td> String <td> Title of the thread (raw)
 <tr><td>`title`<td>String<td> Title of the game (Processed to remove neddless information)
 <tr><td>`banner_url`<td>String<td>URL to the banner of the thread
 <tr><td>`preview_url`<td>String[]<td>URLs to preview images
 <tr><td>`version`<td>String<td> Version of the game
 <tr><td>`thread_updated`<td>String<td>Date of the last thread update
 <tr><td>`release_date`<td>String<td>Release date given
 <tr><td>`developer`<td>
 	<table>
 		<caption id="f95_dev_keys">F95 Dev Info</caption>
 		<tr><th>Key<th>Type<th>Desc
 		<tr><td>`name`<td>String<td>Name of develeoper/producer
 		<tr><td>`urls`<td>String[]<td>Urls (if any) to sizes
	</table><td>Developer information
 <tr><td>`censored`<td>String<td>Censor information
 <tr><td>`censored_bool`<td>Boolean<td>If censored or not (Any is true)
 <tr><td>`os_support`<td>String<td>List of supported operating systems
 <tr><td>`linux_support`<td>Boolean<td>If linux is supported
 <tr><td>`mac_support`<td>Boolean<td>If mac os supported
 <tr><td>`windows_support`<td>Boolean<td>If Windows is supported
 <tr><td>`langauge`<td>String[]<td> List of languages supported
 <tr><td>`genre`<td>String[]<td> List of genres
 <tr><td>`changelog`<td>String<td> Changelog text
 <tr><td>`dev_notes`<td>String<td> Developer notes
 <tr><td>`downloads`<td>
 	<table>
 		<caption id="f95_download_keys">F95 Download Info</caption>
 		<tr><th>Key<th>Type<th>Desc
 		<tr><td>`win_dl`<td>String[]<td> Windows download links
 		<tr><td>`linux_dl`<td>String[]<td> Linux download links (Seems most of the time this will be the same as `win_dl`)
 		<tr><td>`mac_dl`<td>String[]<td> Mac download links
 		<tr><td>`android_dl`<td>String[]<td> Android download links
 		<tr><td>`other_dl`<td>String[]<td> Other download links
	</table>
 <td>Download links scraped
 </table>
 */


#endif	//HYDRUS95_PARSERS_HPP
