#include "Character.h"

Character::Character() : state(DEFAULT), frame(NULL), yoyo(FALSE)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	defaultPosition = Vec2(origin.x + visibleSize.width * 0.15, visibleSize.height - visibleSize.width * 9 / 16 + visibleSize.height * 0.1);

	// ĳ���� ����
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/character.plist");

	character = Sprite::create();
	character->setScale(SCALE);
	character->setAnchorPoint(Point(0.5f, 0.0f));
	character->setPosition(defaultPosition);

	// �׼� ����
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
	// �׼� ����
	jumping->release();
	comeback->release();
}

Sprite* Character::action(float* _fps, bool _downPress)
{
	switch (state)
	{
	// ���� ���� ��
	case J_SINGLE:
	case J_DOUBLE:
		if (*_fps >= JUMP_FPS)
		{
			// fps �ʱ�ȭ
			*_fps = NULL;

			// ������ �ѱ��
			animationJump(JUMP_FRAMES);

			// �ִϸ��̼� ���
			animation(JUMP_FRAME_START);
		}
		break;

	// �����̵� ���� ��
	case S_ING:
		if (*_fps >= SLIDE_FPS)
		{
			// fps �ʱ�ȭ
			*_fps = NULL;

			// ������ �ѱ��
			animationSlide(SLIDE_FRAMES, TRUE);

			// �ִϸ��̼� ���
			animation(SLIDE_FRAME_START);
		}
		break;

	// �����̵尡 ������ ��
	case S_ED:
		if (*_fps >= SLIDED_FPS)
		{
			// fps �ʱ�ȭ
			*_fps = NULL;

			// ������ �ѱ��
			animationSlide(SLIDED_FRAMES, FALSE);

			// �ִϸ��̼� ���
			animation(SLIDED_FRAME_START);
		}
		break;

	// �޸��� ���� ��
	default:
		if (*_fps >= RUN_FPS)
		{
			// fps �ʱ�ȭ
			*_fps = NULL;

			// ������ �ѱ��
			animationRun(RUN_FRAMES);

			// �ִϸ��̼� ���
			animation(RUN_FRAME_START);
		}
	}

	return character;
}

void Character::animationRun(const int _FRAMES)
{
	// ������ �ѱ��
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
		// ������ �ѱ��
		frame++;
	}
	else if (frame == _FRAMES - 1 &&
		!_ing)
	{
		// ������ �ѱ��
		frame++;

		// ���� ����
		state = DEFAULT;
	}
}

void Character::animationJump(const int _FRAMES)
{
	// ������ �ѱ��
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
	// ���� �׼�
	if (!_downPress)
	{
		// �̱� ���� �׼�
		if (state < J_SINGLE)
		{
			// �ʱ�ȭ
			state = J_SINGLE;
			yoyo = FALSE;
			resetFrame();

			// ����
			character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", JUMP_FRAME_START)));

			// �׼� ����
			character->runAction(jumping);
		}
		// ���� ���� �׼�
		else if (state < J_DOUBLE)
		{
			// �ʱ�ȭ
			state = J_DOUBLE;
			yoyo = FALSE;
			resetFrame();

			// ����
			character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", JUMP_FRAME_START)));
			character->stopAllActions();

			// �׼� ����
			character->runAction(jumping);
		}
	}
}

void Character::slideArim(bool _downPress, int _BPM)
{
	if (_downPress)
	{
		// �ʱ�ȭ
		state = S_ING;
		yoyo = FALSE;
		resetFrame();

		// ��ġ �ʱ�ȭ
		resetAction();

		// �����̵�
		character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", SLIDE_FRAME_START)));
	}
	else
	{
		// �ʱ�ȭ
		state = S_ED;
		resetFrame();

		// ���ƿ���
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