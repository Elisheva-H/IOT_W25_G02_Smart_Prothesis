#ifndef SHARED_YAMEL_PARSER_H
#define SHARED_YAMEL_PARSER_H

#include <vector>
#include <map>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <YAMLDuino.h>
#include "create_yaml_file.h"

// Communication struct
struct Communication {
    String name;
    String status;
    String ssid;
    String password;
    String mac;
    String serviceUUID;
    String characteristicUUID;
};

// General struct
struct General {
    String name;
    int code;
};

// Parameter struct
struct Parameter {
    int current_val;
    int min;
    int max;
    bool modify_permission;
};

// Function struct
struct SensorFunction {
    String name;
    std::map<String, Parameter> parameters;  // Key: Parameter name
};

// Sensor struct
struct Sensor {
    String name;
    String status;
    String type;
    SensorFunction function;
};

// MotorPin struct
struct MotorPin {
    String type;
    int pin_number;
};

// Motor struct
struct Motor {
    String name;
    String type;
    std::vector<MotorPin> pins;
    Parameter safety_threshold;
};

// Function struct
struct Function {
    String name;
    String protocol_type;
};

// Global vectors for parsed data
String fileType;
std::vector<General> generalEntries;
std::vector<Communication> communications;
std::vector<Sensor> sensors;
std::vector<Motor> motors;
std::vector<Function> functions;


const char* create_default_yaml_string(){
  const char* yamlContent = R"(
sensors:
  - name: 'leg_pressure_sensor'
    status: 'on'
    type: 'BLE_input'
    function:
      name: 'leg_function'
      parameters:
        param_1: [80,20,100,true]
        high_thld: [90,20,100,true]
        low_thld: [60,20,100,true]

  - name: 'shoulder_sensor'
    status: 'off'
    type: 'BLE_input'
    function:
      name: 'shoulder_function'
      parameters:
        param_1: [60,20,100,true]
        high_thld: [30,20,100,true]
        low_thld: [80,20,100,true]
)";
  return yamlContent;
}

void parseYAML(const String& yamlContent) {
  JsonDocument doc; 
  // Parse the YAML string into a JsonDocument
  //const char * yamlContent2=create_default_yaml_string();
  //DeserializationError error = deserializeYml(doc, yamlContent2);
  DeserializationError error = deserializeYml(doc, yamlContent.c_str());
  Serial.printf("\nafter deserialize\n");
  if ( error ) {
    Serial.print("Failed to parse YAML: ");
    Serial.println(error.f_str());
    return;
  }
  Serial.println();
  // Parse File Type
  fileType = doc["file_type"].as<String>();

  // Parse General Entries
  JsonArray generalArray = doc["general"];
  for (JsonObject entry : generalArray) {
      General gen;
      gen.name = entry["name"].as<String>();
      gen.code = entry["code"].as<int>();
      generalEntries.push_back(gen);
  }

  // Parse Communications
  JsonArray commArray = doc["communications"];
  for (JsonObject entry : commArray) {
      Communication comm;
      comm.name = entry["name"].as<String>();
      comm.status = entry["status"].as<String>();
      comm.ssid = entry["ssid"].as<String>();
      comm.password = entry["password"].as<String>();
      comm.mac = entry["mac"].as<String>();
      comm.serviceUUID = entry["SERVICE1_UUID"].as<String>();
      comm.characteristicUUID = entry["CHARACTERISTIC1_UUID"].as<String>();
      communications.push_back(comm);
  }

  // Parse Sensors
  JsonArray sensorArray = doc["sensors"];
  for (JsonObject entry : sensorArray) {
      Sensor sensor;
      Serial.printf("\n%s, %s, %s\n" ,entry["name"].as<String>(),entry["status"].as<String>(),entry["type"].as<String>());
      sensor.name = entry["name"].as<String>();
      sensor.status = entry["status"].as<String>();
      sensor.type = entry["type"].as<String>();

      JsonObject funcObj = entry["function"];
      sensor.function.name = funcObj["name"].as<String>();

      JsonObject params = funcObj["parameters"];
      for (JsonPair param : params) {
          Parameter paramData;
          JsonArray paramArray = param.value().as<JsonArray>();
          paramData.current_val = paramArray[0];
          paramData.min = paramArray[1];
          paramData.max = paramArray[2];
          paramData.modify_permission = paramArray[3];
          sensor.function.parameters[param.key().c_str()] = paramData;
      }
      sensors.push_back(sensor);
  }

  // Parse Motors
  JsonArray motorArray = doc["motors"];
  for (JsonObject entry : motorArray) {
      Motor motor;
      motor.name = entry["name"].as<String>();
      motor.type = entry["type"].as<String>();

      JsonArray pinsArray = entry["pins"];
      for (JsonObject pin : pinsArray) {
          MotorPin motorPin;
          motorPin.type = pin["type"].as<String>();
          motorPin.pin_number = pin["pin_number"].as<int>();
          motor.pins.push_back(motorPin);
      }

      JsonArray threshold = entry["safety_threshold"].as<JsonArray>();
      motor.safety_threshold.current_val = threshold[0];
      motor.safety_threshold.min = threshold[1];
      motor.safety_threshold.max = threshold[2];
      motor.safety_threshold.modify_permission = threshold[3];
      motors.push_back(motor);
  }

  // Parse Actions
  JsonArray actionArray = doc["functions"];
  for (JsonObject entry : actionArray) {
      Function function;
      function.name = entry["name"].as<String>();
      function.protocol_type = entry["protocol_type"].as<String>();
      functions.push_back(function);
  }
}


void printGeneral(const General& general) {
    Serial.println("General:");
    Serial.print("  Name: ");
    Serial.println(general.name);
    Serial.print("  Code: ");
    Serial.println(general.code);
}

void printParameter(const Parameter& parameter, const String& parameterName = "") {
    if (!parameterName.isEmpty()) {
        Serial.print("  Parameter Name: ");
        Serial.println(parameterName);
    }
    Serial.print("    Current Value: ");
    Serial.println(parameter.current_val);
    Serial.print("    Min: ");
    Serial.println(parameter.min);
    Serial.print("    Max: ");
    Serial.println(parameter.max);
    Serial.print("    Modify Permission: ");
    Serial.println(parameter.modify_permission ? "true" : "false");
}

void printSensorFunction(const SensorFunction& function) {
    Serial.println("  Function:");
    Serial.print("    Name: ");
    Serial.println(function.name);

    Serial.println("    Parameters:");
    for (const auto& [paramName, param] : function.parameters) {
        printParameter(param, paramName);
    }
}

void printSensor(const Sensor& sensor) {
    Serial.println("Sensor:");
    Serial.print("  Name: ");
    Serial.println(sensor.name);
    Serial.print("  Status: ");
    Serial.println(sensor.status);
    Serial.print("  Type: ");
    Serial.println(sensor.type);
    printSensorFunction(sensor.function);
}

void printMotorPin(const MotorPin& pin) {
    Serial.print("    Pin Type: ");
    Serial.println(pin.type);
    Serial.print("    Pin Number: ");
    Serial.println(pin.pin_number);
}

void printMotor(const Motor& motor) {
    Serial.println("Motor:");
    Serial.print("  Name: ");
    Serial.println(motor.name);
    Serial.print("  Type: ");
    Serial.println(motor.type);

    Serial.println("  Pins:");
    for (const auto& pin : motor.pins) {
        printMotorPin(pin);
    }

    Serial.println("  Safety Threshold:");
    printParameter(motor.safety_threshold);
}

void printFunction(const Function& function) {
    Serial.println("Function:");
    Serial.print("  Name: ");
    Serial.println(function.name);
    Serial.print("  Protocol Type: ");
    Serial.println(function.protocol_type);
}

void printAllStructs(
    const std::vector<Sensor>& sensors,
    const std::vector<Motor>& motors,
    const std::vector<Function>& functions) {
!
    Serial.println("=== Sensors ===");
    for (const auto& sensor : sensors) {
        printSensor(sensor);
        Serial.println();
    }

    Serial.println("=== Motors ===");
    for (const auto& motor : motors) {
        printMotor(motor);
        Serial.println();
    }

    Serial.println("=== Functions ===");
    for (const auto& function : functions) {
        printFunction(function);
        Serial.println();
    }

    Serial.println("=== End of Configuration ===");
}

void init_yaml() {
  String DefaultYamlContent = create_default_yaml_string();

  // // turn on if you want to read the yaml from SPIFFS 
  String yamlContent=ReadYmlUsingSPIFFS(DefaultYamlContent);
  
  // turn on if you want only to create a string in a yaml file for debuging
  //const char* yamlContent = create_default_yaml_string();

  parseYAML(yamlContent);
  printAllStructs(sensors, motors, functions);
}

#endif //SHARED_YAMEL_PARSER_H