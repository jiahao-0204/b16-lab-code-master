/** file: springmass.cpp
 ** brief: SpringMass simulation implementation
 ** author: Andrea Vedaldi
 **/

#include "springmass.h"

#include <iostream>

/* ---------------------------------------------------------------- */
// class Mass
/* ---------------------------------------------------------------- */

Mass::Mass() : position(), velocity(), force(), mass(1), radius(1) {}

Mass::Mass(Vector2 position, Vector2 velocity, double mass, double radius) 
: position(position), velocity(velocity), force(), mass(mass), radius(radius), xmin(-1),xmax(1),ymin(-1),ymax(1) {}


void Mass::setForce(Vector2 f) {
  force = f ;
}

void Mass::addForce(Vector2 f) {
  force = force + f ;
}

Vector2 Mass::getForce() const {
  return force ;
}

Vector2 Mass::getPosition() const {
  return position ;
}

Vector2 Mass::getVelocity() const {
  return velocity ;
}

double Mass::getRadius() const {
  return radius ;
}

double Mass::getMass() const {
  return mass ;
}

double Mass::getEnergy(double gravity) const {
  double energy = 0;

  // potential energy
  double height = position.y - ymin;  
  energy += mass * gravity * height ;

  // kinetic energy 
  energy += 1/2 * mass * velocity.norm2();

  return energy;

}

void Mass::step(double dt) {
  // new position and velocity
  Vector2 position_pending = position + velocity * dt; //  + 1/2 * force / mass * dt * dt 
  Vector2 velocity_pending = velocity + force / mass * dt; 

  // x direction
  if (xmin <= position_pending.x - radius && position_pending.x + radius <= xmax) {

    position.x = position_pending.x;
    velocity.x = velocity_pending.x;

  } else {

    velocity.x = - velocity.x;

  }

  // y direction
  if (ymin <= position_pending.y - radius && position_pending.y + radius <= ymax) {
    position.y = position_pending.y;
    velocity.y = velocity_pending.y;
  } else {
    velocity.y = - velocity.y;
  }

}

/* ---------------------------------------------------------------- */
// class Spring
/* ---------------------------------------------------------------- */

Spring::Spring(Mass * mass1, Mass * mass2, double naturalLength, double stiffness, double damping)
: mass1(mass1), mass2(mass2),
naturalLength(naturalLength), stiffness(stiffness), damping(damping) {}

Mass * Spring::getMass1() const {
  return mass1 ;
}

Mass * Spring::getMass2() const {
  return mass2 ;
}

Vector2 Spring::getForce() const {
  // mass information
  Vector2 x1 = mass1 -> getPosition();
  Vector2 x2 = mass2 -> getPosition();
  Vector2 v1 = mass1 -> getVelocity();
  Vector2 v2 = mass2 -> getVelocity();

  // spring information
  double l = getLength();                   // spring length
  Vector2 u12 = 1/l * (x2 - x1);            // spring direction
  Vector2 v12 = dot((v2 - v1), u12) * u12;  // contraction/expansion speed in spring direction
  
  // forces
  Vector2 F1 = stiffness * (l - naturalLength) * u12;
  Vector2 F2 = damping * v12;
  Vector2 F = F1 + F2;

  return F ;
}

double Spring::getLength() const {
  Vector2 u = mass2->getPosition() - mass1->getPosition() ;
  return u.norm() ;
}

double Spring::getStiffness() const {
  return stiffness;
}


double Spring::getEnergy() const {
  double length = getLength() ;
  double dl = length - naturalLength;
  return 0.5 * stiffness * dl * dl ;
}


// just saw this, could have been useful when display results
std::ostream& operator << (std::ostream& os, const Mass& m) {
  os<<"("
  <<m.getPosition().x<<","
  <<m.getPosition().y<<")" ;
  return os ;
}

std::ostream& operator << (std::ostream& os, const Spring& s) {
  return os<<"$"<<s.getLength() ;
}














/* ---------------------------------------------------------------- */
// class SpringMass : public Simulation
/* ---------------------------------------------------------------- */

SpringMass::SpringMass() { 
  gravity = EARTH_GRAVITY;
}



void SpringMass::addSpring(std::vector<Spring> more_springs) {
  for (std::vector<Spring>::iterator it = begin(more_springs); it != end (more_springs); ++it) {
    // add springs
    spring_list.push_back(*it) ; 

    // add mass
    addMass(*it); 
  }
}

void SpringMass::addMass(Spring _spring) {
  Mass * mass1 = _spring.getMass1();
  Mass * mass2 = _spring.getMass2();

  // append if not present
  if ( std::find(mass_list.begin(), mass_list.end(), mass1) == mass_list.end() ) {
    mass_list.push_back(mass1);
  }
  if ( std::find(mass_list.begin(), mass_list.end(), mass2) == mass_list.end() ) {
    mass_list.push_back(mass2);
  }


}


void SpringMass::loadSample() {
  // mass
  const double mass = 1 ;
  const double radius = 0.1 ;
  Mass * m1 = new Mass(Vector2(-0.5,0), Vector2(0, 0), mass, radius) ;
  Mass * m2 = new Mass(Vector2(+0.5,0), Vector2(0, 0), mass, radius) ; // need destructor
  
  // spring
  const double naturalLength = 0.5;
  const double stiff = 0;
  const double damping = 0;
  Spring spring1(m1, m2, naturalLength, stiff, damping) ;

  // spring vector
  std::vector<Spring> more_springs(1, spring1);

  // springmass
  addSpring(more_springs);
}

void SpringMass::setGravity(double _gravity) {
  gravity = _gravity;
}

void SpringMass::display() {
  // multiple mass per line
  for (std::vector<Mass *>::iterator it = begin(mass_list); it != end (mass_list); ++it) {
    Vector2 position = (*it) -> getPosition();
    std::cout << position.x << " " << position.y << " ";
  } 
  // end line
  std::cout << std::endl;
}

double SpringMass::getEnergy() {
  double energy = 0 ;
  
  // mass
  for (std::vector<Mass *>::iterator it = mass_list.begin(); it != mass_list.end(); ++it) {
    energy += (*it)->getEnergy(gravity);
  }

  // spring
  for (std::vector<Spring>::iterator it = spring_list.begin(); it != spring_list.end(); ++it) {
    energy += (*it).getEnergy();
  }
  
  return energy ;
}

void SpringMass::step(double dt) {

  // set initial force 
  Vector2 g(0, -gravity) ;
  for (std::vector<Mass *>::iterator it = mass_list.begin(); it != mass_list.end(); ++it) {
    (*it) -> setForce(g * (*it)->getMass());
  } 
  

  // get spring force and add force
  for (std::vector<Spring>::iterator it = spring_list.begin(); it != spring_list.end(); ++it) {
    // get mass
    Mass * mass1 = (*it).getMass1();
    Mass * mass2 = (*it).getMass2();

    // get force
    Vector2 F1 = (*it).getForce();
    Vector2 F2 = -1 * F1;
    
    // add force to mass
    mass1 -> addForce(F1);
    mass2 -> addForce(F2);
  }


  // update
  for (std::vector<Mass *>::iterator it = mass_list.begin(); it != mass_list.end(); ++it) {
    (*it) -> step(dt);
  } 
  
}
