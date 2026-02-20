#include <Geode/ui/GeodeUI.hpp>
#include "AboutMDPopup.hpp"

using namespace geode::prelude;

bool AboutMDPopup::init() {
	if (!geode::Popup::init(440.f, 280.f, "GJ_square01.png")) return false;

	m_notifc = nullptr;
	const std::filesystem::path mdFile = Mod::get()->getSettingValue<std::filesystem::path>("aboutMDFile");
	const std::string& filePathString = pathToString(mdFile);
	const std::string& fileName = pathToString(mdFile.filename());
	const std::string& parentPath = pathToString(mdFile.parent_path().filename());

	this->setTag(7042025);
	this->setID("AboutMDSimulatorPopup"_spr);

	this->setTitle("AboutMDSimulator");
	m_originalTitleScale = m_title->getScale();

	m_tmr = CCLabelBMFont::create(fmt::format("", m_privateFloatVariableForAutoRelod).c_str(), "chatFont.fnt");
	m_mainLayer->addChildAtPosition(m_tmr, Anchor::Center);
	m_tmr->setPositionY(-7.5f);
	m_tmr->setScale(.75f);

	updateTitle();

	m_autoReloadIntervalInSeconds = Mod::get()->getSettingValue<double>("autoReloadInterval");

	const CCSize size = {
		// (440 - 20 - 145 - 7.5) / .85
		(m_originalMainLayerWidth - m_mainContainerPadding - m_originalLeftColumnWdth - m_originalLayoutGapWidth) / m_originalMDTxtAreaScale,
		// (280 - 20 - 30) / .85
		(m_originalMainLayerHight - m_mainContainerPadding - m_morHghtAdjustmentMagic) / m_originalMDTxtAreaScale
	};

	const std::string& fileContents = file::readString(filePathString).unwrapOr("fooBar"_spr);
	if (fileContents == "fooBar"_spr) {
		m_notifc = geode::Notification::create("Oh no! Something went wrong with parsing your Markdown file. Try again", NotificationIcon::Error, 10.f);
		m_notifc->show();
	}

	m_textArea = MDTextArea::create(fileContents, size);
	m_textArea->setScale(m_originalMDTxtAreaScale);

	m_mainLayer->addChildAtPosition(m_textArea, Anchor::Center);
	m_textArea->setPositionY(m_textArea->getPositionY() - 10.f);

	CCSprite* optionsButtonSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	optionsButtonSprite->setTag(7042025);
	optionsButtonSprite->setScale(.750f);

	CCMenuItemSpriteExtra* settingsButton = CCMenuItemSpriteExtra::create(optionsButtonSprite, this, menu_selector(AboutMDPopup::onModSettings));
	m_buttonMenu->addChild(settingsButton);

	CCSprite* refreshButtonSprite = CCSprite::createWithSpriteFrameName("GJ_getSongInfoBtn_001.png");
	refreshButtonSprite->setTag(7042025);
	refreshButtonSprite->setScale(.975f);

	CCMenuItemSpriteExtra* refreshButton = CCMenuItemSpriteExtra::create(refreshButtonSprite, this, menu_selector(AboutMDPopup::onRefreshPreview));
	m_buttonMenu->addChild(refreshButton);

	settingsButton->setPosition({m_closeBtn->getPositionX(), 3.f});
	refreshButton->setPosition({m_mainLayer->getContentWidth() - 3.f, settingsButton->getPositionY()});

	this->scheduleUpdate();

	return true;
}

void AboutMDPopup::update(float dt) {
	if (m_autoReloadIntervalInSeconds < m_hardcodedMinimumRefrsh) return;
	m_privateFloatVariableForAutoRelod += dt;
	updateTimer();

	if (m_privateFloatVariableForAutoRelod < m_autoReloadIntervalInSeconds) return;
	updatePreview();
}

void AboutMDPopup::onRefreshPreview(CCObject* sender) {
	if (sender && sender->getTag() == 7042025 && CCScene::get()->getChildByTag(7042025) && CCScene::get()->getChildByID("AboutMDSimulatorPopup"_spr)) updatePreview();
}

void AboutMDPopup::updatePreview() {
	const std::filesystem::path file = Mod::get()->getSettingValue<std::filesystem::path>("aboutMDFile");
	if (!std::filesystem::exists(file) || !utils::string::endsWith(pathToString(file), ".md") || !m_textArea) {
		if (m_notifc) m_notifc->cancel();
		m_notifc = geode::Notification::create("File either doesnt exist, or does not end with \".md\". Try again.", NotificationIcon::Error, 10.f);
		return m_notifc->show();
	}

	m_privateFloatVariableForAutoRelod = 0.f;
	m_autoReloadIntervalInSeconds = Mod::get()->getSettingValue<double>("autoReloadInterval");
	const std::string& fileContents = file::readString(pathToString(Mod::get()->getSettingValue<std::filesystem::path>("aboutMDFile"))).unwrapOr("fooBar"_spr);
	if (fileContents == "fooBar"_spr) {
		if (m_notifc) m_notifc->cancel();
		m_notifc = geode::Notification::create("Invalid file contents! Try again.", NotificationIcon::Error, 10.f);
		m_notifc->show();
	}

	m_textArea->setString(fileContents.c_str());
	updateTitle();
}

void AboutMDPopup::updateTitle() {
	const std::filesystem::path mdFile = Mod::get()->getSettingValue<std::filesystem::path>("aboutMDFile");
	const std::string& fileName = pathToString(mdFile.filename());
	const std::string& parentPath = pathToString(mdFile.parent_path().filename());

	this->setTitle(fmt::format("AboutMDSimulator - {}/{}", parentPath, fileName));
	m_title->limitLabelWidth(m_mainLayer->getContentWidth() * .9f, m_originalTitleScale, .0001f);

	updateTimer();
}

void AboutMDPopup::updateTimer() {
	if (m_autoReloadIntervalInSeconds >= m_hardcodedMinimumRefrsh) m_tmr->setString(fmt::format("Next refresh: {:0.0}s", m_autoReloadIntervalInSeconds - m_privateFloatVariableForAutoRelod).c_str());
	else m_tmr->setString("");
}

void AboutMDPopup::onExit() {
	if (m_notifc) m_notifc->cancel();
	Popup::onExit();
}

void AboutMDPopup::onModSettings(CCObject* sender) {
	// i don't want any sleazy mfs callbacking this nilly willy
	if (sender && sender->getTag() == 7042025 && CCScene::get()->getChildByTag(7042025) && CCScene::get()->getChildByID("AboutMDSimulatorPopup"_spr)) openSettingsPopup(Mod::get());
}


AboutMDPopup* AboutMDPopup::create() {
	AboutMDPopup* ret = new AboutMDPopup();
	if (ret->init())) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}