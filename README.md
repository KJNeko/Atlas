<div align="center">
  <a href="https://atlas-gamesdb.com/">
    <img height="180px" src="https://github.com/KJNeko/Atlas/blob/master/atlas/assets/Atlas_logo_v2.svg" alt="atlas logo">
  </a>
  
![GitHub release (with filter)](https://img.shields.io/github/v/release/KJNeko/Atlas?style=flat&logo=github&logoColor=white&label=)
![Static Badge](https://img.shields.io/badge/-C++14/17-3F63B3.svg?style=flat&logo=C%2B%2B&logoColor=white")
![Static Badge](https://img.shields.io/badge/-Windows-6E46A2.svg?style=flat&logo=windows-11&logoColor=white)
![Static Badge](https://img.shields.io/badge/-Linux-9C2A91.svg?style=flat&logo=linux&logoColor=white)
![Static Badge](https://img.shields.io/badge/MinGW%2013+%20-flag.svg?color=555555&style=flat&logo=cmake&logoColor=white&label=)
![Static Badge](https://img.shields.io/badge/-docs-green.svg?logo=Wikipedia)
<br>
![GitHub Workflow Status (with event)](https://img.shields.io/github/actions/workflow/status/KJNeko/Atlas/.github%2Fworkflows%2Frelease.yml?event=push&label=release)
![GitHub Workflow Status (with event)](https://img.shields.io/github/actions/workflow/status/KJNeko/Atlas/.github%2Fworkflows%2Fpr-test-build.yml?event=push&label=pr)
![GitHub Workflow Status (with event)](https://img.shields.io/github/actions/workflow/status/KJNeko/Atlas/.github%2Fworkflows%2Fnightly.yml?event=push&label=nightly)
![GitHub issues](https://img.shields.io/github/issues/KJNeko/Atlas)
![GitHub pull requests](https://img.shields.io/github/issues-pr-raw/KJNeko/Atlas?label=pull%20request)
![GitHub all releases](https://img.shields.io/github/downloads/KJNeko/Atlas/total)
</div>

#
Atlas is an open source game manager and launcher for Windows and Linux with a modern interface for viewing, finding, and easily installing 3rd party games and visual novels.
We currently support adding games from [F95Zone](https://f95zone.to/) and [DLSite](https://www.dlsite.com/) and plan on adding more sources as the project progresses (See features below).
It's meant to replace [Game-List](www.game-list.org) ([Github](https://github.com/game-list/game-list)) but also expand on it's feature set.

Atlas is for the user that wants ease of use, quality of life and have the software do the heavy lifting for you.

For discussion about future releases and new features please visit our [Discord Server](https://discord.gg/XpTHvYbYyz).

## Features:
- Fast and Snappy UI.
- Easy to understand UI written in Qt6.
- Custom themes and color schemes.
- Game file management (moves or copies game install files for you)
- Easy to edit metadata and tags per title.
- Navigate titles with a Banner/Thumbnail gallery grid (Images are not automatically downloaded yet).
- View or add previews for a title.
- Filter titles based on titles, creators, tags, ID's, description contents and urls.
- Bulk install/import (See [Issue #6](https://github.com/KJNeko/Atlas/issues/6)] multiple titles and versions with a single approval.
- Compresses all banner and preview images to smaller webp files for storage optimization.

### F95Zone integration [Issue #2](https://github.com/KJNeko/Atlas/issues/2)
- [ ] Scrape Metadata (titles, tags, description, version and image urls)
- [ ] Downloads Banner images and preview images automatically.
- [ ] In app browser with custom hooks to allow 1 click game installs.
- [ ] Update/release checker/notifier.

### DLSite integration [Issue #3](https://github.com/KJNeko/Atlas/issues/3)
- [ ] Scrape Metadata (titles, tags, description, version and image urls)
- [ ] Game updates/notifications
- [ ] Follow your popular creators for new releases

### Steam/SteamCMD integration [Issue #4](https://github.com/KJNeko/Atlas/issues/4)
- [ ] Add SteamID to Atlas
- [ ] Run, download and uninstall with SteamID
- [ ] Show Steam Link to push users to optionally buy the title.

