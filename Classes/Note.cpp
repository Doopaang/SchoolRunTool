#include "Note.h"
#include "PreviewLayer.h"
#include "InterfaceLayer.h"

Note::Note() :head(NULL), tail(NULL)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// ���� ����
	file = fopen("sample", "rb");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/notes.plist");
}

Note::~Note()
{
	int size = getListSize();

	for (int count = 1; count <= size; count++)
	{
		// ��Ʈ ����
		delList(1);

		// �±� ����
		setListTag();
	}
}

void Note::addList(unsigned int _time, int _kind)
{
	// ���� �Ҵ�
	List* temp = new List;

	// ���� �Է�
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

	// ù ����Ʈ�� ��
	if (!head)
	{
		// �Ӹ��� ���� ����
		head = temp;
		tail = temp;
	}
	// ù ����Ʈ�� �ƴ� ��
	else
	{
		int size = getListSize();
		List* _temp = head;

		for (int count = 1; count <= size; count++)
		{
			// �̹� ���� �ð��� ��Ʈ�� ���� ��
			if (_temp->noteTime == _time)
			{
				// ��������Ʈ ����
				previewLayer->removeNote(_temp->noteTag);

				// ���� �Է�
				_temp->noteTag = temp->noteTag;
				_temp->noteKind = _kind;
				_temp->noteSprite = temp->noteSprite;
				_temp->subBackSprite = temp->subBackSprite;
				_temp->subFrontSprite = temp->subFrontSprite;

				// �Ҵ� ����
				delete temp;

				return;
			}
			// ��Ʈ �ð��� ���� �ð����� Ŀ���� ��
			else if (_temp->noteTime > _time)
			{
				// ���� �Է�
				temp->next = _temp;

				// �Ӹ��� ��
				if (_temp == head)
				{
					// �Ӹ� ����
					head = temp;
				}
				// ������ ��
				else
				{
					// ����Ʈ ����
					searchList(_temp->noteTag - 1)->next = temp;
				}

				return;
			}

			// ���� ����Ʈ�� �̵�
			_temp = _temp->next;
		}

		// ���� �ð��� ��Ʈ �ð����� Ŭ ��
		// ���� �Է�
		tail->next = temp;

		// ���� ����
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
		// �±� ����
		temp->noteTag = count;

		// ���� ����Ʈ�� �̵�
		temp = temp->next;
	}
}

void Note::delList(int _tag)
{
	List* temp = searchList(_tag);

	// �Ӹ��� ��
	if (temp == head)
	{
		// �Ӹ� ����
		head = temp->next;

		// ���� �Ҵ� ����
		delete temp;
	}
	// ������ ��
	else if (temp == tail)
	{
		List* _temp = searchList(_tag - 1);

		// ���� ����
		_temp->next = NULL;
		tail = _temp;

		// ���� �Ҵ� ����
		delete temp;
	}
	// ������ ��
	else
	{
		List* _temp = searchList(_tag - 1);

		// ���� �̾��ֱ�
		_temp->next = _temp->next->next;

		// ���� �Ҵ� ����
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
	// ���� �� ����
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
	// ���� �� ����
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
		// ���� �ۼ�
		fprintf_s(_fp, "%d %d %d\n", temp->noteTag, temp->noteTime, temp->noteKind);

		// ���� ����Ʈ�� �̵�
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
		// �ʱ�ȭ
		tag = NULL;

		// ���� �ҷ�����
		fscanf_s(_fp, "%d %d %d", &tag, &time, &kind);

		// ���� ������ ����
		if (!tag)
		{
			return;
		}

		// ����Ʈ �߰�
		addList(time, kind);

		// ��������Ʈ ���
		previewLayer->printNote(time);

		// �±� ����
		setListTag();

	} while (tag);
}