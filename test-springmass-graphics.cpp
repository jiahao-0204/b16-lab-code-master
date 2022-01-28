/** file: test-springmass-graphics.cpp
 ** brief: Tests the spring mass simulation with graphics
 ** author: Andrea Vedaldi
 **/

#include "graphics.h"
#include "springmass.h"

#include <iostream>
#include <sstream>
#include <iomanip>


class SpringMassDrawable : public SpringMass, public Drawable {

  private:
    Figure figure ;

  public:
    SpringMassDrawable() : figure("Spring Mass") {
      figure.addDrawable(this) ;
    }

    void draw() {

      // draw mass
      for (std::vector<Mass *>::iterator it = mass_list.begin() ; it != end (mass_list); ++it) {
        
        // position and radius
        Vector3 position = (*it)->getPosition();
        double x = position.x;
        double y = position.y;
        double z = position.z;

        double r_scaled = (*it)->getScaledR();
        
        // draw
        figure.drawCircle(x, y, r_scaled) ;
      }
      
      // draw spring
      for (std::vector<Spring>::iterator it = begin(spring_list); it != end (spring_list); ++it) {
        // position
        Mass * m1 = (*it).getMass1();
        Mass * m2 = (*it).getMass2();

        Vector3 p1 = m1->getPosition();
        Vector3 p2 = m2->getPosition();

        // thickness
        double thickness = (*it).getStiffness();
        
        // draw
        figure.drawLine(p1.x, p1.y, p2.x, p2.y, thickness) ;
      }


      // draw energy
      // x and y
      double x = 0;
      double y = 0;
      // energy  
      double energy = getEnergy();
      std::stringstream ss;
      ss << energy;
      std::string energy_str = ss.str();

      figure.drawString(x, y, energy_str);

    }

    void display() {
      figure.update() ;
    }
} ;



int main(int argc, char** argv) {
  // GLUT
  glutInit(&argc,argv);

  // run simulation
  SpringMassDrawable springmass;


  // springmass.loadSample();
  // mass
  const double mass = 1 ;
  const double radius = 0.1 ;
  Mass * m1 = new Mass(Vector3(-0.5,0,0), Vector3(0, 0, 0), mass, radius) ;
  Mass * m2 = new Mass(Vector3(+0.5,0,0), Vector3(1, 2, 0), mass, radius) ; // need destructor
  Mass * m3 = new Mass(Vector3(+0.5,0.5,0), Vector3(0, 0, 0), mass, radius) ; // need destructor
  
  // spring
  const double naturalLength = 0.5;
  const double stiff = 1;
  const double damping = 1;
  Spring spring1(m1, m2, naturalLength, 0, 0) ;
  Spring spring2(m2, m3, naturalLength, 1, 0) ;
  Spring spring3(m3, m1, naturalLength, 0, 0) ;

  // spring vector
  std::vector<Spring> more_springs(1, spring1);
  more_springs.push_back(spring2);
  more_springs.push_back(spring3);

  // springmass
  springmass.addSpring(more_springs);

  run(&springmass, 1.0/240.0);

  // return 
  return 0 ;
}
