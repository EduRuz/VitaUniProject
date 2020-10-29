#include "platform.h"


/**
Initializes world object variables. Calls parent constructor.
@param creates a pointer to the world the object are to be added to.
*/
platform::platform(b2World* world) : gameFloor(world)
{
}

platform::platform() {}


/**
	Initializes bottom part of platform for seperate texture to the top object.
	Made up of multiple smaller objects so the texture doesnt look stretched.
	@param GEF PrimitiveBuilder for creating object Mesh.
	@param GEF Platform for drawing to window.
	@param int for amount of segments the platform has.
	@param xStart for x location to start drawing the ground.
	@param yStart for y location to start drawing the ground.
*/
void platform::initPlatform(PrimitiveBuilder * primitive_builder_, gef::Platform & plat, int segments, float xStart, float yStart)
{
	if (segments > 15) { 
		segments = 15;
		initGround(primitive_builder_, plat, xStart, yStart);
		initGrass(primitive_builder_, plat, xStart, yStart);
	}
	else {
		bodyAmount = segments;
		initGround(primitive_builder_, plat, xStart, yStart);
		initGrass(primitive_builder_, plat, xStart, yStart);
	}
	
}

/**
Render platform to screen.
Overloaded.
@param GEF Renderer3D for rendering objects in 3D.
@param GEF PrimitiveBuilder for setting materials.
*/
void platform::render(gef::Renderer3D * renderer_3d_, PrimitiveBuilder * primitive_builder_)
{
	// draw ground
	renderer_3d_->set_override_material(mat);


	for (int i = 0; i < bodyAmount; i++) {
		renderer_3d_->DrawMesh(groundObjects[i]);
	}

	renderer_3d_->set_override_material(&primitive_builder_->green_material());
	renderer_3d_->DrawMesh(grass);

	renderer_3d_->set_override_material(NULL);

}

