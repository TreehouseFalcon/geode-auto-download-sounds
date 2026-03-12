#include <Geode/Enums.hpp>
#include <Geode/Geode.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/binding/CustomSongDelegate.hpp>
#include <Geode/binding/CustomSongWidget.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/binding/SongInfoObject.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/binding/MusicDownloadManager.hpp>
#include "../managers/SettingsManager.hpp"
#include "AutoDownloadCustomSongWidget.hpp"
#include "AutoDownloadLevelInfoLayer.hpp"

using namespace geode::prelude;

bool AutoDownloadCustomSongWidget::init(SongInfoObject* songInfo, CustomSongDelegate* songDelegate, bool showSongSelect, bool showPlayMusic, bool showDownload, bool isRobtopSong, bool unkBool, bool isMusicLibrary, int unk) {
	if (!CustomSongWidget::init(songInfo, songDelegate, showSongSelect, showPlayMusic, showDownload, isRobtopSong, unkBool, isMusicLibrary, unk)) {
		return false;
	}

	// Download on level view for levels with already fetched song info //
	this->runAction(CCSequence::create(
		CCDelayTime::create(0.f),
		CCCallFunc::create(this, callfunc_selector(AutoDownloadCustomSongWidget::downloadSongsOnLevelView)),
		nullptr
	));

	return true;
}

// Prevents Newgrounds policy crash (@RayDeeUx) //
void AutoDownloadCustomSongWidget::forceAcceptNewgroundsPolicy() {
	if (Settings::shouldAutoAcceptNewgroundsPolicy() && !GameManager::get()->getGameVariable(GameVar::ShownMusicTOS)) {
		GameManager::get()->setGameVariable(GameVar::ShownMusicTOS, true);
	}
}

void AutoDownloadCustomSongWidget::loadSongInfoFinished(SongInfoObject* songInfo) {
	CustomSongWidget::loadSongInfoFinished(songInfo);

	// Download on level view for levels where song info fetching is required //
	geode::queueInMainThread([this] {
		downloadSongsOnLevelView();
	});
}

AutoDownloadLevelInfoLayer* AutoDownloadCustomSongWidget::getAutoDownloadLevelInfoLayer() {
	auto scene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
	if (!scene) return nullptr;

	auto* base = scene->getChildByType<LevelInfoLayer>(0);
	if (!base) return nullptr;

	return static_cast<AutoDownloadLevelInfoLayer*>(base);
}

void AutoDownloadCustomSongWidget::downloadSongsOnLevelView() {
	if (!Settings::shouldDownloadSoundsOnLevelView()) return;
	if (!GameManager::get()->getGameVariable(GameVar::ShownMusicTOS)) return;

	forceAcceptNewgroundsPolicy();

	if (!m_fields->m_startedAutoDownload && m_downloadBtn) {
		m_fields->m_startedAutoDownload = true;

		if (m_downloadBtn->isVisible()) {
			m_downloadBtn->activate();
		}
	}
}

void AutoDownloadCustomSongWidget::downloadSongsOnLevelPlay() {
	if (!GameManager::get()->getGameVariable(GameVar::ShownMusicTOS)) return;
	bool downloadButtonVisible = m_downloadBtn && m_downloadBtn->isVisible();
	auto autoDownloadLevelInfoLayer = getAutoDownloadLevelInfoLayer();
	if ((downloadButtonVisible || m_fields->m_startedAutoDownload) && autoDownloadLevelInfoLayer) {
		autoDownloadLevelInfoLayer->showDownloadingPopup();
	}

	if (!(Settings::shouldDownloadSoundsOnLevelPlay() || Settings::shouldDownloadSoundsOnLevelView())) return;

	if (!m_fields->m_startedAutoDownload && downloadButtonVisible) {
		forceAcceptNewgroundsPolicy();
		m_fields->m_startedAutoDownload = true;
		m_downloadBtn->activate();
	}
}

void AutoDownloadCustomSongWidget::tryPlayIfInLevelInfo() {
	auto scene = CCScene::get();
	if (!scene) return;

	LevelInfoLayer* levelInfo = scene->getChildByType<LevelInfoLayer>(0);
	if (!levelInfo) return;

	AutoDownloadLevelInfoLayer* autoLayer = static_cast<AutoDownloadLevelInfoLayer*>(levelInfo);
	if (!autoLayer) return;

	autoLayer->tryPlayIfDownloadingPopupShown();
}

void AutoDownloadCustomSongWidget::allAudiosDownloaded() {
	auto scene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
	if (!scene) return;

	m_deleteBtn->setVisible(true);

	AutoDownloadLevelInfoLayer* autoLayer;
	if (auto* base = scene->getChildByType<LevelInfoLayer>(0)) {
		autoLayer = static_cast<AutoDownloadLevelInfoLayer*>(base);
		if (!autoLayer) return;
	}

	if (Settings::shouldAutoPlayOnDownloadFinish()) {
		tryPlayIfInLevelInfo();
	}

	autoLayer->closeDownloadingPopup();
}

void AutoDownloadCustomSongWidget::showError(bool p0) {
	CustomSongWidget::showError(p0);

	auto levelInfoLayer = CCScene::get()->getChildByType<LevelInfoLayer>(0);
	if (!levelInfoLayer || !levelInfoLayer->m_songWidget || this != levelInfoLayer->m_songWidget) return;

	if (!m_errorLabel->isVisible() || utils::string::toLower(m_errorLabel->getString()) != "download complete.") return;
	if (m_errorLabel->getColor().r != 0 || m_errorLabel->getColor().b != 0 || m_errorLabel->getColor().g != 255) return;

	auto undownloadedAssetCount = m_undownloadedAssets.size();
	if (undownloadedAssetCount == 0 || (undownloadedAssetCount == 1 && m_undownloadedAssets.at(0).m_id == m_songInfoObject->m_songID)) {
		allAudiosDownloaded();
	}
}

void AutoDownloadCustomSongWidget::startDownload() {
	m_fields->m_startedAutoDownload = true;
	CustomSongWidget::startDownload();
}

void AutoDownloadCustomSongWidget::retriggerDownloadButton() {
	if (m_fields->m_startedAutoDownload && m_downloadBtn && geode::cocos::nodeIsVisible(m_downloadBtn)) {
		m_downloadBtn->activate();
	}
}

void AutoDownloadCustomSongWidget::downloadSongFinished(int id) {
	CustomSongWidget::downloadSongFinished(id);
	retriggerDownloadButton();
}

void AutoDownloadCustomSongWidget::downloadSFXFinished(int id) {
	CustomSongWidget::downloadSFXFinished(id);
	retriggerDownloadButton();
}

void AutoDownloadCustomSongWidget::updateWithMultiAssets(gd::string songList, gd::string sfxList, int bytes) {
	CustomSongWidget::updateWithMultiAssets(songList, sfxList, bytes);
	retriggerDownloadButton();
}