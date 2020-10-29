#include "mainMenu.h"
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
	Main Menu State constructor.
	@param calls parent constructer called to pass window through.
*/
mainMenu::mainMenu(gef::Platform& platform) : stateMachine(platform)
{

}


mainMenu::~mainMenu()
{

}

/**
	Initialize variables used by Main Menu.
	Overidden.
	@param audio manager for music/sound effects to be played.
*/
void mainMenu::onLoad(gef::AudioManager* audioManager)
{
	xButtonIcon = CreateTextureFromPNG("playstation-cross-dark-icon.png", plat);
	triangleButtonIcon = CreateTextureFromPNG("playstation-triangle-dark-icon.png", plat);
	oButtonIcon = CreateTextureFromPNG("playstation-circle-dark-icon.png", plat);
	leftDPadIcon = CreateTextureFromPNG("PlayStationDirectionalPadLeft.png", plat);
	rightDPadIcon = CreateTextureFromPNG("PlayStationDirectionalPadRight.png", plat);
}

/**
	Clean up variables used by the Start Screen State.
	Overidden.
	@param audio manager for music/sound effects to be cleaned up.
*/
void mainMenu::onExit(gef::AudioManager* audioManager)
{
	delete xButtonIcon;
	xButtonIcon = NULL;

	delete triangleButtonIcon;
	triangleButtonIcon = NULL;

	delete oButtonIcon;
	oButtonIcon = NULL;

	delete leftDPadIcon;
	leftDPadIcon = NULL;

	delete rightDPadIcon;
	rightDPadIcon = NULL;

}

/**
	Update function called every frame.
	Overidden.
	@param Frame Time.
	@param GEF inputmanager for controlling input.
	@param Audio Manager for passing into on Exit Functions.
*/
void mainMenu::update(float frame_time, gef::InputManager* inputManager, gef::AudioManager* audioManager)
{

	const gef::SonyController* controller = inputManager->controller_input()->GetController(0);

	if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS) {
	
		onExit(audioManager);
		stateChange = GAME;
		setStatebool(true);
	}

	if (controller->buttons_pressed() & gef_SONY_CTRL_TRIANGLE) {

		onExit(audioManager);
		stateChange = OPTIONS;
		setStatebool(true);
	}

	if (controller->buttons_pressed() & gef_SONY_CTRL_CIRCLE) {

		onExit(audioManager);
		stateChange = START;
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
void mainMenu::render(float fps, gef::SpriteRenderer* spriteRenderer, gef::Font* font_)
{
	spriteRenderer->Begin();

	// Render cross button icon
	gef::Sprite Xbutton;
	Xbutton.set_texture(xButtonIcon);
	Xbutton.set_position(gef::Vector4(plat.width()*0.05f, plat.height()*0.75f, -0.99f));
	Xbutton.set_height(32.0f);
	Xbutton.set_width(32.0f);
	spriteRenderer->DrawSprite(Xbutton);

	// Render triangle button icon
	gef::Sprite triangleButton;
	triangleButton.set_texture(triangleButtonIcon);
	triangleButton.set_position(gef::Vector4(plat.width()*0.10f, plat.height()*0.85f, -0.99f));
	triangleButton.set_height(32.0f);
	triangleButton.set_width(32.0f);
	spriteRenderer->DrawSprite(triangleButton);


	// Render circle button icon
	gef::Sprite circleButton;
	circleButton.set_texture(oButtonIcon);
	circleButton.set_position(gef::Vector4(plat.width()*0.15f, plat.height()*0.95f, -0.99f));
	circleButton.set_height(32.0f);
	circleButton.set_width(32.0f);
	spriteRenderer->DrawSprite(circleButton);

	// Render left d pad icon
	gef::Sprite LeftDPadButton;
	LeftDPadButton.set_texture(leftDPadIcon);
	LeftDPadButton.set_position(gef::Vector4(plat.width()*0.05f, plat.height()*0.25f, -0.99f));
	LeftDPadButton.set_height(32.0f);
	LeftDPadButton.set_width(32.0f);
	spriteRenderer->DrawSprite(LeftDPadButton);

	// Render right d pad icon
	gef::Sprite rightDPadButton;
	rightDPadButton.set_texture(rightDPadIcon);
	rightDPadButton.set_position(gef::Vector4(plat.width()*0.05f + 64.0f, plat.height()*0.25f, -0.99f));
	rightDPadButton.set_height(32.0f);
	rightDPadButton.set_width(32.0f);
	spriteRenderer->DrawSprite(rightDPadButton);

	Xbutton.set_position(gef::Vector4(plat.width()*0.05f + 32.0f, plat.height()*0.25f + 64.0f, -0.99f));
	spriteRenderer->DrawSprite(Xbutton);

	// render "Controls" text
	font_->RenderText(
		spriteRenderer,
		gef::Vector4(plat.width()*0.05f + 32.0f, plat.height()*0.10f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"Controls");

	// render "Left & Right to Move" text
	font_->RenderText(
		spriteRenderer,
		gef::Vector4(plat.width()*0.30f, plat.height()*0.22f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"Left & Right to Move");

	// render "X to Jump" text
	font_->RenderText(
		spriteRenderer,
		gef::Vector4(plat.width()*0.25f, plat.height()*0.35f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"X to Jump");

	if (getHardMode()) {
		// render "Play Game" text
		font_->RenderText(
			spriteRenderer,
			gef::Vector4(plat.width()*0.05f + 126.0f, plat.height()*0.75f - 16.0f, -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"Play Game (Hard)");
	}
	else {
	
		font_->RenderText(
			spriteRenderer,
			gef::Vector4(plat.width()*0.05f + 126.0f, plat.height()*0.75f - 16.0f, -0.99f),
			1.0f,
			0xffffffff,
			gef::TJ_CENTRE,
			"Play Game (Easy)");
	
	
	}
	// render "Options" text
	font_->RenderText(
		spriteRenderer,
		gef::Vector4(plat.width()*0.10f + 96.0f, plat.height()*0.85f - 16.0f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"Options");

	// render "Back" text
	font_->RenderText(
		spriteRenderer,
		gef::Vector4(plat.width()*0.15f + 96.0f, plat.height()*0.95f - 16.0f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"back");


	
	spriteRenderer->End();
}
