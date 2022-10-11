#ifndef __CHARACTER_INFO_H__
#define __CHARACTER_INFO_H__

#include "cocos2d.h"

USING_NS_CC;

class Character
{
public:
	// ĳ���� ����
	int state;

	// �׼�
	Sprite* action(float* _fps, bool _downPress);

	// ���� �˸�
	void jumpArim(bool _downPress, int _BPM);

	// �����̵� �˸�
	void slideArim(bool _downPress, int _BPM);

	// ������ �ʱ�ȭ �Լ�
	void resetFrame();

	// �׼� �ӵ� ���� �Լ�
	void setActionSpeed(int _BPM);

	// ���
	enum STATE
	{
		DEFAULT, S_ING, S_ED, J_SINGLE, J_DOUBLE
	};

private:
	Size visibleSize;
	Vec2 origin;

	// �⺻ ������
	Vec2 defaultPosition;

	// ��������Ʈ
	Sprite* character;

	// �ִϸ��̼� ������
	int frame;

	// ���� ����
	bool yoyo;

	// ���ƿ��� �׼�
	MoveTo* comeback;

	// ������ �׼�
	Sequence* jumping;

	// ���
	const int RUN_FRAME_START = 1;
	const int RUN_FRAMES = 6;
	const int JUMP_FRAME_START = 7;
	const int JUMP_FRAMES = 5;
	const int SLIDE_FRAME_START = 12;
	const int SLIDE_FRAMES = 2;
	const int SLIDED_FRAME_START = 14;
	const int SLIDED_FRAMES = 1;
	const int FALL_FRAME_START = 16;
	const int FALL_FRAMES = 6;

	const float JUMP_TIME = 0.9f;
	const float EASE = 4.0f;

	const int JUMP_HEIGHT = 200;

	const int RUN_FPS = 10;
	const int JUMP_FPS = 7;
	const int SLIDE_FPS = 8;
	const int SLIDED_FPS = 8;
	const int FALL_FPS = 9;

	const float SCALE = 0.75;

	// �޸��� �ִϸ��̼�
	void animationRun(const int _FRAMES);

	// �����̵� �ִϸ��̼�
	void animationSlide(const int _FRAMES, bool _ing);

	// ���� �ִϸ��̼�
	void animationJump(const int _FRAMES);

	// �ִϸ��̼� ����
	void animation(const int _FRAME_START);

	// �׼� ���� �Լ�
	void resetAction();

	// ���� ���� �ݹ� �Լ�
	void jumpEndCallback();

public:
	// ������
	Character();

	// �Ҹ���
	~Character();
};

#endif
