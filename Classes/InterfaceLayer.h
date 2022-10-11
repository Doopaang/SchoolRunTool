#ifndef __INTERFACE_LAYER_H__
#define __INTERFACE_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui\UISlider.h"
#include "ui\UIRadioButton.h"
#include "AudioEngine.h"
#include <io.h>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace experimental;
using namespace ui;

class PreviewLayer;
class Note;
class ToolScene;

class InterfaceLayer : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(InterfaceLayer);

	// 툴 신 변수
	ToolScene* toolScene;

	// 프리뷰 레이어 변수
	PreviewLayer* previewLayer;

	// 노트 리스트 변수
	Note* note;

	// 오디오 아이디 변수
	int audioID;

	// 노래 체크 함수
	void checkSong(const char* _name);

	// 스크롤 조종 설정 함수
	void setContainer(float _offset);

	// 노래 시간 조절 함수
	void setCurrentTime(bool _back);

	// 노래 재생 조절 함수
	void setPlaySong(bool _select);

	// 노트 목록 선택 반환 함수
	int getSelectList();

	// 초기화 함수
	void reset();

private:
	Size visibleSize;
	Vec2 origin;
	
	// 기본 버튼 스프라이트
	Sprite* buttonNormalSprite;

	// 눌림 버튼 스프라이트
	Sprite* buttonPressSprite;

	// 재생/일시정지 메뉴 변수
	MenuItemToggle* menuPlayPause;

	// 저장 메뉴 변수
	MenuItemSprite* menuSave;

	// 불러오기 메뉴 변수
	MenuItemSprite* menuLoad;

	// 추가 메뉴 변수
	MenuItemSprite* menuAdd;

	// 추가 라벨 변수
	Label* labelAdd;

	// 삭제 메뉴 변수
	MenuItemSprite* menuDelete;

	// 난이도 증가 메뉴 변수
	MenuItemSprite* menuUp;

	// 난이도 감소 메뉴 변수
	MenuItemSprite* menuDown;

	// 슬라이더 변수
	Slider* slider;

	// 라디오 버튼 그룹 변수
	RadioButtonGroup* radioGroup;

	// 노트 목록 컬러 레이어 변수
	LayerColor* noteListLayer;

	// 노트 목록 스크롤 뷰 변수
	ScrollView* noteListScroll;

	// 노트 목록 라디오 그룹 변수
	RadioButtonGroup* noteListRadioGroup;

	// 노래 이름 변수
	const char* name;

	// 노래 길이 변수
	float duration;

	// 노래 현재 시간 변수
	float currentTime;

	// 노래 라벨 변수
	Label* songLabel;

	// 시간 변수
	int time;

	// 시간 라벨 변수
	Label* timeLabel;

	// 레벨 변수
	int level;

	// 레벨 라벨 변수
	Label* levelLabel;

	// 노트 종류 선택 변수
	int selectKind;

	// 노트 목록 선택 변수
	int selectList;

	// 슬라이더 생성 함수
	void createSlider();

	// 재생/일시정지 버튼 생성 함수
	void createPlayPause();

	// 스크롤 뷰 생성 함수
	void createScroll();

	// 저장 버튼 생성 함수
	void createSave();

	// 불러오기 버튼 생성 함수
	void createLoad();

	// 추가 버튼 생성 함수
	void createAdd();

	// 삭제 버튼 생성 함수
	void createDelete();

	// 난이도 증가 버튼 생성 함수
	void createLevelUp();

	// 난이도 감소 버튼 생성 함수
	void createLevelDown();

	// 노트 목록 메뉴 생성 함수
	void createNoteMenu();

	// 메뉴 생성 함수
	void createMenu();

	// 노래 라벨 생성 함수
	void createSongLabel();

	// 시간 라벨 생성 함수
	void createTimeLabel();

	// 난이도 라벨 생성 함수
	void createLevelLabel();

	// 업데이트
	void update(float delta);

	// 재생/일시정지 메뉴 콜백 함수
	void playMenuCallback(Ref *sender);

	// 저장 메뉴 콜백 함수
	void saveMenuCallback(Ref *sender);

	// 불러오기 메뉴 콜백 함수
	void loadMenuCallback(Ref *sender);

	// 추가 메뉴 콜백 함수
	void addMenuCallback(Ref *sender);

	// 삭제 메뉴 콜백 함수
	void deleteMenuCallback(Ref *sender);

	// 난이도 증가 메뉴 콜백 함수
	void levelUpMenuCallback(Ref *sender);

	// 난이도 감소 메뉴 콜백 함수
	void levelDownMenuCallback(Ref *sender);

	// 슬라이더 이벤트 리스너 함수
	void sliderEvent(Ref *sender, SliderEventType type);

	// 라디오 버튼 콜백 함수
	void radioButtonCallback(Ref *sender);

	// 노트 목록 라디오 버튼 콜백 함수
	void noteListRadioButtonCallback(Ref *sender);

	// 노래 로딩 폴더 콜백 함수
	void InterfaceLayer::songLoadFolderCallback(bool isSuccess);

	// 노래 로딩 콜백 함수
	void InterfaceLayer::songLoadCallback(bool isSuccess);

	// 노래 끝남 콜백 함수
	void songFinishCallback();

	// 노래에 따라 슬라이더 변경 함수
	void setBySong();

	// 시간 설정
	void setTime();

	// 문자열 반환 함수
	char* getString(const int _NUM);

	// 노트 목록 사이즈 설정 함수
	void setNoteListSize(int _size);

	// 노트 목록 라디오 버튼 삭제 함수
	void removeNoteListRadioButton(int _tag);

	// 노트 목록 라디오 버튼 생성 함수
	void createNoteListRadioButton();

	// 노트 목록 선택 설정 함수
	void setSelectList();

	// 노래 정보 저장 함수
	void saveData();

	// 노래 정보 불러오기 함수
	void loadData();

	// 파일 이동 함수
	void moveFile(const char* _pastDir, const char* _newDir);

	// 라디오 버튼 높이 상수
	const int RADIO_HEIGHT = 32;

	// 라디오 버튼 너비 상수
	const int RADIO_WIDTH = 160;

	// 스크롤 뷰 높이 상수
	const int SCROLL_HEIGHT = 200;

	// 스크롤 뷰 너비 상수
	const int SCROLL_WIDTH = 180;

	// 시간 넘기기 상수
	const float SKIP = 3.0f;

	// 난이도 최대치 상수
	const int LEVEL_MAX = 3;

public:
	// 생성자
	InterfaceLayer();

	// 소멸자
	~InterfaceLayer();
};

#endif
