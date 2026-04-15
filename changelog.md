# v1.0.2

- Attempts to fix crash on Android when downloading. If you still experience this issue on Android, please report it on Github Issues or in the Geode Discord server by pinging @treehousefalcon with your crashlog
- Removes `geode.node-ids` as a dependency since it is not used

## v1.0.1

- Fixes issue with SFX downloading where some levels would not start downloading songs on level view

## v1.0.0

- First stable release! ![:aga:](assets/aga.png)

## v0.2.5

- Fixes bug where downloading popup would not appear when trying to play after manually starting audio download

## v0.2.3 / 0.2.4

- Fixes bug where entering levels is blocked if there is no delete button when the CustomSongWidget is in a settled state

## v0.2.1 / v0.2.2

- Adds support for GD 2.2081
- Changes Newgrounds policy auto-accept setting to conform to Geode index rules
- Updates the release job to build for all platforms

## v0.2.0

This version includes a major refactor to how popups are handled, improving code organization, overall stability and compatibility with other mods.

Changes by [@RayDeeUx](https://github.com/RayDeeUx) ([#1](https://github.com/TreehouseFalcon/geode-auto-download-sounds/pull/1)):

- Removes `FLAlertLayer` hooks
- Uses `CustomSongWidget::showError()` hook for detecting asset download completion
- Adds soft-toggle for auto-downloading assets
- Turns "automatically play on download completion" into a togglable setting
- Supports auto-accepting Newgrounds song usage policy to prevent crashes for first-time players

More changes and improvements:

- Uses `geode::createQuickPopup` instead of manually creating `FLAlertLayers`
- Uses `GameVar` for settings aliases
- Adds new setting to control auto-acceptance of Newgrounds song usage policy

## v0.1.4

- Adds `support.md` with links to star/sponsor the project

## v0.1.3

- Fixes handling of sound download popup when closing

## v0.1.2

- Fixes issue with detecting active FLAlertLayer instances on LevelInfoLayer
- Opening the level leaderboard should not automatically play level anymore
- Adds "Skip" button to downloading popup

## v0.1.1

it's 4 am and im tired

## v0.1.0

- Initial release
