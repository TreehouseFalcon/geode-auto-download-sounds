#include "AutoDownloadLevelInfoLayer.hpp"
#include "AutoDownloadCustomSongWidget.hpp"
#include <Geode/Bindings.hpp>
#include <Geode/Enums.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include "../managers/SettingsManager.hpp"

void AutoDownloadLevelInfoLayer::levelDownloadFinished(GJGameLevel *level) {
    LevelInfoLayer::levelDownloadFinished(level);
    if (level->m_normalPercent.value() != 100) {
        // geode::queueInMainThread([this] {
        //     log::info("PLAYING");
        // });
        onPlay(nullptr);
    }
}

void AutoDownloadLevelInfoLayer::showDownloadingPopup() {
    if (Settings::shouldDownloadSoundsNever()) return;
    if (m_fields->downloadingPopup != nullptr) closeDownloadingPopup();

    auto popup = geode::createQuickPopup(
        "DOWNLOADING AUDIO",
        "Please wait for audio assets to download...",
        "Back", "Skip",
        [this](auto, bool shouldSkip) {
            if (shouldSkip) {
                m_fields->skipTriggered = true;
                bool disableSongAlertState = GameManager::get()->getGameVariable(GameVar::DisableSongAlert);
                if (!disableSongAlertState) {
                    GameManager::get()->setGameVariable(GameVar::DisableSongAlert, true);
                }
                onPlay(nullptr);
                GameManager::get()->setGameVariable(GameVar::DisableSongAlert, disableSongAlertState);
            }
        },
        true, true
    );

    m_fields->downloadingPopup = popup;
}

void AutoDownloadLevelInfoLayer::closeDownloadingPopup() {
    auto downloadingPopup = m_fields->downloadingPopup;
    if (downloadingPopup != nullptr) {
        downloadingPopup->removeFromParentAndCleanup(true);
        m_fields->downloadingPopup = nullptr;
    }
}

void AutoDownloadLevelInfoLayer::tryPlayIfDownloadingPopupShown() {
    bool downloadingPopupShown = m_fields->downloadingPopup && m_fields->downloadingPopup->isRunning();
    if (downloadingPopupShown) {
        onPlay(nullptr);
    }
}

void AutoDownloadLevelInfoLayer::onPlay(cocos2d::CCObject* sender) {
    // Disable onPlay behavior if settings don't permit //
    if (Settings::shouldDownloadSoundsNever()) {
        LevelInfoLayer::onPlay(sender);
        return;
    }

    bool disableSongAlertState = GameManager::get()->getGameVariable(GameVar::DisableSongAlert);

    auto scene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
    if (!scene) return;

    CustomSongWidget* songWidget = this->m_songWidget;
    AutoDownloadCustomSongWidget* autoWidget = nullptr;
    if (!songWidget) return;
    autoWidget = static_cast<AutoDownloadCustomSongWidget*>(songWidget);
    if (!autoWidget) return;

    bool songWidgetReady =
        (songWidget->m_deleteBtn && songWidget->m_deleteBtn->isVisible()) ||
        (songWidget->m_errorLabel && utils::string::toLower(songWidget->m_errorLabel->getString()) == "song is not allowed for use.") ||
        !(
            (songWidget->m_cancelDownloadBtn && songWidget->m_cancelDownloadBtn->isVisible()) ||
            (songWidget->m_downloadBtn && songWidget->m_downloadBtn->isVisible())
        );

    bool shouldEnterLevel = songWidgetReady || m_fields->skipTriggered;
    GameManager::get()->setGameVariable(GameVar::DisableSongAlert, true);
    if (shouldEnterLevel) LevelInfoLayer::onPlay(sender);
    GameManager::get()->setGameVariable(GameVar::DisableSongAlert, disableSongAlertState);
    if (shouldEnterLevel) return;

    // Trigger download on level play //
    autoWidget->downloadSongsOnLevelPlay();
}