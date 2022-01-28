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
      for (std::vector<Mass *>::iterator it = begin(mass_list); it != end (mass_list); ++it) {
        // position and radius
        Vector2 position = (*it)->getPosition();
        double r = (*it)->getRadius();
        
        // draw
        figure.drawCircle(position.x, position.y, r) ;
      }
      
      // draw spring
      for (std::vector<Spring>::iterator it = begin(spring_list); it != end (spring_list); ++it) {
        // position
        Mass * m1 = (*it).getMass1();
        Mass * m2 = (*it).getMass2();

        Vector2 p1 = m1->getPosition();
        Vector2 p2 = m2->getPosition();

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

  springmass.loadSample();
  
  run(&springmass, 1.0/240.0);

  // return 
  return 0 ;
}
