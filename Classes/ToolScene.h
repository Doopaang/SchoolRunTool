#ifndef __TOOL_SCENE_H__
#define __TOOL_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
using namespace ui;

class Character;

class ToolScene : public Layer, public ui::EditBoxDelegate
{
public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(ToolScene);

	// BPM 변수
	int BPM;

	// 보이기 설정 함수
	void setVisibleItem(bool _isTrue);

	// BPM 설정 함수
	void setBPM();

private:
	Size visibleSize;
	Vec2 origin;

	// 캐릭터 클래스 변수
	Character* character;

	// 캐릭터 스프라이트 변수
	Sprite* characterSprite;

	// 에디트 박스 변수
	ui::EditBox* editBox;

	// 노래 제목 변수
	const char* name;

	// 노래 라벨 변수
	Label* nameLabel;
	
	// BPM 증가 메뉴 변수
	MenuItemSprite* menuUp;

	// BPM 증가 라벨 변수
	Label* labelUp;

	// BPM 감소 메뉴 변수
	MenuItemSprite* menuDown;

	// BPM 감소 라벨 변수
	Label* labelDown;

	// BPM 메뉴 변수
	Menu* menu;

	// BPM 라벨 변수
	Label* BPMLabel;

	// 아래 키 누름 변수
	bool downPress;

	// fps 변수
	float fps;

	// 업데이트
	void update(float delta);

	// 키 누름 함수
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)override;

	// 키 땜 함수
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)override;

	// BPM 증가 버튼 콜백 함수
	void BPMUpCallback(Ref* sender);

	// BPM 감소 버튼 콜백 함수
	void BPMDownCallback(Ref* sender);

	// 캐릭터 생성 함수
	void createCharacter();

	// 배경 생성 함수
	void createBackground();

	// 에디트 박스 생성 함수
	void createEditBox();

	// BPM 증가 버튼 생성 함수
	void createBPMUp();

	// BPM 감소 버튼 생성 함수
	void createBPMDown();

	// 메뉴 생성 함수
	void createMenu();

	// BPM 생성 함수
	void createBPM();

	// 레이어 순서 열거형
	enum LAYER_ZORDER
	{
		PREVIEWLAYER, TOOLSCENE, INTERFACELAYER
	};

	// BPM 최대치 상수
	const int BPM_MAX = 3;

protected:
	// 에디트 박스 콜백 함수
	virtual void editBoxReturn(ui::EditBox* editBox);

public:
	// 생성자
	ToolScene();

	// 소멸자
	~ToolScene();
};

#endif
