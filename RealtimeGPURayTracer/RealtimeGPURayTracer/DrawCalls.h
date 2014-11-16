#ifndef _DRAW_CALL_H_
#define _DRAW_CALL_H_

#include "ShaderWithGeometry.h"
#include "ShaderWithLight.h"
#include "ShaderWithMaterial.h"
#include "GBufferGenPass.h"
#include "GBufferGenPassShader.h"
#include "LightViewPass.h"
#include "LightViewPassShader.h"
#include "LayerPass.h"
#include "LayerPassShader.h"
#include "ShowTexturePass.h"
#include "SceneMeshObject.h"
#include "Camera.h"

#include <vector>
#include <glm/vec2.hpp>

void GenGBufferPass(CGBufferGenPass *pPass);
void GenRSMPass(CLightViewPass *pPass);
void ShowColorTexPass(CShowTexturePass *pPass, GLuint pColorTex);
GLuint RandomSampleTest(std::vector<glm::i32vec2> &samplePoints);
void GenLayerPass(CLayerPass *pPass,CLayerPass *pMin);
void GenLayerPass(CLayerPass *pPass, GLuint pColorTex);

#endif