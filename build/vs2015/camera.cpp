#include "camera.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>
#include <graphics/sprite.h>
#include "load_texture.h"

/**
	default constructor sets the camera to a basic set up.
*/
camera::camera()
{
	lookAtVector.set_x(0.0f); lookAtVector.set_y(0.0f); lookAtVector.set_z(0.0f);
	eyeVector.set_x(0.0f); eyeVector.set_y(0.0f); eyeVector.set_z(10.0f);
	upVector.set_x(0.0f); upVector.set_y(1.0f); upVector.set_z(0.0f);
}

/**
	Overloaded constructor to input camera values at load.
	@param float LookAt X value.
	@param float LookAt Y value.
	@param float LookAt Z value.
	@param float Eye X value.
	@param float Eye Y value.
	@param float Eye Z value.
	@param float Up X value.
	@param float Up Y value.
	@param float Up Z Value.
*/
camera::camera(float LAX, float LAY, float LAZ, float EX, float EY, float EZ, float UX, float UY, float UZ)
{
	lookAtVector.set_x(LAX); lookAtVector.set_y(LAY); lookAtVector.set_z(LAZ);
	eyeVector.set_x(EX); eyeVector.set_y(EY); eyeVector.set_z(EZ);
	upVector.set_x(UX); upVector.set_y(UY); upVector.set_z(UZ);
}


camera::~camera()
{
}

/**
	Update camera values. forwardVector and strafeVector.
*/
void camera::update()
{
	forwardVector.set_x((lookAtVector.x() - eyeVector.x()));
	forwardVector.set_y((lookAtVector.y() - eyeVector.y()));
	forwardVector.set_z((lookAtVector.z() - eyeVector.z()));

	strafeVector = forwardVector.CrossProduct(upVector);
}

/**
	set LookAt Values.
	@param vectorPos, x for X value, y for Y value, z for Z value.
	@param value to be set to.
*/
void camera::setLookAt(char vectorPos, float value)
{
	switch (vectorPos) {
	case 'x': 
		lookAtVector.set_x(value);
		break;
	case 'y': 
		lookAtVector.set_y(value);
		break;
	case 'z': 
		lookAtVector.set_z(value);
		break;
	}
}
/**
	Get LookAt Values.
	@param vectorPos, x for X value, y for Y value, z for Z value.
*/
float camera::getLookAt(char vectorPos)
{
	switch (vectorPos) {
	case 'x': 
		return lookAtVector.x();
		break;
	case 'y': 
		return lookAtVector.y();
		break;
	case 'z': 
		return lookAtVector.z();
		break;
	}
	return 0.0f;
}

/**
	@return lookAtVector.
*/
gef::Vector4 camera::getLookAtVector()
{
	return lookAtVector;
}

/**
	set eye Values.
	@param vectorPos, x for X value, y for Y value, z for Z value.
	@param value to be set to.
*/
void camera::setEye(char vectorPos, float value)
{
	switch (vectorPos) {
	case 'x': 
		eyeVector.set_x(value);
		break;
	case 'y': 
		eyeVector.set_y(value);
		break;
	case 'z': 
		eyeVector.set_z(value);
		break;
	}
}

/**
	Get Eye Values.
	@param vectorPos, x for X value, y for Y value, z for Z value.
*/
float camera::getEye(char vectorPos)
{
	switch (vectorPos) {
	case 'x': 
		return eyeVector.x();
		break;
	case 'y': 
		return eyeVector.y();
		break;
	case 'z': 
		return eyeVector.z();
		break;
	}
	return 0.0f;
}


/**
	@return EyeVector.
*/
gef::Vector4 camera::getEyeVector()
{
	return eyeVector;
}

/**
	set Up Values.
	@param vectorPos, x for X value, y for Y value, z for Z value.
	@param value to be set to.
*/
void camera::setUp(char vectorPos, float value)
{
	switch (vectorPos) {
	case 'x': 
		upVector.set_x(value);
		break;
	case 'y': 
		upVector.set_y(value);
		break;
	case 'z': 
		upVector.set_z(value);
		break;
	}
}

/**
	Get Up Values.
	@param vectorPos, x for X value, y for Y value, z for Z value.
*/
float camera::getUp(char vectorPos)
{
	switch (vectorPos) {
	case 'x': 
		return upVector.x();
		break;
	case 'y': 
		return upVector.y();
		break;
	case 'z': 
		return upVector.z();
		break;
	}
	return 0.0f;
}

/**
	@return UpVector.
*/
gef::Vector4 camera::getUpVector()
{
	return upVector;
}

/**
	Move camera left by decrementing the eyeVector and the lookAtVector by the same value.
	@param float for how fast to move to the left.
*/
void camera::moveLeft(float movementSpeed)
{
	eyeVector -= strafeVector * movementSpeed;
	lookAtVector -= strafeVector * movementSpeed;
}

/**
	Move camera right by incrementing the eyeVector and the lookAtVector by the same value.
	@param float for how fast to move to the right.
*/
void camera::moveRight(float movementSpeed)
{
	eyeVector += strafeVector * movementSpeed;
	lookAtVector += strafeVector * movementSpeed;
}
