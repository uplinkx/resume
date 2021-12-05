/***************************************************************************
 * FILENAME:    SDLX_scenes.c
 * DESCRIPTION: Some default code for looping through scenes.
 *
 * ENVIRONMENT:
 *     macOS Catalina 10.15.7
 *     Visual Studio Code 1.56.2
 * AUTHORS:
 *     Kevin Colour
 * DATES:
 *     Created: 30Oct2021
***************************************************************************/

#include "SDLX.h"

void	*SDLX_NullScene(SDL_UNUSED SDLX_scene_cxt *context, SDL_UNUSED void *meta) { return (NULL); }

void	SDLX_MainSceneLoop(void *context_addr)
{
	SDLX_scene_cxt	*context;

	context = context_addr;
	if (context->shouldChange == SDL_TRUE)
	{
		context->init_fn(context, context->scene_meta);
		context->shouldChange = SDL_FALSE;
	}

	context->shouldQuit = SDLX_poll();

	SDLX_record_input(NULL);
	SDLX_KeyMap(&(g_GameInput.key_mapper), g_GameInput.keystate);
	SDLX_GameInput_Mouse_Fill(&(g_GameInput), SDL_TRUE);

	context->update_fn(context, context->scene_meta);

	if (context->shouldQuit != SDL_TRUE && SDLX_discrete_frames(NULL) != EXIT_FAILURE)
	{
		SDLX_RenderQueue_Flush(NULL, NULL, SDL_TRUE);
		SDLX_ScreenReset(SDLX_GetDisplay()->renderer, &(context->background_color));
	}

	if (context->shouldChange == SDL_TRUE)
	{
		SDLX_CollisionBucket_Flush(NULL);
		SDLX_RenderQueue_Flush(NULL, SDLX_GetDisplay()->renderer, SDL_FALSE);
		context->close_fn(context, context->scene_meta);
	}
}

void	SDLX_MainSceneLoop_Post(void *context_addr)
{
	SDLX_scene_cxt	*context;

	context = context_addr;
	if (context->shouldChange == SDL_TRUE)
	{
		context->init_fn(context, context->scene_meta);
		context->shouldChange = SDL_FALSE;
	}

	context->shouldQuit = SDLX_poll();
	SDLX_record_input(NULL);
	SDLX_KeyMap(&(g_GameInput.key_mapper), g_GameInput.keystate);
	SDLX_GameInput_Mouse_Fill(&(g_GameInput), SDL_TRUE);

#ifndef EMCC
	SDL_GameController	*controller;
	controller = NULL;
	controller = SDLX_XboxController_link(0);
	if (controller != NULL)
	{
		SDLX_ControllerMap(&(g_GameInput.pad_mapper), controller);
		SDLX_FillXbox_Axis(&(g_GameInput), controller);
	}
#endif

	context->update_fn(context, context->scene_meta);

	if (context->shouldQuit != SDL_TRUE && SDLX_discrete_frames(&(g_SDLX_Context.ticks_num2)) != EXIT_FAILURE)
	{
		SDLX_RenderQueue_Flush(NULL, NULL, SDL_FALSE);
		SDLX_ScreenReset_Post(SDLX_GetDisplay()->renderer, &(context->background_color), &(g_PostProcess));
	}

	if (context->shouldChange == SDL_TRUE)
	{
		SDLX_CollisionBucket_Flush(NULL);
		SDLX_Reset_PostProcess(&(g_PostProcess));
		SDLX_RenderQueue_Flush(NULL, SDLX_GetDisplay()->renderer, SDL_FALSE);

		context->close_fn(context, context->scene_meta);
	}
}

void	SDLX_SceneContext_Init(SDLX_scene_cxt *context)
{
	context->shouldQuit = SDL_TRUE;
	context->shouldChange = SDL_TRUE;

	context->def_font = NULL;
	context->def_font_outline = NULL;

	context->score_num1 = 0;
	context->ticks_num2 = 0;
	context->reset_num3 = 0;
	context->level_num4 = 0;

	context->extra_num5 = 0;
	context->extra_num6 = 0;
	context->extra_num7 = 0;
	context->extra_num8 = 0;

	context->scene_meta = NULL;

	context->init_fn = SDLX_NullScene;
	context->update_fn = SDLX_NullScene;
	context->close_fn = SDLX_NullScene;

	context->background_color = (SDL_Color){ 0, 0, 0, 0 };
	context->capture_texture = NULL;
}

void	*SDLX_NewScene(size_t size, SDLX_scene_cxt *context, char *background_path, void *(close)(SDLX_scene_cxt *, void *), void *(update)(SDLX_scene_cxt *, void *))
{
	void *result;

	result = SDL_calloc(1, size);

	SDL_assert(close != NULL);
	SDL_assert(update != NULL);

	context->close_fn = close;
	context->update_fn = update;
	context->scene_meta = result;
	context->shouldChange = SDL_FALSE;

	if (background_path != NULL)
	{
		context->background = SDLX_Sprite_Static(background_path);
		SDLX_SetBackground(&(context->background));
	}
	else
		SDLX_SetBackground(NULL);

	return (result);
}
