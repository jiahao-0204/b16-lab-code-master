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

Mass::Mass(Vector3 position, Vector3 velocity, double mass, double radius) 
: position(position), velocity(velocity), force(), mass(mass), radius(radius), xmin(-1),xmax(1),ymin(-1),ymax(1),zmin(-1),zmax(1) {}

void Mass::setForce(Vector3 f) {
  force = f ;
}

void Mass::addForce(Vector3 f) {
  force = force + f ;
}

Vector3 Mass::getForce() const {
  return force ;
}

Vector3 Mass::getPosition() const {
  return position ;
}

Vector3 Mass::getVelocity() const {
  return velocity ;
}

double Mass::getRadius() const {
  return radius ;
}

double Mass::getMass() const {
  return mass ;
}

double Mass::getEnergy(double gravity) const {

  // potential energy
  double potential = mass * gravity * position.y;
  
  // kinetic energy 
  double kinetic = 0.5 * mass * velocity.norm2();

  // total energy
  double total = potential + kinetic;

  return total;

}

void Mass::step(double dt) {
  
  // new position and velocity
  // assuming constant acceleration
  Vector3 end_position = position + velocity * dt + 0.5 * force / mass * dt * dt; 
  Vector3 end_velocity = velocity + force / mass * dt; 

  // x direction
  if (xmin <= end_position.x - radius && end_position.x + radius <= xmax) {
    position.x = end_position.x;
    velocity.x = end_velocity.x;
  } else {
    velocity.x = - velocity.x;
  }

  // y direction
  if (ymin <= end_position.y - radius && end_position.y + radius <= ymax) {
    position.y = end_position.y;
    velocity.y = end_velocity.y;
  } else {
    velocity.y = - velocity.y;
  }

  // z direction
  if (zmin <= end_position.z - radius && end_position.z + radius <= zmax) {
    position.z = end_position.z;
    velocity.z = end_velocity.z;
  } else {
    velocity.z = - velocity.z;
  }
}

double Mass::getScaledR() {
  double z = position.z;

  double rmin = 0.5 * radius;
  double rmax = 1.5 * radius;

  double scaled_r = (z - zmin) / (zmax - zmin) * (rmax - rmin) + rmin;

  return scaled_r;
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

Vector3 Spring::getForce() const {
  // mass information
  Vector3 x1 = mass1 -> getPosition();
  Vector3 x2 = mass2 -> getPosition();
  Vector3 v1 = mass1 -> getVelocity();
  Vector3 v2 = mass2 -> getVelocity();

  // spring information
  double l = getLength();                   // spring length
  Vector3 u12 = 1/l * (x2 - x1);            // spring direction
  Vector3 v12 = dot((v2 - v1), u12) * u12;  // contraction/expansion speed in spring direction
  
  // forces
  Vector3 F1 = stiffness * (l - naturalLength) * u12;
  Vector3 F2 = damping * v12;
  Vector3 F = F1 + F2;

  return F ;
}

double Spring::getLength() const {
  Vector3 u = mass2->getPosition() - mass1->getPosition() ;
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
  Mass * m1 = new Mass(Vector3(-0.5,0,0), Vector3(0, 0, 0), mass, radius) ;
  Mass * m2 = new Mass(Vector3(+0.5,0,0), Vector3(1, 2, 0), mass, radius) ; // need destructor
  
  // spring
  const double naturalLength = 0;
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
    Vector3 position = (*it) -> getPosition();
    std::cout << position.x << " " << position.y << " " << position.z << " ";
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
  Vector3 g(0, -gravity, 0) ;
  for (std::vector<Mass *>::iterator it = mass_list.begin(); it != mass_list.end(); ++it) {
    (*it) -> setForce(g * (*it)->getMass());
  } 
  

  // get spring force and add force
  for (std::vector<Spring>::iterator it = spring_list.begin(); it != spring_list.end(); ++it) {
    // get mass
    Mass * mass1 = (*it).getMass1();
    Mass * mass2 = (*it).getMass2();

    // get force
    Vector3 F1 = (*it).getForce();
    Vector3 F2 = -1 * F1;
    
    // add force to mass
    mass1 -> addForce(F1);
    mass2 -> addForce(F2);
  }


  // update
  for (std::vector<Mass *>::iterator it = mass_list.begin(); it != mass_list.end(); ++it) {
    (*it) -> step(dt);
  } 
  
}
