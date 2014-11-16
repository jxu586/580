#ifndef _SIMPLEPASS_H_
#define _SIMPLEPASS_H_

#include "ShaderProgram.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class CSimplePassShader:
	public CShaderProgram 
{
public:
	CSimplePassShader();
	virtual ~CSimplePassShader();

	int Init();
	void SetColor(const glm::vec4 &color);
	void SetWVPMat(const glm::mat4 &WVPMat);
protected:
	virtual int AssignVariable();
private:
	GLint hColor;
	GLint hWVPMatrix;
};

#endif /* SIMPLEPASS_H_ */
