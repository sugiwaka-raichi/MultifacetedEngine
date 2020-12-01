#pragma once
//=================================
//オブジェクトの基底クラス
//=================================
class OBJECT {
private:
protected:
	int layer;		//レイヤー　表示順決定に使う
	bool show_flag;	//表示フラグ
public:
	virtual void Init() = 0;		//初期化純粋仮想関数
	virtual void Update() = 0;		//更新純粋仮想関数
	virtual void Draw() = 0;		//描画純粋仮想関数
	virtual void UnInit() = 0;		//解放純粋仮想関数
};

