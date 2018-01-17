#include "game.h"
#include "constants.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>

Game::Game() {
    t = 0;
    m_mallet1.x = 0.4;
    m_mallet1.y = 0.9;
    m_mallet2.x = 0.9;
    m_mallet2.y = 3.0;
    m_puck.x = TABLE_WIDTH / 2;
    m_puck.y = TABLE_LENGTH / 2;
    v_mallet2.x = 0.0015f;
    v_mallet2.y = 0.0013f;
}

Game::~Game() {
}

bool Game::collide_wall(const Pos2d& obj, const float radius, Vec2d& objv) {
    bool collide_flag = false;
    // check left and right
    if ((obj.x - radius < EPS && objv.x < 0) || 
        (obj.x + radius > TABLE_WIDTH - EPS && objv.x > 0)) {
            collide_flag = true;
            objv.x = -objv.x * WALL_COLLISION_ENERGY_LOSS_RATIO;
    }
    // check top and bottom
    if ((obj.y - radius < EPS && objv.y < 0) || 
        (obj.y + radius > TABLE_LENGTH - EPS && objv.y > 0)) {
            collide_flag = true;
            objv.y = -objv.y * WALL_COLLISION_ENERGY_LOSS_RATIO;
    }
    return collide_flag;
}

bool Game::collide_free_mallet(const Pos2d& obj1, const float radius1, Vec2d& objv1, 
                const Pos2d& obj2, const float radius2, Vec2d& objv2) {
    // first check if collide
    float dist = (obj1 - obj2).norm();
    if (dist > (radius1 + radius2) + EPS) return false;
    // check if distance increase if stay on current situation
    if ((obj1 + objv1 - obj2 - objv2).norm() > dist) return false;

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
    objv1 = objv1 - norm12 * cos1 * v1 + norm12 * cos2 * v2 * MALLET_COLLISION_ENERGY_LOSS_RATIO;
    objv2 = objv2 - norm12 * cos2 * v2 + norm12 * cos1 * v1 * MALLET_COLLISION_ENERGY_LOSS_RATIO;

    return true;
}

bool Game::collide_controlled_mallet(const Pos2d& obj1, const float radius1, Vec2d& objv1, 
                const Pos2d& obj2, const float radius2, const Vec2d& objv2) {
    // first check if collide
    float dist = (obj1 - obj2).norm();
    if (dist > (radius1 + radius2) + EPS) return false;

    // check if distance increase if stay on current situation
    if ((obj1 + objv1 - obj2 - objv2).norm() > dist) return false;

    // translate to mallet coordinates
    Vec2d v1_ = objv1 - objv2;

    // calculate elastic collision
    Vec2d norm12 = (obj2 - obj1).unit();
    float cos1 = v1_.unit().dot(norm12);
    Vec2d newv = v1_ - norm12 * cos1 * v1_.norm() * (1 + MALLET_COLLISION_ENERGY_LOSS_RATIO);

    // translate back to normal coordinates
    objv1 = newv + objv2;

    return true;
}

void Game::update_positions() {
    collide_wall(m_puck, PUCK_DIAMETER / 2, v_puck);
    collide_wall(m_mallet2, MALLET_DIAMETER / 2, v_mallet2);
    // TODO: also regard mallet2 as controlled object (controlled by AI)
    
    collide_controlled_mallet(m_puck, PUCK_DIAMETER / 2, v_puck, m_mallet1, MALLET_DIAMETER / 2, v_mallet1);
    collide_free_mallet(m_puck, PUCK_DIAMETER / 2, v_puck, m_mallet2, MALLET_DIAMETER / 2, v_mallet2);
    collide_controlled_mallet(m_mallet2, MALLET_DIAMETER / 2, v_mallet2, m_mallet1, MALLET_DIAMETER / 2, v_mallet1);
    
    // update object positions
    //if (!collide_wall(m_puck, PUCK_DIAMETER / 2, v_puck.copy()))
    m_puck += v_puck;
    //else v_puck = Vec2d(0, 0);

    //if (!collide_controlled_mallet(m_puck, PUCK_DIAMETER / 2, v_puck.copy(), m_mallet1, MALLET_DIAMETER / 2, v_mallet1.copy()))
    m_mallet1 += v_mallet1;
    
    //if (!collide_controlled_mallet(m_puck, PUCK_DIAMETER / 2, v_puck.copy(), m_mallet2, MALLET_DIAMETER / 2, v_mallet2.copy()))
    m_mallet2 += v_mallet2;

    // update mallet1 velocity according to current mouse position
    v_mallet1 = (current_mouse_pos - m_mallet1) / MOUSE_SPEED_RATIO;
}

void Game::moveMouse(float x, float y) {
    if (x < MALLET_DIAMETER / 2) x = MALLET_DIAMETER / 2 + EPS;
    else if (x > TABLE_WIDTH - MALLET_DIAMETER / 2) x = TABLE_WIDTH - MALLET_DIAMETER / 2 - EPS;
    if (y < MALLET_DIAMETER / 2) y = MALLET_DIAMETER / 2 + EPS;
    else if (y > TABLE_WIDTH - MALLET_DIAMETER / 2) y = TABLE_WIDTH - MALLET_DIAMETER / 2 - EPS;

    // save current valid mallet destination
    current_mouse_pos = Vec2d(x, y);

}

void Game::update() {
    t ++;
    update_positions();
}