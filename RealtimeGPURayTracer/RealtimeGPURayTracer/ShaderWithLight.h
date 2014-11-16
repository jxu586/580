#ifndef _SHADER_WITH_LIGHT_H_
#define _SHADER_WITH_LIGHT_H_

#include "OpenGLHeader.h"
#include "SceneObject.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

class CShaderWithLight
{
protected:
	GLint hLightWorldPos;
	GLint hLightWorldDir;
	GLint hLightColor;
	GLint hLightFOV;
	GLint hLightAttenu;
public:
	CShaderWithLight() {
		hLightWorldPos = -1;
		hLightWorldDir = -1;
		hLightFOV = -1;
		hLightColor = -1;
		hLightAttenu = -1;
	}

	void SetSpotLight(const SPOT_LIGHT &light) {
		glUniform4fv(hLightColor, 1, glm::value_ptr(light.lightColor));

		glUniform3fv(hLightWorldPos, 1, glm::value_ptr(light.lightPosition));
		glUniform3fv(hLightWorldDir, 1, glm::value_ptr(light.lightDir));

		glUniform1f(hLightFOV, cosf(light.fov));
		glUniform1f(hLightAttenu, light.lightAttenu);
	}
};

#endif