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

// C++ Headers
#include <cmath>

// ObjexxFCL Headers
#include <ObjexxFCL/Array.functions.hh>
#include <ObjexxFCL/Fmath.hh>

// EnergyPlus Headers
#include <EnergyPlus/Autosizing/Base.hh>
#include <EnergyPlus/BranchNodeConnections.hh>
#include <EnergyPlus/Data/EnergyPlusData.hh>
#include <EnergyPlus/DataContaminantBalance.hh>
#include <EnergyPlus/DataEnvironment.hh>
#include <EnergyPlus/DataHVACGlobals.hh>
#include <EnergyPlus/DataLoopNode.hh>
#include <EnergyPlus/DataPrecisionGlobals.hh>
#include <EnergyPlus/DataSizing.hh>
#include <EnergyPlus/EMSManager.hh>
#include <EnergyPlus/FluidProperties.hh>
#include <EnergyPlus/General.hh>
#include <EnergyPlus/GeneralRoutines.hh>
#include <EnergyPlus/GlobalNames.hh>
#include <EnergyPlus/InputProcessing/InputProcessor.hh>
#include <EnergyPlus/NodeInputManager.hh>
#include <EnergyPlus/OutputProcessor.hh>
#include <EnergyPlus/OutputReportPredefined.hh>
#include <EnergyPlus/Plant/DataPlant.hh>
#include <EnergyPlus/PlantUtilities.hh>
#include <EnergyPlus/Psychrometrics.hh>
#include <EnergyPlus/ScheduleManager.hh>
#include <EnergyPlus/TempSolveRoot.hh>
#include <EnergyPlus/SteamToWaterHeatExchanger.hh>
#include <EnergyPlus/UtilityRoutines.hh>

namespace EnergyPlus {

namespace SteamToWaterHeatExchanger {



    //using namespace DataLoopNode;
    //using namespace DataHVACGlobals;
    //using namespace Psychrometrics;
    //using namespace FluidProperties;
    //using DataPlant::PlantLoop;
    //using DataPlant::TypeOf_SteamToWaterHeatExchanger;
    //using PlantUtilities::MyPlantSizingIndex;
    //using PlantUtilities::ScanPlantLoopsForObject;
    //using namespace ScheduleManager;

    std::string const ComponentClassName("HeatExchanger:SteamToWater");
    static std::string const fluidNameSteam("STEAM");

    int NumberOfSteamToWaterHXs(0);
    bool GetInput(true);
    Array1D<HeatExchangerSpecs> SteamToWaterHX;
    Array1D_bool CheckSteamToWaterHXs;

    void clear_state()
    {
        NumberOfSteamToWaterHXs = 0;
        GetInput = true;
        SteamToWaterHX.deallocate();
        CheckSteamToWaterHXs.deallocate();
    }

    PlantComponent *HeatExchangerSpecs::factory(EnergyPlusData &state, std::string const &objectName)
    {
        // Process the input data for heat exchangers if it hasn't been done already
        if (GetInput) {
            GetSteamToWaterHeatExchangerInput(state);
            GetInput = false;
        }
        // Now look for this particular object
        for (auto &obj : SteamToWaterHX) {
            if (obj.Name == objectName) {
                return &obj;
            }
        }
        // If we didn't find it, fatal
        ShowFatalError(state, "LocalSteamToWaterHXFactory: Error getting inputs for object named: " + objectName); // LCOV_EXCL_LINE
        // Shut up the compiler
        return nullptr; // LCOV_EXCL_LINE
    }

    void HeatExchangerSpecs::onInitLoopEquip(EnergyPlusData &state, const PlantLocation &calledFromLocation)
    {
        this->initialize(state);
    }

    void HeatExchangerSpecs::getDesignCapacities(EnergyPlusData &state, const PlantLocation &calledFromLocation, Real64 &MaxLoad, Real64 &MinLoad, Real64 &OptLoad)
    {
    }

    void HeatExchangerSpecs::simulate(
        EnergyPlusData &state, const PlantLocation &calledFromLocation, bool FirstHVACIteration, Real64 &CurLoad, bool RunFlag)
    {
    }

    void GetSteamToWaterHeatExchangerInput(EnergyPlusData &state)
    {

        static std::string const RoutineName("GetSteamToWaterHeatExchangerInput: ");

        bool ErrorsFound(false);
        int NumAlphas;        // Number of elements in the alpha array
        int NumNums;          // Number of elements in the numeric array
        int IOStat;           // IO Status when calling get input subroutine
        int MaxNumAlphas(0);  // argument for call to GetObjectDefMaxArgs
        int MaxNumNumbers(0); // argument for call to GetObjectDefMaxArgs
        int TotalArgs(0);     // argument for call to GetObjectDefMaxArgs
        Array1D_string cAlphaFieldNames;
        Array1D_string cNumericFieldNames;
        Array1D_bool lNumericFieldBlanks;
        Array1D_bool lAlphaFieldBlanks;
        Array1D_string cAlphaArgs;
        Array1D<Real64> rNumericArgs;
        std::string cCurrentModuleObject;

        cCurrentModuleObject = "HeatExchanger:SteamToWater";

        NumberOfSteamToWaterHXs = inputProcessor->getNumObjectsFound(state, cCurrentModuleObject);
        if (NumberOfSteamToWaterHXs == 0) return;

        inputProcessor->getObjectDefMaxArgs(state, cCurrentModuleObject, TotalArgs, NumAlphas, NumNums);
        MaxNumNumbers = NumNums;
        MaxNumAlphas = NumAlphas;

        cAlphaFieldNames.allocate(MaxNumAlphas);
        cAlphaArgs.allocate(MaxNumAlphas);
        lAlphaFieldBlanks.dimension(MaxNumAlphas, false);
        cNumericFieldNames.allocate(MaxNumNumbers);
        rNumericArgs.dimension(MaxNumNumbers, 0.0);
        lNumericFieldBlanks.dimension(MaxNumNumbers, false);

        if (NumberOfSteamToWaterHXs > 0) {
            SteamToWaterHX.allocate(NumberOfSteamToWaterHXs);
            CheckSteamToWaterHXs.dimension(NumberOfSteamToWaterHXs, true);
            for (int CompLoop = 1; CompLoop <= NumberOfSteamToWaterHXs; ++CompLoop) {
                inputProcessor->getObjectItem(state,
                                              cCurrentModuleObject,
                                              CompLoop,
                                              cAlphaArgs,
                                              NumAlphas,
                                              rNumericArgs,
                                              NumNums,
                                              IOStat,
                                              lNumericFieldBlanks,
                                              lAlphaFieldBlanks,
                                              cAlphaFieldNames,
                                              cNumericFieldNames);
                UtilityRoutines::IsNameEmpty(state, cAlphaArgs(1), cCurrentModuleObject, ErrorsFound);

                SteamToWaterHX(CompLoop).Name = cAlphaArgs(1);

                if (lAlphaFieldBlanks(2)) {
                    SteamToWaterHX(CompLoop).AvailSchedNum = DataGlobalConstants::ScheduleAlwaysOn;
                } else {
                    SteamToWaterHX(CompLoop).AvailSchedNum = ScheduleManager::GetScheduleIndex(state, cAlphaArgs(2));
                    if (SteamToWaterHX(CompLoop).AvailSchedNum <= 0) {
                        ShowSevereError(state, RoutineName + cCurrentModuleObject + "=\"" + cAlphaArgs(1) + "\", invalid entry.");
                        ShowContinueError(state, "Invalid " + cAlphaFieldNames(2) + " = " + cAlphaArgs(2));
                        ShowContinueError(state, "Schedule was not found ");
                        ErrorsFound = true;
                    }
                }

                SteamToWaterHX(CompLoop).DemandSideOfSteamLoop.inletNodeNum = NodeInputManager::GetOnlySingleNode(state,
                                                                                                 cAlphaArgs(3),
                                                                                                 ErrorsFound,
                                                                                                 cCurrentModuleObject,
                                                                                                 cAlphaArgs(1),
                                                                                                 DataLoopNode::NodeType_Steam,
                                                                                                 DataLoopNode::NodeConnectionType_Inlet,
                                                                                                 2,
                                                                                                 DataLoopNode::ObjectIsNotParent);
                SteamToWaterHX(CompLoop).DemandSideOfSteamLoop.outletNodeNum = NodeInputManager::GetOnlySingleNode(state,
                                                                                                  cAlphaArgs(4),
                                                                                                  ErrorsFound,
                                                                                                  cCurrentModuleObject,
                                                                                                  cAlphaArgs(1),
                                                                                                  DataLoopNode::NodeType_Steam,
                                                                                                  DataLoopNode::NodeConnectionType_Outlet,
                                                                                                  2,
                                                                                                  DataLoopNode::ObjectIsNotParent);
                BranchNodeConnections::TestCompSet(
                    state, cCurrentModuleObject, cAlphaArgs(1), cAlphaArgs(3), cAlphaArgs(4), "Steam Loop Plant Nodes");

                SteamToWaterHX(CompLoop).SupplySideOfWaterLoop.inletNodeNum = NodeInputManager::GetOnlySingleNode(state,
                                                                                                 cAlphaArgs(5),
                                                                                                 ErrorsFound,
                                                                                                 cCurrentModuleObject,
                                                                                                 cAlphaArgs(1),
                                                                                                 DataLoopNode::NodeType_Water,
                                                                                                 DataLoopNode::NodeConnectionType_Inlet,
                                                                                                 1,
                                                                                                 DataLoopNode::ObjectIsNotParent);
                SteamToWaterHX(CompLoop).SupplySideOfWaterLoop.outletNodeNum = NodeInputManager::GetOnlySingleNode(state,
                                                                                                  cAlphaArgs(6),
                                                                                                  ErrorsFound,
                                                                                                  cCurrentModuleObject,
                                                                                                  cAlphaArgs(1),
                                                                                                  DataLoopNode::NodeType_Water,
                                                                                                  DataLoopNode::NodeConnectionType_Outlet,
                                                                                                  1,
                                                                                                  DataLoopNode::ObjectIsNotParent);
                BranchNodeConnections::TestCompSet(
                    state, cCurrentModuleObject, cAlphaArgs(1), cAlphaArgs(5), cAlphaArgs(6), "Water Loop Plant Nodes");

                SteamToWaterHX(CompLoop).TempSetPointNodeNum = NodeInputManager::GetOnlySingleNode(state,
                                                                                                   cAlphaArgs(7),
                                                                                                   ErrorsFound,
                                                                                                   cCurrentModuleObject,
                                                                                                   cAlphaArgs(1),
                                                                                                   DataLoopNode::NodeType_Water,
                                                                                                   DataLoopNode::NodeConnectionType_Sensor,
                                                                                                   1,
                                                                                                   DataLoopNode::ObjectIsNotParent);
                if (DataLoopNode::Node(SteamToWaterHX(CompLoop).TempSetPointNodeNum).TempSetPoint == DataLoopNode::SensedNodeFlagValue) {
                    if (!state.dataGlobal->AnyEnergyManagementSystemInModel) {
                        ShowSevereError(state, RoutineName + " Missing temperature setpoint for DataLoopNode::Node = " + cAlphaArgs(9));
                        ShowContinueError(state, "Occurs for " + cCurrentModuleObject + "=\"" + cAlphaArgs(1));
                        ShowContinueError(state, " Use a setpoint manager to place a single temperature setpoint on the node");
                        ErrorsFound = true;
                    } else {
                        // need call to EMS to check node
                        bool NodeEMSSetPointMissing = false;
                        EMSManager::CheckIfNodeSetPointManagedByEMS(state,
                                                                    SteamToWaterHX(CompLoop).TempSetPointNodeNum,
                                                                    EMSManager::SPControlType::iTemperatureSetPoint,
                                                                    NodeEMSSetPointMissing);
                        if (NodeEMSSetPointMissing) {
                            ShowSevereError(state, RoutineName + " Missing temperature setpoint for node = " + cAlphaArgs(9));
                            ShowContinueError(state, "Occurs for " + cCurrentModuleObject + "=\"" + cAlphaArgs(1));
                            ShowContinueError(state, "Use a setpoint manager or EMS actuator to place a single temperature setpoint on the node");
                            ErrorsFound = true;
                        }
                    }
                }

                SteamToWaterHX(CompLoop).DemandSideOfSteamLoop.VolFlowRateMax = rNumericArgs(1);
                SteamToWaterHX(CompLoop).DegOfSubcooling = rNumericArgs(2);
                SteamToWaterHX(CompLoop).LoopSubcoolReturn = rNumericArgs(3);
            }
        }

    }

    void HeatExchangerSpecs::setupOutputVars(EnergyPlusData &state)
    {
        SetupOutputVariable(state,
                            "Steam To Water Heat Exchanger Heat Transfer Energy",
                            OutputProcessor::Unit::J,
                            this->TotHeatTransferEnergy,
                            "System",
                            "Sum",
                            this->Name,
                            _,
                            "ENERGYTRANSFER",
                            "HEATINGCOILS",
                            _,
                            "System");
        SetupOutputVariable(state,
                            "Steam to Water Heat Exchanger Heat Transfer Rate",
                            OutputProcessor::Unit::W,
                            this->TotHeatTransferRate,
                            "System",
                            "Average",
                            this->Name);
        SetupOutputVariable(state,
                            "Steam to Water Heat Exchanger Steam Mass Flow Rate",
                            OutputProcessor::Unit::kg_s,
                            this->OutletSteamMassFlowRate,
                            "System",
                            "Average",
                            this->Name);
        SetupOutputVariable(state,
                            "Steam to Water Heat Exchanger Steam Inlet Temperature",
                            OutputProcessor::Unit::C,
                            this->InletSteamTemp,
                            "System",
                            "Average",
                            this->Name);
        SetupOutputVariable(state,
                            "Steam to Water Heat Exchanger Steam(Condensate) Outlet Temperature",
                            OutputProcessor::Unit::C,
                            this->OutletSteamTemp,
                            "System",
                            "Average",
                            this->Name);
        SetupOutputVariable(state,
                            "Steam to Water Heat Exchanger Steam Trap Loss Rate",
                            OutputProcessor::Unit::W,
                            this->LoopLoss,
                            "System",
                            "Average",
                            this->Name);
    }

    void HeatExchangerSpecs::initialize(EnergyPlusData &state)
    {
        static std::string const RoutineNameNoColon("InitSteamToWaterHeatExchanger");
        static std::string const RoutineName("InitSteamToWaterHeatExchanger: ");

        if (this->MyOneTimeFlag) {
            this->setupOutputVars(state);
            this->MyFlag = true;
            this->MyEnvrnFlag = true;
            this->MyOneTimeFlag = false;
        }

        if (this->MyFlag) {
            bool errFlag = false;
            PlantUtilities::ScanPlantLoopsForObject(state,
                                                    this->Name,
                                                    DataPlant::TypeOf_SteamToWaterHeatExchanger,
                                                    this->DemandSideOfSteamLoop.loopNum,
                                                    this->DemandSideOfSteamLoop.loopSideNum,
                                                    this->DemandSideOfSteamLoop.branchNum,
                                                    this->DemandSideOfSteamLoop.compNum,
                                                    errFlag,
                                                    _,
                                                    _,
                                                    _,
                                                    this->DemandSideOfSteamLoop.inletNodeNum,
                                                    _);
            if (this->DemandSideOfSteamLoop.loopSideNum != DataPlant::DemandSide) { // throw error
                ShowSevereError(state, RoutineName + " Invalid connections for " +
                                    DataPlant::ccSimPlantEquipTypes(DataPlant::TypeOf_SteamToWaterHeatExchanger) + " name = \"" + this->Name + "\"");
                ShowContinueError(state, "The \"Loop Demand Side\" connections are not on the Demand Side of a steam loop");
                errFlag = true;
            }

            PlantUtilities::ScanPlantLoopsForObject(state,
                                                    this->Name,
                                                    DataPlant::TypeOf_FluidToFluidPlantHtExchg,
                                                    this->SupplySideOfWaterLoop.loopNum,
                                                    this->SupplySideOfWaterLoop.loopSideNum,
                                                    this->SupplySideOfWaterLoop.branchNum,
                                                    this->SupplySideOfWaterLoop.compNum,
                                                    errFlag,
                                                    _,
                                                    _,
                                                    _,
                                                    this->SupplySideOfWaterLoop.inletNodeNum,
                                                    _);

            if (this->SupplySideOfWaterLoop.loopSideNum != DataPlant::SupplySide) { // throw error
                ShowSevereError(state, RoutineName + " Invalid connections for " +
                                    DataPlant::ccSimPlantEquipTypes(DataPlant::TypeOf_SteamToWaterHeatExchanger) + " name = \"" + this->Name + "\"");
                ShowContinueError(state, "The \"Loop Supply Side\" connections are not on the Supply Side of a water loop");
                errFlag = true;
            }

            // make sure it is not the same loop on both sides.
            if (this->SupplySideOfWaterLoop.loopNum == this->DemandSideOfSteamLoop.loopNum) { // user is being too tricky, don't allow
                ShowSevereError(state, RoutineName + " Invalid connections for " +
                                    DataPlant::ccSimPlantEquipTypes(DataPlant::TypeOf_SteamToWaterHeatExchanger) + " name = \"" + this->Name + "\"");
                ShowContinueError(state, R"(The "Loop Supply Side" and "Loop Demand Side" need to be on different loops.)");
                errFlag = true;
            } else {

                PlantUtilities::InterConnectTwoPlantLoopSides(this->SupplySideOfWaterLoop.loopNum,
                                                              this->SupplySideOfWaterLoop.loopSideNum,
                                                              this->DemandSideOfSteamLoop.loopNum,
                                                              this->DemandSideOfSteamLoop.loopSideNum,
                                                              DataPlant::TypeOf_FluidToFluidPlantHtExchg,
                                                              true);
            }

            if (errFlag) {
                ShowFatalError(state, RoutineName + "Program terminated due to previous condition(s).");
            }
            this->MyFlag = false;

        } // plant setup

        if (state.dataGlobal->BeginEnvrnFlag && this->MyEnvrnFlag && (DataPlant::PlantFirstSizesOkayToFinalize)) {

            Real64 rho = FluidProperties::GetDensityGlycol(state,
                                                           DataPlant::PlantLoop(this->DemandSideOfSteamLoop.loopNum).FluidName,
                                                           DataGlobalConstants::InitConvTemp,
                                                           DataPlant::PlantLoop(this->DemandSideOfSteamLoop.loopNum).FluidIndex,
                                                           RoutineNameNoColon);
            this->DemandSideOfSteamLoop.MassFlowRateMax = rho * this->DemandSideOfSteamLoop.DesignVolumeFlowRate;
            PlantUtilities::InitComponentNodes(this->DemandSideOfSteamLoop.MassFlowRateMin,
                                               this->DemandSideOfSteamLoop.MassFlowRateMax,
                                               this->DemandSideOfSteamLoop.inletNodeNum,
                                               this->DemandSideOfSteamLoop.outletNodeNum,
                                               this->DemandSideOfSteamLoop.loopNum,
                                               this->DemandSideOfSteamLoop.loopSideNum,
                                               this->DemandSideOfSteamLoop.branchNum,
                                               this->DemandSideOfSteamLoop.compNum);

            rho = FluidProperties::GetDensityGlycol(state,
                                                    DataPlant::PlantLoop(this->SupplySideOfWaterLoop.loopNum).FluidName,
                                                    DataGlobalConstants::InitConvTemp,
                                                    DataPlant::PlantLoop(this->SupplySideOfWaterLoop.loopNum).FluidIndex,
                                                    RoutineNameNoColon);
            this->SupplySideOfWaterLoop.MassFlowRateMax = rho * this->SupplySideOfWaterLoop.DesignVolumeFlowRate;
            PlantUtilities::InitComponentNodes(this->SupplySideOfWaterLoop.MassFlowRateMin,
                                               this->SupplySideOfWaterLoop.MassFlowRateMax,
                                               this->SupplySideOfWaterLoop.inletNodeNum,
                                               this->SupplySideOfWaterLoop.outletNodeNum,
                                               this->SupplySideOfWaterLoop.loopNum,
                                               this->SupplySideOfWaterLoop.loopSideNum,
                                               this->SupplySideOfWaterLoop.branchNum,
                                               this->SupplySideOfWaterLoop.compNum);
            this->MyEnvrnFlag = false;
        }
        if (!state.dataGlobal->BeginEnvrnFlag) {
            this->MyEnvrnFlag = true;
        }

        this->DemandSideOfSteamLoop.InletTemp = DataLoopNode::Node(this->DemandSideOfSteamLoop.inletNodeNum).Temp;
        this->SupplySideOfWaterLoop.InletTemp = DataLoopNode::Node(this->SupplySideOfWaterLoop.inletNodeNum).Temp;
    }

    void HeatExchangerSpecs::size(EnergyPlusData &state)
    {
    }

    void HeatExchangerSpecs::calculate(EnergyPlusData &state, Real64 SupSideMdot, Real64 DmdSideMdot)
    {
    }

    void HeatExchangerSpecs::control(EnergyPlusData &state, int LoopNum, Real64 MyLoad, bool FirstHVACIteration)
    {
    }



    



} // namespace namespace SteamToWaterHeatExchanger

} // namespace EnergyPlus
