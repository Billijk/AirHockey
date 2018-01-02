#pragma once
#include "common.h"

enum GameState { ON, WIN, LOSE };

class Game {
private:
    Pos2d mallet1, mallet2, puck;
    int score1, score2;
	
	GameState state;

public:
	Game();
	~Game();

	int score1() { return score1; }
	int score2() { return score2; }
	Pos2d mallet1Pos() { return mallet1; }
	Pos2d mallet2Pos() { return mallet2; }
	Pos2d puckPos() { return puck; }
	GameState state() { return state; }

	void moveMouse(float x, float y);	// ��������ƶ��¼�������malletλ��
	void update(void);					// ����puckλ�ã����¶���λ�ã�������ײ�¼��ȡ���
};