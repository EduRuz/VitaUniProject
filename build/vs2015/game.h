#pragma once
#include "stateMachine.h"
#include <audio/audio_manager.h>
#include <input/input_manager.h>
#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include <box2d/Box2D.h>
#include "game_object.h"
#include "gameWorld.h"
#include "camera.h"
#include <animation/skeleton.h>
#include "motion_clip_player.h"


//Struct to hold object and player information for safe control and deletion.
struct ObjectInfo {
	GameObject object;
	b2Body* body;
	GameObject* objptr;

};
class game :
	public stateMachine
{
public:
	game(gef::Platform& platform);

	void onLoad(gef::AudioManager* audio_manager_) override;
	void onExit(gef::AudioManager* audio_manager_) override;
	void update(float frame_time, gef::InputManager* inputManager, gef::AudioManager* audio_manager_) override;
	void render(float fps, gef::SpriteRenderer* spriteRenderer, gef::Font* font_) override;

	int getScore() { return score; }
	

private:

	void SetupLights();
	void initPlayer();
	void initOrbs();
	void initKey();

	void animationControl(float frame_time);
	gef::Animation* LoadAnimation(const char* anim_scene_file, const char* anim_name = NULL);

	
	int maxTime;
	float elapsedSecondsF;
	int elapsedSecondsI;
	int score;

	int numOfOrbs;
	int getNumofOrbs() { return numOfOrbs; }
	void setNumOfOrbs(int val) { numOfOrbs = val; }
	void decrementNumOfOrbs() { numOfOrbs--; }
	
	void UpdateSimulation(float frame_time, gef::AudioManager* audio_manager_);
	void inputControl(gef::InputManager* inputManager, gef::AudioManager* audio_manager_);
	void removeFromWorld();

	//
	// GAME DECLARATIONS
	//
	gef::Renderer3D* renderer_3d_;
	PrimitiveBuilder* primitive_builder_;

	// create the physics world
	b2World* world_;

	//Orb Variables
	const static int orbAmount = 32;
	Orb Orbs[orbAmount];
	b2Body* orbBodys[orbAmount];

	std::vector<ObjectInfo>	orbsInfo;
	gef::Material* mat;

	//Removal vector
	std::vector<ObjectInfo> toBeRemoved;

	//Guy variables
	Player playerGuy;
	b2Body* playerGuyBody;
	bool isJumping;

	class gef::Mesh* mesh_;
	const gef::Skeleton* skeleton_;
	std::vector<gef::Matrix44> bone_matrices_;
	gef::SkeletonPose bind_pose_;

	//Animation variables
	MotionClipPlayer anim_player_;
	gef::Animation* running_anim_;
	gef::Animation* idle_anim_;
	bool face_right_;
	float forward;

	//Key variables
	bool keyFound;
	bool keyDestroyed;
	Key level1Key;
	b2Body* keyBody;

	//Game World class
	gameWorld gameWorld_;

	camera cam;
	float32 movementSpeed;

	// audio variables
	int sfxOpeningID;
	int sfxKeyCollectID;
	int sfxOrbCollectID;

	gef::Scene* model_scene_;

	

	

	
};

