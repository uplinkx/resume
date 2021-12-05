/***************************************************************************
 * FILENAME:    input.c
 * DESCRIPTION:  Has the keymapper.
 *
 * ENVIRONMENT:
 *     macOS Catalina 10.15.7
 *     Visual Studio Code 1.56.2
 * AUTHORS:
 *     Kevin Colour
 * DATES:
 *     Created: 31Oct2021
***************************************************************************/

#include "main.h"

SDLX_iMap	*resume_key_map(size_t *size)
{
	static SDLX_iMap	key_map[] =
	{
		// (SDLX_iMap){&BMAP(button_START),	1, SDL_SCANCODE_KP_ENTER},
		// (SDLX_iMap){&BMAP(button_START),	1, SDL_SCANCODE_ESCAPE},

		// (SDLX_iMap){&BMAP(button_num1),	1, SDL_SCANCODE_1},
		// (SDLX_iMap){&BMAP(button_num2),	1, SDL_SCANCODE_2},
		// (SDLX_iMap){&BMAP(button_num3),	1, SDL_SCANCODE_3},
		// (SDLX_iMap){&BMAP(button_num4),	1, SDL_SCANCODE_4},

		(SDLX_iMap){&BMAP(button_DPAD_LEFT),	1, SDL_SCANCODE_LEFT},
		(SDLX_iMap){&BMAP(button_DPAD_RIGHT),	1, SDL_SCANCODE_RIGHT},
		(SDLX_iMap){&BMAP(button_DPAD_UP),		1, SDL_SCANCODE_UP},
		(SDLX_iMap){&BMAP(button_DPAD_DOWN),	1, SDL_SCANCODE_DOWN},

		(SDLX_iMap){&BMAP(button_DPAD_UP),		1, SDL_SCANCODE_W},
		(SDLX_iMap){&BMAP(button_DPAD_LEFT),	1, SDL_SCANCODE_A},
		(SDLX_iMap){&BMAP(button_DPAD_DOWN),	1, SDL_SCANCODE_S},
		(SDLX_iMap){&BMAP(button_DPAD_RIGHT),	1, SDL_SCANCODE_D},

		(SDLX_iMap){&BMAP(button_A),	1, SDL_SCANCODE_SPACE},

		(SDLX_iMap){&BMAP(button_RIGHTSHOULDER),	1, SDL_SCANCODE_O},
		(SDLX_iMap){&BMAP(button_LEFTSHOULDER),		1, SDL_SCANCODE_M},

		(SDLX_iMap){&BMAP(button_left_trigger),		1, SDL_SCANCODE_LSHIFT},
	};

	*size = sizeof(key_map) / sizeof(*key_map);
	return (&key_map[0]);
}

SDLX_iMap	*resume_pad_map(size_t *size)
{
	static SDLX_iMap	key_map[] =
	{
		(SDLX_iMap){&BMAP(button_num5),	1, SDL_CONTROLLER_BUTTON_DPAD_LEFT},
		(SDLX_iMap){&BMAP(button_num6),	1, SDL_CONTROLLER_BUTTON_DPAD_RIGHT},
		(SDLX_iMap){&BMAP(button_num7),	1, SDL_CONTROLLER_BUTTON_DPAD_UP},
		(SDLX_iMap){&BMAP(button_num8),	1, SDL_CONTROLLER_BUTTON_DPAD_DOWN},

		(SDLX_iMap){&BMAP(button_Y),	1, SDL_CONTROLLER_BUTTON_Y},
		(SDLX_iMap){&BMAP(button_X),	1, SDL_CONTROLLER_BUTTON_X},
		(SDLX_iMap){&BMAP(button_B),	1, SDL_CONTROLLER_BUTTON_B},
		(SDLX_iMap){&BMAP(button_A),	1, SDL_CONTROLLER_BUTTON_A},

		(SDLX_iMap){&BMAP(button_START),	1, SDL_CONTROLLER_BUTTON_START},

		(SDLX_iMap){&BMAP(button_LEFTSHOULDER),		1, SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
		(SDLX_iMap){&BMAP(button_RIGHTSHOULDER),	1, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},

	};

	*size = sizeof(key_map) / sizeof(*key_map);
	return (&key_map[0]);
}

void	resume_joystick_to_gameinput(void)
{
	SDLX_JoyStick_toDPAD(g_GameInput.GameInput.leftaxis,
		&(g_GameInput.GameInput.button_DPAD_LEFT), &(g_GameInput.GameInput.button_DPAD_RIGHT),
		&(g_GameInput.GameInput.button_DPAD_UP), &(g_GameInput.GameInput.button_DPAD_DOWN)
	);

	SDLX_JoyStick_toDPAD(g_GameInput.GameInput.rightaxis,
		&(g_GameInput.GameInput.button_num0), &(g_GameInput.GameInput.button_num1),
		&(g_GameInput.GameInput.button_num2), &(g_GameInput.GameInput.button_num3)
	);
}

void	keyboard_mouse_input(SDL_Rect *player)
{
	if (g_GameInput.GameInput.button_left_trigger == 0)
	{
		if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft))
			g_GameInput.GameInput.button_X = 1;
	}
	else
	{
		if (SDLX_GAME_PRESS(g_GameInput, g_GameInput_prev, primleft))
			g_GameInput.GameInput.button_B = 1;
	}

	g_GameInput.GameInput.leftaxis.x = (g_GameInput.GameInput.primary.x - player->x) * 1000;
	g_GameInput.GameInput.leftaxis.y = (g_GameInput.GameInput.primary.y - player->y) * 1000;
}