#include "PreviewLayer.h"
#include "InterfaceLayer.h"
#include "Note.h"
#include "ToolScene.h"

PreviewLayer::PreviewLayer()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
}

bool PreviewLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	CCLOG("PreviewLayer::init()");

	// ��ũ�� �� ����
	createScroll();

	// ��� ȭ�� ���
	printBackground();

	// ������Ʈ
	this->scheduleUpdate();

    return true;
}

void PreviewLayer::update(float delta)
{
	// ���ȭ�� �̵�
	moveBackground();

	// ��ũ�� �信 ���� ����
	setByScroll();
}

void PreviewLayer::createScroll()
{
	// �÷� ���̾� ����
	layer = LayerColor::create(Color4B(255, 255, 255, 255));
	layer->setContentSize(Size(960, 540));

	// ��ũ�� �� ���
	scroll = ScrollView::create(Size(960, 540), layer);
	scroll->setAnchorPoint(Point::ZERO);
	scroll->setPosition(Vec2(origin.x, origin.y + visibleSize.height - visibleSize.width * 9 / 16));
	scroll->setDirection(ScrollView::Direction::HORIZONTAL);
	scroll->setBounceable(false);
	this->addChild(scroll);
}

void PreviewLayer::printBackground()
{
	// ���ȭ�� ���
	background = Sprite::create("./img/ingame_background.png");
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Point::ZERO);
	background->setScale(0.75f);
	layer->addChild(background);

	// ���ȭ�� ���� ���
	backgroundSub = Sprite::create("./img/ingame_background.png");
	backgroundSub->setAnchorPoint(Point::ZERO);
	backgroundSub->setPosition(Point(background->getContentSize().width * 0.75, 0));
	backgroundSub->setScale(0.75f);
	layer->addChild(backgroundSub);
}

void PreviewLayer::setDuration(float _time)
{
	// �뷡 ���� ����
	duration = _time;

	// ���̾� ũ�� ����
	layer->setContentSize(Size(960 + 60 * ((toolScene->BPM + 1) * 5) * duration * 0.75, 540));
}

void PreviewLayer::setCurrentTime(float _time)
{
	if (currentTime != _time)
	{
		// �뷡 ���� �ð� ����
		currentTime = _time;
	}
}

void PreviewLayer::moveBackground()
{
	float offset = 0 - scroll->getContentOffset().x;
	float position = background->getPositionX();
	float size = background->getContentSize().width * 0.75;
	float positionSub = backgroundSub->getPositionX();
	float sizeSub = backgroundSub->getContentSize().width * 0.75;

	// ���ȭ�� �ű��
	if (offset > position + size)
	{
		background->setPositionX(positionSub + sizeSub);
	}
	else if (offset + visibleSize.width < position)
	{
		background->setPositionX(positionSub - size);
	}

	// ���ȭ�� ���� �ű��
	if (offset > positionSub + sizeSub)
	{
		backgroundSub->setPositionX(position + size);
	}
	else if (offset + visibleSize.width < positionSub)
	{
		backgroundSub->setPositionX(position - sizeSub);
	}
}

void PreviewLayer::setByScroll()
{
	if (scroll->isTouchMoved())
	{
		// ��ũ�ѿ� ���� �뷡 ����
		interfaceLayer->setContainer(0 - scroll->getContentOffset().x / 3 * 4);
	}
	else
	{
		// �뷡�� ���� ��ũ�� ����
		scroll->setContentOffset(Vec2(-1 * 60 * ((toolScene->BPM + 1) * 5) * currentTime * 0.75, 0));
	}
}

void PreviewLayer::printNote(unsigned int _time)
{
	Note::List* temp = note->searchList(NULL);

	// ��������Ʈ ���
	temp->noteSprite->setPositionX(960 + _time * (toolScene->BPM + 1) * 5 * 0.75);
	layer->addChild(temp->noteSprite, 1);

	// ���� �� ��������Ʈ ���
	if (note->checkSub(temp, Note::SUB::BACK))
	{
		temp->subBackSprite->setPositionX(960 + _time * (toolScene->BPM + 1) * 5 * 0.75);
		layer->addChild(temp->subBackSprite, 0);
	}

	// ���� �� ��������Ʈ ���
	if (note->checkSub(temp, Note::SUB::FRONT))
	{
		temp->subFrontSprite->setPositionX(960 + _time * (toolScene->BPM + 1) * 5 * 0.75);
		layer->addChild(temp->subFrontSprite, 2);
	}
}

void PreviewLayer::removeNote(int _tag)
{
	Note::List* temp = note->searchList(_tag);

	// ��������Ʈ ����
	temp->noteSprite->setPositionX(temp->noteSprite->getPositionX() + scroll->getContentOffset().x * -1);
	layer->removeChild(temp->noteSprite);

	// ���� �� ��������Ʈ ����
	if (note->checkSub(temp, Note::SUB::BACK))
	{
		temp->subBackSprite->setPositionX(temp->subBackSprite->getPositionX() + scroll->getContentOffset().x * -1);
		layer->removeChild(temp->subBackSprite);
	}

	// ���� �� ��������Ʈ ����
	if (note->checkSub(temp, Note::SUB::FRONT))
	{
		temp->subFrontSprite->setPositionX(temp->subFrontSprite->getPositionX() + scroll->getContentOffset().x * -1);
		layer->removeChild(temp->subFrontSprite);
	}
}