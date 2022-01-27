/** file: test-springmass-graphics.cpp
 ** brief: Tests the spring mass simulation with graphics
 ** author: Andrea Vedaldi
 **/

#include "graphics.h"
#include "springmass.h"

#include <iostream>
#include <sstream>
#include <iomanip>

/* ---------------------------------------------------------------- */
class SpringMassDrawable : public SpringMass, public Drawable
/* ---------------------------------------------------------------- */
{

/* INCOMPLETE: TYPE YOUR CODE HERE */

} ;

int main(int argc, char** argv)
{
  glutInit(&argc,argv) ;

  SpringMassDrawable springmass ;


/* INCOMPLETE: TYPE YOUR CODE HERE */

  run(&springmass, 1/120.0) ;
}
