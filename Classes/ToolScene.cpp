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

	// ��Ʋ�� �ҷ�����
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/sprites.plist");
}

ToolScene::~ToolScene()
{
	// ���� �Ҵ� ����
	delete character;

	// ���� �Ҵ� ����
	delete note;
}

Scene* ToolScene::createScene()
{
	auto scene = Scene::create();

	// �⺻ �� ���̾� ����
	auto layer = ToolScene::create();
	scene->addChild(layer, TOOLSCENE);

	// �̸����� ���̾� ����
	previewLayer = PreviewLayer::create();
	scene->addChild(previewLayer, PREVIEWLAYER);

	// �������̽� ���̾� ����
	interfaceLayer = InterfaceLayer::create();
	scene->addChild(interfaceLayer, INTERFACELAYER);

	// ������ ���̾ �� �� ����
	previewLayer->toolScene = layer;

	// �������̽� ���̾ �� �� ����
	interfaceLayer->toolScene = layer;

	// ������ ���̾ �������̽� ���̾� ����
	previewLayer->interfaceLayer = interfaceLayer;

	// �������̽� ���̾ ������ ���̾� ����
	interfaceLayer->previewLayer = previewLayer;

	// ��Ʈ �����Ҵ�
	note = new Note;

	// �������̽� ���̾ ��Ʈ Ŭ���� ����
	interfaceLayer->note = note;

	// ������ ���̾�� ��Ʈ Ŭ���� ����
	previewLayer->note = note;

	// ��Ʈ Ŭ������ ������ ���̾� ����
	note->previewLayer = previewLayer;

	return scene;
}

bool ToolScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	// ĳ���� ���� �Ҵ�
	character = new Character;

	// ĳ���� ����
	createCharacter();

	// ��� ����
	createBackground();

	// ����Ʈ �ڽ� ����
	createEditBox();

	// BPM ���� ��ư ����
	createBPMUp();

	// BPM ���� ��ư ����
	createBPMDown();

	// �޴� ����
	createMenu();

	// BPM ����
	createBPM();

	// Ű���� �̺�Ʈ ������
	EventListenerKeyboard* keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(ToolScene::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(ToolScene::onKeyReleased, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	// ������Ʈ
	this->scheduleUpdate();

    return true;
}

void ToolScene::update(float delta)
{
	// ������ ����
	fps += ((float)BPM + 1.0) / 2;

	// �ӵ� ����
	character->setActionSpeed(BPM);

	// ĳ���� �׼�
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
		// BPM ����
		BPM++;
	}

	// BPM �� ����
	setBPM();
}

void ToolScene::BPMDownCallback(Ref* sender)
{
	CCLOG("BPMDownCallback");

	if (BPM > 1)
	{
		// BPM ����
		BPM--;
	}

	// BPM �� ����
	setBPM();
}

void ToolScene::editBoxReturn(ui::EditBox* editBox)
{
	CCLOG("--- editBoxReturn ---");

	// �ʱ�ȭ
	interfaceLayer->reset();

	// �̸� �޾ƿ�
	name = editBox->getText();
	
	// �� ���
	nameLabel->setString(StringUtils::format("%s", name));

	// �뷡 üũ
	interfaceLayer->checkSong(name);
}

void ToolScene::createCharacter()
{
	characterSprite = character->action(&fps, downPress);
	this->addChild(characterSprite);
}

void ToolScene::createBackground()
{
	// �÷� ���̾� ����
	auto layer = LayerColor::create(Color4B(171, 171, 171, 255));
	layer->setContentSize(Size(visibleSize.width, visibleSize.height - visibleSize.width * 9 / 16));
	this->addChild(layer);
}

void ToolScene::createEditBox()
{
	// �뷡 ���� ����Ʈ �ڽ� ����
	editBox = ui::EditBox::create(Size(300, 50), ui::Scale9Sprite::createWithSpriteFrameName("edit_box.png"));
	editBox->setAnchorPoint(Point::ZERO);
	editBox->setPosition(Vec2(origin.x + visibleSize.width * 0.02, origin.y + visibleSize.height * 0.22));
	editBox->setPlaceHolder("Song Name");
	this->addChild(editBox);

	// �� ���
	nameLabel = Label::createWithSystemFont("", "Thonburi", 34, Size(300, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
	nameLabel->setAnchorPoint(Point::ZERO);
	nameLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.02 + 10, origin.y + visibleSize.height * 0.22));
	nameLabel->setColor(Color3B::BLACK);
	this->addChild(nameLabel);

	// ����Ʈ �ڽ� ��������Ʈ
	editBox->setDelegate(this);
}

void ToolScene::createBPMUp()
{
	// ��ư ��������Ʈ ����
	auto buttonNormal = Sprite::createWithSpriteFrameName("button_normal.png");
	auto buttonPress = Sprite::createWithSpriteFrameName("button_press.png");

	// BPM ���� ��ư ����
	menuUp = MenuItemSprite::create(buttonNormal, buttonPress, CC_CALLBACK_1(ToolScene::BPMUpCallback, this));
	menuUp->setAnchorPoint(Point::ZERO);
	menuUp->setPosition(Vec2(origin.x + visibleSize.width * 0.4, origin.y + visibleSize.height * 0.22));

	// BPM ���� �� ����
	labelUp = Label::createWithSystemFont("Up", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	labelUp->setAnchorPoint(Point::ZERO);
	labelUp->setPosition(Vec2(origin.x + visibleSize.width * 0.4, origin.y + visibleSize.height * 0.22));
	labelUp->setColor(Color3B::BLACK);
	this->addChild(labelUp, 1);
}

void ToolScene::createBPMDown()
{
	// ��ư ��������Ʈ ����
	auto buttonNormal = Sprite::createWithSpriteFrameName("button_normal.png");
	auto buttonPress = Sprite::createWithSpriteFrameName("button_press.png");

	// BPM ���� ��ư ����
	menuDown = MenuItemSprite::create(buttonNormal, buttonPress, CC_CALLBACK_1(ToolScene::BPMDownCallback, this));
	menuDown->setAnchorPoint(Point::ZERO);
	menuDown->setPosition(Vec2(origin.x + visibleSize.width * 0.47, origin.y + visibleSize.height * 0.22));

	// BPM ���� �� ����
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
	// �� ���
	auto label = Label::createWithSystemFont("BPM", "Thonburi", 16);
	label->setAnchorPoint(Point::ZERO);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.35, origin.y + visibleSize.height * 0.26));
	label->setColor(Color3B::BLACK);
	this->addChild(label);

	// BPM �� ���
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