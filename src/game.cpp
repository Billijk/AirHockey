#include "game.h"
#include "constants.h"
#include <cstdio>

Game::Game() {
    m_mallet1.x = -0.3;
    m_mallet1.y = -0.3;
    m_puck.x = TABLE_WIDTH / 2;
    m_puck.y = TABLE_LENGTH / 2;
    v_puck.x = 0.0005f;
    v_puck.y = 0.0003f;
}

Game::~Game() {
}

bool Game::check_collide_wall(const Pos2d& obj, const float radius, Vec2d& objv) {
    // check left and right
    if ((obj.x - radius < 0 && objv.x < 0) || 
        (obj.x + radius > TABLE_WIDTH && objv.x > 0)) objv.x = -objv.x;
    // check top and bottom
    if ((obj.y - radius < 0 && objv.y < 0) || 
        (obj.y + radius > TABLE_LENGTH && objv.y > 0)) objv.y = -objv.y;
    return true;
}

bool Game::check_collide_objs(const Pos2d& obj1, const float radius1, Vec2d& objv1, 
                const Pos2d& obj2, const float radius2, Vec2d& objv2) {
    return true;
}
    

void Game::update_positions() {
    check_collide_wall(m_puck, PUCK_DIAMETER / 2, v_puck);
    m_puck += v_puck;
    //printf("Puck: x(%.3f), y(%.3f)", m_puck.x, m_puck.y);
}

void Game::moveMouse(float x, float y) {
}

void Game::update() {
    update_positions();
}