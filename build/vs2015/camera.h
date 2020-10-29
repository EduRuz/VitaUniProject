#pragma once
#include "game_object.h"

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class Renderer3D;
}

class camera
{
public:
	camera();
	camera(float LAX, float LAY, float LAZ, float EX, float EY, float EZ, float UX, float UY, float UZ );
	virtual ~camera();

	void update();

	void setLookAt(char vectorPos, float value);
	float getLookAt(char vectorPos);
	gef::Vector4 getLookAtVector();

	void setEye(char vectorPos, float value);
	float getEye(char vectorPos);
	gef::Vector4 getEyeVector();

	void setUp(char vectorPos, float value);
	float getUp(char vectorPos);
	gef::Vector4 getUpVector();

	void moveLeft(float movementSpeed);
	void moveRight(float movementSpeed);

private:

	gef::Vector4 lookAtVector;
	gef::Vector4 eyeVector;
	gef::Vector4 upVector;
	gef::Vector4 forwardVector;
	gef::Vector4 strafeVector;

};

