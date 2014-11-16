#ifndef _LAYER_PASS_SHADER__H_
#define _LAYER_PASS_SHADER__H_

#include "ShaderProgram.h"
class CLayerPassShader :
	public CShaderProgram
{
public:
	CLayerPassShader(void);
	~CLayerPassShader(void);

	void SetMin(GLuint pMin, GLint pMinSlot);
	void SetMax(GLuint pMax, GLint pMaxSlot);
	void SetWidth(int w);
	void SetHeight(int h);

	int Init();
protected:
	virtual int AssignVariable();
private:
	GLint hMin;
	GLint hMax;
	GLint hHeight;
	GLint hWidth;
};

#endif