# ATLAS: Arduino Laser Tripwire Alarm System 

A smart laser tripwire system built with Arduino that detects motion or interruption of a laser beam and triggers visual and audio alarms. Includes IR remote override control and an LCD screen for real-time system status.

## Features

- **Laser-Based Intrusion Detection**
- **Multistate Logic: Normal / Intrusion / Override**
- **Buzzer and LED Alerts**
- **IR Remote Control Override**
- **TFT LCD Screen Status Display**

## Components Used

- Arduino Uno
- 5V Laser Module
- Photoresistor (LDR)
- 10kΩ Resistor (Voltage Divider)
- IR Receiver + Remote
- Servo Motor
- Buzzer
- LEDs (x3)
- 1.8" ST7735 TFT Display
- Breadboard, Jumper Wires

## How It Works

- A laser is aligned with a photoresistor.
- When the beam is obstructed, the system enters **intrusion mode**.
- A buzzer beeps, a red LED blinks, and the servo swings.
- The display changes to alert the user.
- An IR remote can be used to override or reset the system.

## Circuit Schematic

You can find an interactive version of the project's schematics here. I developed this using CirkitDesigner <sup>TM</sup> <br>
[ATLAS Schematic](https://app.cirkitdesigner.com/project/25f5dbe2-e3fa-4537-9047-c97edf9c66f3)

## Demo

I put the demo video on my portfolio site. The hypertext is a link to that section on the webpage.
[Demo Video](https://munachimsohenry.wixsite.com/my-site/copy-of-project-6-atmoslog#:~:text=in%20the%20system.-,Demonstration%20Video,-In%20the%20demo)

## Notes

- LDR readings rise when laser is blocked.
- Non-blocking code uses `millis()` for buzzer toggling.
- I made `myTone()` function to avoid timer conflicts with IRremote.
- Override LED stays on when system is manually disabled via remote.

## License

MIT License

## Author

Munachimso Henry — EEE @ Covenant University
