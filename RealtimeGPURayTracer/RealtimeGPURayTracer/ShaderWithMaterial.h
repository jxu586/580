#ifndef _SHADER_WITH_MATERIAL_H_
#define _SHADER_WITH_MATERIAL_H_

#include "OpenGLHeader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tiny_obj_loader.h>

class CShaderWithMaterial
{
protected:
	GLint hMatKa;
	GLint hMatKd;
	GLint hMatKs;
	GLint hSpecExp;

	GLint hTexMap;
public:
	CShaderWithMaterial() {
		hMatKs = -1;
		hMatKd = -1;
		hMatKa = -1;
		hSpecExp = -1;

		hTexMap = -1;
	}

	virtual void SetMaterial(const tinyobj::material_t &material) {
		glm::vec4 ambientV = glm::vec4(material.ambient[0], material.ambient[1], material.ambient[2], 1.0f);
		glUniform4fv(hMatKa, 1, glm::value_ptr(ambientV));

		glm::vec4 diffuseV = glm::vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1.0f);
		glUniform4fv(hMatKd, 1, glm::value_ptr(diffuseV));

		glm::vec4 specularV = glm::vec4(material.specular[0], material.specular[1], material.specular[2], 1.0f);
		glUniform4fv(hMatKs, 1, glm::value_ptr(specularV));

		glUniform1f(hSpecExp, material.shininess);
	}

	void SetTexture(GLuint diffuseTextureMap, GLint textureSlot) {
		int slotId = textureSlot - GL_TEXTURE0;

		glUniform1i(hTexMap, slotId);
		glActiveTexture(textureSlot);
		glBindTexture(GL_TEXTURE_2D, diffuseTextureMap);
	}
};

#endif