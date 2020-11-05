#pragma once
template <typename T>
class LIST {
private:
	//typedef struct _list {
	LIST * next = nullptr;		//���A�h���X
	LIST *current = nullptr;	//���݃A�h���X
	LIST *prev = nullptr;		//�O�A�h���X

	T*	container = nullptr;	//�i�[
public:
	LIST();
	~LIST();

	//--------------------------------------------------
	//���X�g����
	//--------------------------------------------------
	bool Next();		//���A�h���X
	bool Prev();		//�O�A�h���X
	bool Front();		//�擪�A�h���X
	bool Last();		//�����A�h���X
						//void FrontPrev();	//�擪�A�h���X�̑O
						//void LastNext();	//�����A�h���X�̐�

	//--------------------------------------------------
	//�Q�b�^�[
	//--------------------------------------------------
	LIST* GetNext();	//���A�h���X
	LIST* GetCurrent();	//���݃A�h���X
	LIST* GetPrev();	//�O�A�h���X
	LIST* GetCenter();	//�擪�A�h���X
	LIST* GetLast();	//�����A�h���X
	T* GetContainer();		//�f�[�^��ԋp(�|�C���^)
	int GetSize();		//�v�f��

	bool AddList(T* _obj);			//���X�g�ɒǉ�
	bool DeleteList();
	bool DeleteList(unsigned int _subscript);		//���X�g����폜
	bool Clear();					//���X�g�폜
};

template <typename T>
LIST<T>::LIST() {
	//current = this;
}

template <typename T>
LIST<T>::~LIST() {
	//delete container;	//�폜���ꂽ�Ƃ��J������
}

//===============================================================================
//�A�h���X�ʒu����
//===============================================================================
//----------------------------------------------------
//���A�h���X��current��
//----------------------------------------------------
template <typename T>
bool LIST<T>::Next() {
	if (current != nullptr) {
		if (current->next != nullptr) {
			current = current->next;		//���݃A�h���X���玟�̃A�h���X��
			return true;
		}
	}
	return false;
}

//----------------------------------------------------
//�O�A�h���X��current��
//----------------------------------------------------
template <typename T>
bool LIST<T>::Prev() {
	if (current != nullptr) {
		if (current->prev != nullptr) {
			current = current->prev;		//���݃A�h���X����O�̃A�h���X��
			return true;
		}
	}
	return false;

}

//----------------------------------------------------
//�擪�A�h���X��current��
//----------------------------------------------------
template <typename T>
bool LIST<T>::Front() {
	if (current != nullptr) {
		if (current->prev != nullptr) {
			while (current->prev != nullptr) {
				current = current->prev;
			}
			return true;
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}

//----------------------------------------------------
//�����A�h���X��current��
//----------------------------------------------------
template <typename T>
bool LIST<T>::Last() {
	if (current != nullptr) {
		if (current->next != nullptr) {
			while (current->next != nullptr) {
				current = current->next;
			}
			return true;
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}

//====================================================================
//�Q�b�^�[
//====================================================================
template <typename T>
LIST<T>* LIST<T>::GetCurrent() {
	return current;		//���݃A�h���X��Ԓl
}

template<typename T>
LIST<T>* LIST<T>::GetNext() {
	return current->next;	//���A�h���X��Ԓl
}

template <typename T>
LIST<T>* LIST<T>::GetPrev() {
	return current->prev;	//�O�A�h���X��Ԓl
}

template <typename T>
LIST<T>* LIST<T>::GetCenter() {
	if (current != nullptr) {
		LIST* _obj = current;		//���݂̃A�h���X���擾
		while (_obj->prev != nullptr) {
			_obj = _obj->prev;		//�O�A�h���X�������Ă���
		}
		return _obj;
	}
	else {
		return nullptr;
	}
}

template <typename T>
LIST<T>* LIST<T>::GetLast() {
	if (this != nullptr) {
		LIST* _obj = current;		//���݂̃A�h���X���擾
		while (_obj->next != nullptr) {
			_obj = _obj->next;		//���A�h���X�������Ă���
		}
		return _obj;
	}
	else {
		return nullptr;
	}
}

template<typename T>
T* LIST<T>::GetContainer() {
	return current->container;		//�I�u�W�F�N�g��ԋp
}

template <typename T>
int LIST<T>::GetSize() {
	int cnt = 0;
	LIST* _obj = GetCenter();		//���I�u�W�F�N�g�ɐ擪�A�h���X������

	while (_obj != nullptr) {
		cnt++;		//�v�f�����J�E���g���Ă���
					//���̃A�h���X�։񂵂Ă���
		if (_obj->next != nullptr) {
			_obj = _obj->next;
		}
		else {
			break;
		}
	}
	return cnt;
}

//==============================================================================
//���X�g
//==============================================================================

//-------------------------------------------------------------------
//�����ɒǉ�
//-------------------------------------------------------------------
template <typename T>
bool LIST<T>::AddList(T* _obj) {
	if (Last()) {
		current->next = new LIST;			//���A�h���X�Ƀ��X�g���쐬
		current->next->prev = current;		//���A�h���X�̑O�A�h���X�Ɏ��A�h���X����
		current->next->current = current->next;
		current = current->next;			//���A�h���X�Ɉړ�
		current->container = _obj;				//�����A�h���X�̎��A�h���X�ʒu�ɒǉ�����
		return true;
	}
	else {
		current = new LIST;
		current->container = _obj;				//�����A�h���X��ݒ�
		return true;
	}

}

//-------------------------------------------------------------------
//���݂̃I�u�W�F�N�g���폜
//-------------------------------------------------------------------
template <typename T>
bool LIST<T>::DeleteList() {
	bool flag = false;
	if (current != nullptr) {
		LIST* _obj = current;
		if (_obj->prev != nullptr) {
			_obj->prev->next = _obj->next;
			current = _obj->prev;
			flag = true;
		}
		if (_obj->next != nullptr) {
			_obj->next->prev = _obj->prev;
			current = _obj->next;
			flag = true;
		}
		if (!flag) {
			current = nullptr;
		}
		delete _obj->container;
		delete _obj;
		return true;
	}
	else {
		return false;
	}
}

//-------------------------------------------------------------------
//�w��̃I�u�W�F�N�g���폜
//-------------------------------------------------------------------
template <typename T>
bool LIST<T>::DeleteList(unsigned int _subscript) {
	bool flag;
	LIST* _obj = GetCenter();//current;
							 //_obj->Front();
							 //�Y�����v�f���Ɏ��܂��Ă��邩�ǂ���
	if (!(current->GetSize() < _subscript)) {
		for (int i = 0; i < _subscript; i++) {
			_obj = _obj->next;
		}

		if (_obj == current) {
			current = nullptr;
		}
		if (_obj->prev != nullptr) {
			_obj->prev->next = _obj->next;
			if (_obj == current) {
				current = _obj->prev;
			}
		}
		if (_obj->next != nullptr) {
			_obj->next->prev = _obj->prev;
			if (_obj == current) {
				current = _obj->next;
			}
		}
		delete _obj->container;
		delete _obj;
		return true;
	}
	else {
		return false;
	}
}

//---------------------------------------------------
//���X�g�폜
//---------------------------------------------------
template <typename T>
bool LIST<T>::Clear() {
	LIST* _obj;
	if (current != nullptr) {
		for (int i = GetSize(); i > 0; i--) {
			DeleteList(0);
		}
		return true;
	}
	else {
		return false;
	}
}

