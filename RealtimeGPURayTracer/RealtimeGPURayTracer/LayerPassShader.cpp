#include "LayerPassShader.h"


CLayerPassShader::CLayerPassShader(void)
{
	hMin = -1;
	hMax = -1;
	hHeight = -1;
	hWidth = -1;

}

CLayerPassShader::~CLayerPassShader(void)
{
}

int CLayerPassShader::Init()
{
	char filenames[5][MAX_PATH] = {
		"Layerpass.vs.glsl",
		"Layerpass.fs.glsl",
		"",
		"",
		""
	};

	int r = BuildShadersFromFiles(filenames);
	if (r < 0)
		return r;

	return 0;
}


int CLayerPassShader::AssignVariable()
{
	ASSGIN_V(Min);
	
	return 0;
}


void CLayerPassShader::SetMax(GLuint pMax, GLint maxSlot)
{
	int slotId = maxSlot - GL_TEXTURE0;

	glActiveTexture(maxSlot);
	glBindTexture(GL_TEXTURE_2D, pMax);

	glUniform1i(hMax, slotId);
}


void CLayerPassShader::SetWidth(int w)
{
	glUniform1f(hWidth, w);
}

void CLayerPassShader::SetHeight(int h)
{
	glUniform1f(hHeight, h);
}



void CLayerPassShader::SetMin(GLuint pMin, GLint minSlot)
{
	int slotId = minSlot - GL_TEXTURE0;

	glActiveTexture(minSlot);
	glBindTexture(GL_TEXTURE_2D, pMin);

	glUniform1i(hMin, slotId);
}


