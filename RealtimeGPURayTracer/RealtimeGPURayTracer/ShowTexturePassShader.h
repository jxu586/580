#ifndef _SHOW_TEXTURE_PASS_SHADER__H_
#define _SHOW_TEXTURE_PASS_SHADER__H_

#include "ShaderProgram.h"
class CShowTexturePassShader :
	public CShaderProgram
{
public:
	CShowTexturePassShader(void);
	~CShowTexturePassShader(void);

	void SetTexture(GLuint pTex, GLint texSlot);
	int Init();
protected:
	virtual int AssignVariable();
private:
	GLint hTex;
};

#endif


