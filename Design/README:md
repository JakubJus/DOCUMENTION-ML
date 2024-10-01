# Design

## System Architecture
The DOCUMENTION-ML system is designed around the nRF52840 microcontroller, which provides efficient processing and communication capabilities. The architecture includes the following key components:

1. **nRF52840 Microcontroller**:
   - The core processing unit of the system.
   - Features Bluetooth Low Energy (BLE) for data transmission.
   - Optimized for low power consumption, making it suitable for continuous use.

2. **Sensors**:
   - **Accelerometers**: Measure acceleration forces to detect movement and orientation changes.
   - **Gyroscopes**: Track angular velocity, helping to understand the user's motion dynamics.

3. **Machine Learning Model**:
   - A pre-trained model that processes data from the sensors to classify movements into distinct categories (sitting, standing, walking, running, falling).
   - The model is designed to operate efficiently on the nRF52840, enabling real-time processing and quick response times.

## Data Flow
1. **Data Collection**:
   - Sensors continuously collect movement data from the user.
   - The nRF52840 microcontroller processes this data in real-time.

2. **Data Processing**:
   - The collected data is fed into the machine learning model, which classifies the user's activity based on predefined categories.
   - The model uses algorithms that can adapt and improve with more data over time.

3. **Data Transmission**:
   - The classified data is transmitted via Bluetooth to a mobile application for monitoring.
   - The system can send alerts in case of detected falls or unusual inactivity, ensuring timely interventions.

## Wearability
- The device is designed for discreet wearability, allowing users to wear it comfortably throughout their daily activities.
- The compact design ensures that it does not interfere with the user's normal movement or lifestyle.

## CAD Models
The design of the physical enclosure and components of the device is represented through CAD models. These models are crucial for prototyping and manufacturing the device. The following details are included:

- **Purpose**: The CAD models illustrate the housing for the nRF52840 microcontroller and sensors, ensuring proper fit and functionality.
- **File Formats**: The models are available in formats such as .STL, .STEP, or .DXF to facilitate 3D printing and manufacturing processes.
- **Location**: CAD models can be found in the `cad_models/` directory of this repository. 

## Educational Application
- The design allows for easy retraining of the machine learning model, providing students and developers with hands-on experience in embedded systems and machine learning.
- Users can modify and redeploy their models, enhancing their understanding of the underlying technologies.
