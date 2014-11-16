#include "DrawCalls.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>

using namespace std;

extern unordered_map<string, CSceneObject*> objects;
extern SPOT_LIGHT *pGlobalLight;
extern CCamera *pCamera;

typedef unordered_map<string, CSceneObject*>::iterator objectPtr;

// draw mesh object
void DrawMeshObject(CShaderWithGeometry *pShaderGeoPart,
	CShaderWithMaterial *pShaderMaterialPart, CSceneMeshObject *pMeshObject)
{
	for (int i = 0; i < pMeshObject->GetNumGroups(); i++) {
		int matid = pMeshObject->GetShapes()[i].mesh.material_ids[0];
		pShaderMaterialPart->SetMaterial(pMeshObject->GetMaterials()[matid]);
		pShaderMaterialPart->SetTexture(pMeshObject->GetTexture(i), GL_TEXTURE0 + i);

		glm::mat4 T, R, P;
		R = pMeshObject->GetRotation();
		P = pMeshObject->GetTranslation();
		T = P * R;

		pShaderGeoPart->SetWMat(T);
		pMeshObject->RenderGroup(i);
	}
}

void GenGBufferPass(CGBufferGenPass *pPass)
{
	pPass->RenderBegin(pCamera);

	CGBufferGenPassShader *pGBGPShader = pPass->GetShaderHandle<CGBufferGenPassShader>();
	pGBGPShader->SetSpotLight(*pGlobalLight);

	for (objectPtr itt = objects.begin(); itt != objects.end(); itt++) {
		CSceneMeshObject *pMeshObject = dynamic_cast<CSceneMeshObject*>(itt->second);
		if (pMeshObject) {
			DrawMeshObject((CShaderWithGeometry*)pGBGPShader, 
				(CShaderWithMaterial*)pGBGPShader, pMeshObject);
		}
	}

	pPass->RenderEnd();
}

void GenRSMPass(CLightViewPass *pPass)
{
	pPass->RenderBegin(pGlobalLight);

	CLightViewPassShader *pLVShader = pPass->GetShaderHandle<CLightViewPassShader>();
	pLVShader->SetSpotLight(*pGlobalLight);

	for (objectPtr itt = objects.begin(); itt != objects.end(); itt++) {
		CSceneMeshObject *pMeshObject = dynamic_cast<CSceneMeshObject*>(itt->second);
		if (pMeshObject) {
			DrawMeshObject((CShaderWithGeometry*)pLVShader, 
				(CShaderWithMaterial*)pLVShader, pMeshObject);
		}
	}

	pPass->RenderEnd();
}

GLuint RandomSampleTest(vector<glm::i32vec2> &samplePoints)
{
	static uint32_t texData[1280 * 720];
	int w = 1280;
	int h = 720;
	int L = 3;

	memset(texData, 0, sizeof(texData));
	for (int i = 0; i <(int)samplePoints.size(); i++) {
		for (int len = 0; len < L; len++) {
			int x[4] = {
				samplePoints[i].x + len,
				samplePoints[i].x - len,
				samplePoints[i].x + len,
				samplePoints[i].x - len
			};

			int y[4] = {
				samplePoints[i].y + len,
				samplePoints[i].y + len,
				samplePoints[i].y - len,
				samplePoints[i].y - len
			};

			for (int j = 0; j < 4; j++) {
				if (x[j] >= 0 && x[j] < w && y[j] >=0 && y[j] < h) {
					texData[y[j] * w + x[j]] = 0xffffffff;
				}
			}
		}
	}

	GLuint pTex;
	glGenTextures(1, &pTex);
	glBindTexture(GL_TEXTURE_2D, pTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return pTex;
}

void ShowColorTexPass(CShowTexturePass *pPass, GLuint pColorTex)
{
	pPass->RenderBegin((void*)pColorTex);
	pPass->RenderEnd();
}

void GenLayerPass(CLayerPass *pPass,CLayerPass *pass){
	pPass->RenderBegin(pass);
	pPass->RenderEnd();
} 

void GenLayerPass(CLayerPass *pPass, GLuint pColorTex){
	pPass->RenderBegin((void*)pColorTex);
	pPass->RenderEnd();
} 