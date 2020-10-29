#pragma once
#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include <box2d/Box2D.h>
#include "game_object.h"
#include "gameFloor.h"
#include "platform.h"
#include "wall.h"

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class Renderer3D;
}

class gameWorld
{
public:
	gameWorld(b2World* world_);

	void onExit();
	void onInit();
	void initGround(PrimitiveBuilder* primitive_builder_, gef::Platform& plat);
	void renderGround(gef::Renderer3D* renderer_3d_, PrimitiveBuilder* primitive_builder_, gef::SpriteRenderer* spriteRenderer, gef::Platform& plat);
	void update(gef::InputManager* inputManager);

	void setKeyFound(bool flag) { keyFound = flag; }

private:
	bool keyFound;

	const int static amountOfFloors = 5;

	wall leftMostWall;
	wall rightMostWall;

	platform platform1;
	platform platform2;
	platform platform3;
	platform platform4;
	platform platform5;
	platform platform6;
	platform platform7;

	gameFloor floors[amountOfFloors];
	// create the physics world
	b2World* world_;

	
protected:
	
};

