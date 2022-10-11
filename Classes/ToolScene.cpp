#include "ToolScene.h"
#include "PreviewLayer.h"
#include "InterfaceLayer.h"
#include "Note.h"
#include "Character.h"

InterfaceLayer* interfaceLayer;
PreviewLayer* previewLayer;
Note* note;

ToolScene::ToolScene() :downPress(FALSE), fps(NULL), BPM(1)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// 아틀라스 불러오기
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/sprites.plist");
}

ToolScene::~ToolScene()
{
	// 동적 할당 해제
	delete character;

	// 동적 할당 해제
	delete note;
}

Scene* ToolScene::createScene()
{
	auto scene = Scene::create();

	// 기본 툴 레이어 생성
	auto layer = ToolScene::create();
	scene->addChild(layer, TOOLSCENE);

	// 미리보기 레이어 생성
	previewLayer = PreviewLayer::create();
	scene->addChild(previewLayer, PREVIEWLAYER);

	// 인터페이스 레이어 생성
	interfaceLayer = InterfaceLayer::create();
	scene->addChild(interfaceLayer, INTERFACELAYER);

	// 프리뷰 레이어에 툴 신 전달
	previewLayer->toolScene = layer;

	// 인터페이스 레이어에 툴 신 전달
	interfaceLayer->toolScene = layer;

	// 프리뷰 레이어에 인터페이스 레이어 전달
	previewLayer->interfaceLayer = interfaceLayer;

	// 인터페이스 레이어에 프리뷰 레이어 전달
	interfaceLayer->previewLayer = previewLayer;

	// 노트 동적할당
	note = new Note;

	// 인터페이스 레이어에 노트 클래스 전달
	interfaceLayer->note = note;

	// 프리뷰 레이어로 노트 클래스 전달
	previewLayer->note = note;

	// 노트 클래스에 프리뷰 레이어 전달
	note->previewLayer = previewLayer;

	return scene;
}

bool ToolScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	// 캐릭터 동적 할당
	character = new Character;

	// 캐릭터 생성
	createCharacter();

	// 배경 생성
	createBackground();

	// 에디트 박스 생성
	createEditBox();

	// BPM 증가 버튼 생성
	createBPMUp();

	// BPM 감소 버튼 생성
	createBPMDown();

	// 메뉴 생성
	createMenu();

	// BPM 생성
	createBPM();

	// 키보드 이벤트 리스너
	EventListenerKeyboard* keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(ToolScene::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(ToolScene::onKeyReleased, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	// 업데이트
	this->scheduleUpdate();

    return true;
}

void ToolScene::update(float delta)
{
	// 프레임 증가
	fps += ((float)BPM + 1.0) / 2;

	// 속도 설정
	character->setActionSpeed(BPM);

	// 캐릭터 액션
	characterSprite = character->action(&fps, downPress);
}

void ToolScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		character->jumpArim(downPress, BPM);
		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		downPress = TRUE;
		character->slideArim(downPress, BPM);
		break;

	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		interfaceLayer->setCurrentTime(TRUE);
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		interfaceLayer->setCurrentTime(FALSE);
		break;

	case EventKeyboard::KeyCode::KEY_SPACE:
		interfaceLayer->setPlaySong(TRUE);
		break;
	}
}

void ToolScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		downPress = FALSE;
		character->slideArim(downPress, BPM);
		break;

	case EventKeyboard::KeyCode::KEY_SPACE:
		interfaceLayer->setPlaySong(FALSE);
		break;
	}
}

void ToolScene::BPMUpCallback(Ref* sender)
{
	CCLOG("BPMUpCallback");

	if (BPM < BPM_MAX)
	{
		// BPM 증가
		BPM++;
	}

	// BPM 라벨 설정
	setBPM();
}

void ToolScene::BPMDownCallback(Ref* sender)
{
	CCLOG("BPMDownCallback");

	if (BPM > 1)
	{
		// BPM 감소
		BPM--;
	}

	// BPM 라벨 설정
	setBPM();
}

void ToolScene::editBoxReturn(ui::EditBox* editBox)
{
	CCLOG("--- editBoxReturn ---");

	// 초기화
	interfaceLayer->reset();

	// 이름 받아옴
	name = editBox->getText();
	
	// 라벨 출력
	nameLabel->setString(StringUtils::format("%s", name));

	// 노래 체크
	interfaceLayer->checkSong(name);
}

void ToolScene::createCharacter()
{
	characterSprite = character->action(&fps, downPress);
	this->addChild(characterSprite);
}

void ToolScene::createBackground()
{
	// 컬러 레이어 생성
	auto layer = LayerColor::create(Color4B(171, 171, 171, 255));
	layer->setContentSize(Size(visibleSize.width, visibleSize.height - visibleSize.width * 9 / 16));
	this->addChild(layer);
}

void ToolScene::createEditBox()
{
	// 노래 제목 에디트 박스 생성
	editBox = ui::EditBox::create(Size(300, 50), ui::Scale9Sprite::createWithSpriteFrameName("edit_box.png"));
	editBox->setAnchorPoint(Point::ZERO);
	editBox->setPosition(Vec2(origin.x + visibleSize.width * 0.02, origin.y + visibleSize.height * 0.22));
	editBox->setPlaceHolder("Song Name");
	this->addChild(editBox);

	// 라벨 출력
	nameLabel = Label::createWithSystemFont("", "Thonburi", 34, Size(300, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
	nameLabel->setAnchorPoint(Point::ZERO);
	nameLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.02 + 10, origin.y + visibleSize.height * 0.22));
	nameLabel->setColor(Color3B::BLACK);
	this->addChild(nameLabel);

	// 에디트 박스 델리게이트
	editBox->setDelegate(this);
}

void ToolScene::createBPMUp()
{
	// 버튼 스프라이트 설정
	auto buttonNormal = Sprite::createWithSpriteFrameName("button_normal.png");
	auto buttonPress = Sprite::createWithSpriteFrameName("button_press.png");

	// BPM 증가 버튼 생성
	menuUp = MenuItemSprite::create(buttonNormal, buttonPress, CC_CALLBACK_1(ToolScene::BPMUpCallback, this));
	menuUp->setAnchorPoint(Point::ZERO);
	menuUp->setPosition(Vec2(origin.x + visibleSize.width * 0.4, origin.y + visibleSize.height * 0.22));

	// BPM 증가 라벨 생성
	labelUp = Label::createWithSystemFont("Up", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	labelUp->setAnchorPoint(Point::ZERO);
	labelUp->setPosition(Vec2(origin.x + visibleSize.width * 0.4, origin.y + visibleSize.height * 0.22));
	labelUp->setColor(Color3B::BLACK);
	this->addChild(labelUp, 1);
}

void ToolScene::createBPMDown()
{
	// 버튼 스프라이트 설정
	auto buttonNormal = Sprite::createWithSpriteFrameName("button_normal.png");
	auto buttonPress = Sprite::createWithSpriteFrameName("button_press.png");

	// BPM 감소 버튼 생성
	menuDown = MenuItemSprite::create(buttonNormal, buttonPress, CC_CALLBACK_1(ToolScene::BPMDownCallback, this));
	menuDown->setAnchorPoint(Point::ZERO);
	menuDown->setPosition(Vec2(origin.x + visibleSize.width * 0.47, origin.y + visibleSize.height * 0.22));

	// BPM 감소 라벨 생성
	labelDown = Label::createWithSystemFont("Down", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	labelDown->setAnchorPoint(Point::ZERO);
	labelDown->setPosition(Vec2(origin.x + visibleSize.width * 0.47, origin.y + visibleSize.height * 0.22));
	labelDown->setColor(Color3B::BLACK);
	this->addChild(labelDown, 1);
}

void ToolScene::createMenu()
{
	menu = Menu::create(menuUp, menuDown, NULL);
	menu->setAnchorPoint(Point::ZERO);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 0);
}

void ToolScene::createBPM()
{
	// 라벨 출력
	auto label = Label::createWithSystemFont("BPM", "Thonburi", 16);
	label->setAnchorPoint(Point::ZERO);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.35, origin.y + visibleSize.height * 0.26));
	label->setColor(Color3B::BLACK);
	this->addChild(label);

	// BPM 라벨 출력
	BPMLabel = Label::createWithSystemFont(StringUtils::format("%d", BPM), "Thonburi", 30);
	BPMLabel->setAnchorPoint(Point::ZERO);
	BPMLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.36, origin.y + visibleSize.height * 0.22));
	BPMLabel->setColor(Color3B::BLACK);
	this->addChild(BPMLabel);
}

void ToolScene::setVisibleItem(bool _isTrue)
{
	menu->setVisible(_isTrue);
	labelUp->setVisible(_isTrue);
	labelDown->setVisible(_isTrue);
}

void ToolScene::setBPM()
{
	BPMLabel->setString(StringUtils::format("%d", BPM));
}