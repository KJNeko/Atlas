[![forthebadge](https://forthebadge.com/images/badges/it-works-why.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/powered-by-qt.svg)](https://forthebadge.com)

[![Release](https://github.com/KJNeko/Atlas/actions/workflows/release.yml/badge.svg)](https://github.com/KJNeko/Atlas/actions/workflows/release.yml)
[![Test Staging](https://github.com/KJNeko/Atlas/actions/workflows/staging-test.yml/badge.svg?branch=staging)](https://github.com/KJNeko/Atlas/actions/workflows/staging-test.yml)
# Atlas
Atlas is a game manager for Windows and Linux, (Possibly mac eventually but probably not). 
We support adding games from [F95Zone](https://f95zone.to/) and [DLSite](https://www.dlsite.com/) and plan on adding more in the future (LINK ISSUE HERE).

It's meant to replace [Game-List](www.game-list.org) ([Github](https://github.com/game-list/game-list)) but also expand on it's feature set.

Atlas is for the user that wants ease of use, quality of life and have the software do the heavy lifting for you.

## Features:
- Fast and Snappy UI.
- Easy to understand UI written in Qt6.
- Custom themes and color schemes. [Issue #17](https://github.com/KJNeko/Atlas/issues/17)
- Game file management (moves or copies game install files for you)
- Easy to edit metadata and tags per title.
- Navigate titles with a Banner/Thumbnail gallery grid.
- View previews of the title scraped from the web or manually added.
- Filter titles based on titles, creators, tags, ID's, description contents and urls.
- Bulk install/import (See [Issue #6](https://github.com/KJNeko/Atlas/issues/6)] multiple titles and versions with a single approval.
- Compresses all banner and preview images to smaller webp files for storage optimization.

### F95Zone integration [Issue $2](https://github.com/KJNeko/Atlas/issues/2)
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

