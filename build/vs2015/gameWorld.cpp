#include "gameWorld.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <graphics/sprite.h>
#include "load_texture.h"
#include <input/sony_controller_input_manager.h>

/**
	Initializes world object variables.
	@param creates a pointer to the world the object are to be added to.
*/
gameWorld::gameWorld(b2World* world)
{
	world_ = world;
}

/**
	Cleans up world objects.
*/
void gameWorld::onExit()
{
	for (int i = 0; i < amountOfFloors; i++) {
		floors[i].onExit();
	}

	leftMostWall.onExit();
	rightMostWall.onExit();
	platform1.onExit();
	platform2.onExit();
	platform3.onExit();
	platform4.onExit();
	platform5.onExit();
	platform6.onExit();
	platform7.onExit();

}

void gameWorld::onInit()
{

	for (int i = 0; i < amountOfFloors; i++) {
		floors[i] = gameFloor(world_);
	}

	leftMostWall = wall(world_);
	rightMostWall = wall(world_);

	platform1 = platform(world_);
	platform2 = platform(world_);
	platform3 = platform(world_);
	platform4 = platform(world_);
	platform5 = platform(world_);
	platform6 = platform(world_);
	platform7 = platform(world_);

	keyFound = false;
}

/**
	Initializes Ground objects.
	@param GEF PrimitiveBuilder for creating object Mesh.
	@param GEF Platform for drawing to window.
*/
void gameWorld::initGround(PrimitiveBuilder* primitive_builder_, gef::Platform& plat)
{
	float start = -7.50;
	float interval = 7.5;
	for (int i = 0; i < amountOfFloors; i++) {
		floors[i].init(primitive_builder_, plat, start, -1.848f);
		start += interval;
	} 

	platform1.initPlatform(primitive_builder_, plat, 5, 5.0f, 0.5f);
	platform2.initPlatform(primitive_builder_, plat, 5, -7.0f, 3.5f);
	platform3.initPlatform(primitive_builder_, plat, 5, -2.0f, 1.5f);
	platform4.initPlatform(primitive_builder_, plat, 5, 10.0f, 2.0f);
	platform5.initPlatform(primitive_builder_, plat, 5, 15.0f, 3.0f);
	platform6.initPlatform(primitive_builder_, plat, 5, 20.0f, 3.5f);
	platform7.initPlatform(primitive_builder_, plat, 5, 25.0f, 2.5f);

	leftMostWall.init(primitive_builder_, plat, 15, -7.5f, -1.346f);
	rightMostWall.init(primitive_builder_, plat, 15, 29.5f, -1.346f);
}

/**
	Render object to screen.
	@param GEF Renderer3D for rendering objects in 3D.
	@param GEF PrimitiveBuilder for setting materials.
	@param GEF Sprite Renderer for rendering 2D parts.
	@param GEF Platform for drawing to window.
*/
void gameWorld::renderGround(gef::Renderer3D* renderer_3d_, PrimitiveBuilder* primitive_builder_, gef::SpriteRenderer* spriteRenderer, gef::Platform& plat)
{

	// draw ground
	for (int i = 0; i < amountOfFloors; i++) {
		floors[i].render(renderer_3d_, primitive_builder_, spriteRenderer, plat);
	}
	platform1.render(renderer_3d_, primitive_builder_);
	platform2.render(renderer_3d_, primitive_builder_);
	platform3.render(renderer_3d_, primitive_builder_);
	platform4.render(renderer_3d_, primitive_builder_);
	platform5.render(renderer_3d_, primitive_builder_);
	platform6.render(renderer_3d_, primitive_builder_);
	platform7.render(renderer_3d_, primitive_builder_);

	leftMostWall.render(renderer_3d_, primitive_builder_, spriteRenderer, plat);
	rightMostWall.render(renderer_3d_, primitive_builder_, spriteRenderer, plat);
}

/**
	Update gameWorld variables
	@param GEF InputManager for controlling inputs.
*/
void gameWorld::update(gef::InputManager* inputManager)
{
	const gef::SonyController* controller = inputManager->controller_input()->GetController(0);
	
	if (keyFound) {
		rightMostWall.sethasWallRisen(true);
	}

	leftMostWall.update(inputManager);
	rightMostWall.update(inputManager);

}

