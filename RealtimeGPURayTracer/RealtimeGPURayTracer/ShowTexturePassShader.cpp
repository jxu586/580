#include "ShowTexturePassShader.h"


CShowTexturePassShader::CShowTexturePassShader(void)
{
	hTex = -1;
}

CShowTexturePassShader::~CShowTexturePassShader(void)
{
}

int CShowTexturePassShader::Init()
{
	char filenames[5][MAX_PATH] = {
		"showtex.vs.glsl",
		"showtex.fs.glsl",
		"",
		"",
		""
	};

	int r = BuildShadersFromFiles(filenames);
	if (r < 0)
		return r;

	return 0;
}

int CShowTexturePassShader::AssignVariable()
{
	ASSGIN_V(Tex);
	return 0;
}

void CShowTexturePassShader::SetTexture(GLuint pTex, GLint texSlot)
{
	int slotId = texSlot - GL_TEXTURE0;

	glActiveTexture(texSlot);
	glBindTexture(GL_TEXTURE_2D, pTex);

	glUniform1i(hTex, slotId);
}
