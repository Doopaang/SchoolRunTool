#ifndef __CHARACTER_INFO_H__
#define __CHARACTER_INFO_H__

#include "cocos2d.h"

USING_NS_CC;

class Character
{
public:
	// 캐릭터 상태
	int state;

	// 액션
	Sprite* action(float* _fps, bool _downPress);

	// 점프 알림
	void jumpArim(bool _downPress, int _BPM);

	// 슬라이드 알림
	void slideArim(bool _downPress, int _BPM);

	// 프레임 초기화 함수
	void resetFrame();

	// 액션 속도 설정 함수
	void setActionSpeed(int _BPM);

	// 상수
	enum STATE
	{
		DEFAULT, S_ING, S_ED, J_SINGLE, J_DOUBLE
	};

private:
	Size visibleSize;
	Vec2 origin;

	// 기본 포지션
	Vec2 defaultPosition;

	// 스프라이트
	Sprite* character;

	// 애니메이션 프레임
	int frame;

	// 점프 상태
	bool yoyo;

	// 돌아오는 액션
	MoveTo* comeback;

	// 시퀀스 액션
	Sequence* jumping;

	// 상수
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

	// 달리기 애니메이션
	void animationRun(const int _FRAMES);

	// 슬라이드 애니메이션
	void animationSlide(const int _FRAMES, bool _ing);

	// 점프 애니메이션
	void animationJump(const int _FRAMES);

	// 애니메이션 실행
	void animation(const int _FRAME_START);

	// 액션 관련 함수
	void resetAction();

	// 점프 종료 콜백 함수
	void jumpEndCallback();

public:
	// 생성자
	Character();

	// 소멸자
	~Character();
};

#endif
