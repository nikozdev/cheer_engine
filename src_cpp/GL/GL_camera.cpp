#include <GL/GL_Camera.h>
#include <math/glm/gtc/matrix_transform.hpp>

namespace GL
{
	//Constructors&Destructors
	GL_Camera::GL_Camera()
	{
		printf("GL::CAMERA::CONSTRUCTOR: Camera has been created without arguments.\n");
	}
	GL_Camera::GL_Camera(glm::vec3 position,
		glm::vec3 up, float yaw, float pitch)
	{
		Position = position;
		WorldUp = up;
		Front = glm::vec3(0.0f, 0.0f, 1.0f);

		Yaw = yaw; Pitch = pitch;
		Sensivity = SENSIVITY;
		Zoom = ZOOM;
		Speed = GL::MAXSPEED;

		NearFrust = NEAR_FRUST;
		FarFrust = FAR_FRUST;

		projMat = glm::mat4(1.0f);
		viewMat = glm::mat4(1.0f);

		updateVectors();
		printf("CAMERA::CONSTRUCTOR: Camera is created\n");
	}

	GL_Camera::GL_Camera(float xPos, float yPos, float zPos, float xUp, float yUp, float zUp,
		float yaw, float pitch)
	{
		Position = glm::vec3(xPos, yPos, zPos);
		WorldUp = glm::vec3(xUp, yUp, zUp);
		Front = glm::vec3(0.0f, 0.0f, 1.0f);

		Yaw = yaw; Pitch = pitch;
		Sensivity = SENSIVITY;
		Zoom = ZOOM;
		Speed = GL::MAXSPEED;

		NearFrust = NEAR_FRUST;
		FarFrust = FAR_FRUST;

		projMat = glm::mat4(1.0f);
		viewMat = glm::mat4(1.0f);

		updateVectors();
		printf("CAMERA::CONSTRUCTOR: Camera is created\n");
	}

	GL_Camera::~GL_Camera()
	{
		printf("DEF_CAMERA::DESTRUCTOR:\nCamera is deleted\n");
	}
	//Functions
	glm::mat4 GL_Camera::getLookAtMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void GL_Camera::keyboardProc(camDirections dir, float deltaTime)
	{
		float velocity = Speed * deltaTime;
		if (dir == MOVETO_W)
		{
			if (projType == PERSPECTIVE)
				Position += Front * velocity;
			else if (projType == ORTHO)
				Position += Up * velocity;
		}
		else if (dir == MOVETO_S)
		{
			if (projType == PERSPECTIVE)
				Position -= Front * velocity;
			else if (projType == ORTHO)
				Position -= Up * velocity;
		}
		else if (dir == LEFT)
		{
			Position -= glm::cross(WorldUp, Front) * velocity;
		}
		else if (dir == RIGHT)
		{
			Position += glm::cross(WorldUp, Front) * velocity;
		}
		updateVectors();
	}

	void GL_Camera::mousePosProc(float offsetX, float offsetY,
		bool constrainPitch)
	{
		if (!canRotate)
			return;
		Yaw = glm::mod((Yaw + offsetX * Sensivity), 360.0f);
		Pitch += offsetY * Sensivity;

		if (constrainPitch)
		{
			if (Pitch > 89.0f)
			{
				Pitch = 89.0f;
			}
			if (Pitch < -80.0f)
			{
				Pitch = -80.0f;
			}
		}
		updateVectors();
	}
	void GL_Camera::scrollProc(float offsetY)
	{
		Zoom += offsetY;
		if (Zoom > ZOOM)
			Zoom = ZOOM;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
	}

	void GL_Camera::updateVectors()
	{
		glm::vec3 direction(1.0f);
		direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)); //We can don't multiply by cos second time
		direction.y = sin(glm::radians(Pitch));
		direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

		Front = glm::normalize(direction);
		Right = glm::normalize(glm::cross(WorldUp, Front));
		Up = glm::normalize(glm::cross(Right, Front));
	}
}