#pragma once
#include "stateMachine.h"
#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include <audio/audio_manager.h>
#include <input/input_manager.h>
#include <box2d/Box2D.h>
#include "game_object.h"

class mainMenu :
	public stateMachine
{
public:
	mainMenu(gef::Platform& platform);
	virtual ~mainMenu();

	void onLoad(gef::AudioManager* audioManager) override;
	void onExit(gef::AudioManager* audioManager) override;
	void update(float frame_time, gef::InputManager* inputManager, gef::AudioManager* audioManager) override;
	void render(float fps, gef::SpriteRenderer* spriteRenderer, gef::Font* font_) override;

private:

	gef::Texture* leftDPadIcon;
	gef::Texture* rightDPadIcon;
};

