#include "gameOver.h"
#include <graphics/sprite.h>
#include "load_texture.h"
#include <input/sony_controller_input_manager.h>
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>

/**
	Game Over State constructor.
	@param calls parent constructer called to pass window through.
*/
gameOver::gameOver(gef::Platform& platform) : stateMachine(platform)
{
}


gameOver::~gameOver()
{
}

/**
	Initialize variables used by the Game Over State.
	@param audio manager for music/sound effects to be played.
*/
void gameOver::onLoad(gef::AudioManager * audio_manager_)
{
	xButtonIcon = CreateTextureFromPNG("playstation-cross-dark-icon.png", plat);
	oButtonIcon = CreateTextureFromPNG("playstation-circle-dark-icon.png", plat);
	gameOverIcon = CreateTextureFromPNG("GameOver.png", plat);
}

/**
	Clean up variables used by the Game Over State.
	Overidden.
	@param audio manager for music/sound effects to be cleaned up.
*/
void gameOver::onExit(gef::AudioManager * audio_manager_)
{
	delete xButtonIcon;
	xButtonIcon = NULL;

	delete oButtonIcon;
	oButtonIcon = NULL;

	delete gameOverIcon;
	gameOverIcon = NULL;
	
}

/**
	Update function called every frame.
	Overidden.
	@param Frame Time.
	@param GEF inputmanager for controlling input.
	@param Audio Manager for passing into on Exit Functions.
*/
void gameOver::update(float frame_time, gef::InputManager * inputManager, gef::AudioManager * audio_manager_)
{
	const gef::SonyController* controller = inputManager->controller_input()->GetController(0);

	if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS) {

		onExit(audio_manager_);
		stateChange = GAME;
		setStatebool(true);
	}

	if (controller->buttons_pressed() & gef_SONY_CTRL_CIRCLE) {

		onExit(audio_manager_);
		stateChange = MENU;
		setStatebool(true);
	}
}

/**
	Renders to the screen, called every frame.
	Overidden.
	@param fps if needing drawn to screen.
	@param GEF SpriteRenderer for rendering  2d object to screen.
	@param GEF Font for Font values.
*/
void gameOver::render(float fps, gef::SpriteRenderer * spriteRenderer, gef::Font * font_)
{
	spriteRenderer->Begin();

	gef::Sprite gameOverSprite;
	gameOverSprite.set_texture(gameOverIcon);
	gameOverSprite.set_position(gef::Vector4(plat.width()*0.5f, plat.height()*0.5f, -0.99f));
	gameOverSprite.set_height(80.0f);
	gameOverSprite.set_width(400.0f);
	spriteRenderer->DrawSprite(gameOverSprite);

	// render "PRESS" text
	font_->RenderText(
		spriteRenderer,
		gef::Vector4(plat.width()*0.33f, plat.height()*0.85f - 56.0f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"Press");

	// Render button icon
	gef::Sprite Xbutton;
	Xbutton.set_texture(xButtonIcon);
	Xbutton.set_position(gef::Vector4(plat.width()*0.33f, plat.height()*0.85f, -0.99f));
	Xbutton.set_height(32.0f);
	Xbutton.set_width(32.0f);
	spriteRenderer->DrawSprite(Xbutton);

	// render "TO RETRY" text
	font_->RenderText(
		spriteRenderer,
		gef::Vector4(plat.width()*0.33f, plat.height()*0.85f + 32.0f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"To Retry");

	// render "PRESS" text
	font_->RenderText(
		spriteRenderer,
		gef::Vector4(plat.width()*0.66f, plat.height()*0.85f - 56.0f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"Press");

	// Render button icon
	gef::Sprite Trianglebutton;
	Trianglebutton.set_texture(oButtonIcon);
	Trianglebutton.set_position(gef::Vector4(plat.width()*0.66f, plat.height()*0.85f, -0.99f));
	Trianglebutton.set_height(32.0f);
	Trianglebutton.set_width(32.0f);
	spriteRenderer->DrawSprite(Trianglebutton);

	// render "TO START" text
	font_->RenderText(
		spriteRenderer,
		gef::Vector4(plat.width()*0.66f, plat.height()*0.85f + 32.0f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"To Quit");

	spriteRenderer->End();
}
