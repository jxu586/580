#include "SimplePassShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

CSimplePassShader::CSimplePassShader() 
{
	hColor = -1;
	hWVPMatrix = -1;
}

CSimplePassShader::~CSimplePassShader() 
{
}

int CSimplePassShader::Init()
{
	char filenames[5][MAX_PATH] = {
		"simplepass.vs.glsl",
		"simplepass.fs.glsl",
		"",
		"",
		""
	};

	int r = BuildShadersFromFiles(filenames);
	if (r < 0)
		return r;

	return 0;
}

int CSimplePassShader::AssignVariable()
{
	hColor = GetVariableHandler("Color", shaderProgramHandle);
	hWVPMatrix = GetVariableHandler("WVPMatrix", shaderProgramHandle);
	return 0;
}

void CSimplePassShader::SetWVPMat(const glm::mat4 &WVPMat)
{
	glUniformMatrix4fv(hWVPMatrix, 1, GL_FALSE, glm::value_ptr(WVPMat));
}

void CSimplePassShader::SetColor(const glm::vec4 &color)
{
	glUniform4fv(hColor, 1, glm::value_ptr(color));
}