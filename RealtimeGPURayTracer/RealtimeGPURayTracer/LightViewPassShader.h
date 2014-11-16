#ifndef _LIGHT_VIEW_PASS_SHADER_
#define _LIGHT_VIEW_PASS_SHADER_

#include "ShaderProgram.h"
#include "ShaderWithLight.h"
#include "ShaderWithMaterial.h"
#include "ShaderWithGeometry.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "SceneObject.h"

class CLightViewPassShader :
	public CShaderProgram,
	public CShaderWithLight,
	public CShaderWithMaterial,
	public CShaderWithGeometry
{
public:
	CLightViewPassShader(void);
	~CLightViewPassShader(void);

	int Init();
	void SetReflectiveCoeff(const glm::vec4 &coeff);
	virtual void SetMaterial(const tinyobj::material_t &material);
protected:
	virtual int AssignVariable();
private:
	GLint hReflectiveCoeff;
};

#endif

