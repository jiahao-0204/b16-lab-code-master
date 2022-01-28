/** file: ball.cpp
 ** brief: Ball class - implementation
 ** author: Andrea Vedaldi
 **/

#include "ball.h"

#include <iostream>

Ball::Ball(double _x, double _y, double _vx, double _vy) : r(0.1), g(9.8), m(1), xmin(-1), xmax(1), ymin(-1), ymax(1) {
  x = _x;
  y = _y;
  vx = _vx;
  vy = _vy;
 }

void Ball::step(double dt) {
  double xp = x + vx * dt ;
  double yp = y + vy * dt - 0.5 * g * dt * dt ;

  if (xmin + r <= xp && xp <= xmax - r) {
    x = xp ;
  } else {
    vx = -vx ;
  }

  if (ymin + r <= yp && yp <= ymax - r) {
    y = yp ;
    vy = vy - g * dt ;
  } else {
    vy = -vy ;
  }
}

void Ball::display() {
  std::cout<<x<<" "<<y<<std::endl ;
}
