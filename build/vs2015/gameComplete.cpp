#include "gameComplete.h"
#include <graphics/sprite.h>
#include "load_texture.h"
#include <input/sony_controller_input_manager.h>
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>

/**
	Game Complete State constructor.
	@param calls parent constructer called to pass window through.
*/
gameComplete::gameComplete(gef::Platform& platform) : stateMachine(platform)
{
}


gameComplete::~gameComplete()
{
}

/**
	Initialize variables used by the Game Complete state.
	Overidden.
	@param audio manager for music/sound effects to be played.
*/
void gameComplete::onLoad(gef::AudioManager * audio_manager_)
{
	congrats = CreateTextureFromPNG("Congratulations.png", plat);
	level1CompleteTexture = CreateTextureFromPNG("Level1Complete.png", plat);
	xButtonIcon = CreateTextureFromPNG("playstation-cross-dark-icon.png", plat);
	goldStar = CreateTextureFromPNG("Star.png", plat);
	blackStar = CreateTextureFromPNG("BlackStar.png", plat);

}

/**
	Clean up variables used by the Game Complete State.
	Overidden.
	@param audio manager for music/sound effects to be cleaned up.
*/
void gameComplete::onExit(gef::AudioManager * audio_manager_)
{
	delete congrats;
	congrats = NULL;

	delete level1CompleteTexture;
	level1CompleteTexture = NULL;

	delete xButtonIcon;
	xButtonIcon = NULL;

	delete goldStar;
	goldStar = NULL;
}

/**
	Update function called every frame.
	Overidden.
	@param Frame Time.
	@param GEF inputmanager for controlling input.
	@param Audio Manager for passing into on Exit Functions.
*/
void gameComplete::update(float frame_time, gef::InputManager * inputManager, gef::AudioManager * audio_manager_)
{
	const gef::SonyController* controller = inputManager->controller_input()->GetController(0);

	if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS) {

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
void gameComplete::render(float fps, gef::SpriteRenderer * spriteRenderer, gef::Font * font_)
{
	spriteRenderer->Begin();

	gef::Sprite congratsSprite;
	congratsSprite.set_texture(congrats);
	congratsSprite.set_position(gef::Vector4(plat.width()*0.5f, plat.height()*0.1f, -0.99f));
	congratsSprite.set_height(80.0f);
	congratsSprite.set_width(400.0f);
	spriteRenderer->DrawSprite(congratsSprite);

	gef::Sprite completeSprite;
	completeSprite.set_texture(level1CompleteTexture);
	completeSprite.set_position(gef::Vector4(plat.width()*0.5f, ((plat.height()*0.1f) + (congratsSprite.height() * 1.0f)), -0.99f));
	completeSprite.set_height(80.0f);
	completeSprite.set_width(400.0f);
	spriteRenderer->DrawSprite(completeSprite);

	// Render button icon
	gef::Sprite starIcon;
	starIcon.set_texture(goldStar);
	starIcon.set_position(gef::Vector4(plat.width()*0.25f, plat.height()*0.5f, -0.99f));
	starIcon.set_height(80.0f);
	starIcon.set_width(80.0f);
	
	if (score > 60) { 
		spriteRenderer->DrawSprite(starIcon); 
	}
	else { 
		starIcon.set_texture(blackStar); spriteRenderer->DrawSprite(starIcon); 
	}
	
	starIcon.set_position(gef::Vector4(plat.width()*0.5f, plat.height()*0.5f, -0.99f));
	
	if (score > 100) { 
		spriteRenderer->DrawSprite(starIcon);
	}
	else { 
		starIcon.set_texture(blackStar); spriteRenderer->DrawSprite(starIcon); 
	}

	starIcon.set_position(gef::Vector4(plat.width()*0.75f, plat.height()*0.5f, -0.99f));

	if (score > 140) { 
		spriteRenderer->DrawSprite(starIcon); 
	}
	else { 
		starIcon.set_texture(blackStar); spriteRenderer->DrawSprite(starIcon); 
	}
	

	// render "Score" text
	font_->RenderText(
		spriteRenderer,
		gef::Vector4(plat.width()*0.5f, plat.height()*0.65f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"Score: %d Points", score);

	// render "PRESS" text
	font_->RenderText(
		spriteRenderer,
		gef::Vector4(plat.width()*0.5f, plat.height()*0.85f - 56.0f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"Press");

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
		"To Continue");

	spriteRenderer->End();
}
