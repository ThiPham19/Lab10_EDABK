/*
 * sprite.c
 *
 *  Created on: Oct 20, 2023
 *      Author: Dell
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "sprite.h"
#include "ssd1306.h"

#define speed_horizDirection 2//bước di chuyển ngang của địch, tiêu chuẩn là 2
#define speed_vertDirection 3//bước di chuyển dọc của địch, có thể thay đổi

PlayerStruct mPlayer;
static EnemyStruct mEnemy[NUM_ENEMY];
static MissileStruct mEnemyMissile[NUM_MISSILE];
static MissileStruct mPlayerMissile[NUM_MISSILE];

static uint8_t mGameOverFlag = 0;
static uint8_t mGameWinFlag = 0;
static uint8_t mPlayerMissileLaunchFlag;

static const uint8_t _acplayer1Bmp[] = //bmp người chơi
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x18, 0x00, 0x00, 0x3C, 0x00, 0x03, 0xFF, 0xC0, 0x0F, 0xFF,
0xF0, 0x3F, 0xFF, 0xFC, 0x7F, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF};

static const uint8_t _acenemy1Bmp[] = //bmp kẻ địch
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x20,
0x04, 0x40, 0x0F, 0xE0, 0x1B, 0xB0, 0x3F, 0xF8, 0x2F, 0xE8,
0x28, 0x28, 0x06, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00};

static const uint8_t _acmissile1Bmp[] = //bmp tên lửa
{
0x18, 0x18, 0x18, 0x0C, 0x0C, 0x0F, 0x18, 0x18};

void Sprite_Init(void)
{
    mPlayerMissileLaunchFlag = 0x00;
//    mGameScore = 0x00;
//    mGameLevel = 1;
    Sprite_Player_Init();
    Sprite_Enemy_Init();
    Sprite_Missile_Init();
//    Sprite_Drone_Init();
}

void Sprite_Player_Init(void)
{

    mPlayer.x = 0;
    mPlayer.y = PLAYER_DEFAULT_Y;
    mPlayer.sizeX = 24;
    mPlayer.sizeY = 10;
}

void Sprite_Player_Move(uint32_t adc_value)
{
    if (mPlayer.last_x != mPlayer.x){
    	mPlayer.last_x = mPlayer.x;
    }
	mPlayer.y = PLAYER_DEFAULT_Y;
	mPlayer.x = adc_value;
}

void Sprite_Player_Draw(void){
	  ssd1306_DrawBitmap(mPlayer.last_x, mPlayer.y, _acplayer1Bmp, 24, 10, Black);
	  ssd1306_DrawBitmap(mPlayer.x, mPlayer.y, _acplayer1Bmp, 24, 10, White);

}


void Sprite_Enemy_Init(void)
{
    uint8_t count = 0;
    for (int i = 0 ; i < NUM_ENEMY_ROWS ; i++)
    {
        for (int j = 0 ; j < NUM_ENEMY_COLS ; j++)
        {
            mEnemy[count].life = 1;
            mEnemy[count].points = 30;                              //points
            mEnemy[count].x = j * Enemy_size_x;//imageEnemy1.xSize;
            mEnemy[count].y = i * Enemy_size_y;//imageEnemy1.ySize;
        	mEnemy[count].last_x = 999;
        	mEnemy[count].last_y = 999;
            mEnemy[count].sizeX = Enemy_size_x;
            mEnemy[count].sizeY = Enemy_size_y;
            mEnemy[count].horizDirection = SPRITE_DIRECTION_LEFT;   //khởi tạo hướng trái
            mEnemy[count].vertDirection = SPRITE_VERTICAL_DOWN;     //khởi tạo hướng đi xuống dưới

            count++;
        }
    }
}

void Sprite_Enemy_Move(void)
{

    int i, j;
    unsigned char flag = 0;
    for (i = 0 ; i < NUM_ENEMY ; i++)
    {
        if (mEnemy[i].last_x != mEnemy[i].x)
        	mEnemy[i].last_x = mEnemy[i].x;
        if (mEnemy[i].last_y != mEnemy[i].y)
        	mEnemy[i].last_y = mEnemy[i].y;
    }

    for(i = 0 ; i < NUM_ENEMY ; i++)
    {
        //sang phải
        if (mEnemy[i].horizDirection == SPRITE_DIRECTION_RIGHT)
        {
            if(((mEnemy[i].x + mEnemy[i].sizeX) < SPRITE_MAX_X) && (mEnemy[i].life == 1))     //right edge
                mEnemy[i].x += speed_horizDirection;
        }

        //sang trái
        else
        {
            if((mEnemy[i].x > SPRITE_MIN_X) && (mEnemy[i].life == 1))     //left edge
                mEnemy[i].x -= speed_horizDirection;
        }
    }

    //kiểm tra đổi hướng - trái
    flag = 0;
    for (i = 0 ; i < NUM_ENEMY ; i++)
    {
        if (((mEnemy[i].x + mEnemy[i].sizeX) >= SPRITE_MAX_X) && (mEnemy[i].life == 1))
            flag = 1;
    }

    if (flag == 1)
    {
        for (j = 0 ; j < NUM_ENEMY ; j++)
        {
            mEnemy[j].horizDirection = SPRITE_DIRECTION_RIGHT;

            //move down on a direction change, if current
            //is moving down... continue moving down
            if (mEnemy[j].vertDirection == SPRITE_VERTICAL_DOWN)
            {
                if(((mEnemy[j].y + mEnemy[j].sizeY) < SPRITE_MAX_Y) && (mEnemy[j].life == 1))
                    mEnemy[j].y+= speed_vertDirection;
            }

            //move up on a direction change, if current
            //is moving up, continue moving up
            else
            {
                if((mEnemy[j].y > SPRITE_MIN_Y) && (mEnemy[j].life == 1))
                    mEnemy[j].y-= speed_vertDirection;
            }
        }
    }
    if (flag == 1)
    {
        for (j = 0 ; j < NUM_ENEMY ; j++)
            mEnemy[j].horizDirection = SPRITE_DIRECTION_LEFT;
    }

    //kiểm tra đổi hướng - phải
    flag = 0;
    for (i = 0 ; i < NUM_ENEMY ; i++)
    {
        if ((mEnemy[i].x <= SPRITE_MIN_X) && (mEnemy[i].life == 1))
            flag = 1;
    }

    if (flag == 1)
    {
        for (j = 0 ; j < NUM_ENEMY ; j++)
        {
            mEnemy[j].horizDirection = SPRITE_DIRECTION_RIGHT;

            //move down on a direction change, if current
            //is moving down... continue moving down
            if (mEnemy[j].vertDirection == SPRITE_VERTICAL_DOWN)
            {
                if(((mEnemy[j].y + mEnemy[j].sizeY) < SPRITE_MAX_Y) && (mEnemy[j].life == 1))
                    mEnemy[j].y+= speed_vertDirection;
            }

            //move up on a direction change, if current
            //is moving up, continue moving up
            else
            {
                if((mEnemy[j].y > SPRITE_MIN_Y) && (mEnemy[j].life == 1))
                    mEnemy[j].y-= speed_vertDirection;
            }
        }
    }

    //kiểm tra đổi hướng - lên
    flag = 0;
    for (i = 0 ; i < NUM_ENEMY ; i++)
    {
        if (((mEnemy[i].y + mEnemy[i].sizeY) >= SPRITE_MAX_Y) && (mEnemy[i].life == 1))
            flag = 1;
    }
    if (flag == 1)
    {
        for (j = 0 ; j < NUM_ENEMY ; j++)
            mEnemy[j].vertDirection = SPRITE_VERTICAL_UP;
    }

    //kiểm tra đổi hướng - xuống
    flag = 0;
    for (i = 0 ; i < NUM_ENEMY ; i++)
    {
        if ((mEnemy[i].y <= SPRITE_MIN_Y) && (mEnemy[i].life == 1))
            flag = 1;
    }
    if (flag == 1)
    {
        for (j = 0 ; j < NUM_ENEMY ; j++)
            mEnemy[j].vertDirection = SPRITE_VERTICAL_DOWN;
    }


}

void Sprite_Enemy_Draw(void)
{
    for (int i = 0 ; i < NUM_ENEMY ; i++)
    {
        if (mEnemy[i].life == 2)
        {
			ssd1306_DrawBitmap(mEnemy[i].last_x, mEnemy[i].last_y, _acenemy1Bmp, 16, 16, Black);
			mEnemy[i].life = 0;
        }
        if (mEnemy[i].life == 1)
        {
			ssd1306_DrawBitmap(mEnemy[i].last_x, mEnemy[i].last_y, _acenemy1Bmp, 16, 16, Black);
			ssd1306_DrawBitmap(mEnemy[i].x, mEnemy[i].y, _acenemy1Bmp, 16, 16, White);
        }
    }
}

void Sprite_Missle_Draw(void)
{
    for (int i = 0 ; i < NUM_MISSILE ; i++)
    {
        //kiểm tra tên lửa địch
        if (mEnemyMissile[i].life == 2){//trường hợp tên lửa ra ngoài màn
        	ssd1306_DrawBitmap(mEnemyMissile[i].last_x, mEnemyMissile[i].last_y, _acmissile1Bmp, 8, 8, Black);
        	mEnemyMissile[i].life = 0;
        }
        if (mEnemyMissile[i].life == 1){
        	ssd1306_DrawBitmap(mEnemyMissile[i].last_x, mEnemyMissile[i].last_y, _acmissile1Bmp, 8, 8, Black);
        	ssd1306_DrawBitmap(mEnemyMissile[i].x, mEnemyMissile[i].y, _acmissile1Bmp, 8, 8, White);
        }


        //kiểm tra tên lửa người chơi
        if (mPlayerMissile[i].life == 2){//trường hợp tên lửa ra ngoài màn
        	ssd1306_DrawBitmap(mPlayerMissile[i].last_x, mPlayerMissile[i].last_y, _acmissile1Bmp, 8, 8, Black);        	mPlayerMissile[i].life = 0;
        }
        if (mPlayerMissile[i].life == 1){
        	ssd1306_DrawBitmap(mPlayerMissile[i].last_x, mPlayerMissile[i].last_y, _acmissile1Bmp, 8, 8, Black);
        	ssd1306_DrawBitmap(mPlayerMissile[i].x, mPlayerMissile[i].y, _acmissile1Bmp, 8, 8, White);
        }
    }
}
void Sprite_Missile_Init(void)
{
    for (int i = 0 ; i < NUM_MISSILE ; i++)
    {
    	//khởi tạo tên lửa địch
        mEnemyMissile[i].x = 0;
        mEnemyMissile[i].last_x = 99;
        mEnemyMissile[i].y = 0;
        mEnemyMissile[i].last_y = 99;

        //khởi tạo tên người chơi
        mPlayerMissile[i].x = 0;
        mPlayerMissile[i].last_x = 99;
        mPlayerMissile[i].y = 0;
        mPlayerMissile[i].last_y = 99;
    }
}

void Sprite_Missle_Move(void)
{
    uint16_t mX, mY, bot, top, left, right = 0x00;

    for (int i = 0 ; i < NUM_MISSILE ; i++)
    {
    	/****************Lưu tọa độ cũ của tên lửa*************/
    	if (mPlayerMissile[i].life == 1) {
			if (mPlayerMissile[i].last_x != mPlayerMissile[i].x){
				mPlayerMissile[i].last_x = mPlayerMissile[i].x;}
			if (mPlayerMissile[i].last_y != mPlayerMissile[i].y){
				mPlayerMissile[i].last_y = mPlayerMissile[i].y;}
		}
    	if (mEnemyMissile[i].life == 1) {
			if (mEnemyMissile[i].last_x != mEnemyMissile[i].x){
				mEnemyMissile[i].last_x = mEnemyMissile[i].x;}
			if (mEnemyMissile[i].last_y != mEnemyMissile[i].y){
				mEnemyMissile[i].last_y = mEnemyMissile[i].y;}
		}
        /******************************************************/

    	/*****************TÊN LỬA CỦA NGƯỜI CHƠI**************/
    	//tên lửa người chơi còn "sống" và ở trong màn hình -> đi lên
        if ((mPlayerMissile[i].life == 1) && (mPlayerMissile[i].y > SPRITE_MIN_Y))
            mPlayerMissile[i].y-=2;

        //tên lửa người chơi còn "sống" ra ngoài màn hình
        if ((mPlayerMissile[i].life == 1) && (mPlayerMissile[i].y <= SPRITE_MIN_Y))
            mPlayerMissile[i].life = 0;

   //player missile hit an enemy?
        //Player missile hit the drone??

        //if the missile is alive...
        //for each live enemy, check the x and y position of the missle, top
        //center of the missile within the enemy box
        //
        //Add player missile hit the drone
        //
        if (mPlayerMissile[i].life == 1)
        {
        	//tên lửa người chơi bắn trúng kẻ địch
            for (int j = 0 ; j < NUM_ENEMY ; j++)
            {
                //kẻ địch còn sống
                if (mEnemy[j].life == 1)
                {
                    mX = mPlayerMissile[i].x + (mPlayerMissile[i].sizeX / 2);
                    mY = mPlayerMissile[i].y;
                    bot = mEnemy[j].y + mEnemy[j].sizeY - ENEMY_IMAGE_PADDING;
                    top = mEnemy[j].y + ENEMY_IMAGE_PADDING;
                    left = mEnemy[j].x + ENEMY_IMAGE_PADDING;
                    right = mEnemy[j].x + mEnemy[j].sizeX - ENEMY_IMAGE_PADDING;

                    //tính điểm cho bắn trúng - chưa làmc cập nhật điểm!!!
                    if ((mX >= left) && (mX <= right) && (mY <= bot) && (mY >= top))
                    {
                    	//bắn trúng cập nhật trạng thái tên lửa người chơi &&
                    	//kẻ địch bị bắn trúng
                        int rem = Sprite_Score_EnemyHit(j, i);

                        //nếu !rem, tất cả kẻ địch đã chết -> WIN
                        if (!rem)
                        {
                            //có thể tạo hàm bật flag_win_game
                        	mGameWinFlag = 1;
                        }
                    }
                }
            }
        }


        ///////////////////////////////////////////////////
        //tên lửa của địch tiếp tục đi xuống màn hình
        if ((mEnemyMissile[i].life == 1) && ((mEnemyMissile[i].y + mEnemyMissile[i].sizeY) < SPRITE_MAX_Y))
            mEnemyMissile[i].y+=2;

        //tên lửa của địch đi quá giới hạn màn hình
        if ((mEnemyMissile[i].life == 1) && ((mEnemyMissile[i].y + mEnemyMissile[i].sizeY) >= SPRITE_MAX_Y))
            mEnemyMissile[i].life = 2;//lưu = 2 để còn xóa vị trí cũ

        //tên lửa kẻ địch bắn trúng người chơi
        if (mEnemyMissile[i].life == 1)
        {
            mX = mEnemyMissile[i].x + (mEnemyMissile[i].sizeX / 2);
            mY = mEnemyMissile[i].y + mEnemyMissile[i].sizeY;

            bot = mPlayer.y + mPlayer.sizeY - PLAYER_IMAGE_PADDING;
            top = mPlayer.y + PLAYER_IMAGE_PADDING;
            left = mPlayer.x + PLAYER_IMAGE_PADDING;
            right = mPlayer.x + mPlayer.sizeX - PLAYER_IMAGE_PADDING;

            if ((mX >= left) && (mX <= right) && (mY <= bot) && (mY >= top))
            {
            	mGameOverFlag = 1;
            	//cần làm hàm trạng thái chết: tương tự trạng thái win
            	//nên test trước trạng thái win
            }
        }
    }
}

void Sprite_Player_Missle_Launch(void)
{
    int nextMissile = Sprite_Player_GetNextMissile();

    //tạo mới tên lửa người chơi trong mảng
    mPlayerMissile[nextMissile].life = 1;
    mPlayerMissile[nextMissile].x = mPlayer.x + (mPlayer.sizeX / 2) - (mPlayerMissile[nextMissile].sizeX / 2);
    mPlayerMissile[nextMissile].y = mPlayer.y - mPlayerMissile[nextMissile].sizeY;

    //bật âm thanh bắn (chưa lm đc)
    //Sound_Play_PlayerFire();
}

int Sprite_Player_GetNextMissile(void)
{
    for (int i = 0 ; i < NUM_MISSILE ; i++)
    {
        if (!(mPlayerMissile[i].life))
            return i;
    }
    return - 1;
}

void Sprite_Enemy_Missle_Launch(void)
{
    int nextMissile = Sprite_Enemy_GetNextMissile();    //next missile
    int index = Sprite_GetRandomEnemy();                //index of random enemy

    if (index >= 0)
    {
    	//tạo mới tên lửa kẻ địch trong mảng
        mEnemyMissile[nextMissile].life = 1;
        mEnemyMissile[nextMissile].x = mEnemy[index].x + (mEnemy[index].sizeX / 2) - (mEnemyMissile[nextMissile].sizeX / 2);
        mEnemyMissile[nextMissile].y = mEnemy[index].y + mEnemy[index].sizeY;
        //bật âm thanh bắn (chưa lm đc)
        //Sound_Play_EnemyFire();
    }

}

int Sprite_Enemy_GetNextMissile(void)
{
    for (int i = 0 ; i < NUM_MISSILE ; i++)
    {
        if (!(mEnemyMissile[i].life))
            return i;
    }
    return - 1;
}

int Sprite_GetRandomEnemy(void)	//hàm random cần fix lại bằng việc sử dụng ADC
								//vì bản chất MCU không có hàm rand()
{
    int numEnemy = Sprite_GetNumEnemy();    //number of enemy

    if (numEnemy > 0)
    {
        int index = rand() % (numEnemy);
        int counter = 0;

        for (int i = 0 ; i < NUM_ENEMY ; i++)
        {
            if (mEnemy[i].life == 1)
            {
                if (index == counter)
                    return counter;
                counter++;
            }
        }
    }
    return -1;
}

int Sprite_Score_EnemyHit(uint8_t enemyIndex, uint8_t missileIndex)
{
    //Sound_Play_EnemyExplode();                                      //play sound
    mEnemy[enemyIndex].life = 2; //lưu = 2 để còn xóa ảnh cũ                                   //remove enemy
    mEnemy[enemyIndex].horizDirection = SPRITE_DIRECTION_RIGHT;
    mEnemy[enemyIndex].vertDirection = SPRITE_VERTICAL_DOWN;

    mPlayerMissile[missileIndex].life = 2;//lưu = 2 để còn xóa ảnh cũ
    mPlayerMissile[missileIndex].x = 0;
    mPlayerMissile[missileIndex].y = 0;

    int remaining = Sprite_GetNumEnemy();

    return remaining;
}

int Sprite_GetNumEnemy(void)//lấy số lượng địch còn sống
{
    int num = 0;
    for (int i = 0 ; i < NUM_ENEMY ; i++)
    {
        if (mEnemy[i].life == 1)
            num++;
    }
    return num;
}

void Sprite_SetPlayerMissileLaunchFlag(void)
{
    mPlayerMissileLaunchFlag = 1;
}

uint8_t Sprite_GetPlayerMissileLaunchFlag(void)
{
    return mPlayerMissileLaunchFlag;
}

void Sprite_ClearPlayerMissileLaunchFlag(void)
{
    mPlayerMissileLaunchFlag = 0;
}

uint8_t check_WIN_GAME(void){
	return mGameWinFlag;
}

