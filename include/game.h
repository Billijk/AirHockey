#pragma once
#include "common.h"

enum GameState { ON, WIN, LOSE };

class Game {
private:
    Pos2d m_mallet1, m_mallet2, m_puck;
    int m_score1, m_score2;
	
    GameState m_state;

public:
    Game();
    ~Game();

    int score1() { return m_score1; }
    int score2() { return m_score2; }
    Pos2d mallet1Pos() { return m_mallet1; }
    Pos2d mallet2Pos() { return m_mallet2; }
    Pos2d puckPos() { return m_puck; }
    GameState state() { return m_state; }

    void moveMouse(float x, float y);	// ��������ƶ��¼�������malletλ��
    void update(void);					// ����puckλ�ã����¶���λ�ã�������ײ�¼��ȡ���
};