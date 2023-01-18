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
<caption id="f95_table_keys">Keys provided by F95 Parser</caption>
<tr><th>Key					<th>Type		<th>Required	<th>Desc
<tr><td>`title_thread`		<td>String		<td>Yes			<td> Title of the thread (raw)
<tr><td>`f95id` 			<td>Integer		<td>Yes 		<td> f95 forum id
<tr><td>`title`				<td>String		<td>Yes			<td>Title of the game (Processed to remove neddless information)
<tr><td>`banner_url`		<td>String		<td>Yes			<td>URL to the banner of the thread
<tr><td>`preview_urls`		<td>String[]	<td>Yes			<td>URLs to preview images
<tr><td>`version`			<td>String		<td>Yes			<td> Version of the game
<tr><td>`thread_updated`	<td>String		<td>Yes			<td>Date of the last thread update
<tr><td>`release_date`		<td>String		<td>Yes			<td>Release date given
<tr>		<td>`developer`			<td>
	<table>
		<caption id="f95_dev_keys">F95 Dev Info</caption>
		<tr><th>Key			<th>Type		<th>Required	<th>Desc
		<tr><td>`name`		<td>String		<td>No			<td>Name of develeoper/producer
		<tr><td>`urls`		<td>String[]	<td>No			<td>Urls (if any) to sizes
	</table>
											<td>Yes			<td>Developer information
<tr><td>`censored`			<td>String		<td>Yes			<td>Censor information
<tr><td>`censored_bool`		<td>Boolean		<td>Yes			<td>If censored or not (Any is true)
<tr><td>`os_support`		<td>String		<td>Yes			<td>List of supported operating systems
<tr><td>`linux_support`		<td>Boolean		<td>Yes			<td>If linux is supported
<tr><td>`mac_support`		<td>Boolean		<td>Yes			<td>If mac os supported
<tr><td>`windows_support`	<td>Boolean		<td>Yes			<td>If Windows is supported
<tr><td>`langauge`			<td>String[]	<td>Yes			<td> List of languages supported
<tr><td>`genre`				<td>String[]	<td>Yes			<td> List of genres
<tr><td>`changelog`			<td>String		<td>Yes			<td> Changelog text
<tr><td>`dev_notes`			<td>String		<td>Yes			<td> Developer notes
<tr><td>`downloads`			<td>
 	<table>
 		<caption id="f95_download_keys">F95 Download Info</caption>
 		<tr><th>Key				<th>Type		<th>Required	<th>Desc
 		<tr><td>`win_dl`		<td>String[]	<td>No			<td> Windows download links
 		<tr><td>`linux_dl`		<td>String[]	<td>No			<td> Linux download links (Seems most of the time this will be the same as `win_dl`)
 		<tr><td>`mac_dl`		<td>String[]	<td>No			<td> Mac download links
 		<tr><td>`android_dl`	<td>String[]	<td>No			<td> Android download links
 		<tr><td>`other_dl`		<td>String[]	<td>No			<td> Other download links
	</table>
 											<td>No			<td>Download links scraped
 </table>

 @subsection DLSiteParser DLSite Parser
 The DLSite parser is primarly designed for game's but can analyze anything on the DLsite

<table>
<caption id="dlsite_table_keys">Keys provided by DLSite parser</caption>
 <tr><th>Key<th>Type<th>Required<th>Desc
 <tr><td>`title`			<td>String			<td>Yes		<td>Title of the game
 <tr><td>`release_date`		<td>String			<td>Yes		<td>Release date
 <tr><td>`scenario`			<td>String			<td>No		<td>??
 <tr><td>`illustration`		<td>String			<td>No		<td>Illustration by
 <tr><td>`voice_actor`		<td>String[]		<td>No		<td>Voice actor list
 <tr><td>`language`			<td>String[]		<td>No		<td>Product languages
 <tr><td>`format`			<td>String[]		<td>No		<td>Product format.
 <tr><td>`genre`			<td>String[]		<td>No		<td>Genre list
 <tr><td>`preview_urls`		<td>String[]		<td>Yes		<td>Preview urls list
</table>

 @page ParserPageSpec Parsers Specification
 A parser contains the methods to obtain a set of keys from a domain. These keys are also displayed to the user based on the settings the user has set.

 The following table describes all the various keys for the parsers and how they are structured
 <table>
<caption id="parser_table_keys">JSON Keys for parsers</caption>
 <tr><th>Key			<th>Value		<th>Desc
 <tr><td>`domain_regex`	<td>String[]	<td>Regex to identify domains the parser should act on
 <tr><td>`content`		<td>
 	<table>
	<tr><th>Key			<th>Type		<th>Desc
 	<tr><td>`key`		<td>String		<td>Key to populate with value gotten.
 	<tr><td>`required`	<td>Boolean		<td>If the parser should fail if the value could not be found
 	<tr><td>`steps`		<td>
 		<table>
		<tr><th>Key					<th>Type									<th>Desc
 		<tr><td>`type`				<td>String (See @ref ParserStepSearch)		<td>Type of scan to use
 		<tr><td>`type_parameters`	<td>Object. (See @ref ParserStepSearch) 	<td>Parameters used with the given type
 		<tr><td>`depth`				<td>Integer 								<td>Depth to perform the scan (Unlimited if not provided)
 		</table>
										<td>Steps to perform to acquire the key
	<tr><td>`type`		<td>String		<td>Type
 	</table>							<td>Content that can be acquired by parser
</table>
 @subsection ParserStepSearch Parser search types
 `Search` is the first and probably most common search type. It will search for a html tag

 <table>
 <caption id="StepTypes">Step types for parsers</caption>
 <tr><th>Name<th>Desc<th>Object Structure
 <tr><td>Search<td>Searches for a specific html tag.<td>
 	<table>
 		<tr><th>Key<th>Type<th>Desc<th>Example(s)
 		<tr><td>`tag`		<td>String 																<td>HTML tag to look for.
 		<tr><td>`attributes`<td> key-attribute pairs store as an array `id=sidebar` -> `id:sidebar` <td>Searches in order from first->last <td>`{"attributes": [{"id":"sidebar"},{"id":"tag-list"},{"id":"sidebar"}]}`
 		<tr><td>`count`		<td> integer 															<td>Number of tags to return that match the filter. (Assumed unlimited if not provided)
	</table>
 <tr><td>Regex<td>Searches the entire page using a regex statement.<td> String with regex with at least one capture group called `value`. Additional capture groups can be used for 'driveby' grabbing of values. The capture group names are 1:1 to the key-value placed into the parser's output
 <tr><td>


 */


#endif	//HYDRUS95_PARSERS_HPP
