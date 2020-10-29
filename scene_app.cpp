/**
	Purpose: 2D Platformer Game goal is to find the key in the level to open the door to the next level
	while collecting as many orbs as possible within the time remaining to gain a higher score.
	Using GEF Framework.

	@author Ruaraidh Canaway
	@version 0.9 (Prototype) 03/05/18
*/
#include "scene_app.h"
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

/**
	Prepares the scene apps variables.
	@param The window to be worked with.
*/
SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	spriteRenderer(NULL),
	inputManager(NULL),
	audioManager(NULL),
	font_(NULL),
	start(platform_),
	mMenu(platform_),
	oMenu(platform_),
	gameO(platform_),
	gameC(platform_),
	game(platform_)
{
}

/**
	Initialize SceneApp Variables and sets state for game to begin at.
*/
void SceneApp::Init()
{
	spriteRenderer = gef::SpriteRenderer::Create(platform_);
	InitFont();

	// initialise input manager
	inputManager = gef::InputManager::Create(platform_);

	// initialise audio manager
	audioManager = gef::AudioManager::Create();

	// initialise states
	currentState = &start;
	state = START;

	currentState->setState(state);
	currentState->onLoad(audioManager);
	currentState->setStatebool(false);

	// load audio assets
	if (audioManager)
	{
		// load in music
		audioManager->LoadMusic("Audio/ShanhaiPassMenuVersion.wav", platform_);

		// play music
		audioManager->PlayMusic();
	}
}

/**
	cleans up variables and pointers that require clean up before program ends.
*/
void SceneApp::CleanUp()
{
	if (audioManager)
	{
		audioManager->StopMusic();
	}

	delete audioManager;
	audioManager = NULL;

	delete inputManager;
	inputManager = NULL;

	CleanUpFont();

	delete spriteRenderer;
	spriteRenderer = NULL;
	
}

/**
	Update function run every frame. 
	Controls what state the game is in.
	@param frame time.
*/
bool SceneApp::Update(float frameTime)
{
	fps_ = 1.0f / frameTime;

	inputManager->Update();
	
	if (currentState != NULL) {
		if (frameTime != NULL){
		currentState->update(frameTime, inputManager, audioManager);
		}
	}

	if (currentState->getStatebool()) {
		state = currentState->getState();
		currentState->setState(state);

		switch (state) {
		case START: {
			currentState = &start; 
			if (audioManager){audioManager->PlayMusic();} 
			} break;
		case MENU: {
			currentState = &mMenu; 
			mMenu.sethardMode(oMenu.getHardMode());
			} break;
		case GAME: {
			currentState = &game; 
			game.sethardMode(oMenu.getHardMode());
			if (audioManager) { audioManager->StopMusic(); }
			} break;
		case OPTIONS: {
			currentState = &oMenu;
			} break;
		case GAMEOVER: {
			currentState = &gameO;
			} break;
		case GAMECOMPLETE: {
			currentState = &gameC; gameC.setScore(game.getScore());
			} break;
		}
		
		currentState->setStatebool(false);
		currentState->onLoad(audioManager);

	}
	return true;
}

/**
	Calls the render function for the current state.
*/
void SceneApp::Render()
{
	currentState->render(fps_, spriteRenderer, font_);
	
}

/**
	Initialize the font.
*/
void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");

}

/**
	Clean up font.
*/
void SceneApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}