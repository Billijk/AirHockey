#include "game.h"
#include "constants.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>

Game::Game() {
    m_mallet1.x = 0.4;
    m_mallet1.y = 0.9;
    m_mallet2.x = 0.9;
    m_mallet2.y = 3.0;
    m_puck.x = TABLE_WIDTH / 2;
    m_puck.y = TABLE_LENGTH / 2;
    v_puck.x = 0.0015f;
    v_puck.y = 0.0013f;
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
    // first check if collide
    if ((obj1 - obj2).norm() > (radius1 + radius2) + EPS) return false;

    // calculate new velocity for two objects
    Vec2d norm12 = (obj2 - obj1).unit();
    Vec2d norm21 = (obj1 - obj2).unit();
    float v1 = objv1.norm();
    float v2 = objv2.norm();
    
    /*  // toy mechanism
    objv1 = norm21 * v2;
    objv2 = norm12 * v1;*/

    // add oblique impact
    Vec2d v1_unit = objv1.unit();
    Vec2d v2_unit = objv2.unit();
    Vec2d d_v1 = Vec2d(norm12.y, norm12.x);
    Vec2d d_v2 = -d_v1;
    float cos1 = v1_unit.dot(norm12);
    float cos2 = v2_unit.dot(norm12);
    objv1 -= norm12 * (cos1 * v1 - cos2 * v2);
    objv2 -= norm12 * (cos2 * v2 - cos1 * v1);

    return true;
}

void Game::update_positions() {
    check_collide_wall(m_puck, PUCK_DIAMETER / 2, v_puck);
    check_collide_wall(m_mallet1, MALLET_DIAMETER / 2, v_mallet1);
    check_collide_wall(m_mallet2, MALLET_DIAMETER / 2, v_mallet2);
    
    check_collide_objs(m_puck, PUCK_DIAMETER / 2, v_puck, m_mallet1, MALLET_DIAMETER / 2, v_mallet1);
    check_collide_objs(m_puck, PUCK_DIAMETER / 2, v_puck, m_mallet2, MALLET_DIAMETER / 2, v_mallet2);
    check_collide_objs(m_mallet1, MALLET_DIAMETER / 2, v_mallet1, m_mallet2, MALLET_DIAMETER / 2, v_mallet2);

    // Randomly update mallet v
    /*if (rand() % 100000 == 0) {
        float x = float(rand() % 10 - 5) / 10000;
        float y = float(rand() % 10 - 5) / 10000;
        v_mallet1.x += x; v_mallet1.y += y;
    }
    if (rand() % 100000 == 0) {
        float x = float(rand() % 10 - 5) / 10000;
        float y = float(rand() % 10 - 5) / 10000;
        v_mallet2.x += x; v_mallet2.y += y;
    }*/
    
    m_puck += v_puck;
    m_mallet1 += v_mallet1;
    m_mallet2 += v_mallet2;
    //printf("Puck: x(%.3f), y(%.3f)", m_puck.x, m_puck.y);
}

void Game::moveMouse(float x, float y) {
}

void Game::update() {
    update_positions();
}