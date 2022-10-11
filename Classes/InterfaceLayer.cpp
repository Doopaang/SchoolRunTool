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
	// 오디오 캐시 삭제
	AudioEngine::uncacheAll();

	// 오디오 엔진 종료
	AudioEngine::end();
}

bool InterfaceLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// 슬라이더 생성
	createSlider();

	// 재생/일시정지 버튼 생성
	createPlayPause();

	// 스크롤 뷰 생성
	createScroll();

	// 저장 버튼 생성
	createSave();

	// 불러오기 버튼 생성
	createLoad();

	// 추가 버튼 생성
	createAdd();

	// 삭제 버튼 생성
	createDelete();

	// 노트 종류 메뉴 생성
	createNoteMenu();

	// 난이도 증가 버튼 생성
	createLevelUp();

	// 난이도 감소 버튼 생성
	createLevelDown();

	// 메뉴 생성
	createMenu();

	// 곡 진행 라벨 생성
	createSongLabel();

	// 시간 라벨 생성
	createTimeLabel();

	// 난이도 라벨 생성
	createLevelLabel();

	// 레이어 숨기기
	this->setVisible(false);

	// 업데이트
	this->scheduleUpdate();

    return true;
}

void InterfaceLayer::update(float delta)
{
	// 노래에 따라 변경
	setBySong();

	// 노래 현재 시간 반환
	previewLayer->setCurrentTime(currentTime);

	// 시간 설정
	setTime();
}

void InterfaceLayer::playMenuCallback(Ref *sender)
{
	CCLOG("playMenuCallback");

	AudioEngine::AudioState state = AudioEngine::getState(audioID);

	if (state == AudioEngine::AudioState::PLAYING)
	{
		// 노래 일시정지
		AudioEngine::pause(audioID);
	}
	else if (state == AudioEngine::AudioState::PAUSED)
	{
		// 노래 재생
		AudioEngine::resume(audioID);
	}
	else if (state == AudioEngine::AudioState::ERROR)
	{
		// 노래 초기화
		AudioEngine::setCurrentTime(audioID, 0);

		// 노래 재생
		audioID = AudioEngine::play2d(StringUtils::format("./song/%s/%s.mp3", name, name), false, 1.0f);
	}
}

void InterfaceLayer::saveMenuCallback(Ref *sender)
{
	CCLOG("saveMenuCallback");

	// 파일 연결
	FILE* fp = fopen(StringUtils::format("./song/%s/%s", name, name).c_str(), "w");

	// 파일 저장
	note->saveList(fp);

	// 파일 연결 해제
	fclose(fp);

	// 데이터 저장
	saveData();
}

void InterfaceLayer::loadMenuCallback(Ref *sender)
{
	CCLOG("loadMenuCallback");

	// 초기화
	reset();

	// 데이터 불러오기
	loadData();

	// 임시 파일 연결
	FILE* _fp = fopen(StringUtils::format("./song/%s/%s", name, name).c_str(), "a");

	// 임시 파일 연결 해제
	fclose(_fp);

	// 파일 연결
	FILE* fp = fopen(StringUtils::format("./song/%s/%s", name, name).c_str(), "r");

	// 파일 불러오기
	note->loadList(fp);

	// 레이어 크기 변경
	setNoteListSize(note->getListSize());

	// 노트 목록 선택 설정
	setSelectList();

	// 라디오 그룹 생성
	createNoteListRadioButton();

	// 파일 연결 해제
	fclose(fp);

	// 노래 길이 반환
	previewLayer->setDuration(duration);
}

void InterfaceLayer::addMenuCallback(Ref *sender)
{
	CCLOG("addMenuCallback");

	// 노트 추가
	note->addList(time, selectKind);

	// 스프라이트 출력
	previewLayer->printNote(time);

	// 태그 정리
	note->setListTag();

	// 레이어 크기 변경
	setNoteListSize(note->getListSize());

	// 노트 목록 선택 설정
	setSelectList();

	// 라디오 그룹 생성
	createNoteListRadioButton();
}

void InterfaceLayer::deleteMenuCallback(Ref *sender)
{
	CCLOG("deleteMenuCallback");

	if (selectList && 
		selectList <= note->getListSize())
	{
		// 라디오 그룹 삭제
		removeNoteListRadioButton(selectList);

		// 스프라이트 삭제
		previewLayer->removeNote(selectList);

		// 노트 삭제
		note->delList(selectList);

		// 태그 정리
		note->setListTag();

		// 레이어 크기 변경
		setNoteListSize(note->getListSize());

		// 노트 목록 선택 설정
		setSelectList();

		// 라디오 그룹 생성
		createNoteListRadioButton();
	}
}

void InterfaceLayer::levelUpMenuCallback(Ref *sender)
{
	CCLOG("levelUpMenuCallback");

	if (level < LEVEL_MAX)
	{
		// 난이도 증가
		level++;
	}

	// 난이도 라벨 설정
	levelLabel->setString(StringUtils::format("%d", level));
}

void InterfaceLayer::levelDownMenuCallback(Ref *sender)
{
	CCLOG("levelDownMenuCallback");

	if (level > 1)
	{
		// 난이도 감소
		level--;
	}

	// 난이도 라벨 설정
	levelLabel->setString(StringUtils::format("%d", level));
}

void InterfaceLayer::sliderEvent(Ref *sender, ui::SliderEventType type)
{
	// 슬라이더가 변할 때
	if (type == SLIDER_PERCENTCHANGED) {
		Slider* sliderTemp = dynamic_cast<Slider*>(sender);
		
		// 슬라이더에 따라 노래 시간 변경
		int percent = sliderTemp->getPercent();

		AudioEngine::setCurrentTime(audioID, duration * percent / 100);
	}
}

void InterfaceLayer::radioButtonCallback(Ref *sender)
{
	CCLOG("radioButtonCallback");
	
	// 선택한 버튼 값 저장
	int select = radioGroup->getSelectedButtonIndex();

	// 선택한 버튼에 따라 노트 종류 선택 변경
	selectKind = select + 1;
}

void InterfaceLayer::noteListRadioButtonCallback(Ref *sender)
{
	CCLOG("noteListRadioButtonCallback");

	// 선택한 버튼 값 저장
	int select = noteListRadioGroup->getSelectedButtonIndex();

	// 선택한 버튼에 따라 변경
	selectList = select + 1;
}

void InterfaceLayer::songLoadFolderCallback(bool isSuccess)
{
	CCLOG("songLoadFolderCallback");

	if (isSuccess)
	{
		// 노래 재생
		audioID = AudioEngine::play2d(StringUtils::format("./song/%s/%s.mp3", name, name), false, 1.0f);

		// 대기
		Sleep(100);

		// 노래 끝남 콜백 설정
		AudioEngine::setFinishCallback(audioID, CC_CALLBACK_0(InterfaceLayer::songFinishCallback, this));

		// 노래 초기화
		AudioEngine::setCurrentTime(audioID, 0);

		// 노래 일시정지
		AudioEngine::pause(audioID);

		// 노래 길이 저장
		duration = AudioEngine::getDuration(audioID);

		// 재생 버튼 초기화
		menuPlayPause->setSelectedIndex(0);

		// 노래 길이 반환
		previewLayer->setDuration(duration);

		// 아이템 숨기기
		toolScene->setVisibleItem(FALSE);

		// 레이어 보이기
		this->setVisible(true);
	}
	else
	{
		// 길이 초기화
		duration = NULL;

		// 아이템 보이기
		toolScene->setVisibleItem(TRUE);

		// 레이어 숨기기
		this->setVisible(false);
	}
}
void InterfaceLayer::songLoadCallback(bool isSuccess)
{
	CCLOG("songLoadCallback");

	if (isSuccess)
	{
		// 폴더 생성
		FileUtils::getInstance()->createDirectory(StringUtils::format("./song/%s", name));

		// 노래 이동
		moveFile(StringUtils::format("./song/%s.mp3", name).c_str(), StringUtils::format("./song/%s/%s.mp3", name, name).c_str());

		// 이미지가 있을 때
		if (fopen(StringUtils::format("./song/%s.png", name).c_str(), "r"))
		{
			// 이미지 이동
			moveFile(StringUtils::format("./song/%s.png", name).c_str(), StringUtils::format("./song/%s/%s.png", name, name).c_str());
		}

		// 노래 재생
		audioID = AudioEngine::play2d(StringUtils::format("./song/%s/%s.mp3", name, name), false, 1.0f);

		// 대기
		Sleep(100);

		// 노래 끝남 콜백 설정
		AudioEngine::setFinishCallback(audioID, CC_CALLBACK_0(InterfaceLayer::songFinishCallback, this));

		// 노래 초기화
		AudioEngine::setCurrentTime(audioID, 0);

		// 노래 일시정지
		AudioEngine::pause(audioID);

		// 노래 길이 저장
		duration = AudioEngine::getDuration(audioID);

		// 재생 버튼 초기화
		menuPlayPause->setSelectedIndex(0);

		// 노래 길이 반환
		previewLayer->setDuration(duration);

		// 아이템 숨기기
		toolScene->setVisibleItem(FALSE);

		// 레이어 보이기
		this->setVisible(true);
	}
	else
	{
		// 길이 초기화
		duration = NULL;

		// 아이템 보이기
		toolScene->setVisibleItem(TRUE);

		// 레이어 숨기기
		this->setVisible(false);
	}
}

void InterfaceLayer::songFinishCallback()
{
	CCLOG("songFinishCallback");

	// 노래 로딩
	AudioEngine::preload(StringUtils::format("./song/%s/%s.mp3", name, name));

	// 버튼 토글
	menuPlayPause->setSelectedIndex(0);
}

void InterfaceLayer::createSlider()
{
	// 슬라이더 생성
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
	// 재생/일시정지 버튼 설정
	auto playNormalSprite = Sprite::createWithSpriteFrameName("play_normal.png");
	auto playPressSprite = Sprite::createWithSpriteFrameName("play_press.png");
	auto pauseNormalSprite = Sprite::createWithSpriteFrameName("pause_normal.png");
	auto pausePressSprite = Sprite::createWithSpriteFrameName("pause_press.png");

	// 재생/일시정지 메뉴 설정
	auto menuPlay = MenuItemSprite::create(playNormalSprite, playPressSprite);
	auto menuPause = MenuItemSprite::create(pauseNormalSprite, pausePressSprite);
	menuPlayPause = MenuItemToggle::createWithCallback(CC_CALLBACK_1(InterfaceLayer::playMenuCallback, this), menuPlay, menuPause, NULL);
	menuPlayPause->setAnchorPoint(Point::ZERO);
	menuPlayPause->setPosition(Vec2(origin.x + visibleSize.width * 0.02, origin.y + visibleSize.height * 0.025));
}

void InterfaceLayer::createScroll()
{
	// 컬러 레이어 배경 생성
	auto background = LayerColor::create(Color4B(80, 80, 80, 255));
	background->setContentSize(Size(SCROLL_WIDTH, SCROLL_HEIGHT));
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Vec2(origin.x + visibleSize.width * 0.52, origin.y + visibleSize.height * 0.025));
	this->addChild(background);

	// 컬러 레이어 생성
	noteListLayer = LayerColor::create(Color4B(150, 150, 150, 255));
	noteListLayer->setContentSize(Size(SCROLL_WIDTH, SCROLL_HEIGHT));

	// 스크롤 뷰 출력
	noteListScroll = ScrollView::create(Size(SCROLL_WIDTH, SCROLL_HEIGHT), noteListLayer);
	noteListScroll->setAnchorPoint(Point::ZERO);
	noteListScroll->setPosition(Vec2(origin.x + visibleSize.width * 0.52, origin.y + visibleSize.height * 0.025));
	noteListScroll->setDirection(ScrollView::Direction::VERTICAL);
	noteListScroll->setBounceable(true);
	this->addChild(noteListScroll);

	// 노트 목록 라디오 버튼 그룹 생성
	noteListRadioGroup = RadioButtonGroup::create();
	noteListLayer->addChild(noteListRadioGroup);
}

void InterfaceLayer::createSave()
{
	// 버튼 스프라이트 설정
	auto saveNormalSprite = Sprite::createWithSpriteFrameName("button_normal.png");
	auto savePressSprite = Sprite::createWithSpriteFrameName("button_press.png");

	// 저장 버튼 설정
	menuSave = MenuItemSprite::create(saveNormalSprite, savePressSprite, CC_CALLBACK_1(InterfaceLayer::saveMenuCallback, this));
	menuSave->setAnchorPoint(Point::ZERO);
	menuSave->setPosition(Vec2(origin.x + visibleSize.width * 0.43, origin.y + visibleSize.height * 0.025));

	// 라벨 출력
	auto label = Label::createWithSystemFont("Save", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setAnchorPoint(Point::ZERO);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.43, origin.y + visibleSize.height * 0.025));
	label->setColor(Color3B::BLACK);
	this->addChild(label, 1);
}

void InterfaceLayer::createLoad()
{
	// 버튼 스프라이트 설정
	auto loadNormalSprite = Sprite::createWithSpriteFrameName("button_normal.png");
	auto loadPressSprite = Sprite::createWithSpriteFrameName("button_press.png");

	// 저장 버튼 설정
	menuLoad = MenuItemSprite::create(loadNormalSprite, loadPressSprite, CC_CALLBACK_1(InterfaceLayer::loadMenuCallback, this));
	menuLoad->setAnchorPoint(Point::ZERO);
	menuLoad->setPosition(Vec2(origin.x + visibleSize.width * 0.35, origin.y + visibleSize.height * 0.025));

	// 라벨 출력
	auto label = Label::createWithSystemFont("Load", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setAnchorPoint(Point::ZERO);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.35, origin.y + visibleSize.height * 0.025));
	label->setColor(Color3B::BLACK);
	this->addChild(label, 1);
}

void InterfaceLayer::createAdd()
{
	// 버튼 스프라이트 설정
	auto addNormalSprite1 = Sprite::createWithSpriteFrameName("button_normal.png");
	auto addPressSprite1 = Sprite::createWithSpriteFrameName("button_press.png");

	// 추가 버튼 설정
	menuAdd = MenuItemSprite::create(addNormalSprite1, addPressSprite1, CC_CALLBACK_1(InterfaceLayer::addMenuCallback, this));
	menuAdd->setAnchorPoint(Point::ZERO);
	menuAdd->setPosition(Vec2(origin.x + visibleSize.width * 0.725, origin.y + visibleSize.height * 0.1));

	// 라벨 출력
	labelAdd = Label::createWithSystemFont("Add", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	labelAdd->setAnchorPoint(Point::ZERO);
	labelAdd->setPosition(Vec2(origin.x + visibleSize.width * 0.725, origin.y + visibleSize.height * 0.1));
	labelAdd->setColor(Color3B::BLACK);
	this->addChild(labelAdd, 1);
}

void InterfaceLayer::createDelete()
{
	// 버튼 스프라이트 설정
	auto deleteNormalSprite = Sprite::createWithSpriteFrameName("button_normal.png");
	auto deletePressSprite = Sprite::createWithSpriteFrameName("button_press.png");

	// 삭제 버튼 설정
	menuDelete = MenuItemSprite::create(deleteNormalSprite, deletePressSprite, CC_CALLBACK_1(InterfaceLayer::deleteMenuCallback, this));
	menuDelete->setAnchorPoint(Point::ZERO);
	menuDelete->setPosition(Vec2(origin.x + visibleSize.width * 0.725, origin.y + visibleSize.height * 0.025));

	// 라벨 출력
	auto label = Label::createWithSystemFont("Del", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setAnchorPoint(Point::ZERO);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.725, origin.y + visibleSize.height * 0.025));
	label->setColor(Color3B::BLACK);
	this->addChild(label, 1);
}

void InterfaceLayer::createLevelUp()
{
	// 버튼 스프라이트 설정
	auto buttonNormalSprite = Sprite::createWithSpriteFrameName("button_normal.png");
	auto buttonPressSprite = Sprite::createWithSpriteFrameName("button_press.png");

	// 난이도 증가 버튼 설정
	menuUp = MenuItemSprite::create(buttonNormalSprite, buttonPressSprite, CC_CALLBACK_1(InterfaceLayer::levelUpMenuCallback, this));
	menuUp->setAnchorPoint(Point::ZERO);
	menuUp->setPosition(Vec2(origin.x + visibleSize.width * 0.12, origin.y + visibleSize.height * 0.025));

	// 라벨 출력
	auto label = Label::createWithSystemFont("Up", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setAnchorPoint(Point::ZERO);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.12, origin.y + visibleSize.height * 0.025));
	label->setColor(Color3B::BLACK);
	this->addChild(label, 1);
}

void InterfaceLayer::createLevelDown()
{
	// 버튼 스프라이트 설정
	auto buttonNormalSprite = Sprite::createWithSpriteFrameName("button_normal.png");
	auto buttonPressSprite = Sprite::createWithSpriteFrameName("button_press.png");

	// 난이도 감소 버튼 설정
	menuDown = MenuItemSprite::create(buttonNormalSprite, buttonPressSprite, CC_CALLBACK_1(InterfaceLayer::levelDownMenuCallback, this));
	menuDown->setAnchorPoint(Point::ZERO);
	menuDown->setPosition(Vec2(origin.x + visibleSize.width * 0.19, origin.y + visibleSize.height * 0.025));

	// 라벨 출력
	auto label = Label::createWithSystemFont("Down", "Thonburi", 18, Size(60, 40), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setAnchorPoint(Point::ZERO);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.19, origin.y + visibleSize.height * 0.025));
	label->setColor(Color3B::BLACK);
	this->addChild(label, 1);
}

void InterfaceLayer::createNoteMenu()
{
	// 컬러 레이어 배경 생성
	auto background = LayerColor::create(Color4B(80, 80, 80, 255));
	background->setContentSize(Size(SCROLL_WIDTH, SCROLL_HEIGHT));
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Vec2(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.025));
	this->addChild(background);

	// 컬러 레이어 생성
	auto layer = LayerColor::create(Color4B(150, 150, 150, 255));
	if ((Note::NOTE_KIND::END - 1) * RADIO_HEIGHT >= SCROLL_HEIGHT)
	{
		layer->setContentSize(Size(SCROLL_WIDTH, (Note::NOTE_KIND::END - 1) * RADIO_HEIGHT));
	}
	else
	{
		layer->setContentSize(Size(SCROLL_WIDTH, SCROLL_HEIGHT));
	}

	// 스크롤 뷰 출력
	auto scroll = ScrollView::create(Size(SCROLL_WIDTH, SCROLL_HEIGHT), layer);
	scroll->setAnchorPoint(Point::ZERO);
	scroll->setPosition(Vec2(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.025));
	scroll->setDirection(ScrollView::Direction::VERTICAL);
	scroll->setBounceable(true);
	scroll->setContentOffset(Vec2(0, -RADIO_HEIGHT * (Note::NOTE_KIND::END - 1) + SCROLL_HEIGHT));
	this->addChild(scroll);

	// 라디오 버튼 그룹 생성
	radioGroup = RadioButtonGroup::create();

	for (int count = Note::NOTE_KIND::NONE + 1; count < Note::NOTE_KIND::END; count++)
	{
		// 라디오 버튼 생성
		auto radio = RadioButton::create("white_box.png", "white_box.png", "selector.png", "white_box.png", "white_box.png", Widget::TextureResType::PLIST);
		radio->setAnchorPoint(Point(0.0, 1.0));
		radio->setPosition(Point(0.0, RADIO_HEIGHT * (Note::NOTE_KIND::END - count)));
		layer->addChild(radio);

		// 라벨 생성
		auto label = Label::createWithSystemFont(StringUtils::format(" %s", getString(count)), "Thonburi", 18, Size(RADIO_WIDTH, RADIO_HEIGHT), TextHAlignment::LEFT, TextVAlignment::CENTER);
		label->setAnchorPoint(Point(0.0, 1.0));
		label->setPosition(Point(0.0, RADIO_HEIGHT * (Note::NOTE_KIND::END - count)));
		label->setColor(Color3B::BLACK);
		layer->addChild(label);

		// 라디오 버튼 그룹 추가
		radioGroup->addRadioButton(radio);
		radioGroup->addEventListener(CC_CALLBACK_1(InterfaceLayer::radioButtonCallback, this));
	}
	radioGroup->getRadioButtonByIndex(0)->setSelected(true);
	layer->addChild(radioGroup);
}

void InterfaceLayer::createMenu()
{
	// 메뉴 출력
	auto menu = Menu::create(menuPlayPause, menuSave, menuLoad, menuAdd, menuDelete, menuUp, menuDown, NULL);
	menu->setAnchorPoint(Point::ZERO);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 0);
}

void InterfaceLayer::createSongLabel()
{
	// 라벨 출력
	songLabel = Label::createWithSystemFont("", "Thonburi", 18, Size::ZERO, TextHAlignment::RIGHT);
	songLabel->setAnchorPoint(Point(1.0, 0.0));
	songLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.49, origin.y + visibleSize.height * 0.095));
	songLabel->setColor(Color3B::BLACK);
	this->addChild(songLabel);
}

void InterfaceLayer::createTimeLabel()
{
	// 라벨 출력
	timeLabel = Label::createWithSystemFont("", "Thonburi", 18, Size::ZERO, TextHAlignment::RIGHT);
	timeLabel->setAnchorPoint(Point(1.0, 0.0));
	timeLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.49, origin.y + visibleSize.height * 0.215));
	timeLabel->setColor(Color3B::BLACK);
	this->addChild(timeLabel);
}

void InterfaceLayer::createLevelLabel()
{
	// 라벨 출력
	auto label = Label::createWithSystemFont("level", "Thonburi", 16);
	label->setAnchorPoint(Point::ZERO);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.15, origin.y + visibleSize.height * 0.1));
	label->setColor(Color3B::BLACK);
	this->addChild(label);

	// 난이도 라벨 출력
	levelLabel = Label::createWithSystemFont(StringUtils::format("%d", level), "Thonburi", 30);
	levelLabel->setAnchorPoint(Point::ZERO);
	levelLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.09));
	levelLabel->setColor(Color3B::BLACK);
	this->addChild(levelLabel);
}

void InterfaceLayer::checkSong(const char* _name)
{
	// 노래 정지
	AudioEngine::stopAll();

	// 노래 삭제
	AudioEngine::uncacheAll();

	// 노래 이름 설정
	name = _name;

	struct _finddata_t c_file;
	intptr_t hFile;
	bool folder;
	std::string tempName = name;

	// 검색 시작
	if ((hFile = _findfirst(".\\song\\*.*", &c_file)) == -1L)
	{
		CCLOG("No file(s) in that directory!\n");
	}
	else
	{
		do
		{
			// 읽은 파일이 폴더일 때
			if ((c_file.attrib & _A_SUBDIR))
			{
				folder = TRUE;

				// 이름 확인
				for (unsigned int count = NULL; count < tempName.length(); count++)
				{
					// 이름이 다를 때
					if (c_file.name[count] != name[count])
					{
						folder = FALSE;
						break;
					}
				}

				// 이름이 같을 때
				if (folder)
				{
					// 검색 종료
					_findclose(hFile);

					// 노래 로딩
					AudioEngine::preload(StringUtils::format("./song/%s/%s.mp3", name, name), CC_CALLBACK_1(InterfaceLayer::songLoadFolderCallback, this));

					return;
				}
			}
		} while (_findnext(hFile, &c_file) == 0);

		// 검색 종료
		_findclose(hFile);

		// 노래 로딩
		AudioEngine::preload(StringUtils::format("./song/%s.mp3", name), CC_CALLBACK_1(InterfaceLayer::songLoadCallback, this));
	}
}

void InterfaceLayer::setBySong()
{
	if (AudioEngine::getState(audioID) != AudioEngine::AudioState::ERROR)
	{
		// 노래 현재 시간 저장
		currentTime = AudioEngine::getCurrentTime(audioID);

		// 노래에 따라 슬라이더 변경
		slider->setPercent(currentTime / duration * 100);

		// 노래에 따라 라벨 변경
		songLabel->setString(StringUtils::format("%d:%d / %d:%d", (int)currentTime / 60, (int)currentTime % 60, (int)duration / 60, (int)duration % 60));
	}
}

void InterfaceLayer::setContainer(float _offset)
{
	// 노래 현재 시간 설정
	AudioEngine::setCurrentTime(audioID, _offset / 60 / ((toolScene->BPM + 1) * 5));

	// 노래 현재 시간 저장
	currentTime = AudioEngine::getCurrentTime(audioID);

	// 슬라이더 위치 설정
	slider->setPercent(currentTime / duration * 100);

	// 노래 현재 시간 반환
	previewLayer->setCurrentTime(currentTime);
}

void InterfaceLayer::setCurrentTime(bool _left)
{
	// 왼쪽 키를 눌렀을 때
	if (_left)
	{
		AudioEngine::setCurrentTime(audioID, AudioEngine::getCurrentTime(audioID) - SKIP);
	}
	// 오른쪽 키를 눌렀을 때
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
	// 시간 설정
	time = currentTime * 60.0f - (1280 / (toolScene->BPM + 1) / 5) + 50 - (5 + toolScene->BPM * 10);

	// 시간에 따라 라벨 변경
	timeLabel->setString(StringUtils::format("%d", time));

	// 시간에 따라 버튼 변경
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

		// 오프셋 초기화
		noteListScroll->setContentOffset(Point::ZERO);

		// 오프셋 이동
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

	// 라디오 그룹에서 삭제
	noteListRadioGroup->removeRadioButton(temp->noteRadio);

	// 라디오 버튼 삭제
	noteListLayer->removeChild(temp->noteRadio);

	// 라벨 삭제
	noteListLayer->removeChild(temp->noteLabel);
}

void InterfaceLayer::createNoteListRadioButton()
{
	int size = note->getListSize();

	// 라디오 그룹 삭제
	for (int count = 1; count <= size; count++)
	{
		removeNoteListRadioButton(count);
	}

	// 라디오 그룹 생성
	for (int count = 1; count <= size; count++)
	{
		Note::List* temp = note->searchList(count);

		// 라디오 버튼 생성
		auto radio = RadioButton::create("white_box.png", "white_box.png", "selector.png", "white_box.png", "white_box.png", Widget::TextureResType::PLIST);
		radio->setAnchorPoint(Point(0.0, 1.0));
		radio->setPosition(Point(0.0, noteListLayer->getContentSize().height - RADIO_HEIGHT * (count - 1)));
		radio->setSelected(false);
		noteListLayer->addChild(radio);

		// 라벨 생성
		auto label = Label::createWithSystemFont(StringUtils::format(" %d  %d  %s", temp->noteTag, temp->noteTime, getString(temp->noteKind)), "Thonburi", 16, Size(RADIO_WIDTH, RADIO_HEIGHT), TextHAlignment::LEFT, TextVAlignment::CENTER);
		label->setAnchorPoint(Point(0.0, 1.0));
		label->setPosition(Point(0.0, noteListLayer->getContentSize().height - RADIO_HEIGHT * (count - 1)));
		label->setColor(Color3B::BLACK);
		noteListLayer->addChild(label);

		// 라디오 그룹 추가
		noteListRadioGroup->addRadioButton(radio);
		noteListRadioGroup->addEventListener(CC_CALLBACK_1(InterfaceLayer::noteListRadioButtonCallback, this));

		// 정보 등록
		note->setRadio(count, radio, label);
	}

	// 해당 라디오 선택
	if (selectList)
	{
		noteListRadioGroup->setSelectedButton(note->searchList(selectList)->noteRadio);
	}
}

void InterfaceLayer::setSelectList()
{
	int size = note->getListSize();

	// 선택한게 사이즈보다 클 때
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
		// 라디오 그룹 삭제
		removeNoteListRadioButton(selectList);

		// 스프라이트 삭제
		previewLayer->removeNote(selectList);

		// 노트 삭제
		note->delList(selectList);

		// 태그 정리
		note->setListTag();

		// 레이어 크기 변경
		setNoteListSize(note->getListSize());

		// 노트 목록 선택 설정
		setSelectList();

		// 라디오 그룹 생성
		createNoteListRadioButton();
	}
}

void InterfaceLayer::saveData()
{
	// 파일 연결
	FILE* fp = fopen(StringUtils::format("./song/%s/%s.txt", name, name).c_str(), "w");

	fprintf(fp, "%f\n%d\n%d", duration, toolScene->BPM, level);

	// 파일 연결 해제
	fclose(fp);
}

void InterfaceLayer::loadData()
{
	// 임시 파일 연결
	FILE* _fp = fopen(StringUtils::format("./song/%s/%s.txt", name, name).c_str(), "a");

	// 임시 파일 연결 해제
	fclose(_fp);

	// 파일 연결
	FILE* fp = fopen(StringUtils::format("./song/%s/%s.txt", name, name).c_str(), "r");

	// 노래 길이 넘기기
	float temp;
	fscanf(fp, "%f", &temp);

	// BPM 받아오기
	if (fscanf(fp, "%d", &toolScene->BPM) == -1)
	{
		toolScene->BPM = 1;
	}
	// BPM 설정
	toolScene->setBPM();

	// 레벨 받아오기
	if (fscanf(fp, "%d", &level) == -1)
	{
		level = 1;
	}
	// 난이도 설정
	levelLabel->setString(StringUtils::format("%d", level));

	// 파일 연결 해제
	fclose(fp);
}

void InterfaceLayer::moveFile(const char* _pastDir, const char* _newDir)
{
	size_t size = 0;
	char buffer[BUFSIZ] = { '\0' };

	// 파일 열기
	FILE* in = fopen(_pastDir, "rb");
	FILE* out = fopen(_newDir, "wb");

	if (in == NULL || out == NULL)
	{
		perror("An error occured while opening files!!!");
		in = out = 0;
	}
	else
	{
		// 파일 복사
		while ((size = fread(buffer, BUFSIZ, 1, in)) > 0)
		{
			fwrite(buffer, BUFSIZ, 1, out);
		}

		// 파일 닫기
		fclose(in);
		fclose(out);

		// 파일 삭제
		remove(_pastDir);
	}
}