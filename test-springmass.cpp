/** file: test-srpingmass.cpp
 ** brief: Tests the spring mass simulation
 ** author: Andrea Vedaldi
 **/

#include "springmass.h"

int main(int argc, char** argv) {
  
  // mass
  const double mass = 0.1 ;
  const double radius = 0.2 ;
  Mass m1(Vector3(-0.5,0,0), Vector3(1, 0, 0), mass, radius) ;
  Mass m2(Vector3(+0.5,0,0), Vector3(0.5, 0, 0), mass, radius) ;
  
  // spring
  const double naturalLength = 1;
  const double stiff = 0;
  const double damping = 0;
  Spring spring1(&m1, &m2, naturalLength, stiff, damping) ;

  // spring vector
  std::vector<Spring> more_springs(1, spring1);


  // springmass
  SpringMass springmass;
  springmass.addSpring(more_springs);


  // simulation
  const double dt = 1.0/30 ;
  for (int i = 0 ; i < 400 ; ++i) {
    springmass.step(dt) ;
    springmass.display() ;
  }

  return 0 ;
}
