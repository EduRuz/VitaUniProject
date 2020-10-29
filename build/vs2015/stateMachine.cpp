#include "stateMachine.h"
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
	stateMachine Constructor, sets pointer to platform variable passed in.
	@param The window to be worked with.
*/
stateMachine::stateMachine(gef::Platform& platform) :
	plat(platform)
{
}


stateMachine::~stateMachine()
{

}

void stateMachine::onLoad(gef::AudioManager* audioManager)
{

}

void stateMachine::onExit(gef::AudioManager* audioManager)
{

}

void stateMachine::update(float frameTime, gef::InputManager* inputManager, gef::AudioManager* audioManager)
{

}

void stateMachine::render(float fps, gef::SpriteRenderer* spriteRenderer, gef::Font* font_)
{

}

/**
	@return STATE enum value to indicate what state to change to next update.
*/
STATE stateMachine::getState()
{
	return stateChange;
}

/**
	@param set state to new state and get bool to true so update know to change state.
*/
void stateMachine::setState(STATE SC)
{
	stateChange = SC;
	setStatebool(true);
}

/**
	@return bool to control if state needs changed.
*/
bool stateMachine::getStatebool()
{
	return hasChanged;
}

/**
	@param set bool to tell update that state needs to change.
*/
void stateMachine::setStatebool(bool set)
{
	hasChanged = set;
}

/**
	Draw fps to screen above everything on screen.
	@param SpriteRenderer to draw with.
	@param fps to be drawn to screen.
	@param font to use.
*/
void stateMachine::DrawHUD(gef::SpriteRenderer* spriteRenderer, float fps_, gef::Font* font_)
{
	if (font_)
	{
		// display frame rate
		font_->RenderText(spriteRenderer, gef::Vector4(850.0f, 510.0f, -0.99f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}






