#include "optionsMenu.h"
#include <graphics/sprite.h>
#include "load_texture.h"
#include <input/sony_controller_input_manager.h>
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>

/**
	Options Menu State constructor.
	@param calls parent constructer called to pass window through.
*/
optionsMenu::optionsMenu(gef::Platform& platform) : stateMachine(platform)
{
}


optionsMenu::~optionsMenu()
{
}

/**
	Initialize variables used by Options Menu.
	@param audio manager for music/sound effects to be played.
*/
void optionsMenu::onLoad(gef::AudioManager * audio_manager_)
{
	xButtonIcon = CreateTextureFromPNG("playstation-cross-dark-icon.png", plat);
	oButtonIcon = CreateTextureFromPNG("playstation-circle-dark-icon.png", plat);
}

/**
	Clean up variables used by the Options Menu State.
	@param audio manager for music/sound effects to be cleaned up.
*/
void optionsMenu::onExit(gef::AudioManager * audio_manager_)
{
	delete xButtonIcon;
	xButtonIcon = NULL;

	delete oButtonIcon;
	oButtonIcon = NULL;
}

/**
	Update function called every frame.
	@param Frame Time.
	@param GEF inputmanager for controlling input.
	@param Audio Manager for passing into on Exit Functions.
*/
void optionsMenu::update(float frame_time, gef::InputManager * inputManager, gef::AudioManager * audio_manager_)
{
	const gef::SonyController* controller = inputManager->controller_input()->GetController(0);

	if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS) {
		if (getHardMode()) { sethardMode(false); }
		else { sethardMode(true); }
		onExit(audio_manager_);
		stateChange = MENU;
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
void optionsMenu::render(float fps, gef::SpriteRenderer * spriteRenderer, gef::Font * font_)
{

	spriteRenderer->Begin();

	gef::Sprite Xbutton;
	Xbutton.set_texture(xButtonIcon);
	Xbutton.set_position(gef::Vector4(plat.width()*0.5f, plat.height()*0.5f, -0.99f));
	Xbutton.set_height(32.0f);
	Xbutton.set_width(32.0f);
	spriteRenderer->DrawSprite(Xbutton);

	gef::Sprite circleButton;
	circleButton.set_texture(oButtonIcon);
	circleButton.set_position(gef::Vector4(plat.width()*0.90f, plat.height()*0.95f, -0.99f));
	circleButton.set_height(32.0f);
	circleButton.set_width(32.0f);
	spriteRenderer->DrawSprite(circleButton);

	// render "Back" text
	font_->RenderText(
		spriteRenderer,
		gef::Vector4(circleButton.position().x() + (circleButton.width() + 20.0f), circleButton.position().y() - (circleButton.height() * 0.5f), -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"back");

	if (hardMode) {

		// render "Hard Mode" text
		font_->RenderText(
			spriteRenderer,
			gef::Vector4(plat.width()*0.5f, plat.height()*0.5f + 32.0f, -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"Easy Mode?");

		// render "Too hard" text
		font_->RenderText(
			spriteRenderer,
			gef::Vector4(plat.width()*0.5f, plat.height()*0.5f + 60.0f, -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"Aww, was it too hard for you?");

	}
	else {
	
		// render "Hard Mode" text
		font_->RenderText(
			spriteRenderer,
			gef::Vector4(plat.width()*0.5f, plat.height()*0.5f + 32.0f, -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"Press for Hard Mode");

		// render "balls?" text
		font_->RenderText(
			spriteRenderer,
			gef::Vector4(plat.width()*0.5f, plat.height()*0.5f + 60.0f, -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"You got the balls?");
	}
	
	spriteRenderer->End();
}

