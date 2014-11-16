#include "SceneMeshObject.h"

#include "utility.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cstdint>
#include <iostream>

using namespace std;

CSceneMeshObject::CSceneMeshObject(void) : pShapes(nullptr), pMaterials(nullptr)
{
}

CSceneMeshObject::CSceneMeshObject(const string &filename)
{
	pShapes = new vector<tinyobj::shape_t>;
	pMaterials = new vector<tinyobj::material_t>;
	pTextureData = new std::vector<CTextureLoader>; 

	cout << "Loading " << filename << "..." << endl;
	string mtlFilenameBasePath(MeshFileBasePath);
	string objFilename  = string(MeshFileBasePath) + filename;
	string err = tinyobj::LoadObj(*pShapes, *pMaterials, objFilename.c_str(), mtlFilenameBasePath.c_str());

	if (!err.empty()) {
		cerr << err << endl;
		return;
	}

	numArrays = (int)pShapes->size();
	pVertexBuffer = new GLuint[numArrays];
	pVertexArray = new GLuint[numArrays];
	pIndexBuffer = new GLuint[numArrays];
	pTextures = new GLuint[numArrays];
	pTextureData->resize(numArrays);

	memset(pTextures, 0, sizeof(GLuint) * numArrays);

	// generate vertex buffer
	glGenBuffers(numArrays, pVertexBuffer);
	glGenBuffers(numArrays, pIndexBuffer);
	glGenTextures(numArrays, pTextures);
	// generate vertex array
	glGenVertexArrays(numArrays, pVertexArray);

	for (size_t i = 0; i < numArrays; i++) {
		tinyobj::shape_t &shape = pShapes->at(i);
		int numVertex = (int)shape.mesh.positions.size() / 3;
		VERTEX *pVertices = new VERTEX[numVertex];

		for (int v = 0; v < numVertex; v++) {
			pVertices[v].Position.x = shape.mesh.positions[v * 3];
			pVertices[v].Position.y = shape.mesh.positions[v * 3 + 1];
			pVertices[v].Position.z = shape.mesh.positions[v * 3 + 2];

			if (shape.mesh.normals.size() > 0) {
				pVertices[v].Normal.x = shape.mesh.normals[v * 3];
				pVertices[v].Normal.y = shape.mesh.normals[v * 3 + 1];
				pVertices[v].Normal.z = shape.mesh.normals[v * 3 + 2];
			}

			if (shape.mesh.texcoords.size() > 0) {
				pVertices[v].Texcoord.x = shape.mesh.texcoords[v * 2];
				pVertices[v].Texcoord.y = shape.mesh.texcoords[v * 2 + 1];
			}
		}

		unsigned int *pIndices = new unsigned int[shape.mesh.indices.size()];
		for (int idx = 0; idx < (int)shape.mesh.indices.size(); idx++) {
			pIndices[idx] = shape.mesh.indices[idx];
		}
/*
		int numTriangles = numVertex / 3;
		for (int tri = 0; tri < numTriangles; tri++) {
			VERTEX vTmp = pVertices[tri * 3 + 1];
			pVertices[tri * 3 + 1] = pVertices[tri * 3];
			pVertices[tri * 3] = vTmp;
		}*/

		glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(VERTEX), pVertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.mesh.indices.size() * sizeof(unsigned int), pIndices, GL_STATIC_DRAW);

		SAFE_DELETE_ARRAY(pVertices);
		SAFE_DELETE_ARRAY(pIndices);

		glBindVertexArray(pVertexArray[i]);
		glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer[i]);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid*)24);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer[i]);

		glBindVertexArray(0);

		SAFE_DELETE_ARRAY(pVertices);

		// generate texture
		int mid = pShapes->at(i).mesh.material_ids[0];
		if (mid >= 0) {
			tinyobj::material_t &mat = pMaterials->at(mid);
			if (mat.diffuse_texname.empty())
				continue;
			string absoluteImagePath = mtlFilenameBasePath + mat.diffuse_texname;
			CTextureLoader &pImg = pTextureData->at(i);
			if (pImg.LoadPNG(absoluteImagePath.c_str()) < 0) {
				cout << "loading texture failed" << endl;
				exit(-1);
			}

			if (pImg.GetWidth() % 4 || pImg.GetHeight() % 4) {
				cout << "The height or the width must be the multiple of 4" << endl;
				exit(-1);
			}

			uint8_t *pImageData = new uint8_t[pImg.GetWidth() * pImg.GetHeight() * 4];
			memset(pImageData, 0, pImg.GetWidth() * pImg.GetHeight() * 4);

			for (int h = 0; h < pImg.GetHeight(); h++) {
				for (int w = 0; w < pImg.GetWidth(); w++) {
					for (int c = 0; c < pImg.GetChannels(); c++) {
						pImageData[h * pImg.GetWidth() * 4 + w * 4 + c] = pImg.GetImage()[h * pImg.GetWidth() * pImg.GetChannels() + w * pImg.GetChannels() + c];
					}
				}
			}
			GLfloat fLargest;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
			printf("Max Anisotropy: %f\n", fLargest);

			glBindTexture(GL_TEXTURE_2D, pTextures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, pImg.GetWidth(), pImg.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pImageData);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest * 0.5f);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			printf("Texture error check %x\n", glGetError());
		}
	}
}

CSceneMeshObject::~CSceneMeshObject(void)
{
	SAFE_DELETE(pShapes);
	SAFE_DELETE(pMaterials);
}

void CSceneMeshObject::Render()
{
	for (int i = 0; i < numArrays; i++) {
		glBindVertexArray(pVertexArray[i]);
		glDrawElements(GL_TRIANGLES, (GLsizei)(pShapes->at(i).mesh.indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void CSceneMeshObject::RenderGroup(int groupId)
{
	if (groupId < (int)pShapes->size()) {
		glBindVertexArray(pVertexArray[groupId]);
		glDrawElements(GL_TRIANGLES, (GLsizei)(pShapes->at(groupId).mesh.indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void CSceneMeshObject::SetRigidTransformation(const glm::vec3 &translation, const glm::mat4 &rotation)
{
	this->translation = glm::translate(glm::mat4(1.0f), translation);
	this->rotation = rotation;
}

char CSceneMeshObject::MeshFileBasePath[MAX_PATH] = "";