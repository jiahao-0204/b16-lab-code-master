# Task 1: Understand the implementation of the member functions.

**What is the initial state of the ball when the object is created (instantiated)?**
- the initial state can be found from the ball.cpp implementation file.
- r(0.1), x(0), y(0), vx(0.3), vy(-0.1), g(9.8), m(1), xmin(-1), xmax(1), ymin(-1), ymax(1)
- the implementation uses initialization list such that constant variables can be initialized.
- although there are no constant variables, gravity however, is a constant and maybe better if done so. 



**How does display() prints information on the screen?**
- the display output x followed by y, for example: '10 20'. 
- this is achieved through the use of ostream 'cout << '.


**The ball is constrained to bounce in the box [−1, 1] × [−1, 1]. How is this handled by the integrator implemented in step()? Is the total (kinetic + potential) energy of the ball conserved by this integrator?**
- the integrator integrates position from velocity if the ball is within bound, if outside bound, velocity vector would simply flip its direction, mimicing a bound action.
- total energy would be conserved, since only the sign of velocity is changed, the magnitude which represents the kinetic energy is unchanged.


**Now look at test-ball.cpp and answer the following question: • What do you expect the output of your program to be?**
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


# Task 10: Understanding interfaces.

**Can you replace the for loop in the main function defined in test-ball.cpp with the instruction run(ball, dt)?**
- yes, because ball is inherited from simulation, and has provided implementation of step and display.


**What would happen if Simulation did not declare step and display to be virtual? Would run still work as expected? If not, what would happen?**
- since input type for the function is 'simulation', calling step and display will call the member function from simulation class if they are not declared virtual.
- run function would not work as expected. 
- what happens depends on how step and display are implemented in simulation. 



