# DOCUMENTION-ML

## Overview
This project demonstrates how to integrate machine learning models into an Arduino-based system and deploy the results to an ML webpage.

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Workflow](#workflow)
- [Contributors](#contributors)

## Installation
Follow these steps to set up the project on your system:

### Clone the repository:
```bash
git clone https://github.com/JakubJus/DOCUMENTION-ML.git
```

### Install the necessary libraries:
Install Arduino IDE and required dependencies:
Go to the specific directory youre intressted in and read the Readmei the directry for the necessary libaries, a good starting point is Machine learing directory. 

### Hardware setup:
Push down the resetbutton to put the device in boot-mode.
Flash the device with the XIAOEI.ino file located in the /Arduino directory.

## Usage
Run the project with the following steps:

- Arduino Setup: Flash the Arduino device with the ML model.
- Deploy ML Model: Use Edge Impulse to deploy the trained model to the Arduino board.
- Data Interaction: Collect data and run it through the Arduino model for real-time predictions.
## Workflow
This section details the additional steps taken by the group after the seed instructions:

1 Train ML Model:
  * Upload training data to Edge Impulse and fine-tune it based on requirements.
  *  Export the model for Arduino (.ino format).
2 Arduino Integration:
  * Push the ML model to the Arduino and test it.
3 ML Webpage Setup:
  * Push data from Arduino to the web-based ML page for further analysis.

## Contributors
- [Jakub Jus](https://github.com/JakubJus)
- [Alb Al](https://github.com/AlbAl03)
- [Porseng](https://github.com/Porseng)
- [Kaito  ](https://github.com/Kaito-gif2003)

Contributions are welcome! Please open an issue or submit a pull request for collaboration.


Examintator:
- [Jonas Willen](https://github.com/JakubJus)
