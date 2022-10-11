#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"
#include "ui\UIRadioButton.h"

USING_NS_CC;

struct List;
class PreviewLayer;

class Note
{
public:
	// 노트 링크드 리스트 구조체
	struct List
	{
		int noteTag;
		unsigned int noteTime;
		int noteKind;
		Sprite* noteSprite;
		Sprite* subBackSprite;
		Sprite* subFrontSprite;
		ui::RadioButton* noteRadio;
		Label* noteLabel;
		List* next;
	};

	// 프리뷰 레이어 변수
	PreviewLayer* previewLayer;

	// 리스트 추가 함수
	void addList(unsigned int _time, int _kind);

	// 리스트 길이 반환 함수
	int getListSize();

	// 리스트 태그 정리 함수
	void setListTag();

	// 리스트 검색 함수
	List* searchList(int _tag);

	// 리스트 삭제 함수
	void delList(int _tag);

	// 라디오 버튼 설정 함수
	void setRadio(int _tag, ui::RadioButton* _radio, Label* _label);

	// 서브 존재 반환 함수
	bool checkSub(List* _temp, bool _sub);

	// 리스트 저장 함수
	void saveList(FILE* _fp);

	// 리스트 불러오기 함수
	void loadList(FILE* _fp);

	// 노트 종류 열거형
	enum NOTE_KIND
	{
		NONE, TIRE, BOX, TRASH, BARRICADE, BIKE, SIGN, LONGBOX, LONGAD, HUMAN, NAVIGATOR, CONTAINER, STONE, CRAIN, H_BEAM, END
	};

	// 서브 종류 열거형
	enum SUB
	{
		BACK, FRONT
	};

private:
	Size visibleSize;
	Vec2 origin;

	// 링크드 리스트 머리 변수
	List* head;

	// 링크드 리스트 꼬리 변수
	List* tail;

	// 파일 변수
	FILE* file;

	// 서브 뒤 스프라이트 설정 함수
	Sprite* getSubBackSprite(List* _temp);

	// 서브 앞 스프라이트 설정 함수
	Sprite* getSubFrontSprite(List* _temp);

	// 스프라이트 설정 함수
	Sprite* getNoteSprite(List* _temp);

	// 스케일 퍼센트 상수
	const float SCALE = 0.75f;

public:
	// 생성자
	Note();

	// 소멸자
	~Note();
};

#endif
