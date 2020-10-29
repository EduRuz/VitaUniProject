#pragma once
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include "primitive_builder.h"
#include <graphics/font.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <box2d/Box2D.h>
#include "game_object.h"
#include <maths/math_utils.h>
#include <graphics/sprite.h>
#include "load_texture.h"
#include <input/input_manager.h>

struct positions {

	float xPos;
	float yPos;
};
class wall
{
public:
	wall(b2World* world);
	wall();

	void init(PrimitiveBuilder* primitive_builder_, gef::Platform& plat, int size, float xStart, float yStart);
	void render(gef::Renderer3D* renderer_3d_, PrimitiveBuilder* primitive_builder_, gef::SpriteRenderer* spriteRenderer, gef::Platform& plat);
	void onExit();
	void update(gef::InputManager* inputManager);
	void sethasWallRisen(bool flag);
	bool gethasWallRisen();
	void raiseWall(float incr);
	void lowerWall(float incr);

private:
	
	bool hasWallRisen;
	float incrValue;

	b2World* world_;

	gef::Vector4 wall_half_dimensions;
	gef::Mesh* wall_mesh_;
	int bodyAmount;

	positions x_y[15];

	GameObject wallObjects[15];
	b2Body* wallBodys[15]; 

	gef::Texture* wallTexture;

	gef::Material mat_;
	gef::Material* mat;
};

