#include "startScreen.h"
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
	Start Screen State constructor.
	@param calls parent constructer called to pass window through.
*/
startScreen::startScreen(gef::Platform& platform) : stateMachine(platform) 
{
}

startScreen::~startScreen()
{
}

/**
	Initialize variables used by the Start Screen State.
	Overidden.
	@param audio manager for music/sound effects to be played.
*/
void startScreen::onLoad(gef::AudioManager* audioManager)
{

	xButtonIcon = CreateTextureFromPNG("playstation-cross-dark-icon.png", plat);
	backgroundIcon = CreateTextureFromPNG("StartScreenBackground.png", plat);
}

/**
	Clean up variables used by the Start Screen State.
	Overidden.
	@param audio manager for music/sound effects to be cleaned up.
*/
void startScreen::onExit(gef::AudioManager* audioManager)
{
	delete xButtonIcon;
	xButtonIcon = NULL;

	delete backgroundIcon;
	backgroundIcon = NULL;

}

/**
	Update function called every frame.
	Overidden.
	@param Frame Time.
	@param GEF inputmanager for controlling input.
	@param Audio Manager for passing into on Exit Functions.
*/
void startScreen::update(float frame_time, gef::InputManager* inputManager, gef::AudioManager* audioManager)
{

	const gef::SonyController* controller = inputManager->controller_input()->GetController(0);

	if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS) {

	onExit(audioManager);
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
void startScreen::render(float fps, gef::SpriteRenderer* spriteRenderer, gef::Font* font_)
{
	spriteRenderer->Begin();

	gef::Sprite background;
	background.set_texture(backgroundIcon);
	background.set_position(gef::Vector4(plat.width()*0.5f, plat.height()*0.5f, -0.99f));
	background.set_height(544.0f);
	background.set_width(960.0f);
	spriteRenderer->DrawSprite(background);
	
	// render "PRESS" text
	font_->RenderText(
		spriteRenderer,
		gef::Vector4(plat.width()*0.5f, plat.height()*0.85f - 56.0f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"PRESS");



	// Render button icon
	gef::Sprite button;
	button.set_texture(xButtonIcon);
	button.set_position(gef::Vector4(plat.width()*0.5f, plat.height()*0.85f, -0.99f));
	button.set_height(32.0f);
	button.set_width(32.0f);
	spriteRenderer->DrawSprite(button);



	// render "TO START" text
	font_->RenderText(
		spriteRenderer,
		gef::Vector4(plat.width()*0.5f, plat.height()*0.85f + 32.0f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"TO START");


	
	spriteRenderer->End();
}
