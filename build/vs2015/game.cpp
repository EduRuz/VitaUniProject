#include "game.h"
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
#include <graphics/scene.h>
#include <animation/skeleton.h>
#include <animation/animation.h>


/**
	Game State constructor.
	@param calls parent constructer called to pass window through.
*/
game::game(gef::Platform& platform) : stateMachine(platform),
	gameWorld_(NULL),
	model_scene_(NULL),
	skeleton_(NULL),
	running_anim_(NULL),
	idle_anim_(NULL)
{
}

/**
	Initialize variables used by Game State.
	@param audio manager for music/sound effects to be played.
*/
void game::onLoad(gef::AudioManager* audio_manager_)
{
	// create the renderer for draw 3D geometry
	renderer_3d_ = gef::Renderer3D::Create(plat);

	// initialise primitive builder to make create some 3D geometry easier
	primitive_builder_ = new PrimitiveBuilder(plat);

	SetupLights();

	// initialise the physics world
	b2Vec2 gravity(0.0f, -9.81f);
	world_ = new b2World(gravity);

	gameWorld_ = gameWorld(world_);
	gameWorld_.onInit();
	gameWorld_.initGround(primitive_builder_, plat);
	
	initPlayer();
	initOrbs();
	initKey();

	cam = camera(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 7.0f, 0.0f, 1.0f, 0.0f);

	movementSpeed = 0.005f;

	// load audio assets
	if (audio_manager_)
	{
		// load a sound effect
		sfxOpeningID = audio_manager_->LoadSample("Audio/hiddenWallOpening.wav", plat);
		sfxKeyCollectID = audio_manager_->LoadSample("Audio/PickUpKey.wav", plat);
		sfxOrbCollectID = audio_manager_->LoadSample("Audio/Collect.wav", plat);

		// load in music
		audio_manager_->LoadMusic("Audio/ShanhaiPassGameVersion.wav", plat);

		// play music
		audio_manager_->PlayMusic();
	}



	if (getHardMode()) { maxTime = 50; }
	else { maxTime = 200; }
	
	elapsedSecondsF = 0.0f;
	elapsedSecondsI = 0;
	score = 0;
	keyFound = false;
	isJumping = false;
}

/**
	Clean up variables used by the Game State.
	Overidden.
	@param audio manager for music/sound effects to be played.
*/
void game::onExit(gef::AudioManager* audio_manager_)
{
	// unload audio resources
	if (audio_manager_)
	{
		audio_manager_->StopMusic();
		audio_manager_->UnloadAllSamples();
		sfxOpeningID = -1;
		sfxKeyCollectID = -1;
		sfxOrbCollectID = -1;
	}

	delete mat;
	mat = NULL;

	bone_matrices_.clear();

	delete model_scene_;
	model_scene_ = NULL;

	//skeleton_ = NULL;

	delete mesh_;
	mesh_ = NULL;

	delete running_anim_;
	running_anim_ = NULL;

	delete idle_anim_;
	idle_anim_ = NULL;

	gameWorld_.onExit();

	// destroying the physics world also destroys all the objects within it
	delete world_;
	world_ = NULL;

	delete primitive_builder_;
	primitive_builder_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;


}

/**
	Update function called every frame.
	Overidden.
	@param Frame Time.
	@param GEF inputmanager for controlling input.
	@param Audio Manager for passing into on Exit Functions.
*/
void game::update(float frame_time, gef::InputManager* inputManager, gef::AudioManager* audio_manager_)
{
	
	gef::Matrix44 playerGuyTransform;
	playerGuyTransform = playerGuy.transform();
	cam.setLookAt('x', playerGuyTransform.GetTranslation().x());
	cam.setLookAt('y', playerGuyTransform.GetTranslation().y() + 1.5f);
	cam.setEye('x', playerGuyTransform.GetTranslation().x());
	cam.setEye('y', playerGuyTransform.GetTranslation().y() + 1.5f);
	cam.update();

	gameWorld_.update(inputManager);

	inputControl(inputManager, audio_manager_);

	animationControl(frame_time);

	UpdateSimulation(frame_time, audio_manager_);

	removeFromWorld();

	if (playerGuyTransform.GetTranslation().y() < -5.0 && playerGuyTransform.GetTranslation().x() > 20.0) {
		onExit(audio_manager_);
		stateChange = GAMECOMPLETE;
		setStatebool(true);
	}
	

	elapsedSecondsF += frame_time;
	elapsedSecondsI = static_cast<int>(elapsedSecondsF);

	if (maxTime - elapsedSecondsI <= 0 || (playerGuyTransform.GetTranslation().y() < -5.0 && playerGuyTransform.GetTranslation().x() < 15.0)) {
		onExit(audio_manager_);
		stateChange = GAMEOVER;
		setStatebool(true);
	}

	if (playerGuyBody->GetLinearVelocity().y == 0.0f) {
		isJumping = false;
	}
}

/**
	Renders to the screen, called every frame.
	Overidden.
	@param fps if needing drawn to screen.
	@param GEF SpriteRenderer for rendering  2d object to screen.
	@param GEF Font for Font values.
*/
void game::render(float fps, gef::SpriteRenderer* spriteRenderer, gef::Font* font_)
{
	// projection
	float fov = gef::DegToRad(45.0f);
	float aspect_ratio = (float)plat.width() / (float)plat.height();
	gef::Matrix44 projection_matrix;
	projection_matrix = plat.PerspectiveProjectionFov(fov, aspect_ratio, 0.1f, 100.0f);
	renderer_3d_->set_projection_matrix(projection_matrix);

	// view
	gef::Matrix44 view_matrix;
	view_matrix.LookAt(cam.getEyeVector(), cam.getLookAtVector(), cam.getUpVector());
	renderer_3d_->set_view_matrix(view_matrix);

	// draw 3d geometry
	renderer_3d_->Begin();

	// draw world
	gameWorld_.renderGround(renderer_3d_, primitive_builder_, spriteRenderer, plat);
	// draw player character
	renderer_3d_->DrawSkinnedMesh(playerGuy, bone_matrices_);

	renderer_3d_->set_override_material(mat);

	if (numOfOrbs == 0) { orbsInfo.clear(); }	

	for (std::vector<ObjectInfo>::iterator objectItr = orbsInfo.begin(); objectItr != orbsInfo.end(); ++objectItr) {
		renderer_3d_->DrawMesh(objectItr->object);
	}
	
	renderer_3d_->set_override_material(NULL);

	if (keyFound == false) {
		renderer_3d_->set_override_material(&primitive_builder_->blue_material());
		renderer_3d_->DrawMesh(level1Key);
		renderer_3d_->set_override_material(NULL);
	}

	renderer_3d_->End();

	// start drawing sprites, but don't clear the frame buffer
	spriteRenderer->Begin(false);
	
	if ((maxTime - elapsedSecondsI) <= 50) {	
		font_->RenderText(spriteRenderer, gef::Vector4(10.0f, 10.0f, -0.99f), 1.0f, 0xff0000ff, gef::TJ_LEFT, "Time Remaining: %d", (maxTime - elapsedSecondsI));
	}
	else {
		font_->RenderText(spriteRenderer, gef::Vector4(10.0f, 10.0f, -0.99f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Time Remaining: %d",(maxTime - elapsedSecondsI));
	}

	font_->RenderText(spriteRenderer, gef::Vector4(260.0f, 10.0f, -0.99f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Score: %d", score);
	
	DrawHUD(spriteRenderer, fps, font_);
	
	spriteRenderer->End();
}

/**
	Sets up basic lighting for the game.
*/
void game::SetupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	// add a point light that is almost white, but with a blue tinge
	// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);
}

/**
	Initilizes Player Object and body.
*/
void game::initPlayer()
{
	playerGuy = Player();
	// create a new scene object and read in the data from the file
	// no meshes or materials are created yet
	// we're not making any assumptions about what the data may be loaded in for
	model_scene_ = new gef::Scene();
	model_scene_->ReadSceneFromFile(plat, "Y_Bot.scn");

	// we do want to render the data stored in the scene file so lets create the materials from the material data present in the scene file
	model_scene_->CreateMaterials(plat);

	// now check to see if there is any mesh data in the file, if so lets create a mesh from it
	
	if (model_scene_->mesh_data.size() > 0)
	{
		mesh_ = model_scene_->CreateMesh(plat, model_scene_->mesh_data.front());

		// get the player mesh instance to use this mesh for drawing
		playerGuy.set_mesh(mesh_);
	}

	// check to see if there is a skeleton in the the scene file
	// if so, pull out the bind pose and create an array of matrices
	// that will be used to store the bone transformations
	if (model_scene_->skeletons.size() > 0)
	{
		skeleton_ = model_scene_->skeletons.front();
		//if (!skeleton_) {
			bind_pose_.CreateBindPose(skeleton_);
			bone_matrices_.resize(skeleton_->joints().size());
		//}
	}

	// anims

	// initialise the animation player with the bind pose
	anim_player_.Update(0.0f, bind_pose_);

	running_anim_ = LoadAnimation("running_InPlace.scn");
	idle_anim_ = LoadAnimation("idle.scn");

	// start with the character facing the right
	face_right_ = true;

	// create a physics body for the player
	b2BodyDef player_body_def;
	player_body_def.type = b2_dynamicBody;

	//player_body_def.type
	player_body_def.position = b2Vec2(-6.5f, -1.0f);
	playerGuyBody = world_->CreateBody(&player_body_def);

	// create the shape for the player
	b2PolygonShape player_shape;
	player_shape.SetAsBox(0.1f, 1.0f);

	// create the fixture
	b2FixtureDef player_fixture_def;
	player_fixture_def.shape = &player_shape;
	player_fixture_def.density = 1.0f;

	// create the fixture on the rigid body
	playerGuyBody->CreateFixture(&player_fixture_def);

	// update visuals from simulation data
	playerGuy.UpdateFromSimulation(playerGuyBody);

	// create a connection between the rigid body and GameObject
	playerGuyBody->SetUserData(&playerGuy);

	//playerGuy.setScale(0.5, 0.5);
}

/**
	Initilizes obejcts and bodys for the collectable Orbs.
*/
void game::initOrbs()
{
	ObjectInfo temp;
	setNumOfOrbs(orbAmount);

	for (int i = 0; i < orbAmount; i++) {
		Orbs[i] = Orb();
		Orbs[i].setid(i);
	
		temp.object = Orbs[i];
		temp.body = orbBodys[i];

		orbsInfo.push_back(temp);
	}

	mat = new gef::Material();
	mat->set_colour(0xFF00FFFF);

	float incr = 0.0f;

	for (std::vector<ObjectInfo>::iterator objectItr = orbsInfo.begin(); objectItr != orbsInfo.end(); ++objectItr) {
		// setup the mesh for the orbs
		objectItr->object.set_mesh(primitive_builder_->CreateSphereMesh(0.1f, 12, 24, gef::Vector4(0.0f, 0.0f, 0.0f), mat));

		// create a physics body for the orbs
		b2BodyDef orb_body_def;
		orb_body_def.type = b2_dynamicBody;
		orb_body_def.position = b2Vec2(-3.0f + incr, 5.0f);

		objectItr->body = world_->CreateBody(&orb_body_def);

		// create the shape for the orbs
		b2CircleShape orb_shape;
		orb_shape.m_radius = 0.1f;

		// create the fixture
		b2FixtureDef orb_fixture_def;
		orb_fixture_def.shape = &orb_shape;
		orb_fixture_def.density = 1.0f;

		// create the fixture on the rigid body
		objectItr->body->CreateFixture(&orb_fixture_def);

		// update visuals from simulation data
		objectItr->object.UpdateFromSimulation(objectItr->body);

		// create a connection between the rigid body and GameObject
		objectItr->body->SetUserData(&objectItr->object);

		incr += 1.0f;
	}
}

/**
	Initilizes Key object and body.
*/
void game::initKey()
{
	gef::Vector4 groundHalfDimensions(0.2f, 0.2f, 0.2f);
	level1Key.set_mesh(primitive_builder_->CreateBoxMesh(groundHalfDimensions, gef::Vector4(0.0f, 0.0f, 0.0f), NULL));

	// create a physics body for the orbs
	b2BodyDef key_body_def;
	key_body_def.type = b2_dynamicBody;
	key_body_def.position = b2Vec2(-6.6f, 8.0f);

	
	keyBody = world_->CreateBody(&key_body_def);

	// create the shape for the key
	b2PolygonShape shape;
	shape.SetAsBox(groundHalfDimensions.x(), groundHalfDimensions.y());

	// create the fixture
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;
	fixture_def.density = 1.0f;

	// create the fixture on the rigid body
	keyBody->CreateFixture(&fixture_def);

	// update visuals from simulation data
	level1Key.UpdateFromSimulation(keyBody);

	// create a connection between the rigid body and GameObject
	keyBody->SetUserData(&level1Key);
}

/**
	Update physics simulation for physics collisions.
	@param frame time.
	@param GEF AudioManager for playing sound effects on collisions.
*/
void game::UpdateSimulation(float frame_time, gef::AudioManager* audio_manager_)
{
	// update physics world
	float32 timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world_->Step(timeStep, velocityIterations, positionIterations);

	// update object visuals from simulation data
	playerGuy.UpdateFromSimulation(playerGuyBody);
	if (keyFound == false) {
		level1Key.UpdateFromSimulation(keyBody);
	}
	for (std::vector<ObjectInfo>::iterator objectItr = orbsInfo.begin(); objectItr != orbsInfo.end(); ++objectItr) {
		objectItr->object.UpdateFromSimulation(objectItr->body);
	}
	// don't have to update the ground visuals as it is static

	// collision detection
	// get the head of the contact list
	b2Contact* contact = world_->GetContactList();
	// get contact count
	int contact_count = world_->GetContactCount();

	for (int contact_num = 0; contact_num<contact_count; ++contact_num)
	{
		if (contact->IsTouching())
		{
			// get the colliding bodies
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			// DO COLLISION RESPONSE HERE
			Player* player = NULL;
			Orb* orb = NULL;
			Key* key = NULL;

			GameObject* gameObjectA = NULL;
			GameObject* gameObjectB = NULL;

			gameObjectA = (GameObject*)bodyA->GetUserData();
			gameObjectB = (GameObject*)bodyB->GetUserData();

			if (gameObjectA)
			{
				if (gameObjectA->type() == PLAYER)
				{
					player = (Player*)bodyA->GetUserData();
				}
			}

			if (gameObjectB)
			{
				if (gameObjectB->type() == PLAYER)
				{
					player = (Player*)bodyB->GetUserData();
				}
			}

			if (gameObjectA)
			{
				if (gameObjectA->type() == ORB)
				{
					orb = (Orb*)bodyA->GetUserData();
				}
			}

			if (gameObjectB)
			{
				if (gameObjectB->type() == ORB)
				{
					orb = (Orb*)bodyB->GetUserData();
				}
			}

			if (gameObjectA)
			{
				if (gameObjectA->type() == KEY)
				{
					key = (Key*)bodyA->GetUserData();
				}
			}

			if (gameObjectB)
			{
				if (gameObjectB->type() == KEY)
				{
					key = (Key*)bodyB->GetUserData();
				}
			}
			
			ObjectInfo temp;

			if (gameObjectA) {
				if (gameObjectB) {
					if (gameObjectA->type() == ORB && gameObjectB->type() == PLAYER){
						score += 5;
						temp.body = bodyB;
						temp.objptr = orb;
						toBeRemoved.push_back(temp);
						if (audio_manager_)
							audio_manager_->PlaySample(sfxOrbCollectID, false);
					}
					if (gameObjectA->type() == PLAYER && gameObjectB->type() == ORB) {
						score += 5;
						temp.body = bodyB;
						temp.objptr = orb;
						toBeRemoved.push_back(temp);
						if (audio_manager_)
							audio_manager_->PlaySample(sfxOrbCollectID, false);
					}
				}
			}
			if (gameObjectA) {
				if (gameObjectB) {
					if (gameObjectA->type() == PLAYER && gameObjectB->type() == KEY) { 
						if (audio_manager_) {
							if (!keyFound) {
								audio_manager_->PlaySample(sfxKeyCollectID, false);
								audio_manager_->PlaySample(sfxOpeningID, false);
							}
						}
						keyFound = true;
						gameWorld_.setKeyFound(true);
						
					}
					if (gameObjectA->type() == KEY && gameObjectB->type() == PLAYER) { 
						if (audio_manager_) {
							if (!keyFound) {
								audio_manager_->PlaySample(sfxKeyCollectID, false);
								audio_manager_->PlaySample(sfxOpeningID, false);
							}
						}
						keyFound = true;
						gameWorld_.setKeyFound(true);
					
					}
				}
			}
		}
		// Get next contact point
		contact = contact->GetNext();
	}
}

/**
	Check for inputs.
	@param GEF InputManager for controlling inputs.
	@param AudioManager for aiding clean up.
*/
void game::inputControl(gef::InputManager* inputManager, gef::AudioManager* audio_manager_)
{
	forward = 0.0f;
	const gef::SonyController* controller = inputManager->controller_input()->GetController(0);

	if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS)
	{
		if (!isJumping) {
			playerGuyBody->ApplyForceToCenter(b2Vec2(0.0f, 200.0f), true);
			isJumping = true;
		}

	}

	if (controller->buttons_pressed() & gef_SONY_CTRL_LEFT)
	{
		playerGuyBody->ApplyForceToCenter(b2Vec2(-5.0f, 0.0f), true);
		forward = -1.0f;
	}

	if (controller->buttons_down() & gef_SONY_CTRL_LEFT)
	{
		playerGuyBody->ApplyForceToCenter(b2Vec2(-2.5f, 0.0f), true);
		forward = -1.0f;
	}

	if (controller->buttons_pressed() & gef_SONY_CTRL_RIGHT)
	{
		playerGuyBody->ApplyForceToCenter(b2Vec2(5.0f, 0.0f), true);
		forward = 1.0f;
	}

	if (controller->buttons_down() & gef_SONY_CTRL_RIGHT)
	{
		playerGuyBody->ApplyForceToCenter(b2Vec2(2.5f, 0.0f), true);
		forward = 1.0f;
	}
}

/**
	Removes any objects from world that have been set for destruction. (Has Problems.)
*/
void game::removeFromWorld()
{
	for (std::vector<ObjectInfo>::iterator it = toBeRemoved.begin(); it != toBeRemoved.end(); ++it) {

		b2Body* dyingBody = it->body;
		GameObject* dyingObject = it->objptr;
		dyingBody->GetWorld()->DestroyBody(dyingBody);
		decrementNumOfOrbs();

		for (std::vector<ObjectInfo>::iterator itTest = orbsInfo.begin(); itTest != orbsInfo.end(); ++itTest) {
			if (itTest->object.getid() == dyingObject->getid()) {
				orbsInfo.erase(itTest);
				break;
			}
		}
	}
	toBeRemoved.clear();

	if (keyFound) {
		if (keyDestroyed == false) { 
			keyBody->GetWorld()->DestroyBody(keyBody);
			keyDestroyed = true;
		}
	}

}

/**
	Update function for controlling animation.
*/
void game::animationControl(float frame_time)
{
	//float forward = 0.0f;
	// store the animation used on the last frame so we can detect if
	// we need to trigger a animation change in this frame
	const gef::Animation* previous_anim = anim_player_.clip();

	// forward == 0.0f, character is in idle state
	// otherwise the character is moving left or right
	if (forward == 0.0f)
	{
		// check to see if we are transiting to idle from a different animation
		// if so start playing idle from the beginning
		if (previous_anim != idle_anim_)
		{
			anim_player_.set_clip(idle_anim_);
			anim_player_.set_looping(true);
			anim_player_.set_anim_time(0.0f);
		}
	}
	else
	{
		if (forward > 0.0f)
			face_right_ = true;
		else
			face_right_ = false;

		// if previous animation is different from running animation the
		// start playing the run animation from the start
		if (previous_anim != running_anim_)
		{
			anim_player_.set_clip(running_anim_);
			anim_player_.set_looping(true);
			anim_player_.set_anim_time(0.0f);
		}
	}

	if (skeleton_)
	{
		anim_player_.Update(frame_time, bind_pose_);

		// calculate bone matrices that need to be passed to the shader
		// this should be the final pose if multiple animations are blended together
		std::vector<gef::Matrix44>::const_iterator pose_matrix_iter = anim_player_.pose().global_pose().begin();
		std::vector<gef::Joint>::const_iterator joint_iter = skeleton_->joints().begin();

		for (std::vector<gef::Matrix44>::iterator bone_matrix_iter = bone_matrices_.begin(); bone_matrix_iter != bone_matrices_.end(); ++bone_matrix_iter, ++joint_iter, ++pose_matrix_iter)
			*bone_matrix_iter = (joint_iter->inv_bind_pose * *pose_matrix_iter);
	}

	// set the transformation matrix for the character based on the way they are facing
	if (face_right_)
		playerGuy.set_yRotation(gef::DegToRad(90.0f));
	else
		playerGuy.set_yRotation(gef::DegToRad(-90.0f));


}

/**
	loads and prepares animations
	@param filename for loading.
	@param Animation name.
	@return pointer to GEF Animation.
*/
gef::Animation* game::LoadAnimation(const char* anim_scene_filename, const char* anim_name)
{
	
	gef::Animation* anim = NULL;

	gef::Scene anim_scene;
	if (anim_scene.ReadSceneFromFile(plat, anim_scene_filename))
	{
		// if the animation name is specified then try and find the named anim
		// otherwise return the first animation if there is one
		std::map<gef::StringId, gef::Animation*>::const_iterator anim_node_iter;
		if (anim_name)
			anim_node_iter = anim_scene.animations.find(gef::GetStringId(anim_name));
		else
			anim_node_iter = anim_scene.animations.begin();

		if (anim_node_iter != anim_scene.animations.end())
			anim = new gef::Animation(*anim_node_iter->second);
	}

	return anim;
}
