#pragma once
template <typename T>
class LIST {
private:
	//typedef struct _list {
	LIST * next = nullptr;		//次アドレス
	LIST *current = nullptr;	//現在アドレス
	LIST *prev = nullptr;		//前アドレス

	T*	container = nullptr;	//格納
public:
	LIST();
	~LIST();

	//--------------------------------------------------
	//リスト操作
	//--------------------------------------------------
	bool Next();		//次アドレス
	bool Prev();		//前アドレス
	bool Front();		//先頭アドレス
	bool Last();		//末尾アドレス
						//void FrontPrev();	//先頭アドレスの前
						//void LastNext();	//末尾アドレスの先

	//--------------------------------------------------
	//ゲッター
	//--------------------------------------------------
	LIST* GetNext();	//次アドレス
	LIST* GetCurrent();	//現在アドレス
	LIST* GetPrev();	//前アドレス
	LIST* GetCenter();	//先頭アドレス
	LIST* GetLast();	//末尾アドレス
	T* GetContainer();		//データを返却(ポインタ)
	int GetSize();		//要素数

	bool AddList(T* _obj);			//リストに追加
	bool DeleteList();
	bool DeleteList(unsigned int _subscript);		//リストから削除
	bool Clear();					//リスト削除
};

template <typename T>
LIST<T>::LIST() {
	//current = this;
}

template <typename T>
LIST<T>::~LIST() {
	//delete container;	//削除されたとき開放する
}

//===============================================================================
//アドレス位置制御
//===============================================================================
//----------------------------------------------------
//次アドレスをcurrentへ
//----------------------------------------------------
template <typename T>
bool LIST<T>::Next() {
	if (current != nullptr) {
		if (current->next != nullptr) {
			current = current->next;		//現在アドレスから次のアドレスへ
			return true;
		}
	}
	return false;
}

//----------------------------------------------------
//前アドレスをcurrentへ
//----------------------------------------------------
template <typename T>
bool LIST<T>::Prev() {
	if (current != nullptr) {
		if (current->prev != nullptr) {
			current = current->prev;		//現在アドレスから前のアドレスへ
			return true;
		}
	}
	return false;

}

//----------------------------------------------------
//先頭アドレスをcurrentへ
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
//末尾アドレスをcurrentへ
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
//ゲッター
//====================================================================
template <typename T>
LIST<T>* LIST<T>::GetCurrent() {
	return current;		//現在アドレスを返値
}

template<typename T>
LIST<T>* LIST<T>::GetNext() {
	return current->next;	//次アドレスを返値
}

template <typename T>
LIST<T>* LIST<T>::GetPrev() {
	return current->prev;	//前アドレスを返値
}

template <typename T>
LIST<T>* LIST<T>::GetCenter() {
	if (current != nullptr) {
		LIST* _obj = current;		//現在のアドレスを取得
		while (_obj->prev != nullptr) {
			_obj = _obj->prev;		//前アドレスを代入していく
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
		LIST* _obj = current;		//現在のアドレスを取得
		while (_obj->next != nullptr) {
			_obj = _obj->next;		//次アドレスを代入していく
		}
		return _obj;
	}
	else {
		return nullptr;
	}
}

template<typename T>
T* LIST<T>::GetContainer() {
	return current->container;		//オブジェクトを返却
}

template <typename T>
int LIST<T>::GetSize() {
	int cnt = 0;
	LIST* _obj = GetCenter();		//仮オブジェクトに先頭アドレスを入れる

	while (_obj != nullptr) {
		cnt++;		//要素数をカウントしていく
					//次のアドレスへ回していく
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
//リスト
//==============================================================================

//-------------------------------------------------------------------
//末尾に追加
//-------------------------------------------------------------------
template <typename T>
bool LIST<T>::AddList(T* _obj) {
	if (Last()) {
		current->next = new LIST;			//次アドレスにリストを作成
		current->next->prev = current;		//次アドレスの前アドレスに自アドレスを代入
		current->next->current = current->next;
		current = current->next;			//次アドレスに移動
		current->container = _obj;				//末尾アドレスの次アドレス位置に追加する
		return true;
	}
	else {
		current = new LIST;
		current->container = _obj;				//初期アドレスを設定
		return true;
	}

}

//-------------------------------------------------------------------
//現在のオブジェクトを削除
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
//指定のオブジェクトを削除
//-------------------------------------------------------------------
template <typename T>
bool LIST<T>::DeleteList(unsigned int _subscript) {
	bool flag;
	LIST* _obj = GetCenter();//current;
							 //_obj->Front();
							 //添字が要素数に収まっているかどうか
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
//リスト削除
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

