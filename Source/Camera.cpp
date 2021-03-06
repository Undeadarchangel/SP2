#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

double modeBounceTime = 0.0;
extern double elapsedTime = 0.0;
extern float FOV;

Camera& Camera::getCam(){ //Static getter for Singleton
	static Camera cam;
	return cam;
}

Camera::Camera(): focusSpd(100.f), freeSpd(50.f){ //Default ctor
	mode = MODE::FOCUS;
	leftMouse = rightMouse = 0;
	canMove[0] = canMove[1] = canMove[2] = canMove[3] = false;
}

void Camera::Init(const Vector3 &pos, const Vector3 &target, const Vector3 &up){ //Init cam
	this->pos = defaultPos = pos;
	this->target = defaultTarget = target;
	this->up = defaultUp = up;
}

void Camera::Update(double dt){ //Update cam
	resetCollision();
	displacement.SetZero();
	if(Application::IsKeyPressed('B') && modeBounceTime <= elapsedTime){ //Change cam mode
		mode = MODE(!bool(mode));
		Vector3 dir = target - pos, front = dir.Normalized(), right = front.Cross(up).Normalized();
		right.y = 0;
		if(mode == MODE::FOCUS){
			target = Vector3(0.f, 0.f, 0.f);
			pos = Vector3(0.f, 5.f, -30.f);
			front = target - pos;
			front.y = 0;
			right = front.Cross(up);
			right.y = 0;
			up = right.Cross(front).Normalized();
		}
		modeBounceTime = elapsedTime + 0.2;
	}

	if(Application::IsKeyPressed('A') - Application::IsKeyPressed('D')){ //Move cam left or right
		Vector3 dir = target - pos, front = dir.Normalized(), right = front.Cross(up).Normalized();
		right.y = 0;
		if(mode == MODE::FOCUS){
			float yaw = float(float(Application::IsKeyPressed('A') - Application::IsKeyPressed('D')) * -focusSpd * dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			front = rotation * (target - pos);
			pos = target - front;
			right = front.Cross(up);
			right.y = 0;
			up = right.Cross(front).Normalized();
		} else if(mode == MODE::FREE){
			displacement += float(Application::IsKeyPressed('A') - Application::IsKeyPressed('D')) * float(-freeSpd * dt) * right;
		}
	}

	if(Application::IsKeyPressed(32) - Application::IsKeyPressed(16)){ //Move cam up or down
		Vector3 dir = target - pos, front = dir.Normalized(), right = front.Cross(up).Normalized();
		right.y = 0;
		if(mode == MODE::FOCUS){
			float pitch = -float(float(Application::IsKeyPressed(32) - Application::IsKeyPressed(16)) * focusSpd * dt);
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			front = rotation * (target - pos);
			pos = target - front;
			right = front.Cross(up);
			right.y = 0;
			up = right.Cross(front).Normalized();
		} else if(mode == MODE::FREE){
			pos += float(Application::IsKeyPressed(32) - Application::IsKeyPressed(16)) * float(freeSpd * dt) * up;
			target += float(Application::IsKeyPressed(32) - Application::IsKeyPressed(16)) * float(freeSpd * dt) * up;
		}
	}

	if(Application::IsKeyPressed('W') || (leftMouse && !rightMouse)){ //Move cam forward or towards the target
		Vector3 dir = target - pos, front = dir.Normalized(), right = front.Cross(up).Normalized();
		right.y = 0;
		if(mode == MODE::FOCUS){
			pos += front * float(focusSpd / 2 * dt) * (dir.Length() > 12);
		} else{
			if(Application::IsKeyPressed('W')){
				front.y = 0;
			}
			displacement += float(freeSpd * dt) * front;
		}
	}
	if(Application::IsKeyPressed('S') || (rightMouse && !leftMouse)){ //Move cam backward or away from the target
		Vector3 dir = target - pos, front = dir.Normalized(), right = front.Cross(up).Normalized();
		right.y = 0;
		if(mode == MODE::FOCUS){
			pos += front * float(-focusSpd / 2 * dt) * (dir.Length() < 200);
		} else{
			if(Application::IsKeyPressed('S')){
				front.y = 0;
			}
			displacement += float(-freeSpd * dt) * front;
		}
	}
	if (canMove[POSX] && displacement.x > 0)
	{
		pos.x += displacement.x;
		target.x += displacement.x;
	}
	else
		if (canMove[NEGX] && displacement.x < 0)
		{
			pos.x += displacement.x;
			target.x += displacement.x;
		}
	if (canMove[POSZ] && displacement.z > 0)
	{
		pos.z += displacement.z;
		target.z += displacement.z;
	}
	else
		if (canMove[NEGZ] && displacement.z < 0)
		{
			pos.z += displacement.z;
			target.z += displacement.z;
		}
}

void Camera::UpdateCamVectors(float yaw, float pitch){ //For cam to respond to mouse movement
	Vector3 front = (target - pos).Normalized(), right = front.Cross(up).Normalized();
	right.y = 0;
	Mtx44 r1, r2;
	r1.SetToRotation(-yaw, 0, 1, 0);
	r2.SetToRotation(-pitch, right.x, right.y, right.z);
	if(mode == MODE::FOCUS){
		front = r1 * r2 * (target - pos);
		pos = target - front;
		right = front.Cross(up);
		right.y = 0;
		up = right.Cross(front).Normalized();
	} else if(mode == MODE::FREE){
		front = r1 * r2 * front;
		target = pos + front;
	}
	up = right.Cross(front).Normalized();
}

void Camera::updateCollision(Object target)
{

	if (((abs(pos.x - target.getPos().x)) - 0.5) <= (target.getDimension().x / 2.f))
	{
		if ((abs(pos.z - target.getPos().z) - 1) <= (target.getDimension().z / 2.f))
		{
			if ((pos.z - target.getPos().z) > 0)
			{
				canMove[NEGZ] = false;
			}
			else
			{
				canMove[POSZ] = false;
			}
		}
	}
	if ((abs((pos.z - target.getPos().z)) - 0.5) <= (target.getDimension().z / 2.f))
	{
		if ((abs(pos.x - target.getPos().x) - 1) <= target.getDimension().x / 2.f)
		{
			if ((pos.x - target.getPos().x) > 0)
			{
				canMove[NEGX] = false;
			}
			else
			{
				canMove[POSX] = false;
			}
		}
	}
}

void Camera::resetCollision()
{
	canMove[0] = true;
	canMove[1] = true;
	canMove[2] = true;
	canMove[3] = true;
}