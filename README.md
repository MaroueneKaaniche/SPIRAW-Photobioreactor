# Spirulina Harvesting System using ESP32

![Project Image](project_image.jpg) <!-- Insert Image: Schematic or representation of the project setup -->

## Overview

Welcome to the Spirulina Harvesting System repository! This project focuses on the development of a smart system for efficient spirulina harvesting. Spirulina, a highly nutritious aquatic organism, is cultivated using various actuators and monitored through sensors. The project revolves around an ESP32 microcontroller which orchestrates the entire process.

## Project Phases

The hardware implementation is divided into three distinct phases:

1. **Phase One:** Initialization and Calibration
2. **Phase Two:** Growth Monitoring and Regulation
3. **Phase Three:** Harvesting and Data Reporting

![Phases Image]([phases_image.jpg](https://github.com/MaroueneKaaniche/SPIRAW-Photobioreactor/blob/master/Solution%20Architecture%20%26%20Diagrams/Architecture%20hardware.jpg)) <!-- Insert Image: Representation of project phases -->

## Software Architecture

The software architecture of the project employs a robust and modular approach, leveraging FreeRTOS for task management and a state machine for efficient control.

### Key Components:
- **FreeRTOS Tasks:** The system is subdivided into tasks that handle specific functionalities, ensuring parallel processing and responsiveness.
- **State Machine:** A state machine governs the overall behavior, coordinating transitions between phases seamlessly.

## Mobile App Integration

The system communicates with users through a mobile app using MQTT (Message Queuing Telemetry Transport) protocol. This integration enables real-time updates and control over the spirulina harvesting process.

### Features:
- Monitoring Sensor Data: Receive live updates on crucial parameters such as temperature, pH levels, and nutrient concentration.
- Actuator Control: Remotely manage actuators responsible for maintaining optimal growth conditions.
- Harvesting Notifications: Get notified when the system enters the harvesting phase and receive detailed reports.

![Architecture Image]([architecture_image.jpg](https://github.com/MaroueneKaaniche/SPIRAW-Photobioreactor/blob/master/Solution%20Architecture%20%26%20Diagrams/Architecture%20software.jpg)) <!-- Insert Image: Diagram depicting the software architecture -->

## Getting Started

To get started with the Spirulina Harvesting System, follow these steps:
1. Set up the hardware components as described in the project documentation.
2. Flash the provided firmware onto the ESP32 microcontroller.
3. Install the accompanying mobile app on your smartphone.
4. Configure the MQTT settings in the app to establish communication with the system.

For in-depth setup instructions, refer to the [Setup Guide](setup_guide.md).

## Contributors

This project was made possible by the collaborative efforts of the following contributors:

- John Doe ([@johndoe](https://github.com/johndoe))

## License

This project is licensed under the [MIT License](LICENSE).

Feel free to explore the repository, contribute, and adapt the system to your requirements!

For any questions or inquiries, please contact us at spirulina.harvest@example.com.
