#include "ShaderProgram.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

CCamera::CCamera(float Radius_, float Phi_, float Theta_, float Fovy_, const glm::vec3 &eyeCenter)
{
	radius = Radius_;
	phi = Phi_;
	theta = Theta_;
	fovy = Fovy_;
	zoom = 0.0f;
	eyePos = eyeCenter;
}

CCamera::~CCamera(void)
{
}

void CCamera::SetMouse(const glm::i32vec2 &mousePos_)
{
	mousePos.x = (float)mousePos_.x;
	mousePos.y = (float)mousePos_.y;
}

void CCamera::MouseMove(const glm::i32vec2 &mousePos_)
{
	float dy = mousePos_.y - mousePos.y;
	float dx = mousePos_.x - mousePos.x;
	float s = 5.0f;
	float pi = 2.0f * glm::pi<float>();

	phi -= dx * 0.5f / CShaderProgram::ScreenWidth * s;
	theta -= dy * 0.5f / CShaderProgram::ScreenHeight * s;

	phi = phi > 2.0f * pi ? phi - 2.0f * pi : phi;
	phi = phi < 0 ? phi + 2.0f * pi : phi;

	theta = theta > 2.0f * pi ? theta - 2.0f * pi : theta;
	theta = theta < 0 ? theta + 2.0f * pi : theta;

	SetMouse(mousePos_);
}

glm::mat4 &CCamera::GetViewMatrix()
{
	glm::vec3 eyeDir = glm::vec3(cosf(theta) * cosf(phi), sinf(theta), cosf(theta) * sinf(phi)) * radius;
	eyeAt = eyeDir + eyePos;
	V = glm::lookAt(eyePos, eyeAt, glm::vec3(0, 1, 0));

	return V;
}

glm::mat4 &CCamera::GetProjMatrix()
{
	P = glm::perspective(fovy / 180.0f * glm::pi<float>(), (float)CShaderProgram::ScreenWidth / (float)CShaderProgram::ScreenHeight, defaultZNear, defaultZFar);

	return P;
}

glm::vec3 &CCamera::GetEyePos()
{
	//eyePos = glm::vec3(cosf(theta) * cosf(phi), sinf(theta), cosf(theta) * sinf(phi)) * radius;
	return eyePos;
}

glm::vec3 &CCamera::GetEyeAt()
{
	eyeAt = glm::vec3(cosf(theta) * cosf(phi), sinf(theta), cosf(theta) * sinf(phi)) * radius + eyePos;
	return eyeAt;
}

void CCamera::Forward(float scalar)
{
	eyePos += glm::normalize(eyeAt - eyePos) * scalar;
}

void CCamera::Backward(float scalar)
{
	Forward(-scalar);
}

void CCamera::Left(float scalar)
{
	eyeAt = GetEyeAt();
	eyePos = GetEyePos();
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 viewDir = glm::normalize(eyeAt - eyePos);
	glm::vec3 leftDir = glm::cross(up, viewDir);

	eyePos += glm::normalize(leftDir) * scalar;
}

void CCamera::Right(float scalar)
{
	Left(-scalar);
}

void CCamera::Step()
{
	if (fabs(zoom) < FLT_EPSILON) {
		zoom = 0.0f;
		return;
	}

	if (zoom < 0) {
		fovy -= expf(-zoom - 20) * 2.0f;
		zoom += 1.0f;
	} else {
		fovy += expf(zoom - 20) * 2.0f;
		zoom -= 1.0f;
	}

	if (fovy < 30.0f) {
		fovy = 30.0f;
	}

	if (fovy > 50.0f) {
		fovy = 50.0f;
	}
}

float CCamera::defaultZFar = 100.0f;
float CCamera::defaultZNear = 0.1f;
