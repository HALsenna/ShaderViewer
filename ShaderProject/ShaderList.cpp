#include "ShaderList.h"
#include "WinDialog.h"

const char* VertexShaderFile[] =
{
	"Assets/Shader/ObjectVS.cso",
	"Assets/Shader/BumpVS.cso",
	"Assets/Shader/OutlineVS.cso",
	"Assets/Shader/MudVS.cso",
	"Assets/Shader/SeaVS.cso",
	"Assets/Shader/ConcreteVS.cso",
	"Assets/Shader/UIVS.cso",
	"Assets/Shader/DepthScreenVS.cso",
	"Assets/Shader/2DVS.cso",
};
const char* PixelShaderFile[] =
{
	"Assets/Shader/ObjectPS.cso",
	"Assets/Shader/LambertPS.cso",
	"Assets/Shader/PhongPS.cso",
	"Assets/Shader/LimlightPS.cso",
	"Assets/Shader/FogPS.cso",
	"Assets/Shader/Dissolve.cso",
	"Assets/Shader/BumpmapPS.cso",
	"Assets/Shader/ToonPS.cso",
	"Assets/Shader/MudPS.cso",
	"Assets/Shader/SeaPS.cso",
	"Assets/Shader/ConcretePS.cso",
	"Assets/Shader/UIPS.cso",
	"Assets/Shader/SkyDomePS.cso",
	"Assets/Shader/MountainPS.cso",
	"Assets/Shader/RockPS.cso",
	"Assets/Shader/FirePS.cso",
	"Assets/Shader/TentPS.cso",
	"Assets/Shader/MeatPS.cso",
	"Assets/Shader/ScreenPS.cso",
	"Assets/Shader/DepthScreenPS.cso",
	"Assets/Shader/PointLightPS.cso",
	"Assets/Shader/PlayerPS.cso",
	"Assets/Shader/2DPS.cso",
	"Assets/Shader/flowerPS.cso",
};

static_assert(VS_KIND_MAX == _countof(VertexShaderFile), "VS - ShaderList[.hの定義と.cppのファイル数が一致しません");
static_assert(PS_KIND_MAX == _countof(PixelShaderFile), "PS - ShaderList[.hの定義と.cppのファイル数が一致しません");
VertexShader* g_pVertexShaderList[VS_KIND_MAX];
PixelShader* g_pPixelShaderList[PS_KIND_MAX];
void InitShaderList()
{
	for (int i = 0; i < VS_KIND_MAX; ++i)
	{
		g_pVertexShaderList[i] = new VertexShader();
		if (FAILED(g_pVertexShaderList[i]->Load(VertexShaderFile[i])))
		{
			DialogError(STACK_DEFAULT, "%sが見つかりません", VertexShaderFile[i]);
		}
	}
	for (int i = 0; i < PS_KIND_MAX; ++i)
	{
		g_pPixelShaderList[i] = new PixelShader();
		if (FAILED(g_pPixelShaderList[i]->Load(PixelShaderFile[i])))
		{
			DialogError(STACK_DEFAULT, "%sが見つかりません", PixelShaderFile[i]);
		}
	}
}
void UninitShaderList()
{
	for (int i = 0; i < VS_KIND_MAX; ++i)
		delete g_pVertexShaderList[i];
	for (int i = 0; i < PS_KIND_MAX; ++i)
		delete g_pPixelShaderList[i];
}
VertexShader* GetVS(VertexShaderKind kind)
{
	return g_pVertexShaderList[kind];
}
PixelShader* GetPS(PixelShaderKind kind)
{
	return g_pPixelShaderList[kind];
}