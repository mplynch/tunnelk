# Requirements #

(Terminology note: shall==threshold && should==objective)

The system shall implement a small-scale physical wind tunnel.

The system shall implement a virtual wind tunnel, also known as a
two-dimensional computational fluid dynamics flow solver.

The system shall provide a "big button" interface to the physical and virtual
wind tunnels.

The system shall include method of applying input wind tunnel conditions. For
example: air speed, angle of attack, temperature, et al.

The system shall include methods for tunnel operation feedback. For example:
time history plots, light-emitting diodes, et al.

The system shall be suitable to a range of children, teenagers, and young adults
in the capacity of visitors to a science center.

The system shall be resilient to power surges.

The wind tunnel system shall include reporting capabilities to select social networks.

The virtual wind tunnel system shall include reporting capabilities to select social networks.

The system shall supply automated earned value sensitivity vectors, both on
demand and spontaneously.

The system should provide a level of synchronization between the physical and
virtual wind tunnels. That is, applying conditions to the physical (or virtual)
wind tunnel should automatically apply conditions to the virtual (or physical)
tunnel.

The system should include a computational geometry input method similar to a whiteboard app to the virtual tunnel via modern hand held devices.

The project should produce designs, code, and lessons learned to replicate the
system by other teams.

The project shall provide durable child proof physical controls.

The hardware and software interfaces shall be highly reliable and intuitive, suitable for use by children.

The power on, power off, and reset procedures shall be simple enough to be performed by an untrained layperson.

The wind tunnel shall reset itself to a default initial state after a period of no input.

The hardware sensors and instruments used by the tunnel shall be off-the-shelf and easily obtainable a third party attempting to recreate the tunnel design.

The software components of the system shall be open source or freely available to a third party attempting to recreate the tunnel design.

The virtual wind tunnel should gracefully fail when given malformed inputs.

The virtual wind tunnel should run a complete visualization and display the outputs in less than 1 minute on a modern hand held device.

The virtual wind tunnel should provide outputs such as lift, drag, mach number, pressure, etc.


# Use Cases #

The aim for the use cases is to think in terms of exhibit flow then document in
terms of high-level things exhibit will do.

![http://tunnelk.googlecode.com/git/models/use-case-diagram.png](http://tunnelk.googlecode.com/git/models/use-case-diagram.png)

## Use case: Tunnel Configuration ##

  * (we need to reword this use case body, per discussions on 2011-09-20)
  * Administrator specifies desired sensors, actuators, etc.
  * Administrator assigns sensors/actuators to I/O channels
  * System begins calibration sequence
  * Administrator calibrates sensors, actuators, etc.

## Use case: Tunnel Startup ##

  * System is powered on
  * System receives initialize command
  * System initializes and turns on displays
  * System begins waiting for visitor commands

## Use case: Tunnel Shutdown ##

  * System receives shutdown command
  * System turns off displays and terminates background processes
  * System is powered off

## Use case: Loss of Power ##

  * System detects clean shutdown vs. power loss
  * System initializes normally from loss of power

## Use case: Timeout ##

  * System detects lack of user interaction for some period of time
  * System shuts down physical tunnel operations
  * System returns to initial state, waiting for the next visitor

## Use case: Perform Physical Tunnel Functions ##

  * Visitor approaches exhibit
  * Visitor tweaks physical tunnel parameters
    * air speed, angle of attack, etc.
  * Visitor presses big green go button
    * System responds by starting up
  * System shows time history plots of key tunnel parameters
  * Visitor presses big red stop button
    * System responds by shutting down the tunnel
  * Visitor presses tunnel "Share" button
    * System sends message to social network of choice with key results

## Use case: Perform Standalone Virtual Tunnel Functions ##

  * Visitor approaches exhibit
  * Visitor selects a virtual model shape
  * Visitor tweaks simulation input parameters
    * air speed, angle of attack, etc.
  * Visitor presses big green go button
    * System computes flow solution
    * System shows result of simulation
  * Visitor presses virual "Share" button
    * System sends message to social network of choice with key results

## Use Case: Perform Integrated Tunnel Functions ##

  * Visitor approaches exhibit
  * Visitor tweaks physical (or virtual) tunnel parameters
    * air speed, angle of attack, etc.
  * System matches the associated virtual (or physical) tunnel parameters
  * Visitor presses big green go button
    * System begins physical tunnel operation
    * System begins virtual tunnel operation
    * System shows time history plots with result comparisions
  * Visitor presses big red stop button
    * System responds by shutting down the tunnels
  * Visitor presses tunnel "Share" button
    * System sends message to social network of choice with key results

## Use Case: Time History Data Display ##

  * Visitor approaches exhibit
  * Visitor presses "time history" display button
  * System displays time history of various wind tunnel sensors, including a modifiable temperature sensor
  * Visitor tweaks temperature sensor by holding it to heat it up from room temperature
  * Visitor has the option to use this new temperature in the wind tunnel airspeed calculation to see demonstrate the effect of temperature on airspeed in the wind tunnel
  * Visitor presses "share" button
    * System sends message to social network of choice of time history graph