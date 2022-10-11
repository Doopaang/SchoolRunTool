#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"
#include "ui\UIRadioButton.h"

USING_NS_CC;

struct List;
class PreviewLayer;

class Note
{
public:
	// ��Ʈ ��ũ�� ����Ʈ ����ü
	struct List
	{
		int noteTag;
		unsigned int noteTime;
		int noteKind;
		Sprite* noteSprite;
		Sprite* subBackSprite;
		Sprite* subFrontSprite;
		ui::RadioButton* noteRadio;
		Label* noteLabel;
		List* next;
	};

	// ������ ���̾� ����
	PreviewLayer* previewLayer;

	// ����Ʈ �߰� �Լ�
	void addList(unsigned int _time, int _kind);

	// ����Ʈ ���� ��ȯ �Լ�
	int getListSize();

	// ����Ʈ �±� ���� �Լ�
	void setListTag();

	// ����Ʈ �˻� �Լ�
	List* searchList(int _tag);

	// ����Ʈ ���� �Լ�
	void delList(int _tag);

	// ���� ��ư ���� �Լ�
	void setRadio(int _tag, ui::RadioButton* _radio, Label* _label);

	// ���� ���� ��ȯ �Լ�
	bool checkSub(List* _temp, bool _sub);

	// ����Ʈ ���� �Լ�
	void saveList(FILE* _fp);

	// ����Ʈ �ҷ����� �Լ�
	void loadList(FILE* _fp);

	// ��Ʈ ���� ������
	enum NOTE_KIND
	{
		NONE, TIRE, BOX, TRASH, BARRICADE, BIKE, SIGN, LONGBOX, LONGAD, HUMAN, NAVIGATOR, CONTAINER, STONE, CRAIN, H_BEAM, END
	};

	// ���� ���� ������
	enum SUB
	{
		BACK, FRONT
	};

private:
	Size visibleSize;
	Vec2 origin;

	// ��ũ�� ����Ʈ �Ӹ� ����
	List* head;

	// ��ũ�� ����Ʈ ���� ����
	List* tail;

	// ���� ����
	FILE* file;

	// ���� �� ��������Ʈ ���� �Լ�
	Sprite* getSubBackSprite(List* _temp);

	// ���� �� ��������Ʈ ���� �Լ�
	Sprite* getSubFrontSprite(List* _temp);

	// ��������Ʈ ���� �Լ�
	Sprite* getNoteSprite(List* _temp);

	// ������ �ۼ�Ʈ ���
	const float SCALE = 0.75f;

public:
	// ������
	Note();

	// �Ҹ���
	~Note();
};

#endif
