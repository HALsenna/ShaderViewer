#pragma once
#include "Model.h"
#include "ConstantBuffer.h"
#include "Sound.h"

using namespace DirectX;
class Cmeat : public Model
{
public:
	Cmeat(Model* pModel, XMFLOAT3 pos);
	~Cmeat();
	void Init();
	void Update();
	void Draw();

public:
	enum STATE
	{
		STATE_IDLE = 0,			// 通常状態
		STATE_GRILLD,			// 焼かれている状態
		STATE_STAGING,			// 演出状態
		STATE_MAX,
	};

	

	// 評価
	enum EVALUTION
	{
		EVA_RARE = 0,		// 生焼け
		EVA_GRILLD,			// こんがり
		EVA_BURNT,			// 焦げ
		EVA_MAX,
	};

public:
	enum KIND
	{
		KIND_ROW = 0,		// 生
		KIND_RARE,			// 生焼け
		KIND_GRILLD,		// こんがり
		KIND_BURNT,			// 焦げ
		KIND_MAX,
	};

private:
	void ChangeState(STATE state);		// 演出状態を変更する
	void UpdateState(STATE state);		// 演出状態の更新処理
	EVALUTION CheckMeatKind();				// お肉の焼け具合を評価する

private:
	XMFLOAT3 m_pos;			// 座標
	Model* m_pModel;		// モデル
	eBGM m_stagingBGM[EVA_MAX];		// 演出BGM
	EVALUTION m_evalution;
public:
	float m_maxCookTime;
	float m_condition = 0.0f;
	KIND m_meatKind = KIND_ROW;		// お肉の種類
	STATE m_stagingState = STATE_IDLE;	// 演出の状態
	ConstantBuffer* m_cb;
	ConstantBuffer* m_specularCB;
	Model* m_pBoneModel;
};
