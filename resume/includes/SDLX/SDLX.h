#ifndef SDLX_H
#define SDLX_H

/*
** Headers
*/

# include "SDL2/SDL.h"
# include "SDL2/SDL_image.h"
# include "SDL2/SDL_ttf.h"

# include "SDLX_const.h"
# include "SDLX_config.h"
# include "SDLX_ticks.h"

# include "SDLX_structs.h"

/*
** SDLX_Library Functions:
*/

void			SDLX_Start(SDLX_Display *dest);
void			SDLX_Close(void);

SDLX_Display	*SDLX_GetDisplay(void);

void	SDLX_MainSceneLoop(void *context_addr);
void	SDLX_MainSceneLoop_Post(void *context_addr);

void	SDLX_SceneContext_Init(SDLX_scene_cxt *context);
void	*SDLX_NewScene(size_t size, SDLX_scene_cxt *context, char *background_path, void *(close)(SDLX_scene_cxt *, void *), void *(update)(SDLX_scene_cxt *, void *));

/*
** Functions that get the screen ready for a redraw.
*/

void			SDLX_ScreenReset(SDL_Renderer *renderer, SDL_Color *bg_color);
void			SDLX_ScreenReset_Post(SDL_Renderer *renderer, SDL_Color *bg_color, SDLX_post_process *post_process);
SDLX_Sprite		*SDLX_GetBackground(void);
void			SDLX_SetBackground(SDLX_Sprite *src);
SDLX_Sprite		*SDLX_ClearBackground(void);
void			SDLX_Reset_PostProcess(SDLX_post_process *post_process);

SDL_Texture		*SDLX_CaptureScreen(SDLX_RenderQueue **Queues, size_t amount, SDL_bool reverse);

/*
** Functions that have to do with rendering the current frame.
*/

SDLX_Sprite		*SDLX_new_Sprite(SDLX_Sprite *dst);
void			SDLX_DrawAnimation(SDL_Renderer *renderer, SDLX_Sprite *animation);
void			SDLX_DrawAnimation_Direct(SDL_Renderer *renderer, SDLX_Sprite *animation);

int				SDLX_RenderQueue_Init(SDLX_RenderQueue *dest);
void			SDLX_RenderQueue_Add(SDLX_RenderQueue *dst, SDLX_Sprite *src);
void			SDLX_RenderQueue_Flush(SDLX_RenderQueue *queue, SDL_Renderer *renderer, SDL_bool reverse);
void			SDLX_RenderQueue_Flush_Direct(SDLX_RenderQueue *queue, SDL_Renderer *renderer, SDL_bool reverse);

void			SDLX_RenderQueue_Skip(SDLX_RenderQueue *queue, SDL_Renderer *renderer);

/*
** Input type functions.
*/

SDL_bool		SDLX_poll(void);

int				SDLX_AxisConvert(SDL_Point *axis);
void			SDLX_FillXbox_Axis(SDLX_GameInput *game_input, SDL_GameController *controller);
void			SDLX_JoyStick_toDPAD(SDL_Point axis, int *left, int *right, int *up, int *down);
void			SDLX_toDPAD(SDLX_GameInput *game_input, int set);
void			SDLX_toTriggers(SDLX_GameInput *game_input);
void			SDLX_GameInput_Mouse_Fill(SDLX_GameInput *dst, SDL_bool convert);

void			SDLX_record_input(SDLX_GameInput *from);
void			SDLX_Mouse_to_Screen(int *x, int *y);

SDL_GameController	*SDLX_XboxController_link(int player_no);

void			SDLX_KeyMap(SDLX_input_mapper *key_map, const Uint8 *keystate);
void			SDLX_ControllerMap(SDLX_input_mapper *key_map, SDL_GameController *controller);

/*
** Functions that deal with SDLX_Button
*/

int				SDLX_Button_Init(SDLX_button *dst, int (*sprite_fn)(SDLX_Sprite_Data **, int),
					int sprite_no, SDL_Rect placement, SDLX_RenderQueue *render_dst);

void			SDLX_Button_Focus(SDLX_button *button);
void			SDLX_Button_ReFocus(SDLX_button *button);
void			SDLX_Button_Lose_Focus(SDLX_button *button);
void			SDLX_Button_Update(SDLX_button *button);
void			SDLX_Button_Update_noDraw(SDLX_button *button);
void			SDLX_Style_Button(SDLX_button *button, int norm, int hover);
void			*SDLX_Button_NULL_fn(SDL_UNUSED SDLX_button *button, SDL_UNUSED void *meta, SDL_UNUSED size_t meta_length);
SDL_bool		SDLX_Button_onHoverFocus(SDLX_button *self, SDL_UNUSED void *meta, SDL_UNUSED size_t meta_length);
SDL_bool		SDLX_Button_onHoverFocus_Mobile(SDLX_button *self, SDL_UNUSED void *meta, SDL_UNUSED size_t meta_length);
void			SDLX_Button_Set_UDLR(SDLX_button *button, void *up, void *down, void *left, void *right);
void			SDLX_Button_Set_fn(SDLX_button *button,
								SDL_bool (*get_focus_fn)(struct SDLX_button *, void *, size_t),
								void *(*focus_fn)(struct SDLX_button *, void *, size_t),
								void *(*focus_once_fn)(struct SDLX_button *, void *, size_t),
								void *(*trigger_fn)(struct SDLX_button *, void *, size_t),
								void *(*update_fn)(struct SDLX_button *, void *, size_t));

/*
** Utility Functions.
*/

void	SDLX_free_null(void **ptr);

SDLX_direction	SDLX_reverse_dir(SDLX_direction direction);
SDL_Texture		*SDLX_LoadTexture(char *path);
SDLX_Sprite		SDLX_Sprite_Static(char *path);

SDL_Point		SDLX_RotatePoint(SDL_Point *point, double angle);

#define SDLX_MOUSE_MOVED (g_GameInput.GameInput.primary_delta.x != 0 || g_GameInput.GameInput.primary_delta.y != 0)

/*
** Collision Functions.
*/

int		SDLX_CollisionBucket_Init(SDLX_collision_bucket *dst, size_t type);
void	SDLX_CollisionBucket_add(SDLX_collision_bucket *dst, SDLX_collision *body);
void	SDLX_attempt_CollisionBucket(SDLX_collision *body, SDLX_collision_bucket *bucket);
void	SDLX_CollisionBucket_Flush(SDLX_collision_bucket *bucket);

SDL_bool	SDLX_Collide_RectToRect(SDLX_collision *hitbox1, SDLX_collision *hitbox2);
SDL_bool	SDLX_Collide_RectToARect(SDLX_collision *hitbox1, SDLX_collision *angle_box);
SDL_bool	SDLX_Collide_ARectToRect(SDLX_collision *angle_box, SDLX_collision *hitbox1);
SDL_bool	SDLX_Collide_RectToCircle(SDLX_collision *hitbox, SDLX_collision *circle);
SDL_bool	SDLX_Collide_CircleToRect(SDLX_collision *circle, SDLX_collision *hitbox);

/*
** Misc. Functions.
*/

double	SDLX_Degree_to_Radian(double degree);
double	SDLX_Radian_to_Degree(double degree);

#define SDLX_CALLOC_M(what, amount) (what = SDL_calloc(amount, sizeof(*(what))))

/*
** UtilityX Functions.
*/

void	SDLX_xIter(void *base, size_t nel, size_t width, void (*apply_fn)(void *));
void	*SDLX_xLowest(void *base, size_t nel, size_t width, int (cmp)(const void *, const void *));
void	*SDLX_xHighest(void *base, size_t nel, size_t width, int (cmp)(const void *, const void *));
void	*SDLX_xFirst(void *base, size_t nel, size_t width, SDL_bool (key_fn)(const void *));
void	*SDLX_xLast(void *base, size_t nel, size_t width, SDL_bool (key_fn)(const void *));

/*
** Dynamic Logic Functions.
*/

void	SDLX_load_xlogic(void);

void	SDLX_axm_NOP(int *loc, int val);
void	SDLX_axm_EQL(int *loc, int val);
void	SDLX_axm_ADD(int *loc, int val);
void	SDLX_axm_SUB(int *loc, int val);
void	SDLX_axm_MUL(int *loc, int val);
void	SDLX_axm_DIV(int *loc, int val);
void	SDLX_axm_MOD(int *loc, int val);
void	SDLX_axm_XOR(int *loc, int val);
void	SDLX_axm_AND(int *loc, int val);
void	SDLX_axm_OR(int *loc, int val);
void	SDLX_axm_LS(int *loc, int val);
void	SDLX_axm_RS(int *loc, int val);

#endif
