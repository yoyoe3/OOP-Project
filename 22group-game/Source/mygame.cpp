/*
 * mygame.cpp: ���ɮ��x�C��������class��implementation
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
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include "windows.h"

namespace game_framework {

/////////////////////////////////////////////////////////////////////////////
// CBall: Ball class
/////////////////////////////////////////////////////////////////////////////
	int score,musicfk;

CBall::CBall()
{
	to_right = is_alive = true;
	is_appear_pizza=to_left = false;
	blood = 100;
	reburnTime = 10*30;
	MonsterType=PAnimate_Y=Pcount=area_number = 0;
	Px=Py=x = y = dx = dy = index = delay_counter = 0;
	Ptimecounter1= Ptimecounter2 = 5;
}

bool CBall::HitEraser(CEraser *eraser)
{
	// �˴����l�Һc�����x�άO�_�I��y
	return HitRectangle(eraser->GetshowX1(), eraser->GetshowY1(),
		   eraser->GetshowX2(), eraser->GetshowY2());
}

bool CBall::HitRectangle(int tx1, int ty1, int tx2, int ty2)
{
	int x1 = x + dx+3;				// �y�����W��x�y��
	int y1 = y + dy + 5;				// �y�����W��y�y��
	int x2 = x1 + leftAnimation.Width()-10;	// �y���k�U��x�y��
	int y2 = y1 + leftAnimation.Height();	// �y���k�U��y�y��
	//
	// �˴��y���x�λP�ѼƯx�άO�_���涰
	//
	return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}

bool CBall::HitEraser_Pizza(CEraser *eraser)
{
	// �˴����l�Һc�����x�άO�_�I��y
	return HitRectangle_Pizza(eraser->GetshowX1(), eraser->GetshowY1(),
		eraser->GetshowX2(), eraser->GetshowY2());
}

bool CBall::HitRectangle_Pizza(int tx1, int ty1, int tx2, int ty2)
{
	int x1 = Px;				// �y�����W��x�y��
	int y1 = Py;				// �y�����W��y�y��
	int x2 = x1 + LifePizza.Width();	// �y���k�U��x�y��
	int y2 = y1 + LifePizza.Height();	// �y���k�U��y�y��
											//
											// �˴��y���x�λP�ѼƯx�άO�_���涰
											//
	return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}

bool CBall::IsAlive()
{
	return is_alive;
}

bool CBall::Is_Appear_Pizza()
{
	return is_appear_pizza;
}

void CBall::LoadBitmap()
{

	//��
	char *monsterFileNameLeft[3] = { ".\\bitmaps\\monsterLeft1.bmp",".\\bitmaps\\monsterLeft2.bmp",".\\bitmaps\\monsterLeft3.bmp" };
	for (int i = 0; i < 3; i++)	// ���J�ʵe(��3�i�ϧκc��)
		leftAnimation.AddBitmap(monsterFileNameLeft[i], RGB(0, 0, 0));

	//�k
	char *monsterFileNameRight[3] = { ".\\bitmaps\\monsterRight1.bmp",".\\bitmaps\\monsterRight2.bmp",".\\bitmaps\\monsterRight3.bmp" };
	for (int i = 0; i < 3; i++)	// ���J�ʵe(��3�i�ϧκc��)
		rightAnimation.AddBitmap(monsterFileNameRight[i], RGB(0, 0, 0));

	LifePizza.LoadBitmap(IDB_Pizza, RGB(0, 0, 0));

	//bmp_center.LoadBitmap(IDB_CENTER,RGB(0,0,0));	// ���J�y��ߪ��ϧ�
}

void CBall::OnMove(GameMap *m, CEraser *eraser)
{
	if (is_appear_pizza) 
	{
		if (Ptimecounter1 > 0)
		{
			PAnimate_Y -= 1;
			Ptimecounter1--;
		}
		else if (Ptimecounter2 > 0)
		{
			PAnimate_Y += 1;
			Ptimecounter2--;
		}
		else
			Ptimecounter1 = Ptimecounter2 = 10;
	}
	
	if (!is_alive)
	{
		reburn();
		return;
	}

	int leftmin, rightmax;
	if (area_number == 1)
	{
		leftmin = 180;
		rightmax = 540;
	}
	else if (area_number == 2)
	{
		leftmin = 670;
		rightmax = 960;
	}
	else if (area_number == 3)
	{
		leftmin = 660;
		rightmax = 870;
	}
	else if (area_number == 4)
	{
		leftmin = 950;
		rightmax = 1220;
	}
	else if (area_number == 5)
	{
		leftmin = 160;
		rightmax = 520;
	}

	int STEP_SIZE = 3;
	if (MonsterType == 1&& y== eraser->GetshowY1() && eraser->GetX1() > leftmin&& eraser->GetX1() < rightmax)
	{
		STEP_SIZE = 4;
		if (x > eraser->GetshowX1() + 25)
		{
			to_right = false;
			leftAnimation.OnMove();
			x -= STEP_SIZE;
		}
		else
		{
			to_right = true;
			rightAnimation.OnMove();
			x += STEP_SIZE;
		}
		leftAnimation.SetDelayCount(4);
		rightAnimation.SetDelayCount(4);
	}
	else if (to_right)
	{
		rightAnimation.OnMove();
		x += STEP_SIZE;
		if (is_appear_pizza)
			Pcount -= STEP_SIZE;
		if (x+m->GetX1() > rightmax)
			to_right = false;
		leftAnimation.SetDelayCount(6);
		rightAnimation.SetDelayCount(6);
	}
	else if (!to_right)
	{
		leftAnimation.OnMove();
		x -= STEP_SIZE;
		if (is_appear_pizza)
			Pcount += STEP_SIZE;
		if (x + m->GetX1() < leftmin)
			to_right = true;
		leftAnimation.SetDelayCount(6);
		rightAnimation.SetDelayCount(6);
	}


	/*delay_counter--;
	if (delay_counter < 0) {

		delay_counter = delay;
		//
		// �p��y�V����ߪ��첾�qdx, dy
		//
		const int STEPS=18;
		static const int DIFFX[]={35, 32, 26, 17, 6, -6, -17, -26, -32, -34, -32, -26, -17, -6, 6, 17, 26, 32, };
		static const int DIFFY[]={0, 11, 22, 30, 34, 34, 30, 22, 11, 0, -11, -22, -30, -34, -34, -30, -22, -11, };
		index++;
		if (index >= STEPS)
			index = 0;
		dx = DIFFX[index];
		dy = DIFFY[index];
	}*/
}

int CBall::GetX1()
{
	return x;
}

int CBall::GetPX()
{
	return Px;
}

int CBall::GetPY()
{
	return Py;
}

int CBall::GetY1()
{
	return y;
}

int CBall::GetX2()
{
	return x + rightAnimation.Width();
}

int CBall::GetY2()
{
	return y + rightAnimation.Height();
}

int CBall::GetBlood()
{
	return blood;
}

void CBall::Hurted(int attack)
{
	if (is_alive)
	{
		blood = blood - attack;
		if (blood <= 0)
		{
			CAudio::Instance()->Play(AUDIO_DIE);
			if (MonsterType == 1)
				score+=2;
			else
				score++;
			PAnimate_Y=Pcount = 0;
			if(rand()%4+1 ==2)
				is_appear_pizza = true;
			is_alive = false;
		}
	}
}

void CBall::SetDelay(int d)
{
	delay = d;
}

void CBall::SetPizzaY()
{
	Px = Py = 0;
}

void CBall::SetIsAlive(bool alive)
{
	is_alive = alive;
}

void CBall::SetIs_Appear_Pizza(bool alive)
{
	is_appear_pizza = alive;
}

void CBall::SetXY(int nx, int ny)
{
	x = nx; y = ny;
}

void CBall::SetX(int nx)
{
	x = nx;
}

void CBall::SetY(int ny)
{
	y = ny;
}

void CBall::SetAreaNumber(int number)
{
	area_number = number;
}

void CBall::SetMonsterType(int type)
{
	MonsterType = type;
}

void CBall::OnShow()
{
	leftAnimation.SetTopLeft(x, y);
	rightAnimation.SetTopLeft(x, y);
	
	if (is_alive) {
		if (to_right)
			rightAnimation.OnShow();
		else
			leftAnimation.OnShow();
	}
	if (is_appear_pizza)
	{
		Px = x+ Pcount;
		Py = y+10+ PAnimate_Y;
		LifePizza.SetTopLeft(Px, Py);
		LifePizza.ShowBitmap();
	}


}

void CBall::reburn() {
	if (!is_alive)
	{
		reburnTime--;
		if (reburnTime < 0)
		{
			is_alive = true;
			Px = Py = 0;
			blood=100;
			reburnTime = 10 * 30;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBouncingBall: BouncingBall class
/////////////////////////////////////////////////////////////////////////////

CBouncingBall::CBouncingBall()
{
	const int INITIAL_VELOCITY = 20;	// ��l�W�ɳt��
	const int FLOOR = 400;				// �a�O�y��
	floor = FLOOR;
	x = 95; y = FLOOR-1;				// y�y�Ф�a�O��1�I(���b�a�O�W)
	rising = true; 
	initial_velocity = INITIAL_VELOCITY;
	velocity = initial_velocity;
}

void CBouncingBall::LoadBitmap()
{
	char *filename[4]={".\\bitmaps\\ball1.bmp",".\\bitmaps\\ball2.bmp",".\\bitmaps\\ball3.bmp",".\\bitmaps\\ball4.bmp"};
	for (int i = 0; i < 4; i++)	// ���J�ʵe(��4�i�ϧκc��)
		animation.AddBitmap(filename[i], RGB(0,0,0));
}

void CBouncingBall::OnMove()
{
	if (rising) {			// �W�ɪ��A
		if (velocity > 0) {
			y -= velocity;	// ��t�� > 0�ɡAy�b�W��(����velocity���I�Avelocity����쬰 �I/��)
			velocity--;		// �����O�v�T�A�U�����W�ɳt�׭��C
		} else {
			rising = false; // ��t�� <= 0�A�W�ɲפ�A�U���אּ�U��
			velocity = 1;	// �U������t(velocity)��1
		}
	} else {				// �U�����A
		if (y < floor-1) {  // ��y�y���٨S�I��a�O
			y += velocity;	// y�b�U��(����velocity���I�Avelocity����쬰 �I/��)
			velocity++;		// �����O�v�T�A�U�����U���t�׼W�[
		} else {
			y = floor - 1;  // ��y�y�ЧC��a�O�A�󥿬��a�O�W
			rising = true;	// �����ϼu�A�U���אּ�W��
			velocity = initial_velocity; // ���]�W�ɪ�l�t��
		}
	}
	animation.OnMove();		// ����@��animation.OnMove()�Aanimation�~�|����
}

void CBouncingBall::OnShow()
{
	animation.SetTopLeft(x,y);
	animation.OnShow();
}

/////////////////////////////////////////////////////////////////////////////
// CEraser: Eraser class  ---Hero
/////////////////////////////////////////////////////////////////////////////

CEraser::CEraser()
{
	Initialize();
}

int CEraser::GetX1()
{
	return x;
}

int CEraser::GetY1()
{
	return y;
}

int CEraser::GetX2()
{
	return x + animation.Width();
}

int CEraser::GetY2()
{
	return y + animation.Height();
}

int CEraser::GetshowX1()
{
	return showx;
}

int CEraser::GetshowY1()
{
	return showy;
}

int CEraser::GetshowX2()
{
	return showx + animation.Width();
}

int CEraser::GetshowY2()
{
	return showy + animation.Height();
}

int CEraser::GetChangeCount()
{
	return changeCount;
}

void CEraser::SetNumber(int number)
{
	numbers = number;
}

void CEraser::SetChangeCount(int count)
{
	changeCount = count;
}

void CEraser::Initialize()
{
	const int X_POS = 250;
	const int Y_POS = 270;
	showx = X_POS;
	showy = Y_POS;
	isFighting = moveState = isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
	changeCount = 1;
}

void CEraser::LoadBitmap()
{
	//animation.AddBitmap(IDB_Hero);


		//����  ���B�k
		char *ffilename[3] = { ".\\bitmaps\\left1.bmp",".\\bitmaps\\left2.bmp",".\\bitmaps\\left3.bmp" };
		char *ffilenameRight[3] = { ".\\bitmaps\\right1.bmp",".\\bitmaps\\right2.bmp",".\\bitmaps\\right3.bmp" };

		//����  ���B�k
		char *ffilenameFightLeft[4] = { ".\\bitmaps\\HeroFightLeft1.bmp",".\\bitmaps\\HeroFightLeft2.bmp",".\\bitmaps\\HeroFightLeft3.bmp",".\\bitmaps\\HeroFightLeft3.bmp" };
		char *ffilenameFightRight[4] = { ".\\bitmaps\\HeroFightRight1.bmp",".\\bitmaps\\HeroFightRight2.bmp",".\\bitmaps\\HeroFightRight3.bmp",".\\bitmaps\\HeroFightRight3.bmp" };

		for (int i = 0; i < 3; i++)	// ���J�ʵe(��3�i�ϧκc��)
		{
			animation.AddBitmap(ffilename[i], RGB(0, 0, 0));
			rightAnimation.AddBitmap(ffilenameRight[i], RGB(0, 0, 0));
		}
		
		for (int i = 0; i < 4; i++)	// ���J�ʵe(��4�i�ϧκc��)
		{
			fightLeftAnimation.AddBitmap(ffilenameFightLeft[i], RGB(0, 0, 0));
			fightRightAnimation.AddBitmap(ffilenameFightRight[i], RGB(0, 0, 0));
		}

		//orange����  ���B�k
		char *filename[2] = { ".\\bitmaps\\orangeleft1.bmp",".\\bitmaps\\orangeleft2.bmp"};
		char *filenameRight[2] = { ".\\bitmaps\\orangeright1.bmp",".\\bitmaps\\orangeright2.bmp" };
		//orange����  ���B�k
		char *filenameFightLeft[4] = { ".\\bitmaps\\orangeFightLeft1.bmp",".\\bitmaps\\orangeFightLeft2.bmp",".\\bitmaps\\orangeFightLeft3.bmp",".\\bitmaps\\orangeFightLeft3.bmp" };
		char *filenameFightRight[4] = { ".\\bitmaps\\orangeFightRight1.bmp",".\\bitmaps\\orangeFightRight2.bmp",".\\bitmaps\\orangeFightRight3.bmp",".\\bitmaps\\orangeFightRight3.bmp" };

		for (int i = 0; i < 2; i++)	// ���J�ʵe(��3�i�ϧκc��)
		{
			orangeanimation.AddBitmap(filename[i], RGB(0, 0, 0));
			orangerightAnimation.AddBitmap(filenameRight[i], RGB(0, 0, 0));
		}
		
		for (int i = 0; i < 4; i++)	// ���J�ʵe(��4�i�ϧκc��)
		{
			orangefightLeftAnimation.AddBitmap(filenameFightLeft[i], RGB(0, 0, 0));
			orangefightRightAnimation.AddBitmap(filenameFightRight[i], RGB(0, 0, 0));
		}


	orangeanimation.SetDelayCount(6);
	orangerightAnimation.SetDelayCount(6);
	orangefightLeftAnimation.SetDelayCount(5);
	orangefightRightAnimation.SetDelayCount(5); 
	
	animation.SetDelayCount(6);
	rightAnimation.SetDelayCount(6);
	fightLeftAnimation.SetDelayCount(5);
	fightRightAnimation.SetDelayCount(5);
	//char *filename[3] = { ".\\bitmaps\\left1.bmp",".\\bitmaps\\left2.bmp",".\\bitmaps\\left3.bmp" };
	// ��q
	char *lifegrape[2] = { ".\\bitmaps\\Life.bmp",".\\bitmaps\\LossLife.bmp"};
	for (int i = 0; i < 5; i++)
	{
		lifebmp[i].LoadBitmap(IDB_HaveLife, RGB(0, 0, 0));
		losslifebmp[i].LoadBitmap(IDB_LossLife, RGB(0, 0, 0));
	}
}

void CEraser::OnMove(GameMap *m)//map *m
{
	x = m->GetX1()+240;
	y = m->GetY1()-500+270;
	const int STEP_SIZE = 3;

	

	if (!isFighting)
	{
		if (isMovingRight) {
			rightAnimation.OnMove();
			orangerightAnimation.OnMove();
			if (m->IsEmpty(this->GetX2() + 2, this->GetY1()) || m->IsEmpty(this->GetX2() + 2, this->GetY2())) 
			{ // �k��O�Ū�
																												//x += STEP_SIZE;
																												//m->SetX1(m->GetX1() - STEP_SIZE);
				m->SetHinder(true);

				//m->SetSX(...); // �ץ� sx �P sy�A���ܿù�����m
			}
			else
				m->SetHinder(false);
		}
		else if (isMovingLeft) {
			animation.OnMove();
			orangeanimation.OnMove();
			if (m->IsEmpty(this->GetX1() - 2, this->GetY1()) || m->IsEmpty(this->GetX1() - 2, this->GetY2())) { // ����O�Ū�
				m->SetHinder(true);
				//m->SetSX(...); // �ץ� sx �P sy�A���ܿù�����m
			}
			else
				m->SetHinder(false);
		}	
	}
	else
	{
		fightLeftAnimation.OnMove();
		fightRightAnimation.OnMove();
		orangefightLeftAnimation.OnMove();
		orangefightRightAnimation.OnMove();
	}


	if (m->IsEmpty(this->GetX1(), this->GetY1() + 1) || m->IsEmpty(this->GetX2(), this->GetY1() + 1)) { // �W���O�Ū�
		m->SetUp(true);
	}
	else
		m->SetUp(false);

	if (m->IsEmpty(this->GetX1(), this->GetY2() + 10) || m->IsEmpty(this->GetX2(), this->GetY2() + 10)) { // �U���O�Ū�
		m->SetDown(true);
		int yy = ((((this->GetY1() + 1) / 50) + 1) * 50);
		int Myy = (((this->GetY1() + 1) / 50) + 1) * 50 - 1000;
		m->SetNewFloor(yy + 500 - 270 - 18);
		m->SetMonsterFloor(yy + 500 - 270 - 18 - 1242 - 2 * Myy);
	}
	else {//y = m->GetY1()-500+270;
		m->SetDown(false);
		m->SetNewFloor(1411);
	}

	
	

	//for (int i = 0; i < 5; i++)
	//	lifebmp[i].OnMove();
}

void CEraser::SetMovingDown(bool flag)
{
	isMovingDown = flag;
}

void CEraser::SetMovingLeft(bool flag)
{
	isMovingLeft = flag;
}

void CEraser::SetMovingRight(bool flag)
{
	isMovingRight = flag;
}

void CEraser::SetMovingUp(bool flag)
{
	isMovingUp = flag;
}
void CEraser::SetMoveState(bool flag)
{
	moveState = flag;
}

void CEraser::SetXY(int nx, int ny)
{
	x = nx; y = ny;
}

void CEraser::SetFighting(bool flag)
{
	isFighting = flag;
}

bool CEraser::AttackMonsterIsSuccess(CBall monster)
{
	// �˴����l�Һc�����x�άO�_�I��y
	return AttackMonster(monster.GetX1(), monster.GetY1(),
		monster.GetX2(), monster.GetY2());
}

bool CEraser::AttackMonster(int tx1, int ty1, int tx2, int ty2)
{
	bool attack;
	
	int X = showx + 25;				// ����x�y��
	int Y = showy + 23;				// ����y�y��
	if (moveState)
		for (int i = 0; i < 16; i = i + 2)
		{
			X = X + i;
			attack = tx2 >= X && tx1 <= X && ty2 >= Y && ty1 <= Y;
			if (attack)break;
		}
	else
	{
		for (int i = 0; i < 16; i = i + 2)
		{
			X = X - i;
			attack = tx2 >= X && tx1 <= X && ty2 >= Y && ty1 <= Y;
			if (attack)break;
		}
	}

	return attack;
}



void CEraser::OnShow(int life,CBall *monster)//GameMap *m
{
	animation.SetTopLeft(showx, showy);
	rightAnimation.SetTopLeft(showx, showy);
	fightLeftAnimation.SetTopLeft(showx, showy);
	fightRightAnimation.SetTopLeft(showx, showy);

	orangeanimation.SetTopLeft(showx, showy);
	orangerightAnimation.SetTopLeft(showx, showy);
	orangefightLeftAnimation.SetTopLeft(showx, showy);
	orangefightRightAnimation.SetTopLeft(showx, showy);
	
	if (changeCount == 1)
	{
		if (!isFighting) {
			if (isMovingLeft)
				animation.OnShow();
			else if (isMovingRight)
				rightAnimation.OnShow();
			else
			{
				if (moveState)
					rightAnimation.OnShow();
				else
					animation.OnShow();
			}
		}
		else {
			if (!moveState)
			{
				fightLeftAnimation.OnShow();
				if (fightLeftAnimation.IsFinalBitmap())
				{
					CAudio::Instance()->Play(AUDIO_ATTACK);
					fightLeftAnimation.Reset();
					fightRightAnimation.Reset();
					isFighting = false;

					for (int i = 0; i < numbers; i++)
						if (AttackMonsterIsSuccess(monster[i]))
							monster[i].Hurted(50);
				}
			}
			else
			{
				fightRightAnimation.OnShow();
				if (fightRightAnimation.IsFinalBitmap())
				{
					CAudio::Instance()->Play(AUDIO_ATTACK);
					fightLeftAnimation.Reset();
					fightRightAnimation.Reset();
					isFighting = false;

					for (int i = 0; i < numbers; i++)
						if (AttackMonsterIsSuccess(monster[i]))
							monster[i].Hurted(50);
				}
			}
		}
	}
	else if (changeCount == 2)
	{
		if (!isFighting) {
			if (isMovingLeft)
				orangeanimation.OnShow();
			else if (isMovingRight)
				orangerightAnimation.OnShow();
			else
			{
				if (moveState)
					orangerightAnimation.OnShow();
				else
					orangeanimation.OnShow();
			}
		}
		else {
			if (!moveState)
			{
				orangefightLeftAnimation.OnShow();
				if (orangefightLeftAnimation.IsFinalBitmap())
				{
					CAudio::Instance()->Play(AUDIO_ORANGEATTACK);
					orangefightLeftAnimation.Reset();
					orangefightRightAnimation.Reset();
					isFighting = false;

					for (int i = 0; i < numbers; i++)
						if (AttackMonsterIsSuccess(monster[i]))
							monster[i].Hurted(50);
				}
			}
			else
			{
				orangefightRightAnimation.OnShow();
				if (orangefightRightAnimation.IsFinalBitmap())
				{
					CAudio::Instance()->Play(AUDIO_ORANGEATTACK);
					orangefightLeftAnimation.Reset();
					orangefightRightAnimation.Reset();
					isFighting = false;

					for (int i = 0; i < numbers; i++)
						if (AttackMonsterIsSuccess(monster[i]))
							monster[i].Hurted(50);
				}
			}
		}
	}

	
	
	
	int lifex=20;
	int lifey=25;
	int lifedistance=43;

	//lifebmp[0].LoadBitmap(IDB_LossLife, RGB(255, 255, 255));



	//����
	for (int i = 0; i < 5; i++)
	{
		lifebmp[i].SetTopLeft(lifex + i*lifedistance, lifey);
		losslifebmp[i].SetTopLeft(lifex + i*lifedistance, lifey);
		if (life == 5) {
			for (int i = 0; i < 5; i++)
				lifebmp[i].ShowBitmap();
		}
		else if (life == 4) {
			losslifebmp[4].ShowBitmap();
			for (int i = 0; i < 4; i++)
				lifebmp[i].ShowBitmap();
		}
		else if (life == 3) {
			for (int i = 3; i < 5; i++)
				losslifebmp[i].ShowBitmap();
			for (int i = 0; i < 3; i++)
				lifebmp[i].ShowBitmap();
		}
		else if (life == 2) {
			for (int i = 2; i < 5; i++)
				losslifebmp[i].ShowBitmap();
			for (int i = 0; i < 2; i++)
				lifebmp[i].ShowBitmap();
		}
		else if (life == 1) {
			for (int i = 1; i < 5; i++)
				losslifebmp[i].ShowBitmap();
			lifebmp[0].ShowBitmap();
		}

		
	}
}


/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g)
: CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	GameSelect = 1;
	//
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
	//
	// �}�l���J���
	//
	StartPage.LoadBitmap(IDB_GameStart);
	HelpPage.LoadBitmap(IDB_GameHelp);
	AboutPage.LoadBitmap(IDB_GameAbout);
	BackButton.LoadBitmap(IDB_Back, RGB(0, 0, 0));
	StartButton.LoadBitmap(IDB_Start,  RGB(0, 0, 0));

	CAudio::Instance()->Load(AUDIO_STARTSOUND, "sounds\\startsound.wav");

	musicfk = 0;
	Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
	//
}

void CGameStateInit::OnBeginState()
{
	if(musicfk==1)
		CAudio::Instance()->Play(AUDIO_STARTSOUND, true);
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_ESC = 27;
	const char KEY_SPACE = ' ';
	if (nChar == KEY_SPACE)
	{
		CAudio::Instance()->Stop(AUDIO_STARTSOUND);
		GotoGameState(GAME_STATE_RUN);						// ������GAME_STATE_RUN
	}
	else if (nChar == KEY_ESC)								// Demo �����C������k
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);	// �����C��
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	POINT test;
	GetCursorPos(&test);
	if (musicfk == 0)
	{
		musicfk = 1;
		CAudio::Instance()->Play(AUDIO_STARTSOUND, true);
	}
	if (GameSelect == 1)
	{
		if (615 >= test.x && 525 <= test.x && 473 >= test.y && 424 <= test.y)
			GameSelect = 2;
		else if (125 >= test.x && 30 <= test.x && 473 >= test.y && 424 <= test.y)
			GameSelect = 3;
		else if (412 >= test.x && 236 <= test.x && 479 >= test.y && 444 <= test.y)
		{
			CAudio::Instance()->Stop(AUDIO_STARTSOUND);
			GotoGameState(GAME_STATE_RUN);
		}
	}
	else if (GameSelect == 2)
	{
		if (635 >= test.x && 565 <= test.x && 479 >= test.y && 405 <= test.y)
			GameSelect = 1;
	}
	else if (GameSelect == 3)
	{
		if (635 >= test.x && 565 <= test.x && 452 >= test.y && 381 <= test.y)
			GameSelect = 1;
	}
}

void CGameStateInit::OnShow()
{
	//
	// �K�Wlogo
	//

	if (GameSelect == 1)
	{
		StartPage.SetTopLeft(0, 0);
		StartPage.ShowBitmap();
		StartButton.SetTopLeft(222, 422);
		StartButton.ShowBitmap();
	}
	else if (GameSelect == 2)
	{
		HelpPage.SetTopLeft(0, 0);
		HelpPage.ShowBitmap();
		BackButton.SetTopLeft(566, 406);
		BackButton.ShowBitmap();
	}
	else if (GameSelect == 3)
	{
		AboutPage.SetTopLeft(0, 0);
		AboutPage.ShowBitmap();
		BackButton.SetTopLeft(566, 378);
		BackButton.ShowBitmap();
	}
}								

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g)
: CGameState(g)
{
}

void CGameStateOver::OnMove()
{
	counter--;
	if (counter < 0)
	{
		GotoGameState(GAME_STATE_INIT);
		CAudio::Instance()->Stop(AUDIO_OVERSOUND);
	}
}

void CGameStateOver::OnBeginState()
{
	counter = 30 * 5; // 5 seconds
	CAudio::Instance()->Play(AUDIO_OVERSOUND, true);
}

void CGameStateOver::OnInit()
{
	gameover.LoadBitmap(IDB_GameOver);
	//
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
	//
	// �}�l���J���
	//

	CAudio::Instance()->Load(AUDIO_OVERSOUND, "sounds\\gameover.mp3");

	Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// �̲׶i�׬�100%
	//
	ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
	gameover.SetTopLeft(0, 0);
	gameover.ShowBitmap();

	CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// ���� font f; 160���16 point���r
	fp=pDC->SelectObject(&f);					// ��� font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	char str[80],scores[80];								// Demo �Ʀr��r�ꪺ�ഫ
	sprintf(str, "Game Over ! (%d)", counter / 30);

	pDC->TextOut(240,360,str);

	pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)

	f.CreatePointFont(250, "Times New Roman");	// ���� font f; 160���16 point���r
	fp = pDC->SelectObject(&f);
	pDC->SetTextColor(RGB(255, 0, 255));

	sprintf(scores, "Your Score : %d", score);
	pDC->TextOut(215, 400, scores);

	
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
	
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g)
: CGameState(g), NUMBALLS(5)
{
	ball = new CBall [NUMBALLS];
}

CGameStateRun::~CGameStateRun()
{
	delete [] ball;
}

void CGameStateRun::OnBeginState()
{
	const int BALL_GAP = 90;
	const int BALL_XY_OFFSET = 45;
	const int BALL_PER_ROW = 7;
	const int HITS_LEFT = 5;
	const int HITS_LEFT_X = 0;
	const int HITS_LEFT_Y = 0;
	const int BACKGROUND_X = 60;
	const int ANIMATION_SPEED = 15;
	for (int i = 0; i < NUMBALLS; i++) {				// �]�w�y���_�l�y��
		//int x_pos = i % BALL_PER_ROW;
		//int y_pos = i / BALL_PER_ROW;
		ball[0].SetXY(350, -30);
		ball[1].SetXY(680, 20);
		ball[2].SetXY(750, 270);
		ball[3].SetXY(1150, -380);
		ball[4].SetXY(370, -680);
		ball[4].SetMonsterType(1);
		ball[i].SetAreaNumber(i+1);
		ball[i].SetDelay(5);
		ball[i].SetIsAlive(true);
		ball[i].SetIs_Appear_Pizza(false);
	}
	eraser.Initialize();
	GameMap.Initialize();
	background.SetTopLeft(BACKGROUND_X,0);				// �]�w�I�����_�l�y��
	help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��
	hits_left.SetInteger(HITS_LEFT);					// ���w�ѤU��������
	hits_left.SetTopLeft(HITS_LEFT_X,HITS_LEFT_Y);		// ���w�ѤU�����ƪ��y��
	CAudio::Instance()->Play(AUDIO_RUN, true);			// ���� WAVE
	CAudio::Instance()->Play(AUDIO_CURE, false);
	counter = 35 * 30;
	changecounter=lifecounter = 0;
	min = 1;
	score = 0;
	change = false;
}

void CGameStateRun::OnMove()							// ���ʹC������
{
	GameMap.SetNumber(NUMBALLS);
	
	//
	// �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
	//
	// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
	//
	// ���ʭI���Ϫ��y��
	//
	/*if (background.Top() > SIZE_Y)
		background.SetTopLeft(60 ,-background.Height());
	background.SetTopLeft(background.Left(),background.Top()+1);
	//
	// ���ʲy
	*/
	if (min == 0 && counter / 35 == 8) 
		CAudio::Instance()->Stop(AUDIO_RUN);	// ���� WAVE
	else if (min == 0 && counter/35 == 7)
		CAudio::Instance()->Play(AUDIO_RECIPROCAL_FIVE);
	else if (min == 0 && counter/35 == 5)
		CAudio::Instance()->Play(AUDIO_RECIPROCAL_FOUR);
	else if (min == 0 && counter / 35 == 4)
		CAudio::Instance()->Play(AUDIO_RECIPROCAL_THREE);
	else if (min == 0 && counter / 35 == 3)
		CAudio::Instance()->Play(AUDIO_RECIPROCAL_TWO);
	else if (min == 0 && counter / 35 == 2)
		CAudio::Instance()->Play(AUDIO_RECIPROCAL_ONE);

	if(counter/35 == 0 && min ==0 || GameMap.GetY1() >1400)
		GotoGameState(GAME_STATE_OVER);
	else if (counter == 0)
	{
		min--;
		counter = 35 * 60;
	}
	else
		counter--;
	

	for (int i = 0; i < NUMBALLS; i++)
		ball[i].OnMove(&GameMap,&eraser);

	GameMap.OnMove(ball);
	eraser.OnMove(&GameMap);
	GameMap.OnJump(ball);


	if (lifecounter>-1)
		lifecounter--;

	if (changecounter>-1)
		changecounter--;

	// �P�_���l�O�_�I��y
	//

	for (int i = 0; i < NUMBALLS; i++)
	{
		if (ball[i].IsAlive() && ball[i].HitEraser(&eraser) && lifecounter <= 0) {
			//ball[i].SetIsAlive(false);
			CAudio::Instance()->Play(AUDIO_DING);
			hits_left.Add(-1);
			//
			// �Y�Ѿl�I�����Ƭ�0�A�h����Game Over���A
			//
			if (hits_left.GetInteger() <= 0) 
				GotoGameState(GAME_STATE_OVER);
			lifecounter = 30 * 2;
		}

		if (ball[i].Is_Appear_Pizza() && ball[i].HitEraser_Pizza(&eraser) && hits_left.GetInteger() < 5) {
			hits_left.Add(1);
			CAudio::Instance()->Play(AUDIO_CURE);
			ball[i].SetIs_Appear_Pizza(false);
			ball[i].SetPizzaY();
		}
	}

	//�ܨ�
	if (change&&eraser.GetY1() == 832 && eraser.GetX1() > 1199 && eraser.GetX1() < 1259&& changecounter<=0) {
		CAudio::Instance()->Play(AUDIO_CHANGEHEROSOUND);
		if (eraser.GetChangeCount() == 1)
			eraser.SetChangeCount(2);
		else
			eraser.SetChangeCount(1);
		changecounter = 30 * 10;
	}

	//
	// ���ʼu�����y
	//
	//bball.OnMove();


	

}

void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
	//
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
	//
	// �}�l���J���
	//


	int i;
	for (i = 0; i < NUMBALLS; i++)	
		ball[i].LoadBitmap();								// ���J��i�Ӳy���ϧ�
	eraser.LoadBitmap();
	GameMap.LoadBitmap();
	background.LoadBitmap(IDB_Gamemap);					// ���J�I�����ϧ�
	//
	// ��������Loading�ʧ@�A�����i��
	//
	ShowInitProgress(50);
	Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// �~����J��L���
	//
	help.LoadBitmap(IDB_HELP,RGB(255,255,255));				// ���J�������ϧ�
	//corner.LoadBitmap(IDB_CORNER);							// ���J�����ϧ�
	//corner.ShowBitmap(background);							// �Ncorner�K��background
	bball.LoadBitmap();										// ���J�ϧ�
	hits_left.LoadBitmap();								
	CAudio::Instance()->Load(AUDIO_DING,  "Sounds\\ding.wav");	// ���J�s��0���n��ding.wav
	CAudio::Instance()->Load(AUDIO_RUN,  "Sounds\\run.wav");	// ���J�s��1���n��lake.mp3
	CAudio::Instance()->Load(AUDIO_ATTACK,  "Sounds\\attack.wav");	// ���J�s��2���n��ntut.mid
	CAudio::Instance()->Load(AUDIO_ORANGEATTACK, "Sounds\\orangeattack.wav");
	CAudio::Instance()->Load(AUDIO_DIE, "Sounds\\monsterDie.wav");
	CAudio::Instance()->Load(AUDIO_RECIPROCAL_ONE, "Sounds\\reciprocalone.wav");
	CAudio::Instance()->Load(AUDIO_RECIPROCAL_TWO, "Sounds\\reciprocaltwo.wav");
	CAudio::Instance()->Load(AUDIO_RECIPROCAL_THREE, "Sounds\\reciprocalthree.wav");
	CAudio::Instance()->Load(AUDIO_RECIPROCAL_FOUR, "Sounds\\reciprocalfour.wav");
	CAudio::Instance()->Load(AUDIO_RECIPROCAL_FIVE, "Sounds\\reciprocalfive.wav");
	CAudio::Instance()->Load(AUDIO_CHANGEHEROSOUND, "Sounds\\changehero.wav");
	CAudio::Instance()->Load(AUDIO_CURE, "Sounds\\cure.wav");

	//AUDIO_RECIPROCAL_
	//
	// ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
	//
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard���b�Y
	const char KEY_UP    = 0x26; // keyboard�W�b�Y
	const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
	const char KEY_DOWN  = 0x28; // keyboard�U�b�Y
	const char KEY_A = 0x41;
	const char KEY_SPACE = 0x20; // keyboard-Space
	const char KEY_ENTER = 0x0D;
	
	if (nChar == KEY_ENTER)
	{
		change = true;
	}

	if (nChar == KEY_LEFT)
	{
		GameMap.SetMovingLeft(true);
		eraser.SetMovingLeft(true);
		eraser.SetMoveState(false);
	}
	if (nChar == KEY_RIGHT)
	{
		GameMap.SetMovingRight(true);
		eraser.SetMovingRight(true);
		eraser.SetMoveState(true);

	}	
	//if (nChar == KEY_UP)
	//	GameMap.SetMovingUp(true);
	//if (nChar == KEY_DOWN)
	//	GameMap.SetMovingDown(true);
	if (nChar == KEY_SPACE)
	{
		GameMap.SetJumping(true);
	}

	if (nChar == KEY_A)
	{
		eraser.SetFighting(true);
	}
		
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard���b�Y
	const char KEY_UP    = 0x26; // keyboard�W�b�Y
	const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
	const char KEY_DOWN  = 0x28; // keyboard�U�b�Y
	const char KEY_ENTER = 0x0D;

	if (nChar == KEY_ENTER)
	{
		change = false;
	}
	if (nChar == KEY_LEFT)
	{
		eraser.SetMovingLeft(false);
		GameMap.SetMovingLeft(false);
		//Mapstall.SetMovingLeft(false);
	}
	if (nChar == KEY_RIGHT)
	{
		eraser.SetMovingRight(false);
		GameMap.SetMovingRight(false);
		//Mapstall.SetMovingRight(false);
	}
	if (nChar == KEY_UP)
		GameMap.SetMovingUp(false);
	if (nChar == KEY_DOWN)
		GameMap.SetMovingDown(false);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
	//eraser.SetMovingLeft(true);
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
	//eraser.SetMovingLeft(false);
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
	// �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
	//eraser.SetMovingRight(true);
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
	//eraser.SetMovingRight(false);
}

void CGameStateRun::OnShow()
{
	eraser.SetNumber(NUMBALLS);
	
	//
	//  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
	//        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
	//        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
	//
	//
	//  �K�W�I���ϡB�����ơB�y�B���l�B�u�����y
	//
	
	GameMap.OnShow();
	for (int i = 0; i < NUMBALLS; i++)
		ball[i].OnShow();				// �K�W��i���y
	eraser.OnShow(hits_left.GetInteger(),ball);

	//-------------------------------Counter-------------------------------------------
	CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CFont f, *fp;
	f.CreatePointFont(120, "Times New Roman");	// ���� font f; 160���16 point���r
	fp = pDC->SelectObject(&f);					// ��� font f
	pDC->SetBkColor(NULL);
	pDC->SetTextColor(RGB(255, 0, 0));
	char str[40];								// Demo �Ʀr��r�ꪺ�ഫ
	if(counter<10*35)
		sprintf(str, "Time : %d:0%d ", min, counter / 35);
	else
		sprintf(str, "Time : %d:%d ", min, counter / 35);
	pDC->TextOut(250, 25, str);

	pDC->SetTextColor(RGB(205, 205, 10));
	char scores[40];
	sprintf(scores, "score: %d ", score);
	pDC->TextOut(20, 60, scores);

	pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
	//-------------------------------------------------------------------------
	//hits_left.ShowBitmap();
}


GameMap::GameMap()
{
	int map_init[24][27] = { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
							 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
					 		 { 0,0,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0 },
							 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0 },
				 			 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0 },
				  			 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0 },
							 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
							 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
							 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0 },
							 { 0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0 },
					 		 { 0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
							 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
							 { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
							 { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
							 { 0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
							 { 0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
							 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0 },
							 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
							 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1 },
							 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0 },
							 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0 },
							 { 1,1,1,0,0,0,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0 },
							 { 0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0 },
							 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } };
	for (int i = 0; i < 24; i++)
		for (int j = 0; j < 27; j++)
			map[i][j] = map_init[i][j];

	MW = MH = 50;
	sx = 100;
	sy = 400;
	Initialize();
}

int GameMap::GetX1()
{
	return x;
}

int GameMap::GetY1()
{
	return y;
}

int GameMap::GetX2()
{
	return x + animation.Width();
}

int GameMap::GetY2()
{
	return y + animation.Height();
}

void GameMap::SetX1(int setx)
{
	x = setx;
}

void GameMap::SetY1(int sety)
{
	y = sety;
}

void GameMap::Initialize()
{
	const int X_POS = -100;
	x = X_POS;
	isJumping=isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
	isDown= isUp=isHinder = true;

	const int INITIAL_VELOCITY = 15;	// ��l�W�ɳt��
	const int FLOOR = 1210;				// �a�O�y��
	floor = FLOOR;
	monsterfloor = floor-1240;
	y = FLOOR + 1;				// y�y�Ф�a�O��1�I(���b�a�O�W)
	rising = true;
	initial_velocity = INITIAL_VELOCITY;
	velocity = initial_velocity;
	foolvelocity = 1;
}

void GameMap::LoadBitmap()
{
	animation.AddBitmap(IDB_CorrectMap);
	blue.LoadBitmap(IDB_blue);
}

void GameMap::OnMove(CBall *Monseter)
{
	const int STEP_SIZE = 3;
	//animation.OnMove();

	if (!isHinder && isMovingLeft && x >= -240)
	{
		x -= STEP_SIZE;
		for (int i = 0; i < numbers; i++)
			Monseter[i].SetX(Monseter[i].GetX1() + STEP_SIZE);
	}
	if (!isHinder && isMovingRight && x <= 1030)
	{
		x += STEP_SIZE;
		for (int i = 0; i < numbers; i++)
			Monseter[i].SetX(Monseter[i].GetX1() - STEP_SIZE);
	}

	if (!isJumping )
	{
		if (isDown) {  // ��y�y���٨S�I��a�O
			y = floor;
			Monseter[0].SetY(monsterfloor);
			Monseter[1].SetY(monsterfloor + 50);
			Monseter[2].SetY(monsterfloor + 300);
			Monseter[3].SetY(monsterfloor - 350);
			Monseter[4].SetY(monsterfloor - 650);
			foolvelocity = 1; // ���]�W�ɪ�l�t��
		}
		else if(floor==1411) {
			y += foolvelocity;	// y�b�U��(����velocity���I�Avelocity����쬰 �I/��)
			for (int i = 0; i < numbers; i++)
				Monseter[i].SetY(Monseter[i].GetY1() - foolvelocity);
			foolvelocity++;		// �����O�v�T�A�U�����U���t�׼W�[
		}
	}
	



//////////////////////////////////////////////////////////////////////
	//if (isMovingUp && y <= 10)
	//	y += STEP_SIZE;
	//if (isMovingDown && y >= -floor)
	//	y -= STEP_SIZE;


}

void GameMap::SetMovingDown(bool flag)
{
	isMovingDown = flag;
}

void GameMap::SetMovingLeft(bool flag)
{
	isMovingLeft = flag;
}

void GameMap::SetMovingRight(bool flag)
{
	isMovingRight = flag;
}

void GameMap::SetMovingUp(bool flag)
{
	isMovingUp = flag;
}
void GameMap::SetJumping(bool flag)
{
	isJumping = flag;
}

void GameMap::SetHinder(bool flag)
{
	isHinder = flag;
}

void GameMap::SetUp(bool flag)
{
	isUp = flag;
}

void GameMap::SetDown(bool flag)
{
	isDown = flag;
}

void GameMap::SetXY(int nx, int ny)
{
	x = nx; y = ny;
}

void GameMap::SetNewFloor(int sfloor)
{
	floor = sfloor;
}

void GameMap::SetMonsterFloor(int sfloor)
{
	monsterfloor = sfloor;
}

void GameMap::SetNumber(int number)
{
	numbers = number;
}


void GameMap::OnShow()
{
	animation.SetTopLeft(-x,-y);
	animation.OnShow();

	//��ê���
	/*for (int i = 0; i < 24; i++)
		for (int j = 0; j < 27; j++)
		{
			int mx = i * 50-y+500 ;
			int my = j * 50 -x;
			switch (map[i][j]) {
			case 0:
				break;
			case 1:
				//blue.SetTopLeft(X + (MW*i), Y + (MH*j));
				blue.SetTopLeft(my,mx);
				blue.ShowBitmap();
				break;
			default:
				ASSERT(0);
			}
		}*/
}


void GameMap::OnJump(CBall *Monseter)
{		
	
	if (isJumping)
	{
		if (rising) {			// �W�ɪ��A
			if ( velocity > 0) {
				y -= velocity;	// ��t�� > 0�ɡAy�b�W��(����velocity���I�Avelocity����쬰 �I/��)
				for (int i = 0; i < numbers; i++)
					Monseter[i].SetY(Monseter[i].GetY1() + velocity);
				velocity--;		// �����O�v�T�A�U�����W�ɳt�׭��C
			}
			else {
				rising = false; // ��t�� <= 0�A�W�ɲפ�A�U���אּ�U��
				velocity = 1;	// �U������t(velocity)��1
			}
		}
		else {				// �U�����A
			if (y < floor ) {  // ��y�y���٨S�I��a�O
				y += velocity;	// y�b�U��(����velocity���I�Avelocity����쬰 �I/��)
				for (int i = 0; i < numbers; i++)
					Monseter[i].SetY(Monseter[i].GetY1() - velocity);
				velocity++;		// �����O�v�T�A�U�����U���t�׼W�[

				if (isDown)
				{
					y = floor;
					Monseter[0].SetY(floor - 1242);
					Monseter[1].SetY(floor - 1242 + 50);
					Monseter[2].SetY(floor - 1242 + 300);
					Monseter[3].SetY(floor - 1242 - 350);
					Monseter[4].SetY(floor - 1242 - 650);
				}
			}
			else {
				y = floor;  // ��y�y�ЧC��a�O�A�󥿬��a�O�W
				Monseter[0].SetY(floor - 1242);
				Monseter[1].SetY(floor - 1242 + 50);
				Monseter[2].SetY(floor - 1242 + 300);
				Monseter[3].SetY(floor - 1242 - 350);
				Monseter[4].SetY(floor - 1242 - 650);
				rising = true;	// �����ϼu�A�U���אּ�W��
				velocity = initial_velocity; // ���]�W�ɪ�l�t��
				isJumping = false;
			}
		}
	}
	
}

bool GameMap::IsEmpty(int salx, int saly)
{
	int gx = saly / 50;
	int gy = salx / 50;
	if (map[gx][gy] == 1)  //1����ê��
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////





}