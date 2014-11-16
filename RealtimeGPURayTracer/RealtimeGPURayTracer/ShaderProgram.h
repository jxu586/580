#ifndef _SHADER_PROGRAM_H_
#define _SHADER_PROGRAM_H_

#include "OpenGLHeader.h"
#include "utility.h"

class CShaderProgram
{
public:
	CShaderProgram();
	virtual ~CShaderProgram();

	int BuildShadersFromFiles(const char (*filenames)[MAX_PATH]);
	int BuildShadersFromStrings(const char **pCodes);
	void BindShader();

public:
	static int ScreenWidth;
	static int ScreenHeight;
	static char ShaderBasePath[MAX_PATH];

protected:
	GLuint shaderProgramHandle;

protected:
	int ReadShader(const char *filename, char *code, int len);
	int CompileShader(const char *pCode, GLenum ShaderType, GLuint &hShader);
	GLint GetVariableHandler(const char *pName, GLuint hsp);

	virtual int AssignVariable() = 0;
};

#define ASSGIN_V(ShaderVariableName) \
	do { \
		h##ShaderVariableName = GetVariableHandler(#ShaderVariableName, shaderProgramHandle);\
	} while(0)

#endif
