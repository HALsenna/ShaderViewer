#pragma once
#include "Model.h"
#include "ConstantBuffer.h"
class CFood
{
public:
	enum KIND
	{
		KIND_PIZZA,
		KIND_MAX
	};
public:
	CFood();
	~CFood();
	Model* Init();
	void SetFood(KIND kind);
	void Draw(ConstantBuffer* cb, XMFLOAT4X4 mat[3]);

private:
public:
	Model* m_foodModels[KIND_MAX];
	KIND m_kind;
};
