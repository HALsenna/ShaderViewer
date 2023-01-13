struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

cbuffer Light : register(b0)
{
	float4 lightDir;
};

cbuffer Camera : register(b1)
{
	float4 camPos;
	float fogStart;
	float fogRange;
	float disRate;
	float dummy;
};

SamplerState samp : register(s0);		
Texture2D tex: register(t0);
Texture2D dissolveTex : register(t1);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̓\��t��
	color.rgb = tex.Sample(samp, pin.uv);
	
	// �����̉摜�̊Y���ӏ��̐F���E���Ă���
	float dissolve = dissolveTex.Sample(samp, pin.uv).r;
	// ���̐F��rate�Ɣ�ׂĈÂ������邢��
	// �V�F�[�_�[��if���͏d���������ɂȂ邩��ɗ͔�����
	// step�֐���1��0�����Ԃ��Ȃ�(x >= a) ? 1:0
	float show = step(disRate, dissolve);

	color.rgb *= show;
	return color;
}