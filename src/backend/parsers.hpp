//
// Created by kj16609 on 1/14/23.
//

#ifndef HYDRUS95_PARSERS_HPP
#define HYDRUS95_PARSERS_HPP

/**


 @page ParserPage Hydrus95 Parsers

 @section DefaultParsers Build-in/Pre-packaged parsers
 @subsection F95ParserDefault
 The F95 parser is build to support searching for updated games by examining the rss feed (and main page if the rss feed moves too quickly between updates)

 The F95 parser provides the following information
 <table>
 <caption id="f95_table_keys">F95 Keys</caption>
 <tr><th>Key<th>Type<th>Desc
 <tr><td>`title_thread`<td> String <td> Title of the thread (raw)
 <tr><td>`title`<td>String<td> Title of the game (Processed to remove neddless information)
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
