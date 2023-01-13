struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

cbuffer Light : register(b0)
{
	float3 lightDir;
	float dummy;
	float4 lightColor;
	float smooth;
	float3 dummy2;
};

cbuffer Camera : register(b1)
{
	float4 cameraPos;
}

cbuffer pointLight : register(b4)
{
	float3 ptPos;
	float ptDummy;
	float4 ptColor;
	float ptRange;
	float3 ptDummy2;
}

cbuffer time : register(b5)
{
	float time;
	float3 timeDummy;
}

cbuffer limlight : register(b6)
{
	float islimlight;
}

Texture2D tex : register(t0);
SamplerState samp : register(s0);

// �֐��錾
float3 calcPointLightDir(float4 world, float3 pointPos);		// ���˂���|�C���g���C�g�̌��̌������v�Z����
float calcAffect(float4 world, float3 pointPos, float ptRange);
float3 calcSpecular(float3 lightDir, float3 normal, float3 world);
float3 calclimlight(float3 normal, float3 view, float3 lightDir);

float4 main(PS_IN pin) : SV_TARGET
{
	//float4 color = float4(0.5f, 0.5f, 0.5f, 1.0f);		// ���C�g�̃J���[
	float4 color = tex.Sample(samp, pin.uv);
	color += float4(lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	float3 N = normalize(pin.normal);		// �@���̕���
	float3 L = normalize(lightDir);		// ���C�g�̕���
	float3 V = normalize(cameraPos.xyz - pin.wPos.xyz);

	// �g�U���ˌ����v�Z����
	float diffuse = dot(N, -L);

	// ���ʔ��ˌ����v�Z����
	float3 specular = calcSpecular(L, N, pin.wPos);

	// �|�C���g���C�g�̕������v�Z����
	float3 pointLightDir = normalize(calcPointLightDir(pin.wPos, ptPos));

	// �|�C���g���C�g�����������
	float lightPower = ptRange * sin(time / 60.0f);
	if (lightPower < 0.0f)
	{
		lightPower = -lightPower;
	}

	// �|�C���g���C�g�̉e�������v�Z����
	float affect = calcAffect(pin.wPos, ptPos, lightPower);

	// �������C�g�̌v�Z������
	float3 limlight = 0.0f;
	if (islimlight != 0.0f)
	{
		limlight = calclimlight(N, V, L);
	}

	//return float4(limlight,  1.0f);

	// �������߂�
	float3 light = (diffuse + specular) + (affect * ptColor);

	light.x += 0.8f;
	light.y += 0.8f;
	light.z += 0.8f;
	
	color.xyz = color.xyz * light + limlight;

	return color;
}

// ���ʔ��ˌ����v�Z����
float3 calcSpecular(float3 lightDir, float3 normal, float3 world)
{
	// ���˃x�N�g�������߂�
	float3 refVec = reflect(lightDir, normal);
	// ���_�x�N�g�������߂�
	float3 eyeVec = normalize(cameraPos.xyz - world.xyz);
	// ���ʔ��˂̋��������߂�
	float specVal = max(0, dot(refVec, eyeVec));
	// ���ʔ��˂��i��
	specVal = pow(specVal, 10.0f);
	// ���ʔ��ˌ������߂�
	float3 specular = specVal;
	
	return specular;
}

// �|�C���g���C�g�̌��̌������v�Z����
float3 calcPointLightDir(float4 world, float3 pos)
{
	float3 _lightDir = world - pos;
	_lightDir = normalize(_lightDir);

	return _lightDir;
}

// �����ɂ��e�������v�Z����
float calcAffect(float4 world, float3 pos, float range)
{
	// �������v�Z����
	float distance = length(world - pos);

	// �e�������v�Z����
	float affect = 1.0f - 1.0f / range * distance;

	if (affect < 0.0f)
	{
		affect = 0.0f;
	}

	affect = pow(affect, 3.0f);

	return affect;
}

// �������C�g�̌v�Z
float3 calclimlight(float3 normal, float3 view, float3 lightDir)
{
	// ���̌v�Z
	float3 edgeColor = float3(0.0f, 1.0f, 0.0f);
	float edge = min(1.0f - dot(normal, view), 1.0f);
	edge = pow(edge, 3.0f);

	// �������C�g�̋��x�̌v�Z
	float lim = max(0, -dot(lightDir, view));		// ���̒l���o�Ȃ��悤�Ɋۂߍ���
	
	return edgeColor * edge * lim;
}