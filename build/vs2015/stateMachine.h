#pragma once
#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include <audio/audio_manager.h>
#include <input/input_manager.h>
#include <box2d/Box2D.h>
#include "game_object.h"

//enum for state control.
enum STATE {START, MENU, GAME, OPTIONS, GAMEOVER, GAMECOMPLETE};

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class Renderer3D;
	class Scene;
	class Skeleton;
	class Mesh;
}

class stateMachine
{
public:
	stateMachine(gef::Platform& platform);
	virtual ~stateMachine();
	
	virtual void onLoad(gef::AudioManager* audioManager);
	virtual void onExit(gef::AudioManager* audioManager);
	virtual void update(float frameTime, gef::InputManager* inputManager, gef::AudioManager* audioManager);
	virtual void render(float fps_, gef::SpriteRenderer* spriteRenderer, gef::Font* font_);

	STATE getState();
	void setState(STATE SC);
	bool getStatebool();
	void setStatebool(bool set);

	inline void sethardMode(bool flag) { hardMode = flag; }
	inline bool getHardMode() { return hardMode; }

	
protected:
	gef::Platform& plat;
	void DrawHUD(gef::SpriteRenderer* spriteRenderer, float fps_, gef::Font* font_);


	bool hardMode = false;
	STATE stateChange;
	bool hasChanged;

	gef::Texture* xButtonIcon;
	gef::Texture* triangleButtonIcon;
	gef::Texture* oButtonIcon;


private:
	

	
};

