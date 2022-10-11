#include "InterfaceLayer.h"
#include "PreviewLayer.h"
#include "Note.h"
#include "ToolScene.h"

InterfaceLayer::InterfaceLayer() :audioID(NULL), duration(NULL), selectKind(1), selectList(1), level(1)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
}

InterfaceLayer::~InterfaceLayer()
{
	// ����� ĳ�� ����
	AudioEngine::uncacheAll();

	// ����� ���� ����
	AudioEngine::end();
}

bool InterfaceLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// �����̴� ����
	createSlider();

	// ���/�Ͻ����� ��ư ����
	createPlayPause();

	// ��ũ�� �� ����
	createScroll();

	// ���� ��ư ����
	createSave();

	// �ҷ����� ��ư ����
	createLoad();

	// �߰� ��ư ����
	createAdd();

	// ���� ��ư ����
	createDelete();

	// ��Ʈ ���� �޴� ����
	createNoteMenu();

	// ���̵� ���� ��ư ����
	createLevelUp();

	// ���̵� ���� ��ư ����
	createLevelDown();

	// �޴� ����
	createMenu();

	// �� ���� �� ����
	createSongLabel();

	// �ð� �� ����
	createTimeLabel();

	// ���̵� �� ����
	createLevelLabel();

	// ���̾� �����
	this->setVisible(false);

	// ������Ʈ
	this->scheduleUpdate();

    return true;
}

void InterfaceLayer::update(float delta)
{
	// �뷡�� ���� ����
	setBySong();

	// �뷡 ���� �ð� ��ȯ
	previewLayer->setCurrentTime(currentTime);

	// �ð� ����
	setTime();
}

void InterfaceLayer::playMenuCallback(Ref *sender)
{
	CCLOG("playMenuCallback");

	AudioEngine::AudioState state = AudioEngine::getState(audioID);

	if (state == AudioEngine::AudioState::PLAYING)
	{
		// �뷡 �Ͻ�����
		AudioEngine::pause(audioID);
	}
	else if (state == AudioEngine::AudioState::PAUSED)
	{
		// �뷡 ���
		AudioEngine::resume(audioID);
	}
	else if (state == AudioEngine::AudioState::ERROR)
	{
		// �뷡 �ʱ�ȭ
		AudioEngine::setCurrentTime(audioID, 0);

		// �뷡 ���
		audioID = AudioEngine::play2d(StringUtils::format("./song/%s/%s.mp3", name, name), false, 1.0f);
	}
}

void InterfaceLayer::saveMenuCallback(Ref *sender)
{
	CCLOG("saveMenuCallback");

	// ���� ����
	FILE* fp = fopen(StringUtils::format("./song/%s/%s", name, name).c_str(), "w");

	// ���� ����
	note->saveList(fp);

	// ���� ���� ����
	fclose(fp);

	// ������ ����
	saveData();
}

void InterfaceLayer::loadMenuCallback(Ref *sender)
{
	CCLOG("loadMenuCallback");

	// �ʱ�ȭ
	reset();

	// ������ �ҷ�����
	loadData();

	// �ӽ� ���� ����
	FILE* _fp = fopen(StringUtils::format("./song/%s/%s", name, name).c_str(), "a");

	// �ӽ� ���� ���� ����
	fclose(_fp);

	// ���� ����
	FILE* fp = fopen(StringUtils::format("./song/%s/%s", name, name).c_str(), "r");

	// ���� �ҷ�����
	note->loadList(fp);

	// ���̾� ũ�� ����
	setNoteListSize(note->getListSize());

	// ��Ʈ ��� ���� ����
	setSelectList();

	// ���� �׷� ����
	createNoteListRadioButton();

	// ���� ���� ����
	fclose(fp);

	// �뷡 ���� ��ȯ
	previewLayer->setDuration(duration);
}

void InterfaceLayer::addMenuCallback(Ref *sender)
{
	CCLOG("addMenuCallback");

	// ��Ʈ �߰�
	note->addList(time, selectKind);

	// ��������Ʈ ���
	previewLayer->printNote(time);

	// �±� ����
	note->setListTag();

	// ���̾� ũ�� ����
	setNoteListSize(note->getListSize());

	// ��Ʈ ��� ���� ����
	setSelectList();

	// ���� �׷� ����
	createNoteListRadioButton();
}

void InterfaceLayer::deleteMenuCallback(Ref *sender)
{
	CCLOG("deleteMenuCallback");

	if (selectList && 
		selectList <= note->getListSize())
	{
		// ���� �׷� ����
		removeNoteListRadioButton(selectList);

		// ��������Ʈ ����
		previewLayer->removeNote(selectList);

		// ��Ʈ ����
		note->delList(selectList);

		// �±� ����
		note->setListTag();

		// ���̾� ũ�� ����
		setNoteListSize(note->getListSize());

		// ��Ʈ ��� ���� ����
		setSelectList();

		// ���� �׷� ����
		createNoteListRadioButton();
	}
}

void InterfaceLayer::levelUpMenuCallback(Ref *sender)
{
	CCLOG("levelUpMenuCallback");

	if (level < LEVEL_MAX)
	{
		// ���̵� ����
		level++;
	}

	// ���̵� �� ����
	levelLabel->setString(StringUtils::format("%d", level));
}

void InterfaceLayer::levelDownMenuCallback(Ref *sender)
{
	CCLOG("levelDownMenuCallback");

	if (level > 1)
	{
		// ���̵� ����
		level--;
	}

	// ���̵� �� ����
	levelLabel->setString(StringUtils::format("%d", level));
}

void InterfaceLayer::sliderEvent(Ref *sender, ui::SliderEventType type)
{
	// �����̴��� ���� ��
	if (type == SLIDER_PERCENTCHANGED) {
		Slider* sliderTemp = dynamic_cast<Slider*>(sender);
		
		// �����̴��� ���� �뷡 �ð� ����
		int percent = sliderTemp->getPercent();

		AudioEngine::setCurrentTime(audioID, duration * percent / 100);
	}
}

void InterfaceLayer::radioButtonCallback(Ref *sender)
{
	CCLOG("radioButtonCallback");
	
	// ������ ��ư �� ����
	int select = radioGroup->getSelectedButtonIndex();

	// ������ ��ư�� ���� ��Ʈ ���� ���� ����
	selectKind = select + 1;
}

void InterfaceLayer::noteListRadioButtonCallback(Ref *sender)
{
	CCLOG("noteListRadioButtonCallback");

	// ������ ��ư �� ����
	int select = noteListRadioGroup->getSelectedButtonIndex();

	// ������ ��ư�� ���� ����
	selectList = select + 1;
}

void InterfaceLayer::songLoadFolderCallback(bool isSuccess)
{
	CCLOG("songLoadFolderCallback");

	if (isSuccess)
	{
		// �뷡 ���
		audioID = AudioEngine::play2d(StringUtils::format("./song/%s/%s.mp3", name, name), false, 1.0f);

		// ���
		Sleep(100);

		// �뷡 ���� �ݹ� ����
		AudioEngine::setFinishCallback(audioID, CC_CALLBACK_0(InterfaceLayer::songFinishCallback, this));

		// �뷡 �ʱ�ȭ
		AudioEngine::setCurrentTime(audioID, 0);

		// �뷡 �Ͻ�����
		AudioEngine::pause(audioID);

		// �뷡 ���� ����
		duration = AudioEngine::getDuration(audioID);

		// ��� ��ư �ʱ�ȭ
		menuPlayPause->setSelectedIndex(0);

		// �뷡 ���� ��ȯ
		previewLayer->setDuration(duration);

		// ������ �����
		toolScene->setVisibleItem(FALSE);

		// ���̾� ���̱�
		this->setVisible(true);
	}
	else
	{
		// ���� �ʱ�ȭ
		duration = NULL;

		// ������ ���̱�
		toolScene->setVisibleItem(TRUE);

		// ���̾� �����
		this->setVisible(false);
	}
}
void InterfaceLayer::songLoadCallback(bool isSuccess)
{
	CCLOG("songLoadCallback");

	if (isSuccess)
	{
		// ���� ����
		FileUtils::getInstance()->createDirectory(StringUtils::format("./song/%s", name));

		// �뷡 �̵�
		moveFile(StringUtils::format("./song/%s.mp3", name).c_str(), StringUtils::format("./song/%s/%s.mp3", name, name).c_str());

		// �̹����� ���� ��
		if (fopen(StringUtils::format("./song/%s.png", name).c_str(), "r"))
		{
			// �̹��� �̵�
			moveFile(StringUtils::format("./song/%s.png", name).c_str(), StringUtils::format("./song/%s/%s.png", name, name).c_str());
		}

		// �뷡 ���
		audioID = AudioEngine::play2d(StringUtils::format("./song/%s/%s.mp3", name, name), false, 1.0f);

		// ���
		Sleep(100);

		// �뷡 ���� �ݹ� ����
		AudioEngine::setFinishCallback(audioID, CC_CALLBACK_0(InterfaceLayer::songFinishCallback, this));

		// �뷡 �ʱ�ȭ
		AudioEngine::setCurrentTime(audioID, 0);

		// �뷡 �Ͻ�����
		AudioEngine::pause(audioID);

		// �뷡 ���� ����
		duration = AudioEngine::getDuration(audioID);

		// ��� ��ư �ʱ�ȭ
		menuPlayPause->setSelectedIndex(0);

		// �뷡 ���� ��ȯ
		previewLayer->setDuration(duration);

		// ������ �����
		toolScene->setVisibleItem(FALSE);

		// ���̾� ���̱�
		this->setVisible(true);
	}
	else
	{
		// ���� �ʱ�ȭ
		duration = NULL;

		// ������ ���̱�
		toolScene->setVisibleItem(TRUE);

		// ���̾� �����
		this->setVisible(false);
	}
}

void InterfaceLayer::songFinishCallback()
{
	CCLOG("songFinishCallback");

	// �뷡 �ε�
	AudioEngine::preload(StringUtils::format("./song/%s/%s.mp3", name, name));

	// ��ư ���
	menuPlayPause->setSelectedIndex(0);
}

void InterfaceLayer::createSlider()
{
	// �����̴� ����
	slider = Slider::create();
	slider->loadBarTexture("slider_track.png", Widget::TextureResType::PLIST);
	slider->loadProgressBarTexture("slider_progress.png", Widget::TextureResType::PLIST);
	slider->loadSlidBallTextureNormal("slider_ball.png", Widget::TextureResType::PLIST);
	slider->setAnchorPoint(Point::ZERO);
	slider->setPosition(Vec2(origin.x + visibleSize.width * 0.02, origin.y + visibleSize.height * 0.14));
	slider->addEventListenerSlider(this, sliderpercentchangedselector(InterfaceLayer::sliderEvent));
	this->addChild(slider);
}

void InterfaceLayer::createPlayPause()
{
	// ���/�Ͻ����� ��ư ����
	auto playNormalSprite = Sprite::createWithSpriteFrameName("play_normal.png");
	auto playPressSprite = Sprite::createWithSpriteFrameName("play_press.png");
	auto pauseNormalSprite = Sprite::createWithSpriteFrameName("pause_normal.png");
	auto pausePressSprite = Sprite::createWithSpriteFrameName("pause_press.png");

	// ���/�Ͻ����� �޴� ����
	auto menuPlay = MenuItemSprite::create(playNormalSprite, playPressSprite);
	auto menuPause = MenuItemSprite::create(pauseNormalSprite, pausePressSprite);
	menuPlayPause = MenuItemToggle::createWithCallback(CC_CALLBACK_1(InterfaceLayer::playMenuCallback, this), menuPlay, menuPause, NULL);
	menuPlayPause->setAnchorPoint(Point::ZERO);
	menuPlayPause->setPosition(Vec2(origin.x + visibleSize.width * 0.02, origin.y + visibleSize.height * 0.025));
}

void InterfaceLayer::createScroll()
{
	// �÷� ���̾� ��� ����
	auto background = LayerColor::create(Color4B(80, 80, 80, 255));
	background->setContentSize(Size(SCROLL_WIDTH, SCROLL_HEIGHT));
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Vec2(origin.x + visibleSize.width * 0.52, origin.y + visibleSize.height * 0.025));
	this->addChild(background);

	// �÷� ���̾� ����
	noteListLayer = LayerColor::create(Color4B(150, 150, 150, 255));
	noteListLayer->setContentSize(Size(SCROLL_WIDTH, SCROLL_HEIGHT));

	// ��ũ�� �� ���
	noteListScroll = ScrollView::create(Size(SCROLL_WIDTH, SCROLL_HEIGHT), noteListLayer);
	noteListScroll->setAnchorPoint(Point::ZERO);
	noteListScroll->setPosition(Vec2(origin.x + visibleSize.width * 0.52, origin.y + visibleSize.height * 0.025));
	noteListScroll->setDirection(ScrollView::Direction::VERTICAL);
	noteListScroll->setBounceable(true);
	this->addChild(noteListScroll);

	// ��Ʈ ��� ���� ��ư �׷� ����
	noteListRadioGroup = RadioButtonGroup::create();
	noteListLayer->addChild(noteListRadioGroup);
}

void InterfaceLayer::createSave()
{
	// ��ư ��������Ʈ ����
	auto saveNormalSprite = Sprite::createWithSpriteFrameName("button_normal.png");
	auto savePressSprite = Sprite::createWithSpriteFrameName("button_press.png");

	// ���� ��ư ����
	menuSave = MenuItemSprite::create(saveNormalSprite, savePressSprite, CC_CALLBACK_1(InterfaceLayer::saveMenuCallback, this));
	menuSave->setAnchorPoint(Point::ZERO);
	menuSave->setPosition(Vec2(origin.x + visibleSize.width * 0.43, origin.y + visibleSize.height * 0.025));

	// �� ���
	auto label = Label::createWithSystemFont("Save", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setAnchorPoint(Point::ZERO);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.43, origin.y + visibleSize.height * 0.025));
	label->setColor(Color3B::BLACK);
	this->addChild(label, 1);
}

void InterfaceLayer::createLoad()
{
	// ��ư ��������Ʈ ����
	auto loadNormalSprite = Sprite::createWithSpriteFrameName("button_normal.png");
	auto loadPressSprite = Sprite::createWithSpriteFrameName("button_press.png");

	// ���� ��ư ����
	menuLoad = MenuItemSprite::create(loadNormalSprite, loadPressSprite, CC_CALLBACK_1(InterfaceLayer::loadMenuCallback, this));
	menuLoad->setAnchorPoint(Point::ZERO);
	menuLoad->setPosition(Vec2(origin.x + visibleSize.width * 0.35, origin.y + visibleSize.height * 0.025));

	// �� ���
	auto label = Label::createWithSystemFont("Load", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setAnchorPoint(Point::ZERO);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.35, origin.y + visibleSize.height * 0.025));
	label->setColor(Color3B::BLACK);
	this->addChild(label, 1);
}

void InterfaceLayer::createAdd()
{
	// ��ư ��������Ʈ ����
	auto addNormalSprite1 = Sprite::createWithSpriteFrameName("button_normal.png");
	auto addPressSprite1 = Sprite::createWithSpriteFrameName("button_press.png");

	// �߰� ��ư ����
	menuAdd = MenuItemSprite::create(addNormalSprite1, addPressSprite1, CC_CALLBACK_1(InterfaceLayer::addMenuCallback, this));
	menuAdd->setAnchorPoint(Point::ZERO);
	menuAdd->setPosition(Vec2(origin.x + visibleSize.width * 0.725, origin.y + visibleSize.height * 0.1));

	// �� ���
	labelAdd = Label::createWithSystemFont("Add", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	labelAdd->setAnchorPoint(Point::ZERO);
	labelAdd->setPosition(Vec2(origin.x + visibleSize.width * 0.725, origin.y + visibleSize.height * 0.1));
	labelAdd->setColor(Color3B::BLACK);
	this->addChild(labelAdd, 1);
}

void InterfaceLayer::createDelete()
{
	// ��ư ��������Ʈ ����
	auto deleteNormalSprite = Sprite::createWithSpriteFrameName("button_normal.png");
	auto deletePressSprite = Sprite::createWithSpriteFrameName("button_press.png");

	// ���� ��ư ����
	menuDelete = MenuItemSprite::create(deleteNormalSprite, deletePressSprite, CC_CALLBACK_1(InterfaceLayer::deleteMenuCallback, this));
	menuDelete->setAnchorPoint(Point::ZERO);
	menuDelete->setPosition(Vec2(origin.x + visibleSize.width * 0.725, origin.y + visibleSize.height * 0.025));

	// �� ���
	auto label = Label::createWithSystemFont("Del", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setAnchorPoint(Point::ZERO);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.725, origin.y + visibleSize.height * 0.025));
	label->setColor(Color3B::BLACK);
	this->addChild(label, 1);
}

void InterfaceLayer::createLevelUp()
{
	// ��ư ��������Ʈ ����
	auto buttonNormalSprite = Sprite::createWithSpriteFrameName("button_normal.png");
	auto buttonPressSprite = Sprite::createWithSpriteFrameName("button_press.png");

	// ���̵� ���� ��ư ����
	menuUp = MenuItemSprite::create(buttonNormalSprite, buttonPressSprite, CC_CALLBACK_1(InterfaceLayer::levelUpMenuCallback, this));
	menuUp->setAnchorPoint(Point::ZERO);
	menuUp->setPosition(Vec2(origin.x + visibleSize.width * 0.12, origin.y + visibleSize.height * 0.025));

	// �� ���
	auto label = Label::createWithSystemFont("Up", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setAnchorPoint(Point::ZERO);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.12, origin.y + visibleSize.height * 0.025));
	label->setColor(Color3B::BLACK);
	this->addChild(label, 1);
}

void InterfaceLayer::createLevelDown()
{
	// ��ư ��������Ʈ ����
	auto buttonNormalSprite = Sprite::createWithSpriteFrameName("button_normal.png");
	auto buttonPressSprite = Sprite::createWithSpriteFrameName("button_press.png");

	// ���̵� ���� ��ư ����
	menuDown = MenuItemSprite::create(buttonNormalSprite, buttonPressSprite, CC_CALLBACK_1(InterfaceLayer::levelDownMenuCallback, this));
	menuDown->setAnchorPoint(Point::ZERO);
	menuDown->setPosition(Vec2(origin.x + visibleSize.width * 0.19, origin.y + visibleSize.height * 0.025));

	// �� ���
	auto label = Label::createWithSystemFont("Down", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setAnchorPoint(Point::ZERO);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.19, origin.y + visibleSize.height * 0.025));
	label->setColor(Color3B::BLACK);
	this->addChild(label, 1);
}

void InterfaceLayer::createNoteMenu()
{
	// �÷� ���̾� ��� ����
	auto background = LayerColor::create(Color4B(80, 80, 80, 255));
	background->setContentSize(Size(SCROLL_WIDTH, SCROLL_HEIGHT));
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Vec2(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.025));
	this->addChild(background);

	// �÷� ���̾� ����
	auto layer = LayerColor::create(Color4B(150, 150, 150, 255));
	if ((Note::NOTE_KIND::END - 1) * RADIO_HEIGHT >= SCROLL_HEIGHT)
	{
		layer->setContentSize(Size(SCROLL_WIDTH, (Note::NOTE_KIND::END - 1) * RADIO_HEIGHT));
	}
	else
	{
		layer->setContentSize(Size(SCROLL_WIDTH, SCROLL_HEIGHT));
	}

	// ��ũ�� �� ���
	auto scroll = ScrollView::create(Size(SCROLL_WIDTH, SCROLL_HEIGHT), layer);
	scroll->setAnchorPoint(Point::ZERO);
	scroll->setPosition(Vec2(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.025));
	scroll->setDirection(ScrollView::Direction::VERTICAL);
	scroll->setBounceable(true);
	scroll->setContentOffset(Vec2(0, -RADIO_HEIGHT * (Note::NOTE_KIND::END - 1) + SCROLL_HEIGHT));
	this->addChild(scroll);

	// ���� ��ư �׷� ����
	radioGroup = RadioButtonGroup::create();

	for (int count = Note::NOTE_KIND::NONE + 1; count < Note::NOTE_KIND::END; count++)
	{
		// ���� ��ư ����
		auto radio = RadioButton::create("white_box.png", "white_box.png", "selector.png", "white_box.png", "white_box.png", Widget::TextureResType::PLIST);
		radio->setAnchorPoint(Point(0.0, 1.0));
		radio->setPosition(Point(0.0, RADIO_HEIGHT * (Note::NOTE_KIND::END - count)));
		layer->addChild(radio);

		// �� ����
		auto label = Label::createWithSystemFont(StringUtils::format(" %s", getString(count)), "Thonburi", 18, Size(RADIO_WIDTH, RADIO_HEIGHT), TextHAlignment::LEFT, TextVAlignment::CENTER);
		label->setAnchorPoint(Point(0.0, 1.0));
		label->setPosition(Point(0.0, RADIO_HEIGHT * (Note::NOTE_KIND::END - count)));
		label->setColor(Color3B::BLACK);
		layer->addChild(label);

		// ���� ��ư �׷� �߰�
		radioGroup->addRadioButton(radio);
		radioGroup->addEventListener(CC_CALLBACK_1(InterfaceLayer::radioButtonCallback, this));
	}
	radioGroup->getRadioButtonByIndex(0)->setSelected(true);
	layer->addChild(radioGroup);
}

void InterfaceLayer::createMenu()
{
	// �޴� ���
	auto menu = Menu::create(menuPlayPause, menuSave, menuLoad, menuAdd, menuDelete, menuUp, menuDown, NULL);
	menu->setAnchorPoint(Point::ZERO);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 0);
}

void InterfaceLayer::createSongLabel()
{
	// �� ���
	songLabel = Label::createWithSystemFont("", "Thonburi", 18, Size::ZERO, TextHAlignment::RIGHT);
	songLabel->setAnchorPoint(Point(1.0, 0.0));
	songLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.49, origin.y + visibleSize.height * 0.095));
	songLabel->setColor(Color3B::BLACK);
	this->addChild(songLabel);
}

void InterfaceLayer::createTimeLabel()
{
	// �� ���
	timeLabel = Label::createWithSystemFont("", "Thonburi", 18, Size::ZERO, TextHAlignment::RIGHT);
	timeLabel->setAnchorPoint(Point(1.0, 0.0));
	timeLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.49, origin.y + visibleSize.height * 0.215));
	timeLabel->setColor(Color3B::BLACK);
	this->addChild(timeLabel);
}

void InterfaceLayer::createLevelLabel()
{
	// �� ���
	auto label = Label::createWithSystemFont("level", "Thonburi", 16);
	label->setAnchorPoint(Point::ZERO);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.15, origin.y + visibleSize.height * 0.1));
	label->setColor(Color3B::BLACK);
	this->addChild(label);

	// ���̵� �� ���
	levelLabel = Label::createWithSystemFont(StringUtils::format("%d", level), "Thonburi", 30);
	levelLabel->setAnchorPoint(Point::ZERO);
	levelLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.09));
	levelLabel->setColor(Color3B::BLACK);
	this->addChild(levelLabel);
}

void InterfaceLayer::checkSong(const char* _name)
{
	// �뷡 ����
	AudioEngine::stopAll();

	// �뷡 ����
	AudioEngine::uncacheAll();

	// �뷡 �̸� ����
	name = _name;

	struct _finddata_t c_file;
	intptr_t hFile;
	bool folder;
	std::string tempName = name;

	// �˻� ����
	if ((hFile = _findfirst(".\\song\\*.*", &c_file)) == -1L)
	{
		CCLOG("No file(s) in that directory!\n");
	}
	else
	{
		do
		{
			// ���� ������ ������ ��
			if ((c_file.attrib & _A_SUBDIR))
			{
				folder = TRUE;

				// �̸� Ȯ��
				for (unsigned int count = NULL; count < tempName.length(); count++)
				{
					// �̸��� �ٸ� ��
					if (c_file.name[count] != name[count])
					{
						folder = FALSE;
						break;
					}
				}

				// �̸��� ���� ��
				if (folder)
				{
					// �˻� ����
					_findclose(hFile);

					// �뷡 �ε�
					AudioEngine::preload(StringUtils::format("./song/%s/%s.mp3", name, name), CC_CALLBACK_1(InterfaceLayer::songLoadFolderCallback, this));

					return;
				}
			}
		} while (_findnext(hFile, &c_file) == 0);

		// �˻� ����
		_findclose(hFile);

		// �뷡 �ε�
		AudioEngine::preload(StringUtils::format("./song/%s.mp3", name), CC_CALLBACK_1(InterfaceLayer::songLoadCallback, this));
	}
}

void InterfaceLayer::setBySong()
{
	if (AudioEngine::getState(audioID) != AudioEngine::AudioState::ERROR)
	{
		// �뷡 ���� �ð� ����
		currentTime = AudioEngine::getCurrentTime(audioID);

		// �뷡�� ���� �����̴� ����
		slider->setPercent(currentTime / duration * 100);

		// �뷡�� ���� �� ����
		songLabel->setString(StringUtils::format("%d:%d / %d:%d", (int)currentTime / 60, (int)currentTime % 60, (int)duration / 60, (int)duration % 60));
	}
}

void InterfaceLayer::setContainer(float _offset)
{
	// �뷡 ���� �ð� ����
	AudioEngine::setCurrentTime(audioID, _offset / 60 / ((toolScene->BPM + 1) * 5));

	// �뷡 ���� �ð� ����
	currentTime = AudioEngine::getCurrentTime(audioID);

	// �����̴� ��ġ ����
	slider->setPercent(currentTime / duration * 100);

	// �뷡 ���� �ð� ��ȯ
	previewLayer->setCurrentTime(currentTime);
}

void InterfaceLayer::setCurrentTime(bool _left)
{
	// ���� Ű�� ������ ��
	if (_left)
	{
		AudioEngine::setCurrentTime(audioID, AudioEngine::getCurrentTime(audioID) - SKIP);
	}
	// ������ Ű�� ������ ��
	else
	{
		AudioEngine::setCurrentTime(audioID, AudioEngine::getCurrentTime(audioID) + SKIP);
	}
}

void InterfaceLayer::setPlaySong(bool _select)
{
	if (_select)
	{
		menuPlayPause->selected();
	}
	else
	{
		menuPlayPause->unselected();
		if (menuPlayPause->getSelectedIndex() == 0)
		{
			menuPlayPause->setSelectedIndex(1);
		}
		else
		{
			menuPlayPause->setSelectedIndex(0);
		}
		playMenuCallback(nullptr);
	}
}

void InterfaceLayer::setTime()
{
	// �ð� ����
	time = currentTime * 60.0f - (1280 / (toolScene->BPM + 1) / 5) + 50 - (5 + toolScene->BPM * 10);

	// �ð��� ���� �� ����
	timeLabel->setString(StringUtils::format("%d", time));

	// �ð��� ���� ��ư ����
	if (time <= NULL)
	{
		menuAdd->setVisible(false);
		labelAdd->setVisible(false);
	}
	else
	{
		menuAdd->setVisible(true);
		labelAdd->setVisible(true);
	}
}

char* InterfaceLayer::getString(const int _NUM)
{
	switch (_NUM)
	{
	case Note::NOTE_KIND::TIRE:
		return "Tire";

	case Note::NOTE_KIND::BOX:
		return "Box";

	case Note::NOTE_KIND::TRASH:
		return "Trash";

	case Note::NOTE_KIND::BARRICADE:
		return "Barricade";

	case Note::NOTE_KIND::BIKE:
		return "Bike";

	case Note::NOTE_KIND::SIGN:
		return "Sign";

	case Note::NOTE_KIND::LONGBOX:
		return "Long Box";

	case Note::NOTE_KIND::LONGAD:
		return "Long Ad";

	case Note::NOTE_KIND::HUMAN:
		return "Human";

	case Note::NOTE_KIND::NAVIGATOR:
		return "Navigator";

	case Note::NOTE_KIND::CONTAINER:
		return "Container";

	case Note::NOTE_KIND::CRAIN:
		return "Crain";

	case Note::NOTE_KIND::H_BEAM:
		return "H-Beam";

	case Note::NOTE_KIND::STONE:
		return "Stone";
	}
	return "";
}

void InterfaceLayer::setNoteListSize(int _size)
{
	if (_size * RADIO_HEIGHT < SCROLL_HEIGHT)
	{
		noteListLayer->setContentSize(Size(SCROLL_WIDTH, SCROLL_HEIGHT));
	}
	else if(noteListLayer->getContentSize().height != _size * RADIO_HEIGHT)
	{
		noteListLayer->setContentSize(Size(SCROLL_WIDTH, _size * RADIO_HEIGHT));

		// ������ �ʱ�ȭ
		noteListScroll->setContentOffset(Point::ZERO);

		// ������ �̵�
		if (noteListScroll->getContentOffset().x >= RADIO_HEIGHT)
		{
			noteListScroll->setContentOffset(noteListScroll->getContentOffset() + Vec2(0, -RADIO_HEIGHT));
		}
		else
		{
			noteListScroll->setContentOffset(Vec2(0, 0));
		}
	}
}

void InterfaceLayer::removeNoteListRadioButton(int _tag)
{
	Note::List* temp = note->searchList(_tag);

	// ���� �׷쿡�� ����
	noteListRadioGroup->removeRadioButton(temp->noteRadio);

	// ���� ��ư ����
	noteListLayer->removeChild(temp->noteRadio);

	// �� ����
	noteListLayer->removeChild(temp->noteLabel);
}

void InterfaceLayer::createNoteListRadioButton()
{
	int size = note->getListSize();

	// ���� �׷� ����
	for (int count = 1; count <= size; count++)
	{
		removeNoteListRadioButton(count);
	}

	// ���� �׷� ����
	for (int count = 1; count <= size; count++)
	{
		Note::List* temp = note->searchList(count);

		// ���� ��ư ����
		auto radio = RadioButton::create("white_box.png", "white_box.png", "selector.png", "white_box.png", "white_box.png", Widget::TextureResType::PLIST);
		radio->setAnchorPoint(Point(0.0, 1.0));
		radio->setPosition(Point(0.0, noteListLayer->getContentSize().height - RADIO_HEIGHT * (count - 1)));
		radio->setSelected(false);
		noteListLayer->addChild(radio);

		// �� ����
		auto label = Label::createWithSystemFont(StringUtils::format(" %d  %d  %s", temp->noteTag, temp->noteTime, getString(temp->noteKind)), "Thonburi", 16, Size(RADIO_WIDTH, RADIO_HEIGHT), TextHAlignment::LEFT, TextVAlignment::CENTER);
		label->setAnchorPoint(Point(0.0, 1.0));
		label->setPosition(Point(0.0, noteListLayer->getContentSize().height - RADIO_HEIGHT * (count - 1)));
		label->setColor(Color3B::BLACK);
		noteListLayer->addChild(label);

		// ���� �׷� �߰�
		noteListRadioGroup->addRadioButton(radio);
		noteListRadioGroup->addEventListener(CC_CALLBACK_1(InterfaceLayer::noteListRadioButtonCallback, this));

		// ���� ���
		note->setRadio(count, radio, label);
	}

	// �ش� ���� ����
	if (selectList)
	{
		noteListRadioGroup->setSelectedButton(note->searchList(selectList)->noteRadio);
	}
}

void InterfaceLayer::setSelectList()
{
	int size = note->getListSize();

	// �����Ѱ� ������� Ŭ ��
	if (selectList > size)
	{
		selectList = size;
	}
	else if (size == 1)
	{
		selectList = 1;
	}
}

int InterfaceLayer::getSelectList()
{
	return selectList;
}

void InterfaceLayer::reset()
{
	selectList = 1;

	int size = note->getListSize();

	for (int count = 1; count <= size; count++)
	{
		// ���� �׷� ����
		removeNoteListRadioButton(selectList);

		// ��������Ʈ ����
		previewLayer->removeNote(selectList);

		// ��Ʈ ����
		note->delList(selectList);

		// �±� ����
		note->setListTag();

		// ���̾� ũ�� ����
		setNoteListSize(note->getListSize());

		// ��Ʈ ��� ���� ����
		setSelectList();

		// ���� �׷� ����
		createNoteListRadioButton();
	}
}

void InterfaceLayer::saveData()
{
	// ���� ����
	FILE* fp = fopen(StringUtils::format("./song/%s/%s.txt", name, name).c_str(), "w");

	fprintf(fp, "%f\n%d\n%d", duration, toolScene->BPM, level);

	// ���� ���� ����
	fclose(fp);
}

void InterfaceLayer::loadData()
{
	// �ӽ� ���� ����
	FILE* _fp = fopen(StringUtils::format("./song/%s/%s.txt", name, name).c_str(), "a");

	// �ӽ� ���� ���� ����
	fclose(_fp);

	// ���� ����
	FILE* fp = fopen(StringUtils::format("./song/%s/%s.txt", name, name).c_str(), "r");

	// �뷡 ���� �ѱ��
	float temp;
	fscanf(fp, "%f", &temp);

	// BPM �޾ƿ���
	if (fscanf(fp, "%d", &toolScene->BPM) == -1)
	{
		toolScene->BPM = 1;
	}
	// BPM ����
	toolScene->setBPM();

	// ���� �޾ƿ���
	if (fscanf(fp, "%d", &level) == -1)
	{
		level = 1;
	}
	// ���̵� ����
	levelLabel->setString(StringUtils::format("%d", level));

	// ���� ���� ����
	fclose(fp);
}

void InterfaceLayer::moveFile(const char* _pastDir, const char* _newDir)
{
	size_t size = 0;
	char buffer[BUFSIZ] = { '\0' };

	// ���� ����
	FILE* in = fopen(_pastDir, "rb");
	FILE* out = fopen(_newDir, "wb");

	if (in == NULL || out == NULL)
	{
		perror("An error occured while opening files!!!");
		in = out = 0;
	}
	else
	{
		// ���� ����
		while ((size = fread(buffer, BUFSIZ, 1, in)) > 0)
		{
			fwrite(buffer, BUFSIZ, 1, out);
		}

		// ���� �ݱ�
		fclose(in);
		fclose(out);

		// ���� ����
		remove(_pastDir);
	}
}