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
class gameFloor 
{
public:
	gameFloor(b2World* world);
	gameFloor();

	void onExit();

	void init(PrimitiveBuilder* primitive_builder_, gef::Platform& plat, float xStart, float yStart);
	void initGround(PrimitiveBuilder* primitive_builder_, gef::Platform& plat, float xStart, float yStart);
	void initGrass(PrimitiveBuilder* primitive_builder_, gef::Platform& plat, float xStart, float yStart);
	virtual void render(gef::Renderer3D* renderer_3d_, PrimitiveBuilder* primitive_builder_, gef::SpriteRenderer* spriteRenderer, gef::Platform& plat);
	
	
	

private:

	
protected:

	b2World* world_;

	// Grass Variables
	gef::Vector4 grass_half_dimensions;
	gef::Mesh* grass_mesh_;
	GameObject grass;
	b2Body* grass_body_;

	// ground variables
	gef::Vector4 ground_half_dimensions;
	gef::Mesh* ground_mesh_;
	int bodyAmount = 15;

	GameObject groundObjects[15];
	b2Body* groundBodys[15];

	gef::Texture* groundTexture;

	gef::Material mat_;
	gef::Material* mat;

};

