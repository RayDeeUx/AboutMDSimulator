#include <Geode/modify/MenuLayer.hpp>
#include "AboutMDPopup.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

		CCNode* rightSideMenu = this->getChildByID("right-side-menu");
		if (!rightSideMenu) return true;

		CCMenuItemSpriteExtra* aboutMDButton = CCMenuItemSpriteExtra::create(CircleButtonSprite::create(CCSprite::create("aboutMD.png"_spr), CircleBaseColor::DarkPurple), this, menu_selector(MyMenuLayer::onAboutMDSimulator));
		aboutMDButton->setID("aboutmd-button"_spr);
		rightSideMenu->addChild(aboutMDButton);
		rightSideMenu->updateLayout();

		return true;
	}

	void onAboutMDSimulator(CCObject* sender) {
		if (!sender || !typeinfo_cast<CCNode*>(sender) || static_cast<CCNode*>(sender)->getID() != "aboutmd-button"_spr) return FLAlertLayer::create("Really now? Sheesh...", "<cy>Click on the button like a normal person.</c>\n\n<c_>Please.</c>", "Aww man :(")->show();
		if (!std::filesystem::exists(Mod::get()->getSettingValue<std::filesystem::path>("aboutMDFile"))) return FLAlertLayer::create("Oh no!", "The file you specified was not found.\n\n<cy>Try again.</c>", "Aww man :(")->show();
		if (const std::string& filePath = pathToString(Mod::get()->getSettingValue<std::filesystem::path>("aboutMDFile")); !utils::string::endsWith(filePath, ".md")) return FLAlertLayer::create("Come on already!", "The file you specified was not a Markdown file.\n\n<c_>GET YOUR ACT TOGETHER, FOR CRYING OUT LOUD!</c>", "Aww man :(")->show();
		AboutMDPopup::create()->show();
	}
};