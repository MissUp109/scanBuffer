//
//  Float3.h
//  ScanLineZBuffer
//
//  Created by yy on 2017/12/24.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#ifndef Float3_h
#define Float3_h
#include <math.h>

class Float3{
public:
    union {
        struct{ float x, y, z; };
        struct{ float r, g, b; };
    };
    
    Float3(){};
    Float3(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    };
    
    inline Float3& operator = (const Float3 &v) { x = v.x; y = v.y; z = v.z; return *this; }
    
    inline Float3& operator += (float num) { x += num; y += num; z += num; return *this; }
    inline Float3& operator += (const Float3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
    
    inline Float3& operator -= (float num) { x -= num; y -= num; z -= num; return *this; }
    inline Float3& operator -= (const Float3 &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    
    inline Float3& operator *= (float num) { x *= num; y *= num; z *= num; return *this; }
    inline Float3& operator *= (const Float3 &v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
    
    inline Float3& operator /= (float num) { x /= num; y /= num; z /= num; return *this; }
    inline Float3& operator /= (const Float3 &v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
    
    inline bool operator == (const Float3 &u) {
        return (x - u.x == 0) && (y - u.y == 0) && (z - u.z == 0);
    }
    inline bool operator != (const Float3 &u) {
        return !((x - u.x == 0) && (y - u.y == 0) && (z - u.z == 0));
    }
    
    friend inline Float3 operator + (const Float3 &u, double num) {
        return Float3(u.x + num, u.y + num, u.z + num);
    }
    friend inline Float3 operator + (double num, const Float3 &u) {
        return Float3(num + u.x, num + u.y, num + u.z);
    }
    friend inline Float3 operator + (const Float3 &u, const Float3 &v) {
        return Float3(u.x + v.x, u.y + v.y, u.z + v.z);
    }
    friend inline Float3 operator - (const Float3 &u, double num) {
        return Float3(u.x - num, u.y - num, u.z - num);
    }
    friend inline Float3 operator - (double num, const Float3 &u) {
        return Float3(num - u.x, num - u.y, num - u.z);
    }
    friend inline Float3 operator - (const Float3 &u, const Float3 &v) {
        return Float3(u.x - v.x, u.y - v.y, u.z - v.z);
    }
    friend inline Float3 operator * (const Float3 &u, double num) {
        return Float3(u.x * num, u.y * num, u.z * num);
    }
    friend inline Float3 operator * (double num, const Float3 &u) {
        return Float3(num * u.x, num * u.y, num * u.z);
    }
    friend inline Float3 operator * (const Float3 &u, const Float3 &v) {
        return Float3(u.x * v.x, u.y * v.y, u.z * v.z);
    }
    friend inline Float3 operator / (const Float3 &u, double num) {
        return Float3(u.x / num, u.y / num, u.z / num);
    }
    friend inline Float3 operator / (double num, const Float3 &u) {
        return Float3(num / u.x, num / u.y, num / u.z);
    }
    friend inline Float3 operator / (const Float3 &u, const Float3 &v) {
        return Float3(u.x / v.x, u.y / v.y, u.z / v.z);
    }
};

inline Float3 cross(const Float3 &u, const Float3 &v)
{
    return Float3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

inline float dot(const Float3 &u, const Float3 &v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline float length(const Float3 &u)
{
    return sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}

inline Float3 normalize(const Float3 &u)
{
    return u * (1.0f / sqrt(u.x * u.x + u.y * u.y + u.z * u.z));
}

#endif /* Float3_h */
