#ifndef __PREVIEW_LAYER_H__
#define __PREVIEW_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class InterfaceLayer;
class Note;
class ToolScene;

class PreviewLayer : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(PreviewLayer);

	// �� �� ����
	ToolScene* toolScene;

	// �������̽� ���̾� ����
	InterfaceLayer* interfaceLayer;

	// ��Ʈ Ŭ���� ����
	Note* note;

	// �뷡 ���� ����
	void setDuration(float _time);

	// �뷡 ���� �ð� ����
	void setCurrentTime(float _time);

	// ��Ʈ ��� �Լ�
	void printNote(unsigned int _time);

	// ��Ʈ ���� �Լ�
	void removeNote(int _tag);

private:
	Size visibleSize;
	Vec2 origin;

	// �÷� ���̾� ����
	LayerColor* layer;

	// ��ũ�� �� ����
	ScrollView* scroll;

	// ��� ȭ�� ����
	Sprite* background;

	// ��� ȭ�� ���� ����
	Sprite* backgroundSub;

	// �뷡 ���� ����
	float duration;

	// �뷡 ���� �ð� ����
	float currentTime;

	// ������Ʈ
	void update(float delta);

	// ���ȭ�� ��� �Լ�
	void printBackground();

	// ��ũ�� �� ���� �Լ�
	void createScroll();

	// ���ȭ�� �̵� �Լ�
	void moveBackground();

	// ��ũ�ѿ� ���� ����
	void setByScroll();

public:
	// ������
	PreviewLayer();
};

#endif
