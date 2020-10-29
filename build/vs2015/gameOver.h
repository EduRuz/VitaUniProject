#pragma once
#include "stateMachine.h"
class gameOver :
	public stateMachine
{
public:
	gameOver(gef::Platform& platform);
	virtual ~gameOver();
	
	void onLoad(gef::AudioManager* audio_manager_) override;
	void onExit(gef::AudioManager* audio_manager_) override;
	void update(float frame_time, gef::InputManager* inputManager, gef::AudioManager* audio_manager_) override;
	void render(float fps, gef::SpriteRenderer* spriteRenderer, gef::Font* font_) override;

private:

	gef::Texture* gameOverIcon;

};

