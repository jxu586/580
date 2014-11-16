#ifndef _GBUFFER_GEN_PASS_SHADER_H_
#define _GBUFFER_GEN_PASS_SHADER_H_

#include "ShaderProgram.h"
#include "ShaderWithLight.h"
#include "ShaderWithMaterial.h"
#include "ShaderWithGeometry.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "SceneObject.h"
#include <tiny_obj_loader.h>

class CGBufferGenPassShader :
	public CShaderProgram, 
	public CShaderWithLight, 
	public CShaderWithMaterial, 
	public CShaderWithGeometry
{
public:
	CGBufferGenPassShader(void);
	~CGBufferGenPassShader(void);

	int Init();
	void SetEyePos(const glm::vec3 &EyePos);	
protected:
	virtual int AssignVariable();
private:
	GLint hEyeWorldPos;
};

#endif


