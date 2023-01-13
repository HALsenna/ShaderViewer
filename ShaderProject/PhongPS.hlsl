struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

// b0��BindPS(0)
cbuffer Light : register(b0)
{
	float4 lightDir;
};

cbuffer Camera : register(b1)
{
	float4 camPos;
}

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 0.0f, 1.0f);

	// ����
	// ���������̃x�N�g�����m�Ȃ琳�̒l
	// ���������̃x�N�g�����m�Ȃ�0
	// ���Ε����̃x�N�g�����m�Ȃ畉�̒l

	// �I�u�W�F�N�g�̖@���ƃ��C�g�̃x�N�g��������ς��v�Z
	// ���̂܂܌v�Z����Ɩ��邭�Ȃ镔�������̒l�ɂȂ�̂ŁA
	// �u�����āv���C�g�̃x�N�g���𔽓]������
	float3 N = normalize(pin.normal);
	float3 L = normalize(-lightDir.xyz);

	// �g�U��(diffuse) ���̂̕\�ʂɒ��ړ�������A���̂̐F��\��
	float diffuse = dot(N, L);
	// ����(ambient) ����̕��̂��甽�˂��ĊԐړI�ɓ�������A���̂̉e�̕����̐F��\��
	float ambient = 0.5f;

	// ���˃x�N�g��
	// ����n�_�ɓ��˂������̔��˂���������������x�N�g��
	// �@����p���Ĉȉ��̎��ƂȂ�
	// R = L + 2 * dot(-L, N) * N
	// R�����˃x�N�g�� / L�����̕����x�N�g�� / N���@��
	// HLSL�ɂ�reflect(���C�g�̌���,�@��)�Ŕ��˃x�N�g�������߂�֐�������
	float3 R = reflect(-L, N);

	// �s�N�Z������J�����֌������x�N�g��
	float3 V = normalize(camPos.xyz - pin.wPos.xyz);

	// ���ʔ���(�X�y�L�����[:specular)
	// ���˃x�N�g���Ǝ����x�N�g���̈�v��Ŕ��˂̋�����\��
	float specular = max(0, dot(R, V));			// ���ς̌v�Z���ʂ��}�C�i�X�ɂȂ���

	// ���̂܂܂��ƑS�̂����邢�̂ŗݏ悷��Ƃ��������ɂȂ�
	specular = pow(specular, 30.0f);


	color.rbg = diffuse * 0.5f + ambient + specular;

	return color;
}