#include "Note.h"
#include "PreviewLayer.h"
#include "InterfaceLayer.h"

Note::Note() :head(NULL), tail(NULL)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// 파일 열기
	file = fopen("sample", "rb");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/notes.plist");
}

Note::~Note()
{
	int size = getListSize();

	for (int count = 1; count <= size; count++)
	{
		// 노트 삭제
		delList(1);

		// 태그 정리
		setListTag();
	}
}

void Note::addList(unsigned int _time, int _kind)
{
	// 동적 할당
	List* temp = new List;

	// 정보 입력
	temp->noteTag = NULL;
	temp->noteTime = _time;
	temp->noteKind = _kind;
	if (checkSub(temp, BACK))
	{
		temp->subBackSprite = getSubBackSprite(temp);
	}
	else
	{
		temp->subBackSprite = NULL;
	}
	if (checkSub(temp, FRONT))
	{
		temp->subFrontSprite = getSubFrontSprite(temp);
	}
	else
	{
		temp->subFrontSprite = NULL;
	}
	temp->noteSprite = getNoteSprite(temp);
	temp->next = NULL;

	// 첫 리스트일 때
	if (!head)
	{
		// 머리와 꼬리 설정
		head = temp;
		tail = temp;
	}
	// 첫 리스트가 아닐 때
	else
	{
		int size = getListSize();
		List* _temp = head;

		for (int count = 1; count <= size; count++)
		{
			// 이미 같은 시간의 노트가 있을 때
			if (_temp->noteTime == _time)
			{
				// 스프라이트 삭제
				previewLayer->removeNote(_temp->noteTag);

				// 정보 입력
				_temp->noteTag = temp->noteTag;
				_temp->noteKind = _kind;
				_temp->noteSprite = temp->noteSprite;
				_temp->subBackSprite = temp->subBackSprite;
				_temp->subFrontSprite = temp->subFrontSprite;

				// 할당 해제
				delete temp;

				return;
			}
			// 노트 시간이 받은 시간보다 커졌을 때
			else if (_temp->noteTime > _time)
			{
				// 정보 입력
				temp->next = _temp;

				// 머리일 때
				if (_temp == head)
				{
					// 머리 설정
					head = temp;
				}
				// 몸통일 때
				else
				{
					// 리스트 연결
					searchList(_temp->noteTag - 1)->next = temp;
				}

				return;
			}

			// 다음 리스트로 이동
			_temp = _temp->next;
		}

		// 받은 시간이 노트 시간보다 클 때
		// 정보 입력
		tail->next = temp;

		// 꼬리 설정
		tail = temp;
	}

	return;
}

int Note::getListSize()
{
	int count = NULL;
	
	if (head == NULL)
	{
		return 0;
	}
	else
	{
		List* temp = head;

		while (temp->next)
		{
			count++;
			temp = temp->next;
		}
		return ++count;
	}
}

Note::List* Note::searchList(int _tag)
{
	List* temp = head;

	int size = getListSize();

	for (int count = 0; count < size; count++)
	{
		if (temp->noteTag == _tag)
		{
			return temp;
		}
		else
		{
			temp = temp->next;
		}
	}
	return nullptr;
}

void Note::setListTag()
{
	int size = getListSize();
	List* temp = head;

	for (int count = 1; count <= size; count++)
	{
		// 태그 설정
		temp->noteTag = count;

		// 다음 리스트로 이동
		temp = temp->next;
	}
}

void Note::delList(int _tag)
{
	List* temp = searchList(_tag);

	// 머리일 때
	if (temp == head)
	{
		// 머리 설정
		head = temp->next;

		// 동적 할당 해제
		delete temp;
	}
	// 꼬리일 때
	else if (temp == tail)
	{
		List* _temp = searchList(_tag - 1);

		// 꼬리 설정
		_temp->next = NULL;
		tail = _temp;

		// 동적 할당 해제
		delete temp;
	}
	// 몸통일 때
	else
	{
		List* _temp = searchList(_tag - 1);

		// 양쪽 이어주기
		_temp->next = _temp->next->next;

		// 동적 할당 해제
		delete temp;
	}
}

void Note::setRadio(int _tag, ui::RadioButton* _radio, Label* _label)
{
	List* temp = searchList(_tag);

	temp->noteRadio = _radio;
	temp->noteLabel = _label;
}

bool Note::checkSub(List* _temp, bool _sub)
{
	// 서브 뒤 존재
	if (_sub == BACK)
	{
		switch (_temp->noteKind)
		{
		case NAVIGATOR:
		case CONTAINER:
			return TRUE;

		default:
			return FALSE;
		}
	}
	// 서브 앞 존재
	else
	{
		switch (_temp->noteKind)
		{
		case STONE:
			return TRUE;

		default:
			return FALSE;
		}
	}
}

Sprite* Note::getSubBackSprite(List* _temp)
{
	switch (_temp->noteKind)
	{
	case NAVIGATOR:
		_temp->subBackSprite = Sprite::createWithSpriteFrameName("sub_back_navigator.png");
		_temp->subBackSprite->setAnchorPoint(Point::ZERO);
		_temp->subBackSprite->setPositionY(origin.y + visibleSize.height * 0.2 * SCALE);
		break;

	case CONTAINER:
		_temp->subBackSprite = Sprite::createWithSpriteFrameName("sub_back_container.png");
		_temp->subBackSprite->setAnchorPoint(Point::ZERO);
		_temp->subBackSprite->setPositionY(origin.y + visibleSize.height * 0.125 * SCALE);
		break;
	}
	_temp->subBackSprite->setScale(SCALE);

	return _temp->subBackSprite;
}

Sprite* Note::getSubFrontSprite(List* _temp)
{
	switch (_temp->noteKind)
	{
	case STONE:
		_temp->subFrontSprite = Sprite::createWithSpriteFrameName("sub_front_stone.png");
		_temp->subFrontSprite->setAnchorPoint(Point::ZERO);
		_temp->subFrontSprite->setPositionY(origin.y + visibleSize.height * 0.13 * SCALE);
		break;
	}
	_temp->subFrontSprite->setScale(SCALE);

	return _temp->subFrontSprite;
}

Sprite* Note::getNoteSprite(List* _temp)
{
	switch (_temp->noteKind)
	{
	case TIRE:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_tire.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPositionY(origin.y + visibleSize.height * 0.145 * SCALE);
		break;

	case CONTAINER:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_container.png");
		_temp->noteSprite->setAnchorPoint(Point(0.0f, 1.0f));
		_temp->noteSprite->setPositionY(720 * SCALE);
		break;

	case NAVIGATOR:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_navigator.png");
		_temp->noteSprite->setAnchorPoint(Point(0.0f, 1.0f));
		_temp->noteSprite->setPositionY(_temp->subBackSprite->getPosition().y + _temp->subBackSprite->getContentSize().height * SCALE);
		break;

	case BOX:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_box.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPositionY(origin.y + visibleSize.height * 0.14 * SCALE);
		break;

	case TRASH:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_trash.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPositionY(origin.y + visibleSize.height * 0.15 * SCALE);
		break;

	case LONGBOX:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_longbox.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPositionY(origin.y + visibleSize.height * 0.145 * SCALE);
		break;

	case LONGAD:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_longad.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPositionY(origin.y + visibleSize.height * 0.15 * SCALE);
		break;

	case HUMAN:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_human.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPositionY(origin.y + visibleSize.height * 0.15 * SCALE);
		break;

	case STONE:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_stone.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPositionY(origin.y + visibleSize.height * 0.13 * SCALE);
		break;

	case BARRICADE:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_barricade.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPositionY(origin.y + visibleSize.height * 0.1 * SCALE);
		break;

	case BIKE:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_bike.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPositionY(origin.y + visibleSize.height * 0.1 * SCALE);
		break;

	case SIGN:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_sign.png");
		_temp->noteSprite->setAnchorPoint(Point(0.0, 0.555f));
		_temp->noteSprite->setPositionY(origin.y + (visibleSize.height * 0.15 - 1) * SCALE);
		break;

	case CRAIN:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_crain.png");
		_temp->noteSprite->setAnchorPoint(Point(0.0f, 1.0f));
		_temp->noteSprite->setPositionY(720 * SCALE);
		break;

	case H_BEAM:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_h-beam.png");
		_temp->noteSprite->setAnchorPoint(Point(0.0f, 1.0f));
		_temp->noteSprite->setPositionY(720 * SCALE);
	}
	_temp->noteSprite->setScale(SCALE);

	return _temp->noteSprite;
}

void Note::saveList(FILE* _fp)
{
	int size = getListSize();
	List* temp = head;

	for (int count = 1; count <= size; count++)
	{
		// 파일 작성
		fprintf_s(_fp, "%d %d %d\n", temp->noteTag, temp->noteTime, temp->noteKind);

		// 다음 리스트로 이동
		temp = temp->next;
	}
}

void Note::loadList(FILE* _fp)
{
	int tag = NULL;
	unsigned int time = NULL;
	int kind = NULL;

	do
	{
		// 초기화
		tag = NULL;

		// 파일 불러오기
		fscanf_s(_fp, "%d %d %d", &tag, &time, &kind);

		// 내용 없으면 종료
		if (!tag)
		{
			return;
		}

		// 리스트 추가
		addList(time, kind);

		// 스프라이트 출력
		previewLayer->printNote(time);

		// 태그 정리
		setListTag();

	} while (tag);
}