#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

enum CameraDirection {
	FORWARD,
	LEFT,
	BACKWARD,
	RIGHT
};

float YAW = -90.0f;
float PITCH = 0.0f;
float SENSITIVITY = 0.1f;
float SPEED = 2.5f;
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
		return glm::lookAt(Position, Position + Front, Up);
	}

	void processKeys(CameraDirection dir, float deltaTime) {

		float velocity = MovementSpeed * deltaTime;

		switch (dir) {
		case FORWARD:  Position += Front * velocity; break;
		case LEFT:	   Position -= Right * velocity; break;
		case BACKWARD: Position -= Front * velocity; break;
		case RIGHT:	   Position += Right * velocity; break;
		}
	}

	void processMouse(float offsetX, float offsetY, bool constrainPitch = false) {
		offsetX *= MouseSensitivity;
		offsetY *= MouseSensitivity;

		Yaw += offsetX;
		Pitch += offsetY;

		if (constrainPitch)
			Pitch = std::max(-89.0f, std::min(Pitch, 89.0f));
		updateCameraVectors();
	}

	void processScroll(float offsetY) {
		Fov -= (float)offsetY;
		Fov = std::max(FOV_MIN, std::min(Fov, FOV_MAX));
	}

private:
	void updateCameraVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};