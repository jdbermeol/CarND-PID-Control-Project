
# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---
## PID Control
PID controllers are simple reactive controllers that are widely used. The difference between the measured and the desired value (setpoint) of a process variable of a system is fed into the PID controller as an error signal. Depending on the PID parameters a control output is generated  to steer the system closer to the setpoint. In the present project, a car simulator produces the error signal as the distance between the actual car position on the road and a reference trajectory, known as cross-track error (cte). The PID controller is designed to minimize the distance to this reference trajectory. The primary control output of the PID controller here is the steering angle. 

### P - proportional gain 
The proportional term computes an output proportional to the cross-track error. A pure P - controller is unstable and at best oscillates about the setpoint. The proportional gain contributes a control output to the steering angle of the form  `-K_p cte` with a positive constant `K_p`.

### D - differential gain
The oscillations caused by purely D control can be mitigated by a term proportional to the derivative of the cross-track error.
The derivative gain contributes a control output of the form `-K_d d/dt cte`, with a positive constant `K_d`. 

### I - integral gain 
A third contribution is given by the integral gain which simply sums up the cross-track error over time. The corresponding contribution to the steering angle is given by `-K_i sum(cte)`. Thereby, biases can be mitigated, for instance if a zero steering angle does not correspond to a straight trajectory. At high speeds this term can also be useful to accumulate a large error signal quickly, for instance when the car is carried out sideways from the reference trajectory. This allows to reduce proportional gain, which causes oscillations at high speeds. It is also beneficial to limit the memory of this term to avoid overshooting. Here, we used an exponentially weighted moving average for this purpose. 

### Extensions
#### Linear parameter-varying control
Using a single PID controller can result in suboptimal performance at different speeds. We therefore implemented the possibility to accomodate different setpoints and to vary the parameters linearly according to the speed the car. For example the coefficient `K_i` is increased linearly as a function of the speed of the car: `K_i = alpha_i * v`. 

#### Smoothened steering angle
The output of the PID controller can result in jerky steering, causing the car to oscillate. A simple heuristic measure to mitigate this instability was to simply use a weighted average of the previous and the current steering angle. While this measure reduces reactivity, it makes the ride much smoother and allows to achieve higher speeds. 

#### Emergency braking
In case the cross-track error increase too quickly a simple emergency measure was to reduce throttle to zero for a few iterations. More severe measures like braking can also be implemented in the same way and lead to reduced cross-track errors at the cost of reduced speed. 

## Hyperparameter Tuning
Initially all parameters were tuned manually, keeping a constant acceleration. The reason for this approach was that slightly wrong parameters quickly lead to unrecoverable crashes of the car and require a manual restart of the simulator.

1. Single P controller for one setpoint, with `K_i` and `K_d` = 0
2. Increase `K_d` until oscillations subside. 
3. Reduce or increase `K_i` and `K_d` to improve performance.

Finally after an initial manual tunning is complete, twiddle algorithm is used for final tunning(Code was commented out on the final main.cpp). 

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 


