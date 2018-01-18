#pragma once
#include "common.h"
#include <string>

enum GameState { WAIT, ON };
enum Difficulty { EASY = 30, HARD = 10, NIGHTMARE = 1 };

class Game {
private:
    Pos2d m_mallet1, m_mallet2, m_puck;
    int m_score1, m_score2;
    Vec2d v_mallet1, v_mallet2, v_puck;

    Pos2d current_mouse_pos;
    
    Difficulty m_difficulty;
    GameState m_state;

	bool puck_collide_wall(void);
    bool collide_wall(const Pos2d& obj, const float radius, Vec2d& objv);
    bool collide_free_mallet(const Pos2d& obj1, const float radius1, Vec2d& objv1, 
                const Pos2d& obj2, const float radius2, Vec2d& objv2);
    bool collide_controlled_mallet(const Pos2d& obj1, const float radius1, Vec2d& objv1, 
                const Pos2d& obj2, const float radius2, const Vec2d& objv2);
    
    void hard_check_positions(void);
    void check_goal(void);

    void update_positions(void);
    void AI_move(void);

public:
    Game();
    ~Game();

    int score1() { return m_score1; }
    int score2() { return m_score2; }
    Pos2d mallet1Pos() { return m_mallet1; }
    Pos2d mallet2Pos() { return m_mallet2; }
    Pos2d puckPos() { return m_puck; }
    GameState state() { return m_state; }
	std::string difficulty() { 
		switch (m_difficulty) {
		case EASY :
			return "EASY";
		case HARD :
			return "HARD";
		case NIGHTMARE :
			return "NIGHTMARE";}
	}

    void init();
	void clear_score();
    void moveMouse(float x, float y);	// 接收鼠标移动事件，更新mallet位置
    void update(void);					// 更新puck位置，更新对手位置，计算碰撞事件等……

    void difficulty_up(void);
    void difficulty_down(void);
};