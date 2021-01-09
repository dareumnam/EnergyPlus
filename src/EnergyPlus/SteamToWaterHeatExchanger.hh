// EnergyPlus, Copyright (c) 1996-2020, The Board of Trustees of the University of Illinois,
// The Regents of the University of California, through Lawrence Berkeley National Laboratory
// (subject to receipt of any required approvals from the U.S. Dept. of Energy), Oak Ridge
// National Laboratory, managed by UT-Battelle, Alliance for Sustainable Energy, LLC, and other
// contributors. All rights reserved.
//
// NOTICE: This Software was developed under funding from the U.S. Department of Energy and the
// U.S. Government consequently retains certain rights. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable,
// worldwide license in the Software to reproduce, distribute copies to the public, prepare
// derivative works, and perform publicly and display publicly, and to permit others to do so.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// (1) Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//
// (2) Redistributions in binary form must reproduce the above copyright notice, this list of
//     conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//
// (3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
//     the University of Illinois, U.S. Dept. of Energy nor the names of its contributors may be
//     used to endorse or promote products derived from this software without specific prior
//     written permission.
//
// (4) Use of EnergyPlus(TM) Name. If Licensee (i) distributes the software in stand-alone form
//     without changes from the version obtained under this License, or (ii) Licensee makes a
//     reference solely to the software portion of its product, Licensee must refer to the
//     software as "EnergyPlus version X" software, where "X" is the version number Licensee
//     obtained under this License and may not use a different name for the software. Except as
//     specifically required in this Section (4), Licensee shall not use in a company name, a
//     product name, in advertising, publicity, or other promotional activities any name, trade
//     name, trademark, logo, or other designation of "EnergyPlus", "E+", "e+" or confusingly
//     similar designation, without the U.S. Department of Energy's prior written consent.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef SteamToWaterHeatExchanger_hh_INCLUDED
#define SteamToWaterHeatExchanger_hh_INCLUDED

// ObjexxFCL Headers
#include <ObjexxFCL/Array1D.hh>
#include <ObjexxFCL/Optional.hh>

// EnergyPlus Headers
#include <EnergyPlus/Data/BaseData.hh>
#include <EnergyPlus/DataGlobals.hh>
#include <EnergyPlus/EnergyPlus.hh>
#include <EnergyPlus/Plant/PlantLocation.hh>
#include <EnergyPlus/PlantComponent.hh>

namespace EnergyPlus {

// Forward declarations
struct EnergyPlusData;

namespace SteamToWaterHeatExchanger {
       
    extern int NumberOfSteamToWaterHXs;

    struct PlantConnectionStruct : PlantLocation
    {
        // Members
        int inletNodeNum;                      // plant loop inlet node index
        int outletNodeNum;                     // plant loop outlet node index
        Real64 MassFlowRateMin;                // minimum (hardware) flow rate for component [kg/s]
        Real64 MassFlowRateMax;                // maximum (hardware) flow rate for component [kg/s]
        Real64 DesignVolumeFlowRate;           // design flow rate [m3/s]
        bool DesignVolumeFlowRateWasAutoSized; // true if design flow rate was autosize on input
        Real64 MyLoad;                         // current load request of supply equip for op scheme control[W]
        Real64 MinLoad;                        // reports back size for load dispatch routines [W]
        Real64 MaxLoad;                        // reports back size for load dispatch [W]
        Real64 OptLoad;                        // reports back size for load dispatch [W]
        Real64 InletTemp;                      // current inlet fluid temperature [C]
        Real64 InletMassFlowRate;              // current inlet mass flow rate [kg/s]
        Real64 OutletTemp;                     // component outlet temperature [C]

        // Default Constructor
        PlantConnectionStruct()
            : inletNodeNum(0), outletNodeNum(0), MassFlowRateMin(0.0), MassFlowRateMax(0.0), DesignVolumeFlowRate(0.0),
              DesignVolumeFlowRateWasAutoSized(false), MyLoad(0.0), MinLoad(0.0), MaxLoad(0.0), OptLoad(0.0), InletTemp(0.0), InletMassFlowRate(0.0),
              OutletTemp(0.0)
        {
        }
    };

    struct HeatExchangerSpecs : PlantComponent
    {
        // Members
        std::string Name;           // Name of the SteamCoil
        int AvailSchedNum;
        Real64 InletWaterMassFlowRate;      // MassFlow through the SteamCoil being Simulated [kg/s]
        Real64 OutletAirMassFlowRate;     // MassFlow throught the SteamCoil being Simulated[kg/s]
        Real64 InletAirTemp;              // Inlet Air Temperature Operating Condition [C]
        Real64 OutletAirTemp;             // Outlet Air Temperature Operating Condition [C]
        Real64 InletAirHumRat;            // Inlet Air Humidity Ratio Operating Condition
        Real64 OutletAirHumRat;           // Outlet Air Humidity Ratio Calculated Condition
        Real64 InletAirEnthalpy;          // Inlet Air enthalpy [J/kg]
        Real64 OutletAirEnthalpy;         // Outlet Air enthalpy [J/kg]
        Real64 TotSteamCoilLoad;          // Total Load on the Coil [W]
        Real64 SenSteamCoilLoad;          // Sensible Load on the Coil [W]
        Real64 TotHeatTransferEnergy;     // Total Heating Coil energy of the Coil [J]
        Real64 TotHeatTransferRate;       // Total Heating Coil Rate on the Coil [W]
        Real64 LoopLoss;                  // Loss in loop due to cond return to atm pressure
        Real64 TotSteamCoolingCoilRate;   // Total Cooling Coil Rate on the Coil [W]
        Real64 SenSteamCoolingCoilRate;   // Sensible Cooling Coil Rate on the Coil [W]
        Real64 LeavingRelHum;             // Simple Coil Latent Model requires User input for leaving RH
        Real64 DesiredOutletTemp;         // Temp desired at the outlet (C)
        Real64 DesiredOutletHumRat;       // Humudity Ratio desired at outlet (C)
        Real64 InletSteamTemp;            // Inlet Steam Temperature [C]
        Real64 OutletSteamTemp;           // Outlet Steam Temperature [C]
        Real64 InletSteamMassFlowRate;    // Inlet Steam Mass Flow Rate [Kg/s]
        Real64 OutletSteamMassFlowRate;   // Outlet Steam Mass Flow Rate [Kg/s]
        Real64 MaxSteamVolFlowRate;       // Maximum water Volume flow rate [m3/s]
        Real64 MaxSteamMassFlowRate;      // Maximum water mass flow rate [Kg/s]
        Real64 InletSteamEnthalpy;        // Inlet Water Enthalpy (J/Kg)
        Real64 OutletWaterEnthalpy;       // Outlet Water Enthalpy (J/kg)
        Real64 InletSteamPress;           // Pressure at steam inlet (Pa)
        Real64 InletSteamQuality;         // Quality of steam at inlet
        Real64 OutletSteamQuality;        // Quality of steam at outlet
        Real64 DegOfSubcooling;
        Real64 LoopSubcoolReturn;
        int WaterInletNodeNum;            // Inlet node number at air side
        int WaterOutletNodeNum;           // Outlet node number at air side
        int SteamInletNodeNum;          // SteamInletNodeNum
        int SteamOutletNodeNum;         // SteamOutletNodeNum
        int TempSetPointNodeNum;        // If applicable : node number that the temp setpoint exists.
        int FluidIndex;                 // Fluid index for FluidProperties (Steam)
        int LoopNum;                    // index for plant loop with steam coil
        int LoopSide;                   // index for plant loop side for steam coil
        int BranchNum;                  // index for plant branch for steam coil
        int CompNum;                    // index for plant component for steam coil
        int Coil_PlantTypeNum;          // plant level index for coil type
        bool MyOneTimeFlag;
        bool MyFlag;
        bool MyEnvrnFlag;


        // Default Constructor
        HeatExchangerSpecs()
            : OutletAirMassFlowRate(0.0), InletAirTemp(0.0), OutletAirTemp(0.0), InletAirHumRat(0.0), OutletAirHumRat(0.0), InletAirEnthalpy(0.0),
              OutletAirEnthalpy(0.0), TotSteamCoilLoad(0.0), SenSteamCoilLoad(0.0), TotHeatTransferEnergy(0.0), TotHeatTransferRate(0.0),
              LoopLoss(0.0), TotSteamCoolingCoilRate(0.0), SenSteamCoolingCoilRate(0.0),
              LeavingRelHum(0.0), DesiredOutletTemp(0.0), DesiredOutletHumRat(0.0), InletSteamTemp(0.0), OutletSteamTemp(0.0),
              InletSteamMassFlowRate(0.0), OutletSteamMassFlowRate(0.0), MaxSteamVolFlowRate(0.0), MaxSteamMassFlowRate(0.0), InletSteamEnthalpy(0.0),
              OutletWaterEnthalpy(0.0), InletSteamPress(0.0), InletSteamQuality(0.0), OutletSteamQuality(0.0), DegOfSubcooling(0.0),
              LoopSubcoolReturn(0.0), WaterInletNodeNum(0), WaterOutletNodeNum(0), SteamInletNodeNum(0), SteamOutletNodeNum(0), TempSetPointNodeNum(0), FluidIndex(0), LoopNum(0), LoopSide(0),
              BranchNum(0), CompNum(0), Coil_PlantTypeNum(0), MyOneTimeFlag(true), MyFlag(true), MyEnvrnFlag(true)
        {
        }

        static PlantComponent *factory(EnergyPlusData &state, std::string const &objectName);

        void onInitLoopEquip([[maybe_unused]] EnergyPlusData &state, const PlantLocation &calledFromLocation) override;

        void getDesignCapacities(
            EnergyPlusData &state, const PlantLocation &calledFromLocation, Real64 &MaxLoad, Real64 &MinLoad, Real64 &OptLoad) override;

        void simulate([[maybe_unused]] EnergyPlusData &state,
                      const PlantLocation &calledFromLocation,
                      bool FirstHVACIteration,
                      Real64 &CurLoad,
                      bool RunFlag) override;

        void setupOutputVars(EnergyPlusData &state);

        void initialize(EnergyPlusData &state);

        void size(EnergyPlusData &state);

        void calculate(EnergyPlusData &state, Real64 SupSideMdot, Real64 DmdSideMdot);

        void control(EnergyPlusData &state, int LoopNum, Real64 MyLoad, bool FirstHVACIteration);
    };

    // Object Data

    extern Array1D<HeatExchangerSpecs> SteamToWaterHX;

    void GetSteamToWaterHeatExchangerInput(EnergyPlusData &state);

    void clear_state();



} // namespace SteamToWaterHeatExchanger

struct SteamToWaterHeatExchangerData : BaseGlobalStruct {

    int const SteamCoil_AirHeating = 2;
    int const TemperatureSetPointControl = 1;
    int const ZoneLoadControl = 3;

    int SteamIndex = 0;
    int NumSteamCoils = 0; // The Number of SteamCoils found in the Input
    Array1D_bool MySizeFlag;
    Array1D_bool CoilWarningOnceFlag;
    Array1D_bool CheckEquipName;
    bool GetSteamCoilsInputFlag = true; // Flag set to make sure you get input once
    bool MyOneTimeFlag = true;          // one time initialization flag

    Array1D<SteamCoils::SteamToWaterHeatExchangerEquipConditions> SteamCoil;

    void clear_state() override
    {
        NumSteamCoils = 0;
        MyOneTimeFlag = true;
        GetSteamCoilsInputFlag = true;
        SteamCoil.deallocate();
        MySizeFlag.deallocate();
        CoilWarningOnceFlag.deallocate();
        CheckEquipName.deallocate();
    }

    // Default Constructor
    SteamCoilsData() = default;
};
} // namespace EnergyPlus

#endif
