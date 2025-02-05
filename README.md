## Smart Prosthesis Management Tool Project

### By:
**Avigail Yampolsky, Elisheva Hammer, and May Abraham**

---

## Project Overview
This project focuses on developing a touchscreen-based server for managing a smart prosthesis controller using an ESP32. The touchscreen interface serves as the UI, enabling users to:
- Customize prosthetic parameters
- Execute preprogrammed movements
- Adjust sensor behavior

However, it does not directly control the prosthesis itself.

The prosthesis controller (see prosthesis mock files under the **ESP32** folder) sends its current system settings to the touchscreen server in **YAML format**. This YAML file contains all relevant data, including:
- Available sensors and motors
- Screen access passwords
- Functions to be executed

When no BLE connection is available, the screen provides an option to load a **mock YAML file**, allowing users to add, modify, and debug different screens.

The YAML file contains **five major fields**:
- General
- Communications
- Sensors
- Motors
- Functions  

An example YAML file can be found in the **assets** folder.

---

## Manager Tool Modes
The manager tool operates in three distinct modes:

### 1. **Daily Mode**
Designed for regular use, this mode is divided into three tabs:
- **Home Tab**: Allows users to execute preprogrammed movements.
- **Status Tab**: Displays the current status of all sensors.
- **Setup Tab**: Enables turning sensors on and off.

### 2. **Tech Mode**
This mode is password-protected (defined in the YAML file). It includes all three tabs from **Daily Mode**, plus an additional **Tech Tab**, which allows:
- Reading and modifying motor and sensor-related data.
- Manually turning motors on and off for testing.

### 3. **Debug Mode**
Also password-protected, this mode includes all four tabs from **Tech Mode** plus a **Debug Tab**, which allows live plotting of sensor or motor output data for debugging purposes.

---

## BLE Communication

### **Initialization & Connection**
- On startup, the management controller displays a **BLE connection screen** and attempts to connect using the predefined **UUID**.
- The prosthesis controller parses the **YAML file** and sends the parsed data to the **management controller**, which stores it in a structured dictionary.
- If reconnection is needed, a button on this screen allows restarting the connection process.



### **Request's interpretation**
All requests are transmitted as a **byte array** and interpreted using the following predefined structure:

```cpp
struct msg_interp {
  int req_type;         // Type of request  
  int cur_msg_count;    // Current message index in a sequence  
  int tot_msg_count;    // Total number of messages in a sequence  
  int msg_length;       // Length of the message data  
  char msg[MAX_MSG_LEN]; // Message payload  
  int checksum;         // Error-checking value  
};
```

Where **MAX_MSG_LEN** is a predefined maximum message size. If a message exceeds this limit, it will be split into multiple messages and sent sequentially.
Additionally, when needed, each **motor, sensor, and parameter** is identified by its respective index in the corresponding vector.

### **Request Types**

- **EMERGENCY_STOP** – A high-priority request running on a separate task, triggered by pressing the **BOOT button** on the management controller. When activated, the management tool sends a request to halt all motors. This remains functional as long as a BLE connection is active.

- **CHANGE_SENSOR_STATE_REQ** – Requests enabling or disabling specific sensors. Multiple sensors and states (1 = ON, 0 = OFF) can be updated simultaneously based on user input in **Daily Mode**.

- **CHANGE_SENSOR_PARAM_REQ** – Requests updating a sensor's parameter value. Multiple parameter modifications can be sent at once, specifying the **sensor ID, parameter ID, and desired value**, based on user input in **Tech Mode**.

- **CHANGE_MOTOR_PARAM_REQ** – Requests changing a motor’s **safety threshold** value. This request requires the **motor ID** and is initiated based on user input in **Tech Mode**.

- **GEST_REQ** – Requests executing a **predefined movement (gesture)**. The movement name is retrieved from the YAML file under the **function field** and categorized as a "gesture" type. The prosthesis must have a matching gesture defined with the same name.

- **YML_SENSOR_REQ, YML_MOTORS_REQ, YML_FUNC_REQ, YML_GENERAL_REQ** – These requests are sent sequentially upon establishing a connection. To accommodate larger YAML files, each YAML field is transmitted separately. Each request is sent **only after** the previous one has been fully processed to prevent data loss.

---

## Folder Description
- **ESP32**: Source code for the ESP32 firmware.
- **Documentation**: Wiring diagram + basic operating instructions.
- **Unit Tests**: Tests for individual hardware components (input/output devices).
- **Parameters**: Contains descriptions of configurable parameters.

---

## Arduino/ESP32 Libraries Used
- **Servo** by Michael Margolis, Arduino - 1.2.2
- **Adafruit BusIO** by Adafruit - 1.16.1
- **Adafruit GFX Library** by Adafruit - 1.11.9
- **Adafruit NeoMatrix** by Adafruit - 1.3.2
- **Adafruit NeoPixel** by Adafruit - 1.12.3
- **ArduinoJson** by Benoit Blanchon - 7.1.0
- **ESP32Servo** by Kevin Harrington, John K. Bennett - 3.0.5
- **NimBLE-Arduino** by h2zero - 1.4.2
- **YAMLDuino** by tobozo - 1.4.2
- **TAMC_GT911** by TAMC - 1.0.2
- **lvgl** by kisvegabor - 8.3.3
- **GFX Library for Arduino** by Moon On Our Nation - 1.2.9
- **Touch_GT911** (Manually installed, ADD REFERENCE)

---

## Project Poster
This project is part of **ICST - The Interdisciplinary Center for Smart Technologies**, Taub Faculty of Computer Science, Technion.

🔗 [ICST Website](https://icst.cs.technion.ac.il/)

