#ifndef __SHADER_LIST_H__
#define __SHADER_LIST_H__

#include "Shader.h"

enum VertexShaderKind
{
	VS_OBJECT,	// pos3-noraml3-uv2
	VS_BUMP,	// pos3-normal3-uv2-tangent3
	VS_OUTLINE,	// pos3-normal3-uv2-tangent3
	VS_MUD,	// pos3-normal3-uv2-tangent3
	VS_SEA,
	VS_CONCRETE,
	VS_UI,
	VS_DEPTHSCREEN,
	VS_2D,
	VS_KIND_MAX
};
enum PixelShaderKind
{
	PS_OBJECT,
	PS_LAMBERT,
	PS_PHONG,
	PS_LIMLIGHT,
	PS_FOG,
	PS_DISSOLVE,
	PS_BUMPMAP,
	PS_TOON,
	PS_MUD,
	PS_SEA,
	PS_CONCRETE,
	PS_UI,
	PS_SKYDOME,
	PS_MOUNTAIN,
	PS_ROCK,
	PS_FIRE,
	PS_TENT,
	PS_MEAT,
	PS_SCREEN,
	PS_DEPTHSCREEN,
	PS_POINTLIGHT,
	PS_PLAYER,
	PS_2D,
	PS_FLOWER,
	PS_KIND_MAX
};

void InitShaderList();
void UninitShaderList();
VertexShader* GetVS(VertexShaderKind kind);
PixelShader* GetPS(PixelShaderKind kind);

#endif // __SHADER_LIST_H__