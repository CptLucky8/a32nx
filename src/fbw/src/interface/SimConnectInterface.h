/*
 * A32NX
 * Copyright (C) 2020 FlyByWire Simulations and its contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <MSFS/Legacy/gauges.h>
#include <SimConnect.h>
#include <string>
#include <vector>

#include "../ThrottleAxisMapping.h"
#include "SimConnectData.h"

class SimConnectInterface {
 public:
  enum Events {
    AXIS_ELEVATOR_SET,
    AXIS_AILERONS_SET,
    AXIS_RUDDER_SET,
    RUDDER_SET,
    RUDDER_LEFT,
    RUDDER_AXIS_PLUS,
    RUDDER_CENTER,
    RUDDER_RIGHT,
    RUDDER_AXIS_MINUS,
    AILERON_SET,
    AILERONS_LEFT,
    AILERONS_RIGHT,
    CENTER_AILER_RUDDER,
    ELEVATOR_SET,
    ELEV_DOWN,
    ELEV_UP,
    AP_MASTER,
    AUTOPILOT_OFF,
    A32NX_FCU_AP_1_PUSH,
    A32NX_FCU_AP_2_PUSH,
    A32NX_FCU_HDG_PUSH,
    A32NX_FCU_HDG_PULL,
    A32NX_FCU_ALT_PUSH,
    A32NX_FCU_ALT_PULL,
    A32NX_FCU_VS_PUSH,
    A32NX_FCU_VS_PULL,
    A32NX_FCU_LOC_PUSH,
    A32NX_FCU_APPR_PUSH,
    AUTO_THROTTLE_ARM,
    A32NX_THROTTLE_MAPPING_LOAD_FROM_FILE,
    A32NX_THROTTLE_MAPPING_LOAD_FROM_LOCAL_VARIABLES,
    A32NX_THROTTLE_MAPPING_SAVE_TO_FILE,
    THROTTLE_SET,
    THROTTLE1_SET,
    THROTTLE2_SET,
    THROTTLE_AXIS_SET_EX1,
    THROTTLE1_AXIS_SET_EX1,
    THROTTLE2_AXIS_SET_EX1,
    THROTTLE_FULL,
    THROTTLE_CUT,
    THROTTLE_INCR,
    THROTTLE_DECR,
    THROTTLE_INCR_SMALL,
    THROTTLE_DECR_SMALL,
    THROTTLE_10,
    THROTTLE_20,
    THROTTLE_30,
    THROTTLE_40,
    THROTTLE_50,
    THROTTLE_60,
    THROTTLE_70,
    THROTTLE_80,
    THROTTLE_90,
    THROTTLE1_FULL,
    THROTTLE1_CUT,
    THROTTLE1_INCR,
    THROTTLE1_DECR,
    THROTTLE1_INCR_SMALL,
    THROTTLE1_DECR_SMALL,
    THROTTLE2_FULL,
    THROTTLE2_CUT,
    THROTTLE2_INCR,
    THROTTLE2_DECR,
    THROTTLE2_INCR_SMALL,
    THROTTLE2_DECR_SMALL,
    THROTTLE_REVERSE_THRUST_TOGGLE,
    THROTTLE_REVERSE_THRUST_HOLD,
  };

  SimConnectInterface() = default;

  ~SimConnectInterface() = default;

  bool connect(bool autopilotStateMachineEnabled,
               bool autopilotLawsEnabled,
               bool flyByWireEnabled,
               const std::vector<std::shared_ptr<ThrottleAxisMapping>>& throttleAxis);

  void disconnect();

  bool requestReadData();

  bool requestData();

  bool readData();

  bool sendData(SimOutput output);

  bool sendData(SimOutputEtaTrim output);

  bool sendData(SimOutputZetaTrim output);

  bool sendData(SimOutputThrottles output);

  bool sendEvent(Events eventId);

  bool setClientDataLocalVariables(ClientDataLocalVariables output);

  bool setClientDataLocalVariablesAutothrust(ClientDataLocalVariablesAutothrust output);

  void resetSimInputAutopilot();

  void resetSimInputThrottles();

  SimData getSimData();

  SimInput getSimInput();

  SimInputAutopilot getSimInputAutopilot();

  SimInputThrottles getSimInputThrottles();

  bool setClientDataAutopilotStateMachine(ClientDataAutopilotStateMachine output);
  ClientDataAutopilotStateMachine getClientDataAutopilotStateMachine();

  bool setClientDataAutopilotLaws(ClientDataAutopilotLaws output);
  ClientDataAutopilotLaws getClientDataAutopilotLaws();

  ClientDataAutothrust getClientDataAutothrust();

 private:
  enum ClientData {
    AUTOPILOT_STATE_MACHINE,
    AUTOPILOT_LAWS,
    AUTOTHRUST,
    LOCAL_VARIABLES,
    LOCAL_VARIABLES_AUTOTHRUST,
  };

  bool isConnected = false;
  HANDLE hSimConnect = 0;

  SimData simData = {};
  SimInput simInput = {};
  SimInputAutopilot simInputAutopilot = {};

  SimInputThrottles simInputThrottles = {};
  std::vector<std::shared_ptr<ThrottleAxisMapping>> throttleAxis;

  ClientDataAutopilotStateMachine clientDataAutopilotStateMachine = {};
  ClientDataAutopilotLaws clientDataAutopilotLaws = {};
  ClientDataAutothrust clientDataAutothrust = {};

  bool prepareSimDataSimConnectDataDefinitions();

  bool prepareSimInputSimConnectDataDefinitions(bool autopilotStateMachineEnabled, bool autopilotLawsEnabled, bool flyByWireEnabled);

  bool prepareSimOutputSimConnectDataDefinitions();

  bool prepareClientDataDefinitions();

  void simConnectProcessDispatchMessage(SIMCONNECT_RECV* pData, DWORD* cbData);

  void simConnectProcessEvent(const SIMCONNECT_RECV_EVENT* event);

  void simConnectProcessSimObjectData(const SIMCONNECT_RECV_SIMOBJECT_DATA* data);

  void simConnectProcessClientData(const SIMCONNECT_RECV_CLIENT_DATA* data);

  bool sendClientData(SIMCONNECT_DATA_DEFINITION_ID id, DWORD size, void* data);
  bool sendData(SIMCONNECT_DATA_DEFINITION_ID id, DWORD size, void* data);

  static bool addDataDefinition(const HANDLE connectionHandle,
                                const SIMCONNECT_DATA_DEFINITION_ID id,
                                const SIMCONNECT_DATATYPE dataType,
                                const std::string& dataName,
                                const std::string& dataUnit);

  static bool addInputDataDefinition(const HANDLE connectionHandle,
                                     const SIMCONNECT_DATA_DEFINITION_ID groupId,
                                     const SIMCONNECT_CLIENT_EVENT_ID eventId,
                                     const std::string& eventName,
                                     const bool maskEvent);

  static bool isSimConnectDataTypeStruct(SIMCONNECT_DATATYPE dataType);

  static std::string getSimConnectExceptionString(SIMCONNECT_EXCEPTION exception);
};