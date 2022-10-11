#include "Character.h"

Character::Character() : state(DEFAULT), frame(NULL), yoyo(FALSE)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	defaultPosition = Vec2(origin.x + visibleSize.width * 0.15, visibleSize.height - visibleSize.width * 9 / 16 + visibleSize.height * 0.1);

	// 캐릭터 설정
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/character.plist");

	character = Sprite::create();
	character->setScale(SCALE);
	character->setAnchorPoint(Point(0.5f, 0.0f));
	character->setPosition(defaultPosition);

	// 액션 설정
	auto action_2 = MoveBy::create(JUMP_TIME * 0.5, Point(0, JUMP_HEIGHT));
	auto action_3 = EaseOut::create(action_2, EASE);
	auto action_4 = MoveTo::create(JUMP_TIME * 0.5, defaultPosition);
	auto action_5 = EaseIn::create(action_4, EASE);
	auto action_6 = CallFuncN::create((CC_CALLBACK_0(Character::jumpEndCallback, this)));
	jumping = Sequence::create(action_3, action_5, action_6, NULL);
	jumping->retain();

	comeback = MoveTo::create(0.2f, defaultPosition);
	comeback->retain();
}

Character::~Character()
{
	// 액션 해제
	jumping->release();
	comeback->release();
}

Sprite* Character::action(float* _fps, bool _downPress)
{
	switch (state)
	{
	// 점프 중일 때
	case J_SINGLE:
	case J_DOUBLE:
		if (*_fps >= JUMP_FPS)
		{
			// fps 초기화
			*_fps = NULL;

			// 프레임 넘기기
			animationJump(JUMP_FRAMES);

			// 애니메이션 출력
			animation(JUMP_FRAME_START);
		}
		break;

	// 슬라이드 중일 때
	case S_ING:
		if (*_fps >= SLIDE_FPS)
		{
			// fps 초기화
			*_fps = NULL;

			// 프레임 넘기기
			animationSlide(SLIDE_FRAMES, TRUE);

			// 애니메이션 출력
			animation(SLIDE_FRAME_START);
		}
		break;

	// 슬라이드가 끝났을 때
	case S_ED:
		if (*_fps >= SLIDED_FPS)
		{
			// fps 초기화
			*_fps = NULL;

			// 프레임 넘기기
			animationSlide(SLIDED_FRAMES, FALSE);

			// 애니메이션 출력
			animation(SLIDED_FRAME_START);
		}
		break;

	// 달리는 중일 때
	default:
		if (*_fps >= RUN_FPS)
		{
			// fps 초기화
			*_fps = NULL;

			// 프레임 넘기기
			animationRun(RUN_FRAMES);

			// 애니메이션 출력
			animation(RUN_FRAME_START);
		}
	}

	return character;
}

void Character::animationRun(const int _FRAMES)
{
	// 프레임 넘기기
	if (frame >= _FRAMES - 1)
	{
		resetFrame();
	}
	else
	{
		frame++;
	}
}

void Character::animationSlide(const int _FRAMES, bool _ing)
{
	if (frame < _FRAMES - 1)
	{
		// 프레임 넘기기
		frame++;
	}
	else if (frame == _FRAMES - 1 &&
		!_ing)
	{
		// 프레임 넘기기
		frame++;

		// 상태 설정
		state = DEFAULT;
	}
}

void Character::animationJump(const int _FRAMES)
{
	// 프레임 넘기기
	if (yoyo)
	{
		if (frame <= NULL + 1)
		{
			state = DEFAULT;
			yoyo = FALSE;
			frame--;
		}
		else
		{
			frame--;
		}
	}
	else
	{
		if (frame >= _FRAMES - 1)
		{
			yoyo = TRUE;
			frame--;
		}
		else
		{
			frame++;
		}
	}
}

void Character::animation(const int _FRAME_START)
{
	character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", frame + _FRAME_START)));
}

void Character::resetAction()
{
	character->stopAllActions();
	character->runAction(comeback);
}

void Character::jumpArim(bool _downPress, int _BPM)
{
	// 점프 액션
	if (!_downPress)
	{
		// 싱글 점프 액션
		if (state < J_SINGLE)
		{
			// 초기화
			state = J_SINGLE;
			yoyo = FALSE;
			resetFrame();

			// 점프
			character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", JUMP_FRAME_START)));

			// 액션 설정
			character->runAction(jumping);
		}
		// 더블 점프 액션
		else if (state < J_DOUBLE)
		{
			// 초기화
			state = J_DOUBLE;
			yoyo = FALSE;
			resetFrame();

			// 점프
			character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", JUMP_FRAME_START)));
			character->stopAllActions();

			// 액션 설정
			character->runAction(jumping);
		}
	}
}

void Character::slideArim(bool _downPress, int _BPM)
{
	if (_downPress)
	{
		// 초기화
		state = S_ING;
		yoyo = FALSE;
		resetFrame();

		// 위치 초기화
		resetAction();

		// 슬라이드
		character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", SLIDE_FRAME_START)));
	}
	else
	{
		// 초기화
		state = S_ED;
		resetFrame();

		// 돌아오기
		character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", SLIDED_FRAME_START)));
	}
}

void Character::resetFrame()
{
	frame = NULL;
}

void Character::setActionSpeed(int _BPM)
{
	Director::getInstance()->getScheduler()->setTimeScale((float)(_BPM + 1) / 2);
}

void Character::jumpEndCallback()
{
	CCLOG("jumpEndCallback");

	switch (state)
	{
	case J_SINGLE:
	case J_DOUBLE:
		state = DEFAULT;
		break;
	}
}