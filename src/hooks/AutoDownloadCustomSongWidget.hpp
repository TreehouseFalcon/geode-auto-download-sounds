#pragma once

#include <Geode/Enums.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/binding/SongInfoObject.hpp>
#include <Geode/binding/CustomSongDelegate.hpp>
#include "AutoDownloadLevelInfoLayer.hpp"

using namespace geode::prelude;

class $modify(AutoDownloadCustomSongWidget, CustomSongWidget) {
public:
    struct Fields {
        bool m_startedAutoDownload = false;
        bool m_loadingPopupShown = false;
    };

    bool init(
        SongInfoObject* songInfo,
        CustomSongDelegate* songDelegate,
        bool showSongSelect,
        bool showPlayMusic,
        bool showDownload,
        bool isRobtopSong,
        bool unkBool,
        bool isMusicLibrary,
        int unk,
        bool unkBool2,
        bool unkBool3
    );
    void loadSongInfoFinished(SongInfoObject* songInfo);
    void downloadSongsOnLevelPlay();
    void downloadSongsOnLevelView();
    // void downloadSongFinished(int id);
    // void downloadSFXFinished(int id);
    void downloadAssetFinished(int id, GJAssetType assetType);
    void updateWithMultiAssets(std::string songList, std::string sfxList, int bytes);
    void retriggerDownloadButton();
    void tryPlayIfInLevelInfo();
    void allAudiosDownloaded();
    void showError(bool p0);
    void startDownload();
    void forceAcceptNewgroundsPolicy();
    AutoDownloadLevelInfoLayer* getAutoDownloadLevelInfoLayer();
};
