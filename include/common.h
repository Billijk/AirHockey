#pragma once

struct Pos2d {
    float x;
    float y;

    Pos2d(): x(0.f), y(0.f) {}
    Pos2d(float _x, float _y): x(_x), y(_y) {}

    friend Pos2d operator + (const Pos2d& a, const Pos2d& b) {
        return Pos2d(a.x + b.x, a.y + b.y);
    }

    Pos2d operator +=(const Pos2d& a) {
        x += a.x; y += a.y;
        return *this;
    }
};

typedef Pos2d Vec2d;