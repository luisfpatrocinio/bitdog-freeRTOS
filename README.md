# FreeRTOS-Reborn

A test environment for embedded systems using the BitDogLab board (RP2040) and FreeRTOS.

This project serves as a flexible template for developing and testing applications on the Raspberry Pi Pico platform with the FreeRTOS real-time operating system. The specific application logic in `main.c` is expected to change frequently, but the core foundation of FreeRTOS on the RP2040 will remain.

## Current Functionality

The current implementation is a simple "blinky" application. It creates a single FreeRTOS task that toggles the red LED on the board every 500 milliseconds. This serves as a basic verification that the FreeRTOS scheduler is running correctly.

## Target Hardware

*   **Board:** BitDogLab with RP2040
*   **Processor:** Raspberry Pi RP2040

While the primary target is the BitDogLab board, the project can be adapted for other RP2040-based boards, such as the Raspberry Pi Pico W (which is the default in the CMake configuration).

## Dependencies

*   **[Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk)**
*   **[FreeRTOS Kernel](https://www.freertos.org/)**

## Building the Project

To build the project, you need to have the Pico SDK and a cross-compiler (like `arm-none-eabi-gcc`) installed and configured.

1.  **Set the Environment Variable:**
    You must set the `FREERTOS_KERNEL_PATH` environment variable to point to the location of the FreeRTOS kernel source code on your system.

    For example (on Linux/macOS):
    ```bash
    export FREERTOS_KERNEL_PATH=/path/to/your/freertos/kernel
    ```

    On Windows (PowerShell):
    ```powershell
    $env:FREERTOS_KERNEL_PATH="C:\path\to\your\freertos\kernel"
    ```

2.  **Configure with CMake:**
    Create a `build` directory and run CMake from within it.

    ```bash
    mkdir build
    cd build
    cmake ..
    ```

3.  **Compile:**
    Run `make` (or your chosen build tool like `nmake` or `ninja`) inside the `build` directory.

    ```bash
    make
    ```
    This will generate the `FreeRTOS-Reborn.uf2` file, which you can then flash to your RP2040 board.

## Author

*   **Luis Felipe Patrocinio**
    *   GitHub: [@luisfpatrocinio](https://github.com/luisfpatrocinio)

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details. 