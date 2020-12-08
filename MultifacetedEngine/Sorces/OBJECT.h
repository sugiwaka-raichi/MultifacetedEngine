#pragma once
//=================================
//オブジェクトの基底クラス
//=================================
class OBJECT {
private:
protected:
	int layer = 0;			//レイヤー　表示順決定に使う
	bool dispFlag = true;	//表示フラグ
public:
	virtual void Init() = 0;		//初期化純粋仮想関数
	virtual void Update() = 0;		//更新純粋仮想関数
	virtual void Draw() = 0;		//描画純粋仮想関数
	virtual void UnInit() = 0;		//解放純粋仮想関数
	
	void SetLayernum(int _layer);
	void SetDispFlag(bool _flag);
	int GetLayernum();				//レイヤー番号取得
	bool GetDispFlag();				//表示フラグ取得

};

