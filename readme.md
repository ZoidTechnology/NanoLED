# NanoLED
Control up to 16 channels of addressable LEDs with a Raspberry Pi Pico over USB.

## Building
1. NanoLED uses the [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk). Ensure all requirements are installed. The SDK itself is included as a Git submodule.
2. Clone this repository and ensure that submodules have been initialised.
3. Create a directory called "build" within the root directory of this project. From within the build directory, run `cmake ..` then `make`.

## Usage
The NanoLED firmware is based on [PicoLED](https://github.com/ZoidTechnology/PicoLED). Both firmwares communicate with a computer over a USB CDC virtual serial port, however, they use slightly different protocols. NanoLED expects a [COBS-encoded](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) packet and uses a zero byte as a delimiter. The packet contains a two-byte header, followed by up to 4096 bytes of data. The first 4 bits of the header encode the output channel. The remaining 12 bits encode the number of data bytes that follow minus one. Once all data has been processed, the firmware will respond with an XOR checksum of the decoded packet.