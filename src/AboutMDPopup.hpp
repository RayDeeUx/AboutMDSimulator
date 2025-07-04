#pragma once

class AboutMDPopup final : public geode::Popup<const std::string&> {
	const float m_mainContainerPadding   =  20.f;
	const float m_heightAdjustmentMagic  =  15.f;
	const float m_morHghtAdjustmentMagic =  30.f;
	/*******************************************/
	const float m_originalMainLayerWidth = 440.f;
	const float m_originalLeftColumnWdth = 145.f;
	const float m_originalLayoutGapWidth =  7.5f;
	/*******************************************/
	const float m_originalMainLayerHight = 280.f;
	/*******************************************/
	const float m_originalMDTxtAreaScale =  .85f;
	/*******************************************/
	const float m_hardcodedMinimumRefrsh =  10.f;
	/*******************************************/
	float m_autoReloadIntervalInSeconds      =   1.f;
	float m_privateFloatVariableForAutoRelod =   0.f;
	float m_originalTitleScale               =   0.f;
protected:
	geode::MDTextArea* m_textArea {};
	geode::Notification* m_notifc {};
	cocos2d::CCLabelBMFont* m_tmr {};
	bool setup(const std::string &) override;
	void update(float dt) override;
	void updatePreview();
	void updateTitle();
	void updateTimer();
	bool setup();
public:
	static AboutMDPopup* create();
	void onModSettings(CCObject* sender);
	void onRefreshPreview(CCObject* sender);
	void onExit() override;
	static std::string pathToString(const std::filesystem::path& path) {
        #ifdef GEODE_IS_WINDOWS
		return geode::utils::string::wideToUtf8(path.wstring());
        #else
		return path.string();
        #endif
	}
};