#pragma once
#include "Model.h"

class CShop
{
public:
	CShop(Model* tableModel);
	~CShop();
	void Init();
	void Update();
private:
	Model* m_tableModel;
};
