#ifndef __PREVIEW_LAYER_H__
#define __PREVIEW_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class InterfaceLayer;
class Note;
class ToolScene;

class PreviewLayer : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(PreviewLayer);

	// 툴 신 변수
	ToolScene* toolScene;

	// 인터페이스 레이어 변수
	InterfaceLayer* interfaceLayer;

	// 노트 클래스 변수
	Note* note;

	// 노래 길이 저장
	void setDuration(float _time);

	// 노래 현재 시간 저장
	void setCurrentTime(float _time);

	// 노트 출력 함수
	void printNote(unsigned int _time);

	// 노트 삭제 함수
	void removeNote(int _tag);

private:
	Size visibleSize;
	Vec2 origin;

	// 컬러 레이어 변수
	LayerColor* layer;

	// 스크롤 뷰 변수
	ScrollView* scroll;

	// 배경 화면 변수
	Sprite* background;

	// 배경 화면 보조 변수
	Sprite* backgroundSub;

	// 노래 길이 변수
	float duration;

	// 노래 현재 시간 변수
	float currentTime;

	// 업데이트
	void update(float delta);

	// 배경화면 출력 함수
	void printBackground();

	// 스크롤 뷰 생성 함수
	void createScroll();

	// 배경화면 이동 함수
	void moveBackground();

	// 스크롤에 따라 변경
	void setByScroll();

public:
	// 생성자
	PreviewLayer();
};

#endif
