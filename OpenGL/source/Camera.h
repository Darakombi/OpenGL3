#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

enum CameraDirection {
	FORWARD,
	LEFT,
	BACKWARD,
	RIGHT,
	UP,
	DOWN
};

float YAW = -90.0f;
float PITCH = 0.0f;
float SENSITIVITY = 0.075f;
float SPEED = 2.0f;
float FOV = 45.0f;
float FOV_MIN = 1.0f;
float FOV_MAX = 45.0f;

class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Fov;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 getViewMatrix() const {
		return
			glm::mat4(
				Right.x, Up.x, -Front.x, 0.0f,
				Right.y, Up.y, -Front.y, 0.0f,
				Right.z, Up.z, -Front.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f)
			*
			glm::mat4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				-Position.x, -Position.y, -Position.z, 1.0f);
		//return glm::lookAt(Position, Position + Front, Up);
	}

	const glm::vec3 getPosition() const { return Position; }

	void processKeys(CameraDirection dir, float deltaTime, bool ignorePitch = true) {

		float velocity = MovementSpeed * deltaTime;

		if (ignorePitch)
		{
			glm::vec3 flatFront = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
			glm::vec3 flatRight = glm::normalize(glm::cross(flatFront, WorldUp));

			switch (dir) {
			case FORWARD:  Position += flatFront * velocity; break;
			case BACKWARD: Position -= flatFront * velocity; break;
			case LEFT:	   Position -= flatRight * velocity; break;
			case RIGHT:	   Position += flatRight * velocity; break;
			case DOWN:	   Position -= glm::vec3(0, 1, 0) * velocity; break;
			case UP:	   Position += glm::vec3(0, 1, 0) * velocity; break;
			}
		}
		else {
			switch (dir) {
			case FORWARD:  Position += Front * velocity; break;
			case BACKWARD: Position -= Front * velocity; break;
			case LEFT:	   Position -= Right * velocity; break;
			case RIGHT:	   Position += Right * velocity; break;
			}
		}
	}

	void processMouse(float offsetX, float offsetY, bool constrainPitch = true) {
		offsetX *= MouseSensitivity;
		offsetY *= MouseSensitivity;

		Yaw = glm::mod(Yaw + offsetX, 360.0f);
		Pitch += offsetY;

		if (constrainPitch)
			Pitch = std::max(-89.0f, std::min(Pitch, 89.0f));
		updateCameraVectors();
	}

	void processScroll(float offsetY) {
		Fov -= (float)offsetY;
		Fov = std::max(FOV_MIN, std::min(Fov, FOV_MAX));
	}

	void getFacing() {
		glm::vec3 dir = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
		if (fabs(dir.x) > fabs(dir.z)) {
			std::cout << (dir.x > 0 ? "+X" : "-X") << std::endl;
		}
		else std::cout << (dir.z > 0 ? "+Z" : "-Z") << std::endl;
	}

private:
	void updateCameraVectors() {
		glm::vec3 front(0.0f);
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};