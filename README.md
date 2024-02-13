# DJI Phantom 3 VPS Analysis Project

## Project Description
This project focuses on the analysis of the Visual Positioning System (VPS) in DJI Phantom 3 Pro and Advanced drones. It provides a component for the ESP-IDF framework, integrable with the PlatformIO IDE. The analysis includes measuring distance using the ultrasonic sensor and interpreting bytes related to camera positioning.

## Repository Structure
- **/src:** Source code for the analysis with comments in the .c file on the communication protocol.
- **/info:** Images of serial port packets and other relevant data.

## Project Requirements
No prior knowledge of ESP-IDF or framework configuration is needed. Detailed comments on progress with the communication protocol can be found in the .c file in /src.

## Serial Port Packets
The VPS communicates through the serial port, receiving two distinct packets. The relevant packet, which we are interested in, has a total length of 98 bytes at a baud rate of 115200.

Packet Structure:
- Byte 0-4: Header - `550B00F803`
- Byte 5-6: Distance in cm (Obtained successfully)
- Byte 7: Unknown
- Byte 8: Counter1
- Byte 9-10: Unknown
- Byte 11-12: Static - `0x5557`
- Byte 13-15: Static - `0x00C202`
- Byte 16-17: Counter2
- Byte 18-22: Zeros
- Byte 23: Camera - `0x00` or `0xff` (with covered camera, value=`0x00`, unknown)
- Byte 24-31: Static - Zeros when disconnected from Phantom 3, else `0x574D3333305F4D56` with connected Phantom 3
- Byte 32-33: Unknown - Two noisy bytes
- Byte 34-35: Camera - Two stable bytes, appear as uint16; affected by hand movement
- Byte 36-37: Static - Changes value on VPS reboot
- Byte 38-39: Camera - Changes values with movement in front of the camera; appears as pairs of 2 bytes (uint16), somewhat noisy
- Byte 40-41: Zeros
- Byte 42-51: Camera - Same pattern as bytes 38-39
- Byte 52-59: Unknown - Noisy values, even with covered camera
- Byte 60-67: Camera - Same pattern as bytes 38-51
- Byte 68-85: Static - All zeros
- Byte 86-87: Camera - Noisy value, zero with covered camera
- Byte 88-89: Static - All zeros
- Byte 90-93: Camera - Static with covered camera, but not zero
- Byte 94-97: Unknown - Noisy bytes, even with covered camera

## Contributions
We welcome contributions! .

## Additional Notes
- The project successfully obtains the distance measurement from the ultrasonic sensor in cm.
- Currently, the analysis focuses on information obtained from the ultrasonic sensor and camera positioning bytes.
- The project aims to expand to include more VPS features in future versions.