#include "wall.h"


/**
Sets classes own world pointer.
@param creates a pointer to the world the object are to be added to.
*/
wall::wall(b2World* world)
{
	world_ = world;
}

wall::wall()
{
}


/**
	Initialize Wall variables.
	@param GEF PrimitiveBuilder for creating object Mesh.
	@param GEF Platform for drawing to window.
	@param int for the amount og segments in the wall.
	@param X value for the wall.
	@param Y value for the start of drawing segments.
*/
void wall::init(PrimitiveBuilder * primitive_builder_, gef::Platform & plat, int size, float xStart, float yStart)
{
	hasWallRisen = false;
	bodyAmount = 0;
	incrValue = 0.0f;

	wallTexture = CreateTextureFromPNG("Wall.png", plat);
	mat_.set_texture(wallTexture);
	mat = &mat_;

	if (size > 15) {
		bodyAmount = 15;
	}
	else {
		bodyAmount = size;
	}
	// wall dimensions
	wall_half_dimensions.set_x(0.25f);
	wall_half_dimensions.set_y(0.25f);
	wall_half_dimensions.set_z(1.0f);

	// setup the mesh for the wall
	wall_mesh_ = primitive_builder_->CreateBoxMesh(wall_half_dimensions);

	for (int i = 0; i < bodyAmount; i++) {
		wallObjects[i].set_mesh(wall_mesh_);
	}

	// create a physics body
	b2BodyDef body_def;
	body_def.type = b2_staticBody;

	float32 Xinterval = (wall_half_dimensions.x() * 2);

	for (int i = 0; i < bodyAmount; i++) {

		body_def.position = b2Vec2(xStart, yStart);
		x_y[i].xPos = xStart;
		x_y[i].yPos = yStart;
		wallBodys[i] = world_->CreateBody(&body_def);
		yStart += Xinterval;
	}

	// create the shape
	b2PolygonShape shape;
	shape.SetAsBox(wall_half_dimensions.x(), wall_half_dimensions.y());

	// create the fixture
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;

	// create the fixture on the rigid body
	for (int i = 0; i < bodyAmount; i++) {
		wallBodys[i]->CreateFixture(&fixture_def);
	}

	// update visuals from simulation data
	for (int i = 0; i < bodyAmount; i++) {
		wallObjects[i].UpdateFromSimulation(wallBodys[i]);
	}
}

/**
Render wall segments to screen.
@param GEF Renderer3D for rendering objects in 3D.
@param GEF PrimitiveBuilder for setting materials.
@param GEF Sprite Renderer for rendering 2D parts.
@param GEF Platform for drawing to window.
*/
void wall::render(gef::Renderer3D * renderer_3d_, PrimitiveBuilder * primitive_builder_, gef::SpriteRenderer * spriteRenderer, gef::Platform & plat)
{
	// draw ground
	renderer_3d_->set_override_material(mat);


	for (int i = 0; i < bodyAmount; i++) {
		renderer_3d_->DrawMesh(wallObjects[i]);
	}

	renderer_3d_->set_override_material(NULL);
}

/**
Clean up variables
*/
void wall::onExit()
{
	delete wall_mesh_;
	wall_mesh_ = NULL;

//	delete world_;
//	world_ = NULL;
}

/**
Update wall variables.
@param GEF InputManager for controlling inputs.
*/
void wall::update(gef::InputManager* inputManager)
{

	if (hasWallRisen == true) {
		
		if (incrValue < 2.0f) {
			raiseWall(incrValue);
			incrValue += 0.05;
		}
	}
	else {
		if (incrValue > 0.0f) {
			lowerWall(incrValue);
			incrValue -= 0.05f;
		}	
	}
}

/**
	set bool that controls if the wall has risen.
	@param true sets the wall to rise false otherwise.
*/
void wall::sethasWallRisen(bool flag)
{
	hasWallRisen = flag;
}

/**
	@return bool that controls if the wall has risen
*/
bool wall::gethasWallRisen()
{
	return hasWallRisen;
}

/**
	Incriments the values so the wall raises slowly.
	@param float to incriment by.
*/
void wall::raiseWall(float incr)
{

	for (int i = 0; i < bodyAmount; i++) {
		wallBodys[i]->SetTransform(b2Vec2(x_y[i].xPos, (x_y[i].yPos + incr)), gef::DegToRad(0.0f));
		wallObjects[i].UpdateFromSimulation(wallBodys[i]);	
	}
}

/**
Decriments the values so the wall raises slowly.
@param float to deccriment by.
*/
void wall::lowerWall(float incr)
{
	for (int i = 0; i < bodyAmount; i++) {
		wallBodys[i]->SetTransform(b2Vec2(x_y[i].xPos, x_y[i].yPos + incr), gef::DegToRad(0.0f));
		wallObjects[i].UpdateFromSimulation(wallBodys[i]);
	}
}
