#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "utility.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class CCamera
{
public:
	CCamera(float Radius, float Phi, float Theta, float Fovy, const glm::vec3 &eyeCenter);
	~CCamera(void);

	void MouseMove(const glm::i32vec2 &mousePos);
	void Forward(float scalar);
	void Backward(float scalar);
	void Left(float scalar);
	void Right(float scalar);
	void SetMouse(const glm::i32vec2 &mousePos);

	glm::mat4 &GetViewMatrix();
	glm::mat4 &GetProjMatrix();
	glm::vec3 &GetEyePos();
	glm::vec3 &GetEyeAt();

	void ZoomOut() { zoom = 20.0f;}
	void ZoomIn() { zoom = -20.0f;}
	void Step();

	float GetFovy() const { return fovy;}
public:
	static float defaultZNear;
	static float defaultZFar;
private:
	float radius;
	float phi;
	float theta;
	float fovy;
	glm::vec2 mousePos;
	float zoom;

	glm::mat4 V, P;
	glm::vec3 eyePos, eyeAt;
}

#endif
;

