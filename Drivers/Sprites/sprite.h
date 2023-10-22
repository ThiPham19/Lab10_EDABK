#ifndef __SPRITE_H
#define __SPRITE_H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

///////////////////////////////////
//defines
#define NUM_ENEMY		6
#define NUM_ENEMY_ROWS	1
#define NUM_ENEMY_COLS	6
#define ENEMY_IMAGE_PADDING   (uint16_t)2
#define Enemy_size_x	16
#define Enemy_size_y	16


#define PLAYER_DEFAULT_Y        55
#define PLAYER_IMAGE_PADDING   ((uint16_t)2)

#define NUM_MISSILE    8

#define SPRITE_MAX_X        128
#define SPRITE_MIN_X        0
#define SPRITE_MAX_Y        50
#define SPRITE_MIN_Y        0
////////////////////////////////

typedef enum
{
	SPRITE_DIRECTION_LEFT,
	SPRITE_DIRECTION_RIGHT,
}SpriteDirection_t;

typedef enum
{
	SPRITE_VERTICAL_DOWN,
	SPRITE_VERTICAL_UP,
}SpriteVerticalDirection_t;
//người chơi
typedef struct
{
//	uint8_t numLives;
	uint32_t x;
	uint32_t last_x;
	uint32_t y;
	uint32_t sizeX;
	uint32_t sizeY;
//	const ImageData* image;
}PlayerStruct;
//kẻ địch
typedef struct
{
	uint8_t life;
	uint32_t x;
	uint32_t last_x;
	uint32_t y;
	uint32_t last_y;
	uint32_t sizeX;
	uint32_t sizeY;
	uint16_t points;
    SpriteDirection_t horizDirection;
    SpriteVerticalDirection_t vertDirection;
//	const ImageData* image;
}EnemyStruct;
//tên lửa
typedef struct
{
	uint8_t life;               //active / not active
	uint32_t x;
	uint32_t last_x;
	uint32_t y;
	uint32_t last_y;
	uint32_t sizeX;
	uint32_t sizeY;
//	const ImageData* image;
}MissileStruct;


void Sprite_Init(void);

void Sprite_Player_Init(void);
void Sprite_Player_Move(uint32_t adc_value);
void Sprite_Player_Draw(void);
void Sprite_Player_Missle_Launch(void);
void Sprite_SetPlayerMissileLaunchFlag(void);
uint8_t Sprite_GetPlayerMissileLaunchFlag(void);
int Sprite_Player_GetNextMissile(void);
void Sprite_ClearPlayerMissileLaunchFlag(void);


void Sprite_Enemy_Init(void);
void Sprite_Enemy_Move(void);
void Sprite_Enemy_Draw(void);
void Sprite_Enemy_Missle_Launch(void);
int Sprite_Enemy_GetNextMissile(void);
int Sprite_GetRandomEnemy(void);

void Sprite_Missile_Init(void);
void Sprite_Missle_Move(void);
void Sprite_Missle_Draw(void);

int Sprite_GetNumEnemy(void);
int Sprite_Score_EnemyHit(uint8_t enemyIndex, uint8_t missileIndex);
int Sprite_Score_PlayerHit(uint8_t missileIndex);
uint8_t check_WIN_GAME(void);
#endif
