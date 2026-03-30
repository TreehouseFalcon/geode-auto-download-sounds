#pragma once

#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/binding/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(AutoDownloadLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        Ref<FLAlertLayer> downloadingPopup = nullptr;
        bool skipTriggered = false;
    };

    void levelDownloadFinished(GJGameLevel *level);
    void onPlay(CCObject* sender);
    void showDownloadingPopup();
    void closeDownloadingPopup();
    void tryPlayIfDownloadingPopupShown();
};
