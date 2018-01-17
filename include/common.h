#pragma once

#include "constants.h"
#include <cmath>

struct Vec2d {
    float x;
    float y;

    Vec2d(): x(0.f), y(0.f) {}
    Vec2d(float _x, float _y): x(_x), y(_y) {}

    friend Vec2d operator + (const Vec2d& a, const Vec2d& b) {
        return Vec2d(a.x + b.x, a.y + b.y);
    }
    friend Vec2d operator - (const Vec2d& a) {
        return Vec2d(-a.x, -a.y);
    }
    friend Vec2d operator - (const Vec2d& a, const Vec2d& b) {
        return Vec2d(a.x - b.x, a.y - b.y);
    }
    friend Vec2d operator * (const Vec2d& a, float b) {
        return Vec2d(a.x * b, a.y * b);
    }
    friend Vec2d operator * (const Vec2d& a, const Vec2d& b) {
        return Vec2d(a.x * b.x, a.y * b.y);
    }
    friend Vec2d operator / (const Vec2d& a, float b) {
        return Vec2d(a.x / b, a.y / b);
    }
    friend Vec2d operator / (const Vec2d& a, const Vec2d& b) {
        return Vec2d(a.x / b.x, a.y / b.y);
    }

    Vec2d operator +=(const Vec2d& a) {
        x += a.x; y += a.y;
        return *this;
    }
    Vec2d operator -=(const Vec2d& a) {
        x -= a.x; y -= a.y;
        return *this;
    }
    Vec2d operator *=(float a) {
        x *= a; y *= a;
        return *this;
    }
    Vec2d operator *=(const Vec2d& a) {
        x *= a.x; y *= a.y;
        return *this;
    }
    Vec2d operator /=(float a) {
        x /= a; y /= a;
        return *this;
    }
    Vec2d operator /=(const Vec2d& a) {
        x /= a.x; y /= a.y;
        return *this;
    }

    float norm() const {
        return sqrt(x * x + y * y) + EPS;
    }

    Vec2d unit() const {
        return (*this / norm());
    }

    // copy a new instance with the same values
    Vec2d copy() const {
        return Vec2d(x, y);
    }

    float dot(const Vec2d& a) const {
        return x * a.x + y * a.y;
    }
    float cross(const Vec2d& a) const {
        return fabs(x * a.y - y * a.x);
    }
};

typedef Vec2d Pos2d;