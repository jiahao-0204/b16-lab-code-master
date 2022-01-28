# Task 1: Understand the implementation of the member functions.

**What is the initial state of the ball when the object is created (instantiated)?**
- the initial state can be found from the ball.cpp implementation file.
- r(0.1), x(0), y(0), vx(0.3), vy(-0.1), g(9.8), m(1), xmin(-1), xmax(1), ymin(-1), ymax(1)
- the implementation uses initialization list such that constant variables can be initialized.
- although there are no constant variables, gravity can be set as a constant, as it is unlikely to change once simulation started.


**How does display() prints information on the screen?**
- through the use of ostream 'cout << '.
- the display output x followed by y, for example: '10 20'. 


**The ball is constrained to bounce in the box [−1, 1] × [−1, 1]. How is this handled by the integrator implemented in step()? Is the total (kinetic + potential) energy of the ball conserved by this integrator?**
- the integrator integrates position from velocity and acceleration if the ball is within bound, if outside bound, velocity vector would simply flip its direction, mimic a bound action.
- total energy would be conserved, since no damping action.


**Now look at test-ball.cpp and answer the following question: What do you expect the output of your program to be?**
- it would be a stream of rows of x and y, for 100 lines.





# Task 2: Running the program.

**This will automatically compile and link the program to create and run the ball executable for you. A console window will pop up, displaying the output of the program. Does it look reasonable?**
- reasonable. 






# Task 3: Use redirection to save the textual output to a file. 


**Does the output seem reasonable?**
- the output curve is parabolic, which matches the prediction that energy is conserved. 
- the ball bounces at both the bottom and at the right.
- there are missing points close to the bottom as the ball crosses the boundary, hence velocity is simply reversed at the next time step.
- however, after setting xlim and ylim to [-1, 1], there seems to be some missing points with collision to the right wall. 
- After reviewing the code, this is due to the non zero radius of the ball. 



# Task 5: Member functions and separation of concerns.

**How should the class be changed so that a user could be able to get and set the position of the ball?**
- either move the double x and double y into public section
- or create Getter and Setter member function in public section



**The member functions of a class are often said to encode its “behaviour”. Can you find a practical example demonstrating why separating the data representation from its behaviour is useful?**
- if we want the ball to be bouncing around a circular object in free space (from gravity), it may be easier to have data representation in polar coordinates. 
- with the behaviour separated from the data representation, the end user can still operate on this object without worrying about all the different coordinates systems the ball is currently in.
- need modification here!

# Task 7: Programming principles.

**What is the distinction between a class declaration, definition, implementation, and instantiation?**
- class declaration is allocation of memory space to the variable defined, the memory size depends on the type the variable is.
- class definition is interface of the class stored in header file. it defines how other program should interact with it.
- class implementation is the behaviour of the class stored in cpp file. it defines the internal processing of the data.
- class instantiation is population of memory occupied by the class variable. default constructor is provided for every class defined.  



# note 
- to display value of the variable, add them in the WATCH tab.
- copy constructor seems to exist by default for custom class pointer. e.g. mass1(_mass)
- type safe language is really really good. it indicates an error as soon as it is typed. this stops the error from happening!
- can check if energy is conserved to see if simulation is running correctly
- visualization in MATLAB shows the ball bouncing higher and higher, something is wrong
- the error of increasing energy is because, 1/2 returns an integer which is 0, 1.0/2.0 returns 0.5. the former equation result in incorrect calculation of end position as well as incorrect kinetic energy.



# Task 10: Understanding interfaces.

**Can you replace the for loop in the main function defined in test-ball.cpp with the instruction run(ball, dt)?**
- yes, because ball is inherited from simulation, and has provided implementation of step and display.


**What would happen if Simulation did not declare step and display to be virtual? Would run still work as expected? If not, what would happen?**
- since input type for the function is 'simulation', calling step and display will call the member function from simulation class if they are not declared virtual.
- run function would not work as expected. 
- what happens depends on how step and display are implemented in simulation. 



# Task 18: Understand how this is implemented by using inheritance.

1. Which objects are of type Simulation and which of type Drawable? 
- BallDrawable is of type Ball which is of type Simulation. Thus BallDrawable is a simulation 
- BallDrawable is of type Drawable.


2. Which objects are creating an instance of Figure and when?
- BallDrawable creates an instance of Figure using initialization list when it is first constructed.


3. Which objects are added as Drawable to the figure and when?
- BallDrawable is added as Drawable to figure during its default construction. 

4. Which objects implement respectively: display, draw, and update? 
- display is a virtual function in simulation, which is later overwritten in Ball, and now overwritten in BallDrawable.  
- draw is implemented in both BallDrawable and in Figure. when draw() is called in figure, it loops over all objects it has and call the draw() function from those objects, in this case, the BallDrawable.
- update is implemented in Figure, which is called from display in BallDrawable. during 'run', BallDrawable is passed in as simulation, and its display() is called. since BallDrawable overwrites teh display() function, it instead calls update from Figure.


5. Which different purposes do these functions serve?
- BallDrawable() is the constructor of this class
- draw() calls the function in Figure to draw in figure. 
- display() calls the update() function in figure to update the drawing. 



# Task 21: Compute and display the energy of the spring-mass system.

**What happens to the energy as the simulation progresses? Why?**
- initially my implementation was incorrect. I used 1/2 instead of 1.0/2.0 when calculating kinetic energy as well as end position, this results in an fluctuating total energy (since kinetic is zero due to 1/2), and the mass is bouncing higher and higher (since end position formula is incorrect )
- after fixing this issue, the total energy accounting for the mass and the spring remains constant, when damping is set to zero. for non-zero damping in the spring, the total energy keeps decreasing. 
- there is however, a slight increase in the total energy. investigation shows this increase in energy occurs from the potential energy of springs. may be numerical error


**Now set the damping factor of the spring to zero. What happens now to the energy? Is this result correct? If not, what may be the problem?**
- answered above.



