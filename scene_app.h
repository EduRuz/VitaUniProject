#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include <audio/audio_manager.h>
#include <input/input_manager.h>
#include <box2d/Box2D.h>
#include "game_object.h"
#include "stateMachine.h"
#include "startScreen.h"
#include "mainMenu.h"
#include "optionsMenu.h"
#include "gameOver.h"
#include "gameComplete.h"
#include "game.h"


// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class Renderer3D;
}

class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);

private:
	void Init();
	void CleanUp();
	bool Update(float frameTime);
	void Render();
	void InitFont();
	void CleanUpFont();


    //GEF variables
	gef::SpriteRenderer* spriteRenderer;
	gef::Font* font_;
	gef::InputManager* inputManager;
	gef::AudioManager* audioManager;

	//Game State Variables
	STATE state;
	startScreen start;
	mainMenu mMenu;
	optionsMenu oMenu;
	game game;
	gameOver gameO;
	gameComplete gameC;
	stateMachine* currentState;

	float fps_;
};

#endif // _SCENE_APP_H
