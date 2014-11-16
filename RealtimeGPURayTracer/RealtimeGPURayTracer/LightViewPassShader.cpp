#include "LightViewPassShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

CLightViewPassShader::CLightViewPassShader(void)
{
	hReflectiveCoeff = -1;
}

CLightViewPassShader::~CLightViewPassShader(void)
{
}

int CLightViewPassShader::Init()
{
	char filenames[5][MAX_PATH] = {
		"buffergen.vs.glsl",
		"lightviewpass.fs.glsl",
		"",
		"",
		""
	};

	int r = BuildShadersFromFiles(filenames);
	if (r < 0)
		return r;

	return 0;
}

int CLightViewPassShader::AssignVariable()
{
	// vs variable
	ASSGIN_V(VPMatrix);
	ASSGIN_V(WMatrix);

	// fs variable
	// light
	ASSGIN_V(LightWorldPos);
	ASSGIN_V(LightWorldDir);
	ASSGIN_V(LightColor);
	ASSGIN_V(LightFOV);
	ASSGIN_V(LightAttenu);

	ASSGIN_V(ReflectiveCoeff);
	ASSGIN_V(TexMap);
	ASSGIN_V(MatKd);

	return 0;
}

void CLightViewPassShader::SetReflectiveCoeff(const glm::vec4 & coeff)
{
	glUniform4fv(hReflectiveCoeff, 1, glm::value_ptr(coeff));
}

void CLightViewPassShader::SetMaterial(const tinyobj::material_t &material)
{
	glm::vec4 diffuseV = glm::vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1.0f);
	glUniform4fv(hMatKd, 1, glm::value_ptr(diffuseV));
}