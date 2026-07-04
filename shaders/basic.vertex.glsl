#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec3 aTan;
layout(location = 4) in vec3 aBitan;
layout(location = 5) in uvec4 aJoints;// 整数
layout(location = 6) in vec4 aWeights;
layout(location = 7) in float aWeaponMask;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

const int MAX_BONES = 128;
uniform mat4 uBones[MAX_BONES];
uniform int uBoneCount;

out vec2 vUV;
out vec3 vNormal;
out float vWeaponMask;

void main()
{
	mat4 skin = mat4(0.0);
	float totalWeight = 0.0;
	for(int i = 0; i < 4; i++)
	{
		uint j = aJoints[i];
		float w = aWeights[i];
		if(int(j) < uBoneCount && w > 0.0)
		{
			skin += uBones[int(j)] * w;
			totalWeight += w;
		}
	}
	if(totalWeight > 0.0 && abs(totalWeight - 1.0) > 1e-3)
	{
		skin /= totalWeight;
	}
	if(totalWeight == 0.0)
	{
		skin = mat4(1.0);
	}

	vec4 worldPos = uModel * skin * vec4(aPos, 1.0);
	mat3 normalMat = transpose(inverse(mat3(uModel * skin)));
	vec3 worldNormal = normalize(normalMat * aNormal);

	gl_Position = uProjection * uView * worldPos;
	vNormal = worldNormal;
	vUV = aUV;
	vWeaponMask = aWeaponMask;
}