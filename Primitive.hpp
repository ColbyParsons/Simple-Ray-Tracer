// Termm--Fall 2020

#pragma once

#include <glm/glm.hpp>
#include "A4.hpp"
#include <iostream>

using namespace std;

using namespace glm;

class Primitive {
public:
  int colour = 0;
  virtual ~Primitive();
  virtual bool check(vec3 & rayOrigin, vec3 & rayDirection, double & result) { return false; };
  virtual void update(vec3 & transform) { };
  virtual vec3 normal(vec3 & intersect) { return vec3(0.0,0.0,0.0); };
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  bool check(vec3 & rayOrigin, vec3 & rayDirection, double & result) override {
    return false;
  }
};

class Cube : public Primitive {
public:
  virtual ~Cube();
  bool check(vec3 & rayOrigin, vec3 & rayDirection, double & result) override {
    return false;
  }
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius), base_m_pos(pos)
  {
  }
  virtual ~NonhierSphere();
  bool check(vec3 & rayOrigin, vec3 & rayDirection, double & result) override {
    bool ret = getSphereIntercept(rayOrigin, rayDirection, m_pos, m_radius, result);
    return ret;
  }

  void update(vec3 & transform) override { 
    // cout << m_pos.x << ", " << m_pos.y << ", " << m_pos.z << endl;
    m_pos = base_m_pos + transform;
    // cout << m_pos.x << ", " << m_pos.y << ", " << m_pos.z << endl;
  };

  vec3 normal(vec3 & intersect) override { return normalize(intersect - m_pos); };

private:
  glm::vec3 m_pos;
  double m_radius;
  glm::vec3 base_m_pos;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  virtual ~NonhierBox();
  bool check(vec3 & rayOrigin, vec3 & rayDirection, double & result) override {
    vec3 i[12];
    vec3 p[8];
    p[0] = m_pos;
    p[1] = vec3(m_pos.x + m_size, m_pos.y         , m_pos.z);
    p[2] = vec3(m_pos.x         , m_pos.y + m_size, m_pos.z);
    p[3] = vec3(m_pos.x         , m_pos.y         , m_pos.z + m_size);
    p[4] = vec3(m_pos.x + m_size, m_pos.y + m_size, m_pos.z);
    p[5] = vec3(m_pos.x         , m_pos.y + m_size, m_pos.z + m_size);
    p[6] = vec3(m_pos.x + m_size, m_pos.y         , m_pos.z + m_size);
    p[7] = vec3(m_pos.x + m_size, m_pos.y + m_size, m_pos.z + m_size);
    i[0] = getTriangleIntercept(rayOrigin, rayDirection, p[0], p[1], p[4]);
    i[1] = getTriangleIntercept(rayOrigin, rayDirection, p[0], p[4], p[2]);
    i[2] = getTriangleIntercept(rayOrigin, rayDirection, p[1], p[6], p[7]);
    i[3] = getTriangleIntercept(rayOrigin, rayDirection, p[1], p[7], p[4]);
    i[4] = getTriangleIntercept(rayOrigin, rayDirection, p[3], p[0], p[2]);
    i[5] = getTriangleIntercept(rayOrigin, rayDirection, p[3], p[2], p[5]);
    i[6] = getTriangleIntercept(rayOrigin, rayDirection, p[3], p[6], p[1]);
    i[7] = getTriangleIntercept(rayOrigin, rayDirection, p[3], p[1], p[0]);
    i[8] = getTriangleIntercept(rayOrigin, rayDirection, p[2], p[4], p[7]);
    i[9] = getTriangleIntercept(rayOrigin, rayDirection, p[2], p[7], p[5]);
    i[10] = getTriangleIntercept(rayOrigin, rayDirection, p[6], p[3], p[5]);
    i[11] = getTriangleIntercept(rayOrigin, rayDirection, p[6], p[5], p[7]);
    //cout << "Beta: " << i[0].x << " Theta: " << i[0].y << endl;
    bool isHit = false;
    double minResult;
    for (int j = 0; j < 12; j++){

      if(i[j].x >= 0 && i[j].y >= 0 && (i[j].y + i[j].x) <= 1) {

        if ((!isHit || i[j].z < minResult) && i[j].z > 0) {
          minResult = i[j].z;
          isHit = true;
        }
      }  
    }
    result = minResult;
    vec3 intersect = rayOrigin + ((float)minResult)*rayDirection;

    // Texture mapping only on top side of cube
    int brickHeight = 20;
    int groutWidth = 4;
    int brickwidth = 40;
    int heightMod = (brickHeight + groutWidth);
    int widthMod = (brickwidth + groutWidth);
    // if top side
    if (abs(m_pos.y + m_size - intersect.y) < 0.01) {
      int val = abs(m_pos.x - intersect.x);
      int val2 = abs(m_pos.z - intersect.z);
      // used to alternate bricks
      if (val % (2 * heightMod) >= heightMod) {
        val2 += widthMod/2;
      }
      if (val % heightMod <= brickHeight && val % heightMod != 0 && val2 % widthMod <= brickwidth && val2 % widthMod != 0) {
        colour = 0;
      } else {
        colour = 1;
      }
    } else {
      colour = 0;
    }

    return isHit;
  }

  vec3 normal(vec3 & intersect) override { 
    vec3 norm = vec3(0.0,0.0,0.0);

    if (abs(m_pos.x - intersect.x) < 0.01) norm.x = -1;
    if (abs(m_pos.y - intersect.y) < 0.01) norm.y = -1;
    if (abs(m_pos.z - intersect.z) < 0.01) norm.z = -1;
    if (abs(m_pos.x + m_size - intersect.x) < 0.01) norm.x = 1;
    if (abs(m_pos.y + m_size - intersect.y) < 0.01) norm.y = 1;
    if (abs(m_pos.z + m_size - intersect.z) < 0.01) norm.z = 1;

    // Bump mapping only on top side of cube
    int brickHeight = 20;
    int groutWidth = 4;
    int brickwidth = 40;
    int heightMod = (brickHeight + groutWidth);
    int widthMod = (brickwidth + groutWidth);

    if (norm.y == 1) {
      int val = abs(m_pos.x - intersect.x);
      if (val % heightMod == brickHeight) {
        norm.x += 0.3;
      } else if (val % heightMod == brickHeight + 1) {
        norm.x += 0.15;
      } else if (val % heightMod == brickHeight + 2) {
        norm.x += 0.1;
      } else if (val % heightMod == heightMod - 2) {
        norm.x -= 0.1;
      } else if (val % heightMod == heightMod - 1) {
        norm.x -= 0.15;
      } else if (val % heightMod == 0) {
        norm.x -= 0.3;
      }

      int val2 = abs(m_pos.z - intersect.z);
      // used to alternate bricks
      if (val % (2 * heightMod) >= heightMod) {
        val2 += widthMod/2;
      }
      if (val2 % widthMod == brickwidth) {
        norm.z -= 0.3;
      } else if (val2 % widthMod == brickwidth + 1) {
        norm.z -= 0.15;
      } else if (val2 % widthMod == brickwidth + 2) {
        norm.z -= 0.1;
      } else if (val2 % widthMod == widthMod - 2) {
        norm.z += 0.1;
      } else if (val2 % widthMod == widthMod - 1) {
        norm.z += 0.15;
      } else if (val2 % widthMod == widthMod) {
        norm.z += 0.3;
      }
    }

    return normalize(norm);
  };

private:
  glm::vec3 m_pos;
  double m_size;
};
