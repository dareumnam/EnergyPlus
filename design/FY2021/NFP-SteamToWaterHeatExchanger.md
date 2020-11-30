Adding Steam to Water Heat Exchanger to EnergyPlus
================

**Dareum Nam, National Renewable Energy Laboratory**

 - Original Date
 - Revision Date
 

## Justification for New Feature ##

Use of steam for heating hot water loops is common in university campuses and cities like New York. Often, steam energy is transferred at the building to a hot water system via a heat exchanger. Currently users can make both steam and hot water systems, but cannot link them together. This forces modelers to use HW boilers/systems to approximate the steam systems, which isn’t accurate and reduces confidence in the energy model.
The request for this new feature came from Bractlet. There has also been several upvotes on the new feature request from EnergyPlus Github.


## E-mail and  Conference Call Conclusions ##


## Overview ##

A new object "HeatExchanger:SteamToFluid" will be added
Current steam loop in EnergyPlus consists of five modules: Steam Boiler, Steam pipe, Stema to air coil, Steam baseboard radiator and condensate pump.
Assumptions from the current steam loop will be used
Steam source can be either steam boiler and district steam
Load Q will be decided by desired outlet water temperature (setpoint controlled)

## Approach ##

insert text

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



