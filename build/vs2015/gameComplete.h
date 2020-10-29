#pragma once
#include "stateMachine.h"
class gameComplete :
	public stateMachine
{
public:
	gameComplete(gef::Platform& platform);
	~gameComplete();

	void onLoad(gef::AudioManager* audio_manager_) override;
	void onExit(gef::AudioManager* audio_manager_) override;
	void update(float frame_time, gef::InputManager* inputManager, gef::AudioManager* audio_manager_) override;
	void render(float fps, gef::SpriteRenderer* spriteRenderer, gef::Font* font_) override;

	void setScore(int scr) { score = scr; }
private:
	gef::Texture* level1CompleteTexture;
	gef::Texture* congrats;
	gef::Texture* goldStar;
	gef::Texture* blackStar;

	int score;
};

