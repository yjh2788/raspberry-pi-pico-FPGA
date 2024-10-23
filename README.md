# License

This project utilizes the Raspberry Pi Pico SDK, which is licensed under the BSD 3-Clause License. The full license text is available in the [License.txt](License.txt) file.

# Project Overview
This is an ongoing project aimed at developing an acceleration system that enables efficient image processing and AI on a microprocessor. The current setup captures images from an OV7670 module and displays them on a TFT LCD. The next step involves integrating an FPGA as a computational accelerator to enhance image processing and AI computations.

# Project block diagram
![project block diagram](https://github.com/user-attachments/assets/bcc22972-8721-490e-83e8-ee553e03e5f6)

# SIMD_v1 structure (on FPGA)
![SIMD_v1 structure](https://github.com/user-attachments/assets/50585e85-3773-4495-819d-5a2743cf094f)
# Importing C++ Program on Raspberrypi Pico

## Build Instructions

To build this project using CMake and MinGW, ensure you have the following prerequisites:

### Prerequisites
- **CMake**: Installed and available in your system's PATH.
- **GNU Toolchain (MinGW)**: Includes tools like `gcc`, `g++`, and `make`. Ensure these are installed and available in your system's PATH.
- **CMakeLists.txt**: The project should contain a `CMakeLists.txt` file in the root directory.

### Build Steps
1. Open a terminal and navigate to the build directory.
2. Run the following command to generate Makefiles using MinGW:
   ```bash
   cd camera_Project
   mkdir build
   cd build
   cmake -G "MinGW Makefiles" ..
3. Compile the project by running:
   ```
   mingw32-make
   ```

# Camera viewer program
![GUI program example](https://github.com/user-attachments/assets/05d73184-0262-4e80-8ae2-412a916f6e0d)

The program is located in `CameraViewer_GUI/CameraViewer.py`
## menual
1. Open the program and connect the board to the computer.
2. Setting the serial port and baudrate and open the port.
3. Setting the image resolution and format. It should be same as OV7670 image setting.
4. If you want to get a frame, press `getframe`.
5. If you want to get framse, press `getframes`.
6. Can stop the frames by pressing `stop frame`.
7. Can save image by pressing `save image`. Image will be save in `save dir` path.
