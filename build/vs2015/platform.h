#pragma once
#include "gameFloor.h"
class platform :
	public gameFloor
{
public:
	platform(b2World* world);
	platform();

	void initPlatform(PrimitiveBuilder* primitive_builder_, gef::Platform& plat, int segments,  float xStart, float yStart);
	void render(gef::Renderer3D* renderer_3d_, PrimitiveBuilder* primitive_builder_);
};

