#ifndef GL_CAMERA_H
#define GL_CAMERA_H

#include <framework.hpp>
#include <GL_header.hpp>

#include <math/glm/vec3.hpp>
#include <math/glm/vec4.hpp>
#include <math/glm/matrix.hpp>
#include <math/glm/trigonometric.hpp>

#undef far
#undef near

namespace GL
{
	enum camDirections {
		RIGHT, LEFT,
		MOVETO_W, MOVETO_S,
	};
	
	enum camProjection {
		PERSPECTIVE = 0, ORTHO = 1
	};

	//pyzss - calculate functions
	static const float PITCH = 0.0f;
	static const float YAW = -90.0f;
	static const float ZOOM = 45.0f;
	static const float SENSIVITY = 0.1f;
	static const float MAXSPEED = 25.0f;
	//Frustrum
	static const float NEAR_FRUST = 0.01f;
	static const float FAR_FRUST = 50.0f;

	class GL_Camera
	{
	private:
		glm::mat4 viewMat, projMat;
		camProjection projType = PERSPECTIVE;
		//Calculate pyzss
		float Pitch, Yaw,
			Zoom, Sensivity, Speed;
		//Frustrum
		float NearFrust, FarFrust;
		//Calculate functions
		void updateVectors();
	public:
		//Directional and 2 positional vectors
		glm::vec3 Position, Front, Right, Up, WorldUp;
		//Flags
		bool canRotate = false;
		bool mouseFirst = true;
		//Constructors&Destructors
		GL_Camera();
		GL_Camera(glm::vec3 position,
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = YAW, float pitch = PITCH);
		GL_Camera(float xPos, float yPos, float zPos, float xUp, float yUp, float zUp,
			float yaw, float pitch);

		~GL_Camera();

		//Accessors
		glm::mat4 getProjMat() { return projMat; }
		void setProjMat(glm::mat4 matrix) { projMat = matrix; }
		glm::mat4 getViewMat() { return viewMat; }
		void setViewMat(glm::mat4 matrix) { viewMat = matrix; }
		camProjection getProjType() const { return projType; }
		void setProjType(camProjection newProj) { projType = newProj; }

		float getSpeed() const { return Speed; }
		void setSpeed(float speed) { Speed = speed; }
		float getZoom() const { return Zoom; }
		void setZoom(float zoom) { Zoom = zoom; }
		float getFar() const { return FarFrust; }
		void setFar(float far) { FarFrust = far; }
		float getNear() const { return NearFrust; }
		void setNear(float near) { NearFrust = near; }
		//Functions
		glm::mat4 getLookAtMatrix();

		void keyboardProc(camDirections dir, float deltaTime);
		void mousePosProc(float offsetX, float offsetY,
			bool constrainPitch = true);
		void scrollProc(float offsetY);
	};
}
#endif