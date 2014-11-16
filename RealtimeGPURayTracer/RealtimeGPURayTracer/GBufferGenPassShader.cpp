#include "GBufferGenPassShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

CGBufferGenPassShader::CGBufferGenPassShader(void)
{
	hEyeWorldPos = -1;
}

CGBufferGenPassShader::~CGBufferGenPassShader(void)
{
}

int CGBufferGenPassShader::Init()
{
	char filenames[5][MAX_PATH] = {
		"buffergen.vs.glsl",
		"buffergen.fs.glsl",
		"",
		"",
		""
	};

	int r = BuildShadersFromFiles(filenames);
	if (r < 0)
		return r;

	return 0;
}

int CGBufferGenPassShader::AssignVariable()
{
	// vs variable
	ASSGIN_V(VPMatrix);
	ASSGIN_V(WMatrix);
	// fs variable
	ASSGIN_V(EyeWorldPos);
	// light
	ASSGIN_V(LightWorldPos);
	ASSGIN_V(LightWorldDir);
	ASSGIN_V(LightColor);
	ASSGIN_V(LightFOV);
	ASSGIN_V(LightAttenu);
	// mat
	ASSGIN_V(MatKa);
	ASSGIN_V(MatKd);
	ASSGIN_V(MatKs);
	ASSGIN_V(SpecExp);
	ASSGIN_V(TexMap);
	return 0;
}

void CGBufferGenPassShader::SetEyePos(const glm::vec3 &EyePos)
{
	glUniform3fv(hEyeWorldPos, 1, glm::value_ptr(EyePos));
}
