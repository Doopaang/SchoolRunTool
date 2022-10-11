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

	// BPM ����
	int BPM;

	// ���̱� ���� �Լ�
	void setVisibleItem(bool _isTrue);

	// BPM ���� �Լ�
	void setBPM();

private:
	Size visibleSize;
	Vec2 origin;

	// ĳ���� Ŭ���� ����
	Character* character;

	// ĳ���� ��������Ʈ ����
	Sprite* characterSprite;

	// ����Ʈ �ڽ� ����
	ui::EditBox* editBox;

	// �뷡 ���� ����
	const char* name;

	// �뷡 �� ����
	Label* nameLabel;
	
	// BPM ���� �޴� ����
	MenuItemSprite* menuUp;

	// BPM ���� �� ����
	Label* labelUp;

	// BPM ���� �޴� ����
	MenuItemSprite* menuDown;

	// BPM ���� �� ����
	Label* labelDown;

	// BPM �޴� ����
	Menu* menu;

	// BPM �� ����
	Label* BPMLabel;

	// �Ʒ� Ű ���� ����
	bool downPress;

	// fps ����
	float fps;

	// ������Ʈ
	void update(float delta);

	// Ű ���� �Լ�
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)override;

	// Ű �� �Լ�
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)override;

	// BPM ���� ��ư �ݹ� �Լ�
	void BPMUpCallback(Ref* sender);

	// BPM ���� ��ư �ݹ� �Լ�
	void BPMDownCallback(Ref* sender);

	// ĳ���� ���� �Լ�
	void createCharacter();

	// ��� ���� �Լ�
	void createBackground();

	// ����Ʈ �ڽ� ���� �Լ�
	void createEditBox();

	// BPM ���� ��ư ���� �Լ�
	void createBPMUp();

	// BPM ���� ��ư ���� �Լ�
	void createBPMDown();

	// �޴� ���� �Լ�
	void createMenu();

	// BPM ���� �Լ�
	void createBPM();

	// ���̾� ���� ������
	enum LAYER_ZORDER
	{
		PREVIEWLAYER, TOOLSCENE, INTERFACELAYER
	};

	// BPM �ִ�ġ ���
	const int BPM_MAX = 3;

protected:
	// ����Ʈ �ڽ� �ݹ� �Լ�
	virtual void editBoxReturn(ui::EditBox* editBox);

public:
	// ������
	ToolScene();

	// �Ҹ���
	~ToolScene();
};

#endif
