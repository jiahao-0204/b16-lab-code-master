/** file: springmass.h
 ** brief: SpringMass simulation
 ** author: Andrea Vedaldi
 **/

#ifndef __springmass__
#define __springmass__

#include "simulation.h"

#include <cmath>
#include <vector>
#include <initializer_list>


#define MOON_GRAVITY 1.62
#define EARTH_GRAVITY 9.82

/* ---------------------------------------------------------------- */
// class Vector2
/* ---------------------------------------------------------------- */

class Vector3 {
  public:
    double x ;
    double y ;
    double z ;

    Vector3() : x(0), y(0), z(0) { }
    Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) { }
    double norm2() const { return x*x + y*y + z*z; }
    double norm() const { return std::sqrt(norm2()) ; }
  } ;

inline Vector3 operator+ (Vector3 a, Vector3 b) { return Vector3(a.x+b.x, a.y+b.y, a.z+b.z) ; }
inline Vector3 operator- (Vector3 a, Vector3 b) { return Vector3(a.x-b.x, a.y-b.y, a.z-b.z) ; }
inline Vector3 operator* (double a, Vector3 b)  { return Vector3(a*b.x, a*b.y, a*b.z) ; }
inline Vector3 operator* (Vector3 a, double b)  { return Vector3(a.x*b, a.y*b, a.z*b) ; }
inline Vector3 operator/ (Vector3 a, double b)  { return Vector3(a.x/b, a.y/b, a.z/b) ; }
inline double dot(Vector3 a, Vector3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }



/* ---------------------------------------------------------------- */
// class Mass
/* ---------------------------------------------------------------- */

class Mass {
  public:
    Mass() ;
    Mass(Vector3 position, Vector3 velocity, double mass, double radius) ;
    void setForce(Vector3 f) ;
    void addForce(Vector3 f) ;
    Vector3 getForce() const ;
    Vector3 getPosition() const ;
    Vector3 getVelocity() const ;
    double getMass() const ;
    double getRadius() const ;
    double getEnergy(double gravity) const ;
    void step(double dt) ;

    double getScaledR();

  protected:
    Vector3 position ;
    Vector3 velocity ;
    Vector3 force ;
    double mass ;
    double radius ;

    double xmin ;
    double xmax ;
    double ymin ;
    double ymax ;
    double zmin ;
    double zmax ;
} ;

/* ---------------------------------------------------------------- */
// class Spring
/* ---------------------------------------------------------------- */

class Spring {
  public:
    Spring(Mass * mass1, Mass * mass2, double naturalLength, double stiff, double damping = 0.01) ;
    Mass * getMass1() const ;
    Mass * getMass2() const ;
    Vector3 getForce() const ;
    double getLength() const ;
    double getEnergy() const ;
    double getStiffness() const;

  protected:

    Mass * mass1 ; 
    Mass * mass2 ;

    double naturalLength ;
    double stiffness ;
    double damping ; 

} ;

/* ---------------------------------------------------------------- */
// class SpringMass : public Simulation
/* ---------------------------------------------------------------- */

class SpringMass : public Simulation {
  public:
    // constructor
    SpringMass();

    // add elements
    void addSpring(std::vector<Spring> more_springs);
    void setGravity(double _gravity);
    
    // simulation
    void step(double dt) ;
    void display() ;

    // calculation
    double getEnergy() ;

    void loadSample();

  protected:

    std::vector<Spring> spring_list;
    std::vector<Mass * > mass_list;
    
    double gravity;
    
    void addMass(Spring);
} ;

#endif /* defined(__springmass__) */

