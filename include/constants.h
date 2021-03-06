#pragma once

const float EPS = 1e-8;
const float PI = 3.1415926536;

const int FRAME_WIDTH = 550;
const int FRAME_HEIGHT = 540;

const float TABLE_WIDTH = 2.0;
const float TABLE_LENGTH = 4.0;
const float GOAL_WIDTH = 0.8;
const float WALL_WIDTH = 0.1;
const float WALL_HEIGHT = 0.1;
const float PUCK_DIAMETER = 0.2;
const float PUCK_HEIGHT = 0.1;
const float MALLET_DIAMETER = 0.3;
const float MALLET_HEIGHT = 0.1;

const float MOUSE_SPEED_RATIO = 50;
const float VIEW_MOVE_SPEED = 0.1;

const float WALL_COLLISION_ENERGY_LOSS_RATIO = 0.95;
const float MALLET_HIT_ENERGY_LOSS_RATIO = 0.15;
const float MALLET_COLLISION_ENERGY_LOSS_RATIO = 0.65;
const float MAX_AI_SPEED_EASY = 0.005;
const float MAX_AI_SPEED = 0.009;

// ASCII字符总共只有0到127，一共128种字符
const int MAX_CHAR = 128;