#ifndef _SCENE_MESH_OBJECT_H_
#define _SCENE_MESH_OBJECT_H_

#include "SceneObject.h"
#include "TextureLoader.h"
#include "utility.h"

#include <tiny_obj_loader.h>
#include <vector>
#include <string>

class CSceneMeshObject :
	public CSceneObject
{
protected:
	std::vector<tinyobj::shape_t> *pShapes;
	std::vector<tinyobj::material_t> *pMaterials;
	std::vector<CTextureLoader> *pTextureData;
	GLuint *pTextures;
	glm::mat4 translation;
	glm::mat4 rotation;

public:
	CSceneMeshObject(void);
	CSceneMeshObject(const std::string &filename);
	virtual ~CSceneMeshObject(void);

	virtual void Render();
	void RenderGroup(int groupId);

	void SetRigidTransformation(const glm::vec3 &translation, const glm::mat4 &rotation);
	glm::mat4 &GetTranslation() { return translation; }
	glm::mat4 &GetRotation() { return rotation; }

	std::vector<tinyobj::material_t> &GetMaterials() { return *pMaterials; }
	std::vector<tinyobj::shape_t> &GetShapes() { return *pShapes; }
	int GetNumGroups() { return (int)pShapes->size(); }
	GLuint GetTexture(int groupId) { return pTextures[groupId]; }
public:
	static char MeshFileBasePath[MAX_PATH];
};

#endif

