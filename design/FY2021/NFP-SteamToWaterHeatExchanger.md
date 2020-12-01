Adding Steam to Water Heat Exchanger to EnergyPlus
================

**Dareum Nam, National Renewable Energy Laboratory**

 - Original Date
 - Revision Date
 

## Justification for New Feature ##

Steam heating for hot water loops is common in university campuses and cities like New York. Often, steam energy is transferred at the building to a hot water system via a heat exchanger. Currently users can make both steam and hot water systems, but cannot link them together. This forces modelers to use HW boilers/systems to approximate the steam systems, which isn’t accurate and reduces confidence in the energy model.
The request for this new feature came from Bractlet. There have also been several upvotes on the new feature request from EnergyPlus Github.



## E-mail and  Conference Call Conclusions ##


## Overview ##

The current steam loop in EnergyPlus has five objects: Steam boiler, steam pipe, Steam to air coil, Steam baseboard radiator, and condensate pump. The steam loop has several assumptions that help simplify loop complexity and increase usability.
- The steam side of the loop operates on constant saturation pressure of steam
- The water side (condensate) of the loop operates at atmospheric pressure
- The steam loop is to operate at saturated conditions, no superheated condition
- The steam loop is assumed to have no transportation losses by friction and heat transfer with surroundings so that it maintains the quality of steam throughout the system constant value of 0 or 1
- The boiler operation is assumed capable to generate steam at quality equal to 1 every time and steam enters the coils at boiler outlet conditions
- Steam coils are designed with steam traps, which only allow condensed steam to leave the coil; hence the steam always condenses and leaves the coil at quality of 0

These assumptions can also be applied to the new module. 

## Approach ##

The algorithm of the steam to hot water heat exchanger module will be similar to the current steam to air coil module. The steam to hot water heat exchanger module will calculate the mass flow rate of steam desired to meet the water loop heating supply setpoint temperature. 
The load on the heat exchanger is calculated based on the desired water supply setpoint.



## Testing/Validation/Data Sources ##

insert text

## Input Output Reference Documentation ##

insert text

## Input Description ##

insert text

## Outputs Description ##

insert text

## Engineering Reference ##

insert text

## Example File and Transition Changes ##

insert text

## References ##

insert text



