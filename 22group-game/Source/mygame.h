/*
 * mygame.h: ���ɮ��x�C��������class��interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
*/

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID {				// �w�q�U�ح��Ī��s��
	AUDIO_DING,				// 0
	AUDIO_RUN ,				// 1
	AUDIO_ATTACK,
	AUDIO_ORANGEATTACK,
	AUDIO_DIE,
	AUDIO_OVERSOUND	,
	AUDIO_RECIPROCAL_FIVE,
	AUDIO_RECIPROCAL_FOUR,
	AUDIO_RECIPROCAL_THREE,
	AUDIO_RECIPROCAL_TWO,
	AUDIO_RECIPROCAL_ONE,
	AUDIO_CHANGEHEROSOUND,
	AUDIO_CURE,
	AUDIO_STARTSOUND
};

namespace game_framework {

class CBall;
class GameMap   //�a��
{
public:
	GameMap();
	int  GetX1();			  	     // ���W�� x �y��
	int  GetY1();					 // ���W�� y �y��
	int  GetX2();					 // �k�U�� x �y��
	int  GetY2();					 // �k�U�� y �y��
	void SetX1(int setx);			 // �]�w���W�� x �y��
	void SetY1(int sety);			 // �]�w���W�� y �y��
	void Initialize();				 // �]�w��l��
	void LoadBitmap();			 	 // ���J�ϧ�
	void OnMove(CBall *Monseter);	 // ����
	void OnShow();					 // �N�ϧζK��e��
	void SetMovingDown(bool flag);   // �]�w�O�_���b���U����
	void SetMovingLeft(bool flag);	 // �]�w�O�_���b��������
	void SetMovingRight(bool flag);  // �]�w�O�_���b���k����
	void SetMovingUp(bool flag);	 // �]�w�O�_���b���W����
	void SetXY(int nx, int ny);		 // �]�wXY�y��
	void SetNewFloor(int sfloor);    //Jump��
	void SetMonsterFloor(int sfloor);//�Ǫ�����
	void SetNumber(int number);      //�Ǫ��ƥ�
	void SetHinder(bool flag);       //��ê
	void SetUp(bool flag);           //�]�w���W
	void SetDown(bool flag);         //�]�w���U
	//-----------------------------------------------------jump
	void SetJumping(bool flag);	     // �]�w�O�_���b���W����
	void OnJump(CBall *Monseter);	 // ����
	//////////////////////////////��ê///////////////////////////////////////
	bool IsEmpty(int X, int Y);
protected:
	int x, y;					// ���l���W���y��
	int numbers;  //�Ǫ��ƶq
	bool isMovingDown;			// �O�_���b���U����
	bool isMovingLeft;			// �O�_���b��������
	bool isMovingRight;			// �O�_���b���k����
	bool isMovingUp;			// �O�_���b���W����

	bool isHinder;
	bool isUp;
	bool isDown;
private://-----------------------------------------------------jump
	int floor;
	int monsterfloor;
	bool rising;
	int initial_velocity;	// ��l�t��
	int velocity;			// �ثe���t��(�I/��)
	CAnimation animation;	// �Q�ΰʵe�@�ϧ�
	bool isJumping;

	//////////////////////////////��ê///////////////////////////////////////
	CMovingBitmap blue;
	int map[24][27];
	int sx, sy;             //�ù�
    int MW, MH;
	int foolvelocity;


};

class CEraser   //�D��
{
public:
	CEraser();
	bool AttackMonsterIsSuccess(CBall monster);						// �O�_�I�����l
	int  GetX1();					// ���l���W�� x �y��
	int  GetY1();					// ���l���W�� y �y��
	int  GetX2();					// ���l�k�U�� x �y��
	int  GetY2();					// ���l�k�U�� y �y��
	int  GetshowX1();					// ���l���W�� x �y��
	int  GetshowY1();					// ���l���W�� x �y��
	int  GetshowX2();					// ���l���W�� x �y��
	int  GetshowY2();					// ���l���W�� x �y��
	int  GetChangeCount();
	void Initialize();				// �]�w���l����l��
	void LoadBitmap();				// ���J�ϧ�
	void OnMove(GameMap *m);					// �tjump
	void OnShow(int life, CBall *monster);					// �N���l�ϧζK��e��GameMap *m
	void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
	void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
	void SetMovingRight(bool flag); // �]�w�O�_���b���k����
	void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
	void SetMoveState(bool flag);
	void SetXY(int nx, int ny);		// �]�w���l���W���y��
	void SetFighting(bool flag);
	void SetNumber(int number);
	void SetChangeCount(int count);
protected:
	CAnimation animation;		// ����(��)���ʵe
	CAnimation rightAnimation;		// ����(�k)���ʵe

	CAnimation fightLeftAnimation;		// ����(��)���ʵe
	CAnimation fightRightAnimation;		// ����(�k)���ʵe

	CAnimation orangeanimation;		// ����(��)���ʵe
	CAnimation orangerightAnimation;		// ����(�k)���ʵe

	CAnimation orangefightLeftAnimation;		// ����(��)���ʵe
	CAnimation orangefightRightAnimation;		// ����(�k)���ʵe

	CMovingBitmap lifebmp[5], losslifebmp[5];
	int x, y;					// ���l���W���y��
	int showx, showy;
	int changeCount;
	int numbers;  //�Ǫ��ƶq
	bool isMovingDown;			// �O�_���b���U����
	bool isMovingLeft;			// �O�_���b��������
	bool isMovingRight;			// �O�_���b���k����
	bool isMovingUp;			// �O�_���b���W����
	bool isFighting;
	bool moveState;
private:
	bool AttackMonster(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
};

/////////////////////////////////////////////////////////////////////////////
// �o��class����¶��骺�y
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class CBall   //monster
{
public:
	CBall();
	bool HitEraser(CEraser *eraser);						// �O�_�I�����l
	bool HitEraser_Pizza(CEraser *eraser);


	bool IsAlive();											// �O�_����
	bool Is_Appear_Pizza();
	void LoadBitmap();										// ���J�ϧ�
	void OnMove(GameMap *m,CEraser *eraser);											// ����
	void OnShow();											// �N�ϧζK��e��
	int  GetX1();
	int  GetPX();
	int  GetPY();
	int  GetY1();
	int  GetX2();
	int  GetY2();
	int  GetBlood();
	void Hurted(int attack);
	void SetXY(int nx, int ny);								// �]�w��ߪ��y��
	void SetX(int nx);
	void SetY(int ny);
	void SetPizzaY();               //�YPizza�᭫�]Pizza�y��
	void SetAreaNumber(int number);
	void SetMonsterType(int type);
	void SetIsAlive(bool alive);							// �]�w�O�_����
	void SetIs_Appear_Pizza(bool alive);
	void SetDelay(int d);									// �]�w���઺�t��
	void reburn();
protected:
	CAnimation leftAnimation;		// ����(��)���ʵe
	CAnimation rightAnimation;		// ����(�k)���ʵe
	CMovingBitmap LifePizza;

	int Px, Py;          //Pizza�y��
	int Ptimecounter1, Ptimecounter2;
	int MonsterType;
	int Pcount;
	int PAnimate_Y;
	int x, y;					// ��ߪ��y��
	int dx, dy;					// �y�Z����ߪ��첾�q
	int index;					// �y���u���סv�A0-17���0-360��
	int delay_counter;			// �վ����t�ת��p�ƾ�
	int delay;					// ���઺�t��
	int blood;                  // ��q
	int area_number;            // �ϰ�s��
	int reburnTime;             // ���ͮɶ�
	bool is_alive;				// �O�_����
	bool to_right;
	bool to_left;
	bool is_appear_pizza;
private:
	bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	bool HitRectangle_Pizza(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���ѷ|�@���u�����y
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class CBouncingBall
{
public:
	CBouncingBall();
	void LoadBitmap();		// ���J�ϧ�
	void OnMove();			// ����
	void OnShow();			// �N�ϧζK��e��
private:
	int x, y;				// �ϧήy��
	int floor;				// �a�O��Y�y��
	bool rising;			// true��W�ɡBfalse��U��
	int initial_velocity;	// ��l�t��
	int velocity;			// �ثe���t��(�I/��)
	CAnimation animation;	// �Q�ΰʵe�@�ϧ�
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState {
public:
	CGameStateInit(CGame *g);
	void OnInit();  								// �C������Ȥιϧγ]�w
	void OnBeginState();							// �]�w�C�������һݪ��ܼ�
	void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
	void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
protected:
	void OnShow();									// ��ܳo�Ӫ��A���C���e��
private:
	CMovingBitmap StartPage;								// csie��logo
	CMovingBitmap HelpPage;
	CMovingBitmap AboutPage;
	CMovingBitmap BackButton;
	CMovingBitmap StartButton;
	int GameSelect;
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState {
public:
	CGameStateRun(CGame *g);
	~CGameStateRun();
	void OnBeginState();							// �]�w�C�������һݪ��ܼ�
	void OnInit();  								// �C������Ȥιϧγ]�w
	void OnKeyDown(UINT, UINT, UINT);
	void OnKeyUp(UINT, UINT, UINT);
	void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
	void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@ 
	void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
protected:
	void OnMove();									// ���ʹC������
	void OnShow();									// ��ܳo�Ӫ��A���C���e��
private:
	const int		NUMBALLS;	// �y���`��
	CMovingBitmap	background;	// �I����
	CMovingBitmap	help;		// ������
	CBall			*ball;		// �y���}�C
	CMovingBitmap	corner;		// ������
	CEraser			eraser;		// ��l
	CInteger		hits_left;	// �ѤU��������
	CBouncingBall   bball;		// ���мu�����y

	GameMap		    GameMap;    // GameMap
	int counter;	// �˼Ƥ��p�ƾ�
	int min;
	int lifecounter,changecounter;
	bool change;
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState {
public:
	CGameStateOver(CGame *g);
	void OnBeginState();							// �]�w�C�������һݪ��ܼ�
	void OnInit();
protected:
	void OnMove();									// ���ʹC������
	void OnShow();									// ��ܳo�Ӫ��A���C���e��
private:
	int counter;	// �˼Ƥ��p�ƾ�
	CMovingBitmap	gameover;		// ������
};

}