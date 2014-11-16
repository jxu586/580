#ifndef _SHADER_WITH_GEOMETRY_H_
#define _SHADER_WITH_GEOMETRY_H_

#include "OpenGLHeader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tiny_obj_loader.h>

class CShaderWithGeometry
{
protected:
	GLint hVPMatrix;
	GLint hWMatrix;
public:
	CShaderWithGeometry() {
		hVPMatrix = -1;
		hWMatrix = -1;
	}

	void SetWMat(const glm::mat4 &WMat) {
		glUniformMatrix4fv(hWMatrix, 1, GL_FALSE, glm::value_ptr(WMat));
	}

	void SetVPMat(const glm::mat4 &VPMat) {
		glUniformMatrix4fv(hVPMatrix, 1, GL_FALSE, glm::value_ptr(VPMat));
	}
};

#endif