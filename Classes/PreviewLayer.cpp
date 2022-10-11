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

	// 스크롤 뷰 생성
	createScroll();

	// 배경 화면 출력
	printBackground();

	// 업데이트
	this->scheduleUpdate();

    return true;
}

void PreviewLayer::update(float delta)
{
	// 배경화면 이동
	moveBackground();

	// 스크롤 뷰에 따라 변경
	setByScroll();
}

void PreviewLayer::createScroll()
{
	// 컬러 레이어 생성
	layer = LayerColor::create(Color4B(255, 255, 255, 255));
	layer->setContentSize(Size(960, 540));

	// 스크롤 뷰 출력
	scroll = ScrollView::create(Size(960, 540), layer);
	scroll->setAnchorPoint(Point::ZERO);
	scroll->setPosition(Vec2(origin.x, origin.y + visibleSize.height - visibleSize.width * 9 / 16));
	scroll->setDirection(ScrollView::Direction::HORIZONTAL);
	scroll->setBounceable(false);
	this->addChild(scroll);
}

void PreviewLayer::printBackground()
{
	// 배경화면 출력
	background = Sprite::create("./img/ingame_background.png");
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Point::ZERO);
	background->setScale(0.75f);
	layer->addChild(background);

	// 배경화면 보조 출력
	backgroundSub = Sprite::create("./img/ingame_background.png");
	backgroundSub->setAnchorPoint(Point::ZERO);
	backgroundSub->setPosition(Point(background->getContentSize().width * 0.75, 0));
	backgroundSub->setScale(0.75f);
	layer->addChild(backgroundSub);
}

void PreviewLayer::setDuration(float _time)
{
	// 노래 길이 저장
	duration = _time;

	// 레이어 크기 변경
	layer->setContentSize(Size(960 + 60 * ((toolScene->BPM + 1) * 5) * duration * 0.75, 540));
}

void PreviewLayer::setCurrentTime(float _time)
{
	if (currentTime != _time)
	{
		// 노래 현재 시간 저장
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

	// 배경화면 옮기기
	if (offset > position + size)
	{
		background->setPositionX(positionSub + sizeSub);
	}
	else if (offset + visibleSize.width < position)
	{
		background->setPositionX(positionSub - size);
	}

	// 배경화면 보조 옮기기
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
		// 스크롤에 따라 노래 변경
		interfaceLayer->setContainer(0 - scroll->getContentOffset().x / 3 * 4);
	}
	else
	{
		// 노래에 따라 스크롤 변경
		scroll->setContentOffset(Vec2(-1 * 60 * ((toolScene->BPM + 1) * 5) * currentTime * 0.75, 0));
	}
}

void PreviewLayer::printNote(unsigned int _time)
{
	Note::List* temp = note->searchList(NULL);

	// 스프라이트 출력
	temp->noteSprite->setPositionX(960 + _time * (toolScene->BPM + 1) * 5 * 0.75);
	layer->addChild(temp->noteSprite, 1);

	// 서브 뒤 스프라이트 출력
	if (note->checkSub(temp, Note::SUB::BACK))
	{
		temp->subBackSprite->setPositionX(960 + _time * (toolScene->BPM + 1) * 5 * 0.75);
		layer->addChild(temp->subBackSprite, 0);
	}

	// 서브 앞 스프라이트 출력
	if (note->checkSub(temp, Note::SUB::FRONT))
	{
		temp->subFrontSprite->setPositionX(960 + _time * (toolScene->BPM + 1) * 5 * 0.75);
		layer->addChild(temp->subFrontSprite, 2);
	}
}

void PreviewLayer::removeNote(int _tag)
{
	Note::List* temp = note->searchList(_tag);

	// 스프라이트 삭제
	temp->noteSprite->setPositionX(temp->noteSprite->getPositionX() + scroll->getContentOffset().x * -1);
	layer->removeChild(temp->noteSprite);

	// 서브 뒤 스프라이트 삭제
	if (note->checkSub(temp, Note::SUB::BACK))
	{
		temp->subBackSprite->setPositionX(temp->subBackSprite->getPositionX() + scroll->getContentOffset().x * -1);
		layer->removeChild(temp->subBackSprite);
	}

	// 서브 앞 스프라이트 삭제
	if (note->checkSub(temp, Note::SUB::FRONT))
	{
		temp->subFrontSprite->setPositionX(temp->subFrontSprite->getPositionX() + scroll->getContentOffset().x * -1);
		layer->removeChild(temp->subFrontSprite);
	}
}