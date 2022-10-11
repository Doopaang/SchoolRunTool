#ifndef __INTERFACE_LAYER_H__
#define __INTERFACE_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui\UISlider.h"
#include "ui\UIRadioButton.h"
#include "AudioEngine.h"
#include <io.h>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace experimental;
using namespace ui;

class PreviewLayer;
class Note;
class ToolScene;

class InterfaceLayer : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(InterfaceLayer);

	// �� �� ����
	ToolScene* toolScene;

	// ������ ���̾� ����
	PreviewLayer* previewLayer;

	// ��Ʈ ����Ʈ ����
	Note* note;

	// ����� ���̵� ����
	int audioID;

	// �뷡 üũ �Լ�
	void checkSong(const char* _name);

	// ��ũ�� ���� ���� �Լ�
	void setContainer(float _offset);

	// �뷡 �ð� ���� �Լ�
	void setCurrentTime(bool _back);

	// �뷡 ��� ���� �Լ�
	void setPlaySong(bool _select);

	// ��Ʈ ��� ���� ��ȯ �Լ�
	int getSelectList();

	// �ʱ�ȭ �Լ�
	void reset();

private:
	Size visibleSize;
	Vec2 origin;
	
	// �⺻ ��ư ��������Ʈ
	Sprite* buttonNormalSprite;

	// ���� ��ư ��������Ʈ
	Sprite* buttonPressSprite;

	// ���/�Ͻ����� �޴� ����
	MenuItemToggle* menuPlayPause;

	// ���� �޴� ����
	MenuItemSprite* menuSave;

	// �ҷ����� �޴� ����
	MenuItemSprite* menuLoad;

	// �߰� �޴� ����
	MenuItemSprite* menuAdd;

	// �߰� �� ����
	Label* labelAdd;

	// ���� �޴� ����
	MenuItemSprite* menuDelete;

	// ���̵� ���� �޴� ����
	MenuItemSprite* menuUp;

	// ���̵� ���� �޴� ����
	MenuItemSprite* menuDown;

	// �����̴� ����
	Slider* slider;

	// ���� ��ư �׷� ����
	RadioButtonGroup* radioGroup;

	// ��Ʈ ��� �÷� ���̾� ����
	LayerColor* noteListLayer;

	// ��Ʈ ��� ��ũ�� �� ����
	ScrollView* noteListScroll;

	// ��Ʈ ��� ���� �׷� ����
	RadioButtonGroup* noteListRadioGroup;

	// �뷡 �̸� ����
	const char* name;

	// �뷡 ���� ����
	float duration;

	// �뷡 ���� �ð� ����
	float currentTime;

	// �뷡 �� ����
	Label* songLabel;

	// �ð� ����
	int time;

	// �ð� �� ����
	Label* timeLabel;

	// ���� ����
	int level;

	// ���� �� ����
	Label* levelLabel;

	// ��Ʈ ���� ���� ����
	int selectKind;

	// ��Ʈ ��� ���� ����
	int selectList;

	// �����̴� ���� �Լ�
	void createSlider();

	// ���/�Ͻ����� ��ư ���� �Լ�
	void createPlayPause();

	// ��ũ�� �� ���� �Լ�
	void createScroll();

	// ���� ��ư ���� �Լ�
	void createSave();

	// �ҷ����� ��ư ���� �Լ�
	void createLoad();

	// �߰� ��ư ���� �Լ�
	void createAdd();

	// ���� ��ư ���� �Լ�
	void createDelete();

	// ���̵� ���� ��ư ���� �Լ�
	void createLevelUp();

	// ���̵� ���� ��ư ���� �Լ�
	void createLevelDown();

	// ��Ʈ ��� �޴� ���� �Լ�
	void createNoteMenu();

	// �޴� ���� �Լ�
	void createMenu();

	// �뷡 �� ���� �Լ�
	void createSongLabel();

	// �ð� �� ���� �Լ�
	void createTimeLabel();

	// ���̵� �� ���� �Լ�
	void createLevelLabel();

	// ������Ʈ
	void update(float delta);

	// ���/�Ͻ����� �޴� �ݹ� �Լ�
	void playMenuCallback(Ref *sender);

	// ���� �޴� �ݹ� �Լ�
	void saveMenuCallback(Ref *sender);

	// �ҷ����� �޴� �ݹ� �Լ�
	void loadMenuCallback(Ref *sender);

	// �߰� �޴� �ݹ� �Լ�
	void addMenuCallback(Ref *sender);

	// ���� �޴� �ݹ� �Լ�
	void deleteMenuCallback(Ref *sender);

	// ���̵� ���� �޴� �ݹ� �Լ�
	void levelUpMenuCallback(Ref *sender);

	// ���̵� ���� �޴� �ݹ� �Լ�
	void levelDownMenuCallback(Ref *sender);

	// �����̴� �̺�Ʈ ������ �Լ�
	void sliderEvent(Ref *sender, SliderEventType type);

	// ���� ��ư �ݹ� �Լ�
	void radioButtonCallback(Ref *sender);

	// ��Ʈ ��� ���� ��ư �ݹ� �Լ�
	void noteListRadioButtonCallback(Ref *sender);

	// �뷡 �ε� ���� �ݹ� �Լ�
	void InterfaceLayer::songLoadFolderCallback(bool isSuccess);

	// �뷡 �ε� �ݹ� �Լ�
	void InterfaceLayer::songLoadCallback(bool isSuccess);

	// �뷡 ���� �ݹ� �Լ�
	void songFinishCallback();

	// �뷡�� ���� �����̴� ���� �Լ�
	void setBySong();

	// �ð� ����
	void setTime();

	// ���ڿ� ��ȯ �Լ�
	char* getString(const int _NUM);

	// ��Ʈ ��� ������ ���� �Լ�
	void setNoteListSize(int _size);

	// ��Ʈ ��� ���� ��ư ���� �Լ�
	void removeNoteListRadioButton(int _tag);

	// ��Ʈ ��� ���� ��ư ���� �Լ�
	void createNoteListRadioButton();

	// ��Ʈ ��� ���� ���� �Լ�
	void setSelectList();

	// �뷡 ���� ���� �Լ�
	void saveData();

	// �뷡 ���� �ҷ����� �Լ�
	void loadData();

	// ���� �̵� �Լ�
	void moveFile(const char* _pastDir, const char* _newDir);

	// ���� ��ư ���� ���
	const int RADIO_HEIGHT = 32;

	// ���� ��ư �ʺ� ���
	const int RADIO_WIDTH = 160;

	// ��ũ�� �� ���� ���
	const int SCROLL_HEIGHT = 200;

	// ��ũ�� �� �ʺ� ���
	const int SCROLL_WIDTH = 180;

	// �ð� �ѱ�� ���
	const float SKIP = 3.0f;

	// ���̵� �ִ�ġ ���
	const int LEVEL_MAX = 3;

public:
	// ������
	InterfaceLayer();

	// �Ҹ���
	~InterfaceLayer();
};

#endif
