/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
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

enum AUDIO_ID {				// 定義各種音效的編號
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
class GameMap   //地圖
{
public:
	GameMap();
	int  GetX1();			  	     // 左上角 x 座標
	int  GetY1();					 // 左上角 y 座標
	int  GetX2();					 // 右下角 x 座標
	int  GetY2();					 // 右下角 y 座標
	void SetX1(int setx);			 // 設定左上角 x 座標
	void SetY1(int sety);			 // 設定左上角 y 座標
	void Initialize();				 // 設定初始值
	void LoadBitmap();			 	 // 載入圖形
	void OnMove(CBall *Monseter);	 // 移動
	void OnShow();					 // 將圖形貼到畫面
	void SetMovingDown(bool flag);   // 設定是否正在往下移動
	void SetMovingLeft(bool flag);	 // 設定是否正在往左移動
	void SetMovingRight(bool flag);  // 設定是否正在往右移動
	void SetMovingUp(bool flag);	 // 設定是否正在往上移動
	void SetXY(int nx, int ny);		 // 設定XY座標
	void SetNewFloor(int sfloor);    //Jump用
	void SetMonsterFloor(int sfloor);//怪物高度
	void SetNumber(int number);      //怪物數目
	void SetHinder(bool flag);       //障礙
	void SetUp(bool flag);           //設定往上
	void SetDown(bool flag);         //設定往下
	//-----------------------------------------------------jump
	void SetJumping(bool flag);	     // 設定是否正在往上移動
	void OnJump(CBall *Monseter);	 // 移動
	//////////////////////////////障礙///////////////////////////////////////
	bool IsEmpty(int X, int Y);
protected:
	int x, y;					// 擦子左上角座標
	int numbers;  //怪物數量
	bool isMovingDown;			// 是否正在往下移動
	bool isMovingLeft;			// 是否正在往左移動
	bool isMovingRight;			// 是否正在往右移動
	bool isMovingUp;			// 是否正在往上移動

	bool isHinder;
	bool isUp;
	bool isDown;
private://-----------------------------------------------------jump
	int floor;
	int monsterfloor;
	bool rising;
	int initial_velocity;	// 初始速度
	int velocity;			// 目前的速度(點/次)
	CAnimation animation;	// 利用動畫作圖形
	bool isJumping;

	//////////////////////////////障礙///////////////////////////////////////
	CMovingBitmap blue;
	int map[24][27];
	int sx, sy;             //螢幕
    int MW, MH;
	int foolvelocity;


};

class CEraser   //主角
{
public:
	CEraser();
	bool AttackMonsterIsSuccess(CBall monster);						// 是否碰到擦子
	int  GetX1();					// 擦子左上角 x 座標
	int  GetY1();					// 擦子左上角 y 座標
	int  GetX2();					// 擦子右下角 x 座標
	int  GetY2();					// 擦子右下角 y 座標
	int  GetshowX1();					// 擦子左上角 x 座標
	int  GetshowY1();					// 擦子左上角 x 座標
	int  GetshowX2();					// 擦子左上角 x 座標
	int  GetshowY2();					// 擦子左上角 x 座標
	int  GetChangeCount();
	void Initialize();				// 設定擦子為初始值
	void LoadBitmap();				// 載入圖形
	void OnMove(GameMap *m);					// 含jump
	void OnShow(int life, CBall *monster);					// 將擦子圖形貼到畫面GameMap *m
	void SetMovingDown(bool flag);	// 設定是否正在往下移動
	void SetMovingLeft(bool flag);	// 設定是否正在往左移動
	void SetMovingRight(bool flag); // 設定是否正在往右移動
	void SetMovingUp(bool flag);	// 設定是否正在往上移動
	void SetMoveState(bool flag);
	void SetXY(int nx, int ny);		// 設定擦子左上角座標
	void SetFighting(bool flag);
	void SetNumber(int number);
	void SetChangeCount(int count);
protected:
	CAnimation animation;		// 走路(左)的動畫
	CAnimation rightAnimation;		// 走路(右)的動畫

	CAnimation fightLeftAnimation;		// 攻擊(左)的動畫
	CAnimation fightRightAnimation;		// 攻擊(右)的動畫

	CAnimation orangeanimation;		// 走路(左)的動畫
	CAnimation orangerightAnimation;		// 走路(右)的動畫

	CAnimation orangefightLeftAnimation;		// 攻擊(左)的動畫
	CAnimation orangefightRightAnimation;		// 攻擊(右)的動畫

	CMovingBitmap lifebmp[5], losslifebmp[5];
	int x, y;					// 擦子左上角座標
	int showx, showy;
	int changeCount;
	int numbers;  //怪物數量
	bool isMovingDown;			// 是否正在往下移動
	bool isMovingLeft;			// 是否正在往左移動
	bool isMovingRight;			// 是否正在往右移動
	bool isMovingUp;			// 是否正在往上移動
	bool isFighting;
	bool moveState;
private:
	bool AttackMonster(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
};

/////////////////////////////////////////////////////////////////////////////
// 這個class提供繞圈圈的球
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class CBall   //monster
{
public:
	CBall();
	bool HitEraser(CEraser *eraser);						// 是否碰到擦子
	bool HitEraser_Pizza(CEraser *eraser);


	bool IsAlive();											// 是否活著
	bool Is_Appear_Pizza();
	void LoadBitmap();										// 載入圖形
	void OnMove(GameMap *m,CEraser *eraser);											// 移動
	void OnShow();											// 將圖形貼到畫面
	int  GetX1();
	int  GetPX();
	int  GetPY();
	int  GetY1();
	int  GetX2();
	int  GetY2();
	int  GetBlood();
	void Hurted(int attack);
	void SetXY(int nx, int ny);								// 設定圓心的座標
	void SetX(int nx);
	void SetY(int ny);
	void SetPizzaY();               //吃Pizza後重設Pizza座標
	void SetAreaNumber(int number);
	void SetMonsterType(int type);
	void SetIsAlive(bool alive);							// 設定是否活著
	void SetIs_Appear_Pizza(bool alive);
	void SetDelay(int d);									// 設定旋轉的速度
	void reburn();
protected:
	CAnimation leftAnimation;		// 走路(左)的動畫
	CAnimation rightAnimation;		// 走路(右)的動畫
	CMovingBitmap LifePizza;

	int Px, Py;          //Pizza座標
	int Ptimecounter1, Ptimecounter2;
	int MonsterType;
	int Pcount;
	int PAnimate_Y;
	int x, y;					// 圓心的座標
	int dx, dy;					// 球距離圓心的位移量
	int index;					// 球的「角度」，0-17表示0-360度
	int delay_counter;			// 調整旋轉速度的計數器
	int delay;					// 旋轉的速度
	int blood;                  // 血量
	int area_number;            // 區域編號
	int reburnTime;             // 重生時間
	bool is_alive;				// 是否活著
	bool to_right;
	bool to_left;
	bool is_appear_pizza;
private:
	bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	bool HitRectangle_Pizza(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
};

/////////////////////////////////////////////////////////////////////////////
// 這個class提供會一直彈跳的球
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class CBouncingBall
{
public:
	CBouncingBall();
	void LoadBitmap();		// 載入圖形
	void OnMove();			// 移動
	void OnShow();			// 將圖形貼到畫面
private:
	int x, y;				// 圖形座標
	int floor;				// 地板的Y座標
	bool rising;			// true表上升、false表下降
	int initial_velocity;	// 初始速度
	int velocity;			// 目前的速度(點/次)
	CAnimation animation;	// 利用動畫作圖形
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState {
public:
	CGameStateInit(CGame *g);
	void OnInit();  								// 遊戲的初值及圖形設定
	void OnBeginState();							// 設定每次重玩所需的變數
	void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
	void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
protected:
	void OnShow();									// 顯示這個狀態的遊戲畫面
private:
	CMovingBitmap StartPage;								// csie的logo
	CMovingBitmap HelpPage;
	CMovingBitmap AboutPage;
	CMovingBitmap BackButton;
	CMovingBitmap StartButton;
	int GameSelect;
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState {
public:
	CGameStateRun(CGame *g);
	~CGameStateRun();
	void OnBeginState();							// 設定每次重玩所需的變數
	void OnInit();  								// 遊戲的初值及圖形設定
	void OnKeyDown(UINT, UINT, UINT);
	void OnKeyUp(UINT, UINT, UINT);
	void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
	void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
	void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
protected:
	void OnMove();									// 移動遊戲元素
	void OnShow();									// 顯示這個狀態的遊戲畫面
private:
	const int		NUMBALLS;	// 球的總數
	CMovingBitmap	background;	// 背景圖
	CMovingBitmap	help;		// 說明圖
	CBall			*ball;		// 球的陣列
	CMovingBitmap	corner;		// 角落圖
	CEraser			eraser;		// 拍子
	CInteger		hits_left;	// 剩下的撞擊數
	CBouncingBall   bball;		// 反覆彈跳的球

	GameMap		    GameMap;    // GameMap
	int counter;	// 倒數之計數器
	int min;
	int lifecounter,changecounter;
	bool change;
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState {
public:
	CGameStateOver(CGame *g);
	void OnBeginState();							// 設定每次重玩所需的變數
	void OnInit();
protected:
	void OnMove();									// 移動遊戲元素
	void OnShow();									// 顯示這個狀態的遊戲畫面
private:
	int counter;	// 倒數之計數器
	CMovingBitmap	gameover;		// 角落圖
};

}