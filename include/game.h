#pragma once
#include "common.h"

enum GameState { ON, WIN, LOSE };

class Game {
private:
    Pos2d m_mallet1, m_mallet2, m_puck;
    int m_score1, m_score2;
    int t;

    Pos2d current_mouse_pos;

    Vec2d v_mallet1, v_mallet2, v_puck;
	
    GameState m_state;

    bool collide_wall(const Pos2d& obj, const float radius, Vec2d& objv);
    bool collide_free_mallet(const Pos2d& obj1, const float radius1, Vec2d& objv1, 
                const Pos2d& obj2, const float radius2, Vec2d& objv2);
    bool collide_controlled_mallet(const Pos2d& obj1, const float radius1, Vec2d& objv1, 
                const Pos2d& obj2, const float radius2, const Vec2d& objv2);

    void update_positions();

public:
    Game();
    ~Game();

    int score1() { return m_score1; }
    int score2() { return m_score2; }
    Pos2d mallet1Pos() { return m_mallet1; }
    Pos2d mallet2Pos() { return m_mallet2; }
    Pos2d puckPos() { return m_puck; }
    GameState state() { return m_state; }

    void moveMouse(float x, float y);	// 接收鼠标移动事件，更新mallet位置
    void update(void);					// 更新puck位置，更新对手位置，计算碰撞事件等……
};