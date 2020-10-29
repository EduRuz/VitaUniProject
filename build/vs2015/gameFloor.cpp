#include "gameFloor.h"


/**
	Sets classes own world pointer.
	@param creates a pointer to the world the object are to be added to.
*/
gameFloor::gameFloor(b2World* world)
{
	world_ = world;
}

gameFloor::gameFloor() 
{
}

/**
	Clean up variables
*/
void gameFloor::onExit()
{
	delete ground_mesh_;
	ground_mesh_ = NULL;

	delete grass_mesh_;
	grass_mesh_ = NULL;
}

/**
	Initializes bottom part of ground for seperate texture to the top objects.
	@param GEF PrimitiveBuilder for creating object Mesh.
	@param GEF Platform for drawing to window.
*/
void gameFloor::init(PrimitiveBuilder * primitive_builder_, gef::Platform & plat, float xStart, float yStart)
{

	initGround(primitive_builder_, plat, xStart, yStart);
	initGrass(primitive_builder_, plat, xStart, yStart);

	
}

/**
	Initializes bottom part of ground for seperate texture to the top objects.
	Made up of multiple smaller objects so the texture doesnt look stretched.
	@param GEF PrimitiveBuilder for creating object Mesh.
	@param GEF Platform for drawing to window.
	@param xStart for x location to start drawing the ground.
	@param yStart for y location to start drawing the ground.
*/
void gameFloor::initGround(PrimitiveBuilder * primitive_builder_, gef::Platform & plat, float xStart, float yStart)
{
	groundTexture = CreateTextureFromPNG("Ground.png", plat);
	mat_.set_texture(groundTexture);
	mat = &mat_;

	// ground dimensions
	ground_half_dimensions.set_x(0.25f);
	ground_half_dimensions.set_y(0.25f);
	ground_half_dimensions.set_z(1.0f);

	// setup the mesh for the ground
	ground_mesh_ = primitive_builder_->CreateBoxMesh(ground_half_dimensions);

	for (int i = 0; i < bodyAmount; i++) {
		groundObjects[i].set_mesh(ground_mesh_);
	}

	// create a physics body
	b2BodyDef body_def;
	body_def.type = b2_staticBody;

	float32 Xinterval = (ground_half_dimensions.x() * 2);

	for (int i = 0; i < bodyAmount; i++) {

		body_def.position = b2Vec2(xStart, yStart);
		groundBodys[i] = world_->CreateBody(&body_def);
		xStart += Xinterval;
	}

	// create the shape
	b2PolygonShape shape;
	shape.SetAsBox(ground_half_dimensions.x(), ground_half_dimensions.y());

	// create the fixture
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;

	// create the fixture on the rigid body
	for (int i = 0; i < bodyAmount; i++) {
		groundBodys[i]->CreateFixture(&fixture_def);
	}

	// update visuals from simulation data
	for (int i = 0; i < bodyAmount; i++) {
		groundObjects[i].UpdateFromSimulation(groundBodys[i]);
	}
}

/**
	Initializes top part of ground for seperate texture to the bottom objects.
	@param GEF PrimitiveBuilder for creating object Mesh.
	@param GEF Platform for drawing to window.
	@param xStart for x location to start drawing the grass.
	@param yStart for y location to start drawing the grass.
*/
void gameFloor::initGrass(PrimitiveBuilder * primitive_builder_, gef::Platform & plat, float xStart, float yStart)
{

	// grass dimensions
	grass_half_dimensions.set_x(ground_half_dimensions.x() * bodyAmount);
	grass_half_dimensions.set_y(0.005f);
	grass_half_dimensions.set_z(1.0f);
	// setup the mesh for the ground
	grass_mesh_ = primitive_builder_->CreateBoxMesh(grass_half_dimensions);
	grass.set_mesh(grass_mesh_);
	// create a physics body
	b2BodyDef body_def;
	body_def.type = b2_staticBody;

	body_def.position = b2Vec2((xStart + (grass_half_dimensions.x() - ground_half_dimensions.x())), (((yStart + ground_half_dimensions.y()) + grass_half_dimensions.y()) + 0.001));
	grass_body_ = world_->CreateBody(&body_def);

	// create the shape
	b2PolygonShape shape;
	shape.SetAsBox(grass_half_dimensions.x(), grass_half_dimensions.y());

	// create the fixture
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;

	// create the fixture on the rigid body
	grass_body_->CreateFixture(&fixture_def);
	
	// update visuals from simulation data
	grass.UpdateFromSimulation(grass_body_);
}

/**
	Render object to screen.
	@param GEF Renderer3D for rendering objects in 3D.
	@param GEF PrimitiveBuilder for setting materials.
	@param GEF Sprite Renderer for rendering 2D parts.
	@param GEF Platform for drawing to window.
*/
void gameFloor::render(gef::Renderer3D * renderer_3d_, PrimitiveBuilder * primitive_builder_, gef::SpriteRenderer* spriteRenderer, gef::Platform& plat)
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

