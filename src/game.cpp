#include "game.h"
#include "constants.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>

Game::Game() {
    reset();
}

Game::~Game() {
}

bool Game::puck_collide_wall() {
	if (m_puck.y - PUCK_DIAMETER / 2 < EPS && 
		m_puck.x - PUCK_DIAMETER / 2 > (TABLE_WIDTH - GOAL_WIDTH) / 2 &&
		m_puck.x + PUCK_DIAMETER / 2 < (TABLE_WIDTH + GOAL_WIDTH) / 2) {
			return false;
	}
	if (m_puck.y + PUCK_DIAMETER / 2 > TABLE_LENGTH - EPS && 
		m_puck.x - PUCK_DIAMETER / 2 > (TABLE_WIDTH - GOAL_WIDTH) / 2 &&
		m_puck.x + PUCK_DIAMETER / 2 < (TABLE_WIDTH + GOAL_WIDTH) / 2) {
			return false;
	}
	if ((m_puck.y - PUCK_DIAMETER / 2 < EPS && v_puck.y < 0) || 
        (m_puck.y + PUCK_DIAMETER / 2 > TABLE_LENGTH - EPS && v_puck.y > 0)) {
		v_puck.y = -v_puck.y * WALL_COLLISION_ENERGY_LOSS_RATIO;
	}
	if ((m_puck.x - PUCK_DIAMETER / 2 < EPS && v_puck.x < 0) || 
        (m_puck.x + PUCK_DIAMETER / 2 > TABLE_WIDTH - EPS && v_puck.x > 0)) {
		v_puck.x = -v_puck.x * WALL_COLLISION_ENERGY_LOSS_RATIO;
	}
    return false;
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
    Vec2d newv;
    if (objv2.norm() < EPS)
        newv = v1_ - norm12 * cos1 * v1_.norm() * (1 + MALLET_COLLISION_ENERGY_LOSS_RATIO);
    else
        newv = v1_ - norm12 * cos1 * v1_.norm() * (1 + MALLET_HIT_ENERGY_LOSS_RATIO);

    // translate back to normal coordinates
    objv1 = newv + objv2;

    return true;
}

void Game::hard_check_positions() {
    // first check puck and wall
    if (m_puck.x < PUCK_DIAMETER / 2) m_puck.x = PUCK_DIAMETER / 2;
    else if (m_puck.x > TABLE_WIDTH - PUCK_DIAMETER / 2) m_puck.x = TABLE_WIDTH - PUCK_DIAMETER / 2;
    if (m_puck.y < PUCK_DIAMETER / 2 && 
        (m_puck.x - PUCK_DIAMETER / 2 < (TABLE_WIDTH - GOAL_WIDTH) / 2 ||
		m_puck.x + PUCK_DIAMETER / 2 > (TABLE_WIDTH + GOAL_WIDTH) / 2)) 
        m_puck.y = PUCK_DIAMETER / 2;
    else if (m_puck.y > TABLE_LENGTH - PUCK_DIAMETER / 2 && 
        (m_puck.x - PUCK_DIAMETER / 2 < (TABLE_WIDTH - GOAL_WIDTH) / 2 ||
		m_puck.x + PUCK_DIAMETER / 2 > (TABLE_WIDTH + GOAL_WIDTH) / 2)) 
        m_puck.y = TABLE_LENGTH - PUCK_DIAMETER / 2;

    // then check mallet and puck
    if ((m_mallet1 - m_puck).norm() < (PUCK_DIAMETER + MALLET_DIAMETER) / 2)
        m_mallet1 = m_puck + (m_mallet1 - m_puck).unit() * (PUCK_DIAMETER + MALLET_DIAMETER) / 2;
    if ((m_mallet2 - m_puck).norm() < (PUCK_DIAMETER + MALLET_DIAMETER) / 2)
        m_mallet2 = m_puck + (m_mallet2 - m_puck).unit() * (PUCK_DIAMETER + MALLET_DIAMETER) / 2;
}

void Game::check_goal() {
    if (m_puck.y < 0) {
		m_score2 += 1;
		init();
	}
	if (m_puck.y > TABLE_LENGTH) {
		m_score1 += 1;
		init();
    }
}

void Game::update_positions() {
    // update object positions
    m_puck += v_puck;
    m_mallet1 += v_mallet1;
    m_mallet2 += v_mallet2;

    // calculate collisions
    puck_collide_wall();
    collide_wall(m_mallet2, MALLET_DIAMETER / 2, v_mallet2);
    
    collide_controlled_mallet(m_puck, PUCK_DIAMETER / 2, v_puck, m_mallet1, MALLET_DIAMETER / 2, v_mallet1);
    collide_controlled_mallet(m_puck, PUCK_DIAMETER / 2, v_puck, m_mallet2, MALLET_DIAMETER / 2, v_mallet2);    

    // deal with exceptions: force positions to be correct
    hard_check_positions();

    // update mallet1 velocity according to current mouse position
    v_mallet1 = (current_mouse_pos - m_mallet1) / MOUSE_SPEED_RATIO;

	// if goal update score
	check_goal();

}

void Game::moveMouse(float x, float y) {
    if (x < MALLET_DIAMETER / 2) x = MALLET_DIAMETER / 2 + EPS;
    else if (x > TABLE_WIDTH - MALLET_DIAMETER / 2) x = TABLE_WIDTH - MALLET_DIAMETER / 2 - EPS;
    if (y < MALLET_DIAMETER / 2) y = MALLET_DIAMETER / 2 + EPS;
    else if (y > TABLE_LENGTH / 2 - MALLET_DIAMETER / 2) y = TABLE_LENGTH / 2 - MALLET_DIAMETER / 2 - EPS;

    // save current valid mallet destination
    current_mouse_pos = Vec2d(x, y);
    m_state = ON;
}

void Game::AI_move() {
    // EASY: always chase the puck
    // HARD: add defence strategy
    // NIGHTMARE: move faster

    // limit mallet2 only move in self side
    if (m_mallet2.y < TABLE_LENGTH / 2 + MALLET_DIAMETER / 2) {
        if (v_mallet2.y < 0) v_mallet2.y = 0;
        m_mallet2.y = TABLE_LENGTH / 2 + MALLET_DIAMETER / 2 + EPS;
    }

    // only by a chance it can react
    if (rand() % (int)m_difficulty) return;

    // limit maximum speed w.r.t. difficulty
    float my_max_speed = m_difficulty == NIGHTMARE ? MAX_AI_SPEED : MAX_AI_SPEED_EASY;

    // different move pattern by difficulty
    if (m_difficulty == EASY) {
        v_mallet2 = m_puck - m_mallet2;
    }
    else {
        if (m_puck.y < TABLE_LENGTH / 2 - PUCK_DIAMETER / 2) {
            // if the puck is in the other half, stay back
            v_mallet2.y = (4 * TABLE_LENGTH / 5 - m_mallet2.y);
            v_mallet2.x = (m_puck.x - m_mallet2.x);
        } else {
            if (m_mallet2.y < m_puck.y + PUCK_DIAMETER / 2) {
                // if the puck is already behind the AI, try to defend
                if (m_puck.x > TABLE_WIDTH / 2)
                    v_mallet2 = Vec2d(TABLE_WIDTH / 2 + GOAL_WIDTH / 4, TABLE_LENGTH) - m_mallet2;
                else
                    v_mallet2 = Vec2d(TABLE_WIDTH / 2 - GOAL_WIDTH / 4, TABLE_LENGTH) - m_mallet2;
            }
            else {
                // else I go hit it
                v_mallet2 = m_puck - m_mallet2;
            }
        }
    }

    // limit maximum speed
    if (m_difficulty == NIGHTMARE || v_mallet2.norm() > my_max_speed)
        v_mallet2 = v_mallet2.unit() * my_max_speed;
}

void Game::init() {
    m_state = WAIT;
    m_mallet1.x = TABLE_WIDTH / 2;
    m_mallet1.y = TABLE_LENGTH / 5;
    m_mallet2.x = TABLE_WIDTH / 2;
    m_mallet2.y = TABLE_LENGTH / 5 * 4;
    m_puck.x = TABLE_WIDTH / 2;
    m_puck.y = TABLE_LENGTH / 2;
    v_puck *= 0;
}

void Game::update() {
    if (m_state == ON) {
        update_positions();
        AI_move();
    }
}

void Game::reset() {
    m_difficulty = EASY;
    m_score1 = m_score2 = 0;
    init();
}

void Game::difficulty_up() {
    if (m_difficulty == EASY) m_difficulty = HARD;
    else if (m_difficulty == HARD) m_difficulty = NIGHTMARE;
}

void Game::difficulty_down() {
    if (m_difficulty == HARD) m_difficulty = EASY;
    else if (m_difficulty == NIGHTMARE) m_difficulty = HARD;
}