#include "ShaderProgram.h"
#include "GLUTHeader.h"
#include "error.h"
#include "Camera.h"
#include "SimplePass.h"
#include "GBufferGenPass.h"
#include "LightViewPass.h"
#include "ShowTexturePass.h"
#include "RoomObject.h"
#include "SceneMeshObject.h"

#include "ConfigFile.h"
#include "DrawCalls.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

#include <glm/gtx/constants.hpp>
#include <glm/vec2.hpp>

using namespace std;

bool enableMultiSampling = false;
char windowTitle[512] = "Realtime GPU Ray Tracer";

glm::i32vec2 mousePos;
int mouseButtons[3];

CCamera *pCamera;
unordered_map<string, CRenderPass*> passes;
unordered_map<string, CSceneObject*> objects;
SPOT_LIGHT *pGlobalLight;
GLuint pSamplePointTex;

void displayFunc()
{
	CLightViewPass *pLVPass = dynamic_cast<CLightViewPass*>(passes["lightview_pass"]);
	GenRSMPass(pLVPass);

	CGBufferGenPass *pGPass = dynamic_cast<CGBufferGenPass*>(passes["gbuffer_gen"]);
	GenGBufferPass(pGPass);

	CLayerPass *pLPass =  dynamic_cast<CLayerPass*>(passes["Layer_pass0"]);
	GenLayerPass(pLPass,pGPass->GetNormalBuffer());

	CLayerPass *pLPass1 =  dynamic_cast<CLayerPass*>(passes["Layer_pass1"]);
	GenLayerPass(pLPass1,pLPass);

	CLayerPass *pLPass2 =  dynamic_cast<CLayerPass*>(passes["Layer_pass2"]);
	GenLayerPass(pLPass2,pLPass1);

	CLayerPass *pLPass3 =  dynamic_cast<CLayerPass*>(passes["Layer_pass3"]);
	GenLayerPass(pLPass3,pLPass2);

	CShowTexturePass *pSTPass = dynamic_cast<CShowTexturePass*>(passes["showtex_pass"]);
//  ShowColorTexPass(pSTPass, pGPass->GetZBuffer());
//  ShowColorTexPass(pSTPass, pLPass->GetMaxBuffer());
	ShowColorTexPass(pSTPass, pGPass->GetColorBuffer());
//  ShowColorTexPass(pSTPass, pSamplePointTex);
	
	glutSwapBuffers();
}

void idleFunc()
{
	pCamera->Step();

	glutPostRedisplay();
}

void reshapeFunc(int w, int h)
{
	if(h == 0)
		h = 1;

	glViewport(0, 0, w, h);
	CShaderProgram::ScreenWidth = w;
	CShaderProgram::ScreenHeight = h;

	glutPostRedisplay();
}

void mouseMotionDragFunc(int x, int y)
{
	mousePos.x = x;
	mousePos.y = y;

	if (mouseButtons[2])
		pCamera->MouseMove(mousePos);
}

void mouseMotionFunc(int x, int y)
{
	mousePos.x = x;
	mousePos.y = y;

	pCamera->SetMouse(mousePos);
}

void mouseButtonFunc(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		mouseButtons[0] = (state == GLUT_DOWN);
		break;
	case GLUT_MIDDLE_BUTTON:
		mouseButtons[1] = (state==GLUT_DOWN);
		break;
	case GLUT_RIGHT_BUTTON:
		mouseButtons[2] = (state == GLUT_DOWN);
		break;
	case 3:
		if (state == GLUT_DOWN) {
			pCamera->ZoomIn();
		}
		break;
	case 4:
		if (state == GLUT_DOWN) {
			pCamera->ZoomOut();
		}
		break;
	}

	mousePos.x = x;
	mousePos.y = y;
}

void keyboardFunc (unsigned char key, int x, int y)
{
	float scalar = 1.0f;
	switch (key)
	{
	case 'z':
		pCamera->ZoomIn();
		break;
	case 'x':
		pCamera->ZoomOut();
		break;
	case 'w':
		pCamera->Forward(scalar);
		break;
	case 's':
		pCamera->Backward(scalar);
		break;
	case 'a':
		pCamera->Left(scalar);
		break;
	case 'd':
		pCamera->Right(scalar);
		break;
	}
}

void InitGL(int argc, char *argv[])
{
	cout << "Initializing GLUT" << endl;
	glutInit(&argc, argv);

	cout << "Initializing OpenGL" << endl;

#ifdef __APPLE__
	if (enableMultiSampling)
		glutInitDisplayMode (GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	else glutInitDisplayMode (GLUT_3_2_CORE_PROFILE |GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);	
#else
	glutInitContextProfile(GLUT_CORE_PROFILE);
	if (enableMultiSampling)
		glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	else glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);	
#endif

	CShaderProgram::ScreenWidth = 1280;
	CShaderProgram::ScreenHeight = 720;

	glutInitWindowSize(CShaderProgram::ScreenWidth, CShaderProgram::ScreenHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(windowTitle);

	cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
	cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	glutDisplayFunc(displayFunc);
	glutIdleFunc(idleFunc);
	glutMotionFunc(mouseMotionDragFunc);
	glutReshapeFunc(reshapeFunc);
	glutPassiveMotionFunc(mouseMotionFunc);
	glutMouseFunc(mouseButtonFunc);
	glutKeyboardFunc(keyboardFunc);

#ifndef __APPLE__
	GLint Result = glewInit();
	if (Result != GLEW_OK) 
	{
		cout << "error: " << glewGetErrorString(Result) << endl;
		exit(EXIT_FAILURE);
	}
#endif
}

void initSceneObjects()
{
	CSceneMeshObject *pRoom = new CSceneMeshObject("room.obj");
	CSceneMeshObject *pBunny = new CSceneMeshObject("bunnyScale.obj");
	CSceneMeshObject *pBall = new CSceneMeshObject("ball.obj");
	CSceneMeshObject *pBox = new CSceneMeshObject("box.obj");
	CSceneMeshObject *pVase = new CSceneMeshObject("vase.obj");

	pBunny->SetRigidTransformation(glm::vec3(0.0f, -8.0f, 0.0f), glm::mat4(1.0f));
	pBall->SetRigidTransformation(glm::vec3(4.0f, -6.0f, 2.0f), glm::mat4(1.0f));
	pBox->SetRigidTransformation(glm::vec3(-4.0f, -6.0f, -2.0f), glm::mat4(1.0f));
	pVase->SetRigidTransformation(glm::vec3(-2.0f, -6.0f, -5.0f), glm::mat4(1.0f));
	pRoom->SetRigidTransformation(glm::vec3(0.0f, 0.0f, 0.0f), glm::mat4(1.0f));

	objects.insert(make_pair("bunny", (CSceneObject*)pBunny));
	objects.insert(make_pair("ball", (CSceneObject*)pBall));
	objects.insert(make_pair("room", (CSceneObject*)pRoom));
	objects.insert(make_pair("box", (CSceneObject*)pBox));
	objects.insert(make_pair("vase", (CSceneObject*)pVase));
}

int initRenderPass()
{
	int r = 0;

	CRenderPass *pPasses = new CSimplePass;
	IF_FAILED(r, pPasses->Init(), init_rnd_err);
	passes.insert(make_pair("simple_pass", pPasses));

	pPasses = new CGBufferGenPass;
	IF_FAILED(r, pPasses->Init(), init_rnd_err);
	passes.insert(make_pair("gbuffer_gen", pPasses));

	pPasses = new CLightViewPass(512);
	IF_FAILED(r, pPasses->Init(), init_rnd_err);
	passes.insert(make_pair("lightview_pass", pPasses));

	pPasses = new CShowTexturePass;
	IF_FAILED(r, pPasses->Init(), init_rnd_err);
	passes.insert(make_pair("showtex_pass", pPasses));

	/*	int width = CShaderProgram::ScreenWidth/2;
	int height = CShaderProgram::ScreenHeight/2;
	pPasses = new CLayerPass(width,height,1);
	IF_FAILED(r, pPasses->Init(), init_rnd_err);
	passes.insert(make_pair("Layer_pass0", pPasses));

	Layer = 1;
	while(width%2==0 && height%2==0){
		width/=2;
		height/=2;
	    pPasses = new CLayerPass(width,height,0);
	    IF_FAILED(r, pPasses->Init(), init_rnd_err);
	    passes.insert(make_pair("Layer_pass"+Layer, pPasses));
		Layer++;
	}
	*/
	pPasses = new CLayerPass(640,360,1);
	IF_FAILED(r, pPasses->Init(), init_rnd_err);
	passes.insert(make_pair("Layer_pass0", pPasses));


	pPasses = new CLayerPass(320,180,0);
	IF_FAILED(r, pPasses->Init(), init_rnd_err);
	passes.insert(make_pair("Layer_pass1", pPasses));
	
	pPasses = new CLayerPass(160,90,0);
	IF_FAILED(r, pPasses->Init(), init_rnd_err);
	passes.insert(make_pair("Layer_pass2", pPasses));

	pPasses = new CLayerPass(80,45,0);
	IF_FAILED(r, pPasses->Init(), init_rnd_err);
	passes.insert(make_pair("Layer_pass3", pPasses));

	return 0;
init_rnd_err:
	return r;
}

int initScene()
{
	int r = 0;

	pCamera = new CCamera(5.0f, glm::pi<float>() * 1.25f, glm::pi<float>() * -0.25f, 60.0f, glm::vec3(7.0f, 7.0f, 7.0f));
	pGlobalLight = new SPOT_LIGHT;
	pGlobalLight->fov = 18.0f / 180.0f * glm::pi<float>();
	pGlobalLight->lightDir = glm::normalize(glm::vec3(0.6f, 1.0f, 0.6f));
	pGlobalLight->lightPosition = glm::vec3(7.5f, 7.5f, 7.5f);
	pGlobalLight->lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pGlobalLight->lightAttenu = 0.0015f;

	IF_FAILED(r, initRenderPass(), init_err);
	initSceneObjects();
/*
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
*/
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	return 0;

init_err:
	return r;
}

int parseConfigFile(const string & filename)
{
	CConfigFile cf;
	int r = 0;
	IF_FAILED(r, cf.ParseConfigFile(filename), pcf_err);

	//"C:\\tmp\\realtime\\580prj\\"
	strcpy(CSceneMeshObject::MeshFileBasePath, cf["meshBasePath"].c_str());
	// "D:\\Document\\VS Prj\\RealtimeGPURayTracer\\RealtimeGPURayTracer\\shader\\"
	strcpy(CShaderProgram::ShaderBasePath, cf["shaderBasePath"].c_str());

	return 0;
pcf_err:
	return r;
}

vector<glm::i32vec2> GenSamplePoints()
{
	vector<glm::i32vec2> samplePoints;
	for (int i = 0; i < 256; i++) {
		samplePoints.push_back(glm::i32vec2(rand() % 1280, rand() % 720));
	}

	return std::move(samplePoints);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		cout << "config file needed" << endl;
		cout << "there is a sample config file in models folder" << endl;
		cout << "file name is config.txt" << endl;
		cout << "please check" << endl;
	}

	if (parseConfigFile(argv[1])) {
		exit(EXIT_FAILURE);
	}

	InitGL(argc, argv);

	if (initScene() < 0) {
		exit (EXIT_FAILURE);
	}

	vector<glm::i32vec2> samplePoints(GenSamplePoints());
	pSamplePointTex = RandomSampleTest(samplePoints);

	glutMainLoop();
}