/***************************************************************************
 * FILENAME:    skullball.c
 * DESCRIPTION: Functionality for the skullball entity
 *
 * ENVIRONMENT:
 *     macOS Catalina 10.15.7
 *     Visual Studio Code 1.56.2
 * AUTHORS:
 *     Kevin Colour
 * DATES:
 *     Created: 18Nov2021
***************************************************************************/

#include "main.h"

typedef struct	s_skullball
{
	int			vel_x;
	int			vel_y;

	SDLX_Sprite	flames;
	SDLX_collision	collision;

	int			score_timer;
	int			score;

	int			heart_cooldown;
}				t_skullball;

SDL_bool	skullball_detect_collide(void *self, void *with, SDL_UNUSED void *data, SDL_UNUSED void *data2)
{
	SDL_bool		result;
	SDLX_collision	*hitbox;
	t_entity		*pickup;
	t_skullball		*skullball;

	hitbox = with;
	pickup = self;
	result = SDL_FALSE;
	skullball = pickup->meta;

	// SDL_SetRenderDrawColor(SDLX_GetDisplay()->renderer, 0, 255, 0, 255);
	// SDL_RenderDrawRect(SDLX_GetDisplay()->renderer, &(hitbox->hitbox));
	// SDL_RenderDrawRect(SDLX_GetDisplay()->renderer, &(pot->collision.hitbox));
	if (hitbox->type == C_MELEE)
	{
		if (SDL_HasIntersection(&(skullball->collision.hitbox), &(hitbox->hitbox)) == SDL_TRUE)
			result = SDL_TRUE;
	}
	return (result);
}

void	*skullball_engage_collision(void *self, void *with, SDL_UNUSED void *data, SDL_UNUSED void *data2)
{
	t_entity		*pickup;
	t_skullball		*skullball;
	t_player		*player;
	SDLX_collision	*hitbox;

	double			angle;
	int				x, y;

	hitbox = with;
	pickup = self;
	skullball = pickup->meta;
	player = g_SDLX_Context.meta1;
	if (hitbox->type == C_MELEE)
	{
		x = pickup->world_x + 2;
		y = pickup->world_y + 2;
		angle = SDL_atan2(pickup->sprite._dst.y - player->sprite._dst.y - 8, pickup->sprite._dst.x - player->sprite._dst.x - 8);

		if (skullball->heart_cooldown >= 30)
		{
			init_red_heart_pickup(spawn_entity_addr(g_SDLX_Context.meta2, ET_DROPS),  x + SDL_cos(angle) * 25, y + SDL_sin(angle) * 25);
			skullball->heart_cooldown = 0;
		}

		skullball->vel_x = cos(angle) * 8;
		skullball->vel_y = sin(angle) * 8;
	}
	return (NULL);
}

void	skullball_update(t_entity *self, int world_x, int world_y)
{
	t_skullball *skullball;

	skullball = self->meta;
	skullball->vel_x = SDL_max(-4, SDL_min(skullball->vel_x, 4));
	skullball->vel_y = SDL_max(-4, SDL_min(skullball->vel_y, 4));

	if (self->world_y < 48) { skullball->vel_y *= -1; }
	if (self->world_y > 320) { skullball->vel_y *= -1; }
	if (self->world_x < 64) { skullball->vel_x *= -1; }
	if (self->world_x > 416) { skullball->vel_x *= -1; }

	self->world_x += skullball->vel_x;
	self->world_y += skullball->vel_y;

	world_align(&(self->sprite), self->world_x, self->world_y, world_x, world_y, 50);
	world_align(&(skullball->flames), self->world_x, self->world_y, world_x, world_y, 49);

	SDLX_RenderQueue_Add(NULL, &(self->sprite));
	SDLX_RenderQueue_Add(NULL, &(skullball->flames));
	skullball->flames.current++;

	skullball->collision.hitbox = self->sprite._dst;
	SDLX_CollisionBucket_add(NULL, &(skullball->collision));

	SDL_bool		scored;
	SDL_Point		center;
	SDL_Rect		goal;

	scored = SDL_FALSE;
	if (skullball->score_timer >= 80)
	{
		center.x = self->world_x + 8;
		center.y = self->world_y + 8;
		goal = (SDL_Rect){80, 64, 32, 36};
		if (SDL_PointInRect(&(center), &(goal)) == SDL_TRUE)
		{
			scored = SDL_TRUE;
			skullball->score -= 1;
		}
		goal = (SDL_Rect){80, 288, 32, 36};
		if (SDL_PointInRect(&(center), &(goal)) == SDL_TRUE)
		{
			scored = SDL_TRUE;
			skullball->score -= 1;
		}


		goal = (SDL_Rect){384, 64, 32, 36};
		if (SDL_PointInRect(&(center), &(goal)) == SDL_TRUE)
		{
			scored = SDL_TRUE;
			skullball->score += 1;
		}
		goal = (SDL_Rect){384, 288, 32, 36};
		if (SDL_PointInRect(&(center), &(goal)) == SDL_TRUE)
		{
			scored = SDL_TRUE;
			skullball->score += 1;
		}

		if (scored == SDL_TRUE)
			skullball->score_timer = 0;
	}

	if (g_SDLX_Context.ticks_num2 % 50 == 0)
		SDL_Log("Current Score: %d", skullball->score);

	skullball->score_timer++;
	skullball->heart_cooldown++;
}

void	skullball_init(t_entity *entity, int world_x, int world_y)
{
	t_skullball *skullball;

	entity->isActive = SDL_TRUE;
	entity->meta = entity->alloc;
	fetch_skullball_sprite(&(entity->sprite.sprite_data), 0);
	entity->sprite.dst = SDLX_NULL_SELF;
	entity->sprite._dst.h = 16;
	entity->sprite._dst.w = 16;
	entity->world_x = world_x;
	entity->world_y = world_y;
	entity->update = skullball_update;

	skullball = entity->meta;
	fetch_skullball_sprite(&(skullball->flames.sprite_data), 1);
	skullball->flames.dst = SDLX_NULL_SELF;
	skullball->flames._dst.h = 16;
	skullball->flames._dst.w = 16;

	skullball->collision.hitbox_ptr = &(skullball->collision.hitbox);

	skullball->collision.detect = skullball_detect_collide;
	skullball->collision.engage = skullball_engage_collision;
	skullball->collision.originator = entity;

	skullball->score = 0;
	skullball->score_timer = 80;

	SDL_assert(sizeof(entity->alloc) >= sizeof(t_skullball));
}