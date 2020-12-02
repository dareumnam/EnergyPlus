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

![eq1](https://github.com/dareumnam/EnergyPlus/blob/SteamOverhaul/design/FY2021/eq1.PNG)

The steam mass flow rate required by the heat exchanger to meet the setpoint temperature is determined by the following equation.

![eq2](https://github.com/dareumnam/EnergyPlus/blob/SteamOverhaul/design/FY2021/eq2.PNG)

The control scheme uses a node setpoint.
The heat exchanger is simulated when the operating conditions are met. Calculate the heating load on the heat exchanger using the water loop heating supply setpoint and inlet water temperatures.

![eq3](https://github.com/dareumnam/EnergyPlus/blob/SteamOverhaul/design/FY2021/eq3.PNG)

In case the heating load is negative (the setpoint is below the water inlet temperature), the heat exchanger operation needs to be shut off.

![eq4](https://github.com/dareumnam/EnergyPlus/blob/SteamOverhaul/design/FY2021/eq4.PNG)

If the load is greater than the maximum heat exchanger load calculated at the maximum steam mass flow rate(user input), the heat exchanger can only deliver to the water loop its maximum heating capacity. In this case, the water outlet temperature will be below the setpoint. The water outlet temperature and the mass flow rate of steam are determined based on this maximum heat transfer value. If the mass flow rate of steam is greater than what the splitter can provide to the heat exchanger, set the mass flow rate to the value delivered by the splitter. The heat exchanger heating capacity and the water outlet temperature are recalculated with the steam mass flow rate by the splitter.

![eq5](https://github.com/dareumnam/EnergyPlus/blob/SteamOverhaul/design/FY2021/eq5.PNG)

If the above two IF and Else block are not true, then the heat exchanger can meet the required setpoint temperature.

![eq6](https://github.com/dareumnam/EnergyPlus/blob/SteamOverhaul/design/FY2021/eq6.PNG)

## Testing/Validation/Data Sources ##

TBD

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



