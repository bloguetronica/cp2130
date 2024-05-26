/* CP2130 class - Version 1.2.6
   Copyright (c) 2021-2024 Samuel Lourenço

   This library is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or (at your
   option) any later version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
   License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library.  If not, see <https://www.gnu.org/licenses/>.


   Please feel free to contact me via e-mail: samuel.fmlourenco@gmail.com */


#ifndef CP2130_H
#define CP2130_H

// Includes
#include <cstdint>
#include <list>
#include <string>
#include <vector>
#include <libusb-1.0/libusb.h>

class CP2130
{
private:
    libusb_context *context_;
    libusb_device_handle *handle_;
    bool disconnected_, kernelWasAttached_;

    std::u16string getDescGeneric(uint8_t command, int &errcnt, std::string &errstr);
    void writeDescGeneric(const std::u16string &descriptor, uint8_t command, int &errcnt, std::string &errstr);

public:
    // Class definitions
    static const uint16_t VID = 0x10c4;    // Default USB vendor ID
    static const uint16_t PID = 0x87a0;    // Default USB product ID
    static const int SUCCESS = 0;          // Returned by open() if successful
    static const int ERROR_INIT = 1;       // Returned by open() in case of a libusb initialization failure
    static const int ERROR_NOT_FOUND = 2;  // Returned by open() if the device was not found
    static const int ERROR_BUSY = 3;       // Returned by open() if the device is already in use

    // Descriptor specific definitions
    static const size_t DESCMXL_MANUFACTURER = 62;  // Maximum length of manufacturer descriptor
    static const size_t DESCMXL_PRODUCT = 62;       // Maximum length of product descriptor
    static const size_t DESCMXL_SERIAL = 30;        // Maximum length of serial descriptor

    // OTP ROM specific definitions
    static const size_t PROM_BLOCKS = 8;                            // Number of blocks of the OTP ROM
    static const size_t PROM_BLOCK_SIZE = 64;                       // Size of each block
    static const size_t PROM_SIZE = PROM_BLOCKS * PROM_BLOCK_SIZE;  // Total size of the OTP ROM
    static const uint16_t PROM_WRITE_KEY = 0xa5f1;                  // OTP ROM write key
    static const size_t PROMIDX_VID = 0;                            // 'VID' field index
    static const size_t PROMSZE_VID = 2;                            // 'VID' field size
    static const size_t PROMIDX_PID = 2;                            // 'PID' field index
    static const size_t PROMSZE_PID = 2;                            // 'PID' field size
    static const size_t PROMIDX_MAX_POWER = 4;                      // 'Max Power' field index
    static const size_t PROMSZE_MAX_POWER = 1;                      // 'Max Power' field size
    static const size_t PROMIDX_POWER_MODE = 5;                     // 'Power Mode' field index
    static const size_t PROMSZE_POWER_MODE = 1;                     // 'Power Mode' field size
    static const size_t PROMIDX_RELEASE_VERSION = 6;                // 'Release Version' field index
    static const size_t PROMSZE_RELEASE_VERSION = 2;                // 'Release Version' field size
    static const size_t PROMIDX_TRANSFER_PRIORITY = 8;              // 'Transfer Priority' field index
    static const size_t PROMSZE_TRANSFER_PRIORITY = 1;              // 'Transfer Priority' field size
    static const size_t PROMIDX_MANUFACTURING_STRING_1 = 9;         // 'Manufacturing String 1' field index
    static const size_t PROMSZE_MANUFACTURING_STRING_1 = 63;        // 'Manufacturing String 1' field size
    static const size_t PROMIDX_MANUFACTURING_STRING_2 = 72;        // 'Manufacturing String 2' field index
    static const size_t PROMSZE_MANUFACTURING_STRING_2 = 63;        // 'Manufacturing String 2' field size
    static const size_t PROMIDX_PRODUCT_STRING_1 = 135;             // 'Product String 1' field index
    static const size_t PROMSZE_PRODUCT_STRING_1 = 63;              // 'Product String 1' field size
    static const size_t PROMIDX_PRODUCT_STRING_2 = 198;             // 'Product String 2' field index
    static const size_t PROMSZE_PRODUCT_STRING_2 = 63;              // 'Product String 2' field size
    static const size_t PROMIDX_SERIAL_STRING = 261;                // 'Serial String' field index
    static const size_t PROMSZE_SERIAL_STRING = 62;                 // 'Serial String' field size
    static const size_t PROMIDX_PIN_CONFIG = 324;                   // 'Pin Config' field index
    static const size_t PROMSZE_PIN_CONFIG = 20;                    // 'Pin Config' field size
    static const size_t PROMIDX_CUSTOMIZED_FIELDS = 344;            // Customized fields index
    static const size_t PROMSZE_CUSTOMIZED_FIELDS = 2;              // Customized fields size
    static const size_t PROMIDX_LOCK_BYTE = 346;                    // 'Lock Byte' field index
    static const size_t PROMSZE_LOCK_BYTE = 2;                      // 'Lock Byte' field size

    // The following values are applicable to bulkTransfer()
    static const uint8_t READ = 0x00;         // Read command
    static const uint8_t WRITE = 0x01;        // Write command
    static const uint8_t WRITEREAD = 0x02;    // WriteRead command
    static const uint8_t READWITHRTR = 0x04;  // ReadWithRTR command

    // The following values are applicable to controlTransfer()
    static const uint8_t GET = 0xc0;                                 // Device-to-Host vendor request
    static const uint8_t SET = 0x40;                                 // Host-to-Device vendor request
    static const uint8_t RESET_DEVICE = 0x10;                        // Reset_Device command
    static const uint16_t RESET_DEVICE_WLEN = 0x0000;                // Reset_Device data stage length
    static const uint8_t GET_READONLY_VERSION = 0x11;                // Get_ReadOnly_Version command
    static const uint16_t GET_READONLY_VERSION_WLEN = 0x0002;        // Get_ReadOnly_Version data stage length
    static const uint8_t GET_GPIO_VALUES = 0x20;                     // Get_GPIO_Values command
    static const uint16_t GET_GPIO_VALUES_WLEN = 0x0002;             // Get_GPIO_Values data stage length
    static const uint8_t SET_GPIO_VALUES = 0x21;                     // Set_GPIO_Values command
    static const uint16_t SET_GPIO_VALUES_WLEN = 0x0004;             // Set_GPIO_Values data stage length
    static const uint8_t GET_GPIO_MODE_AND_LEVEL = 0x22;             // Get_GPIO_Mode_And_Level command
    static const uint16_t GET_GPIO_MODE_AND_LEVEL_WLEN = 0x0004;     // Get_GPIO_Mode_And_Level data stage length
    static const uint8_t SET_GPIO_MODE_AND_LEVEL = 0x23;             // Set_GPIO_Mode_And_Level command
    static const uint16_t SET_GPIO_MODE_AND_LEVEL_WLEN = 0x0003;     // Set_GPIO_Mode_And_Level data stage length
    static const uint8_t GET_GPIO_CHIP_SELECT = 0x24;                // Get_GPIO_Chip_Select command
    static const uint16_t GET_GPIO_CHIP_SELECT_WLEN = 0x0004;        // Get_GPIO_Chip_Select data stage length
    static const uint8_t SET_GPIO_CHIP_SELECT = 0x25;                // Set_GPIO_Chip_Select command
    static const uint16_t SET_GPIO_CHIP_SELECT_WLEN = 0x0002;        // Set_GPIO_Chip_Select data stage length
    static const uint8_t GET_SPI_WORD = 0x30;                        // Get_SPI_Word command
    static const uint16_t GET_SPI_WORD_WLEN = 0x000b;                // Get_SPI_Word data stage length
    static const uint8_t SET_SPI_WORD = 0x31;                        // Set_SPI_Word command
    static const uint16_t SET_SPI_WORD_WLEN = 0x0002;                // Set_SPI_Word data stage length
    static const uint8_t GET_SPI_DELAY = 0x32;                       // Get_SPI_Delay command
    static const uint16_t GET_SPI_DELAY_WLEN = 0x0008;               // Get_SPI_Delay data stage length
    static const uint8_t SET_SPI_DELAY = 0x33;                       // Set_SPI_Delay command
    static const uint16_t SET_SPI_DELAY_WLEN = 0x0008;               // Set_SPI_Delay data stage length
    static const uint8_t GET_FULL_THRESHOLD = 0x34;                  // Get_Full_Threshold command
    static const uint16_t GET_FULL_THRESHOLD_WLEN = 0x0001;          // Get_Full_Threshold data stage length
    static const uint8_t SET_FULL_THRESHOLD = 0x35;                  // Set_Full_Threshold command
    static const uint16_t SET_FULL_THRESHOLD_WLEN = 0x0001;          // Set_Full_Threshold data stage length
    static const uint8_t GET_RTR_STATE = 0x36;                       // Get_RTR_State command
    static const uint16_t GET_RTR_STATE_WLEN = 0x0001;               // Get_RTR_State data stage length
    static const uint8_t SET_RTR_STOP = 0x37;                        // Set_RTR_Stop command
    static const uint16_t SET_RTR_STOP_WLEN = 0x0001;                // Set_RTR_Stop data stage length
    static const uint8_t GET_EVENT_COUNTER = 0x44;                   // Get_Event_Counter command
    static const uint16_t GET_EVENT_COUNTER_WLEN = 0x0003;           // Get_Event_Counter data stage length
    static const uint8_t SET_EVENT_COUNTER = 0x45;                   // Set_Event_Counter command
    static const uint16_t SET_EVENT_COUNTER_WLEN = 0x0003;           // Set_Event_Counter data stage length
    static const uint8_t GET_CLOCK_DIVIDER = 0x46;                   // Get_Clock_Divider command
    static const uint16_t GET_CLOCK_DIVIDER_WLEN = 0x0001;           // Get_Clock_Divider data stage length
    static const uint8_t SET_CLOCK_DIVIDER = 0x47;                   // Set_Clock_Divider command
    static const uint16_t SET_CLOCK_DIVIDER_WLEN = 0x0001;           // Set_Clock_Divider data stage length
    static const uint8_t GET_USB_CONFIG = 0x60;                      // Get_USB_Config command
    static const uint16_t GET_USB_CONFIG_WLEN = 0x0009;              // Get_USB_Config data stage length
    static const uint8_t SET_USB_CONFIG = 0x61;                      // Set_USB_Config command
    static const uint16_t SET_USB_CONFIG_WLEN = 0x000a;              // Set_USB_Config data stage length
    static const uint8_t GET_MANUFACTURING_STRING_1 = 0x62;          // Get_Manufacturing_String_1 command
    static const uint16_t GET_MANUFACTURING_STRING_1_WLEN = 0x0040;  // Get_Manufacturing_String_1 data stage length
    static const uint8_t SET_MANUFACTURING_STRING_1 = 0x63;          // Set_Manufacturing_String_1 command
    static const uint16_t SET_MANUFACTURING_STRING_1_WLEN = 0x0040;  // Set_Manufacturing_String_1 data stage length
    static const uint8_t GET_MANUFACTURING_STRING_2 = 0x64;          // Get_Manufacturing_String_2 command
    static const uint16_t GET_MANUFACTURING_STRING_2_WLEN = 0x0040;  // Get_Manufacturing_String_2 data stage length
    static const uint8_t SET_MANUFACTURING_STRING_2 = 0x65;          // Set_Manufacturing_String_2 command
    static const uint16_t SET_MANUFACTURING_STRING_2_WLEN = 0x0040;  // Set_Manufacturing_String_2 data stage length
    static const uint8_t GET_PRODUCT_STRING_1 = 0x66;                // Get_Product_String_1 command
    static const uint16_t GET_PRODUCT_STRING_1_WLEN = 0x0040;        // Get_Product_String_1 data stage length
    static const uint8_t SET_PRODUCT_STRING_1 = 0x67;                // Set_Product_String_1 command
    static const uint16_t SET_PRODUCT_STRING_1_WLEN = 0x0040;        // Set_Product_String_1 data stage length
    static const uint8_t GET_PRODUCT_STRING_2 = 0x68;                // Get_Product_String_2 command
    static const uint16_t GET_PRODUCT_STRING_2_WLEN = 0x0040;        // Get_Product_String_2 data stage length
    static const uint8_t SET_PRODUCT_STRING_2 = 0x69;                // Set_Product_String_2 command
    static const uint16_t SET_PRODUCT_STRING_2_WLEN = 0x0040;        // Set_Product_String_2 data stage length
    static const uint8_t GET_SERIAL_STRING = 0x6a;                   // Get_Serial_String command
    static const uint16_t GET_SERIAL_STRING_WLEN = 0x0040;           // Get_Serial_String data stage length
    static const uint8_t SET_SERIAL_STRING = 0x6b;                   // Set_Serial_String command
    static const uint16_t SET_SERIAL_STRING_WLEN = 0x0040;           // Set_Serial_String data stage length
    static const uint8_t GET_PIN_CONFIG = 0x6c;                      // Get_Pin_Config command
    static const uint16_t GET_PIN_CONFIG_WLEN = 0x0014;              // Get_Pin_Config data stage length
    static const uint8_t SET_PIN_CONFIG = 0x6d;                      // Set_Pin_Config command
    static const uint16_t SET_PIN_CONFIG_WLEN = 0x0014;              // Set_Pin_Config data stage length
    static const uint8_t GET_LOCK_BYTE = 0x6e;                       // Get_Lock_Byte command
    static const uint16_t GET_LOCK_BYTE_WLEN = 0x0002;               // Get_Lock_Byte data stage length
    static const uint8_t SET_LOCK_BYTE = 0x6f;                       // Set_Lock_Byte command
    static const uint16_t SET_LOCK_BYTE_WLEN = 0x0002;               // Set_Lock_Byte data stage length
    static const uint8_t GET_PROM_CONFIG = 0x70;                     // Get_PROM_Config command
    static const uint16_t GET_PROM_CONFIG_WLEN = 0x0040;             // Get_PROM_Config data stage length
    static const uint8_t SET_PROM_CONFIG = 0x71;                     // Set_PROM_Config command
    static const uint16_t SET_PROM_CONFIG_WLEN = 0x0040;             // Set_PROM_Config data stage length

    // The following masks are applicable to the value returned by getLockWord()
    static const uint16_t LWVID = 0x0001;      // Mask for the vendor ID lock bit
    static const uint16_t LWPID = 0x0002;      // Mask for the product ID lock bit
    static const uint16_t LWMAXPOW = 0x0004;   // Mask for the max power lock bit
    static const uint16_t LWPOWMODE = 0x0008;  // Mask for the power mode lock bit
    static const uint16_t LWREL = 0x0010;      // Mask for the release version lock bit
    static const uint16_t LWMANUF = 0x0060;    // Mask for the manufacturer descriptor lock bits
    static const uint16_t LWTRFPRIO = 0x0080;  // Mask for the transfer priority lock bit
    static const uint16_t LWUSBCFG = 0x009f;   // Mask for the USB config lock bits
    static const uint16_t LWPROD = 0x0300;     // Mask for the product descriptor lock bits
    static const uint16_t LWSER = 0x0400;      // Mask for the serial descriptor lock bit
    static const uint16_t LWPINCFG = 0x0800;   // Mask for the pin config lock bit
    static const uint16_t LWALL = 0x0fff;      // Mask for all but the reserved lock bits

    // The following values are applicable to SPIMode/configureSPIMode()/getSPIMode()
    static const bool CSMODEOD = false;     // Boolean corresponding to chip select open-drain mode
    static const bool CSMODEPP = true;      // Boolean corresponding to chip select push-pull mode
    static const uint8_t CFRQ12M = 0x00;    // Value corresponding to a clock frequency of 12MHz
    static const uint8_t CFRQ6M = 0x01;     // Value corresponding to a clock frequency of 6MHz
    static const uint8_t CFRQ3M = 0x02;     // Value corresponding to a clock frequency of 3MHz
    static const uint8_t CFRQ1500K = 0x03;  // Value corresponding to a clock frequency of 1.5MHz
    static const uint8_t CFRQ750K = 0x04;   // Value corresponding to a clock frequency of 750KHz
    static const uint8_t CFRQ375K = 0x05;   // Value corresponding to a clock frequency of 375KHz
    static const uint8_t CFRQ1875= 0x06;    // Value corresponding to a clock frequency of 187.5KHz
    static const uint8_t CFRQ938 = 0x07;    // Value corresponding to a clock frequency of 93.8KHz
    static const bool CPOL0 = false;        // Boolean corresponding to CPOL = 0 (clock is active high and idles low)
    static const bool CPOL1 = true;         // Boolean corresponding to CPOL = 1 (clock is active low and idles high)
    static const bool CPHA0 = false;        // Boolean corresponding to CPHA = 0 (data is valid on the leading edge of the clock)
    static const bool CPHA1 = true;         // Boolean corresponding to CPHA = 1 (data is valid on the trailing edge of the clock)

    // The following values are applicable to PinConfig/getPinConfig()/writePinConfig()
    static const uint8_t PCIN = 0x00;         // GPIO as input - Also applicable to configureGPIO()
    static const uint8_t PCOUTOD = 0x01;      // GPIO as open-drain output - Also applicable to configureGPIO()
    static const uint8_t PCOUTPP = 0x02;      // GPIO as push-pull output - Also applicable to configureGPIO()
    static const uint8_t PCCS = 0x03;         // GPIO as chip select
    static const uint8_t PCNRTR = 0x04;       // GPIO as !RTR input, only applicable to GPIO.3
    static const uint8_t PCRTR = 0x05;        // GPIO as RTR input, only applicable to GPIO.3
    static const uint8_t PCEVTCNTRRE = 0x04;  // GPIO as EVTCNTR rising edge input, only applicable to GPIO.4 - Also applicable to getEventCounter()/setEventCounter()
    static const uint8_t PCEVTCNTRFE = 0x05;  // GPIO as EVTCNTR falling edge input, only applicable to GPIO.4 - Also applicable to getEventCounter()/setEventCounter()
    static const uint8_t PCEVTCNTRNP = 0x06;  // GPIO as EVTCNTR negative pulse input, only applicable to GPIO.4 - Also applicable to getEventCounter()/setEventCounter()
    static const uint8_t PCEVTCNTRPP = 0x07;  // GPIO as EVTCNTR positive pulse input, only applicable to GPIO.4 - Also applicable to getEventCounter()/setEventCounter()
    static const uint8_t PCCLKOUT = 0x04;     // GPIO as CLKOUT push-pull output, only applicable to GPIO.5
    static const uint8_t PCSPIACT = 0x04;     // GPIO as SPIACT push-pull output, only applicable to GPIO.8
    static const uint8_t PCSSPND = 0x04;      // GPIO as SUSPEND push-pull output, only applicable to GPIO.9
    static const uint8_t PCNSSPND = 0x04;     // GPIO as !SUSPEND push-pull output, only applicable to GPIO.10
    static const uint16_t BMSCK = 0x0001;     // Bitmap for the SCK pin
    static const uint16_t BMMISO = 0x0002;    // Bitmap for the MISO pin
    static const uint16_t BMMOSI = 0x0004;    // Bitmap for the MOSI pin
    static const uint16_t BMGPIO0 = 0x0008;   // Bitmap for the GPIO.0 pin - Also applicable to getGPIOs()/setGPIOs()
    static const uint16_t BMGPIO1 = 0x0010;   // Bitmap for the GPIO.1 pin - Also applicable to getGPIOs()/setGPIOs()
    static const uint16_t BMGPIO2 = 0x0020;   // Bitmap for the GPIO.2 pin - Also applicable to getGPIOs()/setGPIOs()
    static const uint16_t BMGPIO3 = 0x0040;   // Bitmap for the GPIO.3 pin - Also applicable to getGPIOs()/setGPIOs()
    static const uint16_t BMGPIO4 = 0x0080;   // Bitmap for the GPIO.4 pin - Also applicable to getGPIOs()/setGPIOs()
    static const uint16_t BMGPIO5 = 0x0100;   // Bitmap for the GPIO.5 pin - Also applicable to getGPIOs()/setGPIOs()
    static const uint16_t BMVPP = 0x0200;     // Bitmap for the VPP pin
    static const uint16_t BMGPIO6 = 0x0400;   // Bitmap for the GPIO.6 pin - Also applicable to getGPIOs()/setGPIOs()
    static const uint16_t BMGPIO7 = 0x0800;   // Bitmap for the GPIO.7 pin - Also applicable to getGPIOs()/setGPIOs()
    static const uint16_t BMGPIO8 = 0x1000;   // Bitmap for the GPIO.8 pin - Also applicable to getGPIOs()/setGPIOs()
    static const uint16_t BMGPIO9 = 0x2000;   // Bitmap for the GPIO.9 pin - Also applicable to getGPIOs()/setGPIOs()
    static const uint16_t BMGPIO10 = 0x4000;  // Bitmap for the GPIO.10 pin - Also applicable to getGPIOs()/setGPIOs()
    static const uint16_t BMGPIOS = 0x7df8;   // Bitmap for all GPIO pins
    static const uint16_t BMENABLE = 0x8000;  // Bitmap for suspend mode and level enable, only applicable to PinConfig.sspndmode (suspend pin mode bitmap)

    // The following values are applicable to USBConfig/getUSBConfig()/writeUSBConfig()
    static const uint8_t PMBUSREGEN = 0x00;   // Value corresponding to USB bus-powered mode with voltage regulator enabled
    static const uint8_t PMBUSREGDIS = 0x01;  // Value corresponding to USB bus-powered mode with voltage regulator disabled
    static const uint8_t PMSELFREGEN = 0x02;  // Value corresponding to USB self-powered mode with voltage regulator enabled
    static const uint8_t PRIOREAD = 0x00;     // Value corresponding to data transfer with high priority read
    static const uint8_t PRIOWRITE = 0x01;    // Value corresponding to data transfer with high priority write

    struct EventCounter {
        bool overflow;   // Overflow flag
        uint8_t mode;    // GPIO.4/EVTCNTR pin mode (see the values applicable to PinConfig/getPinConfig()/writePinConfig())
        uint16_t value;  // Count value (big-endian)

        bool operator ==(const EventCounter &other) const;
        bool operator !=(const EventCounter &other) const;
    };

    struct PinConfig {
        uint8_t gpio0;       // GPIO.0 pin config
        uint8_t gpio1;       // GPIO.1 pin config
        uint8_t gpio2;       // GPIO.2 pin config
        uint8_t gpio3;       // GPIO.3 pin config
        uint8_t gpio4;       // GPIO.4 pin config
        uint8_t gpio5;       // GPIO.5 pin config
        uint8_t gpio6;       // GPIO.6 pin config
        uint8_t gpio7;       // GPIO.7 pin config
        uint8_t gpio8;       // GPIO.8 pin config
        uint8_t gpio9;       // GPIO.9 pin config
        uint8_t gpio10;      // GPIO.10 pin config
        uint16_t sspndlvl;   // Suspend pin level bitmap (big-endian - see Silicon Labs AN792 for details)
        uint16_t sspndmode;  // Suspend pin mode bitmap (big-endian - see Silicon Labs AN792 for details)
        uint16_t wkupmask;   // Wakeup pin mask bitmap (big-endian - see Silicon Labs AN792 for details)
        uint16_t wkupmatch;  // Wakeup pin match bitmap (big-endian - see Silicon Labs AN792 for details)
        uint8_t divider;     // GPIO.5/!CS5/CLKOUT OTP ROM clock divider value (see Silicon Labs AN792 for details)

        bool operator ==(const PinConfig &other) const;
        bool operator !=(const PinConfig &other) const;
    };

    struct PROMConfig {
        uint8_t blocks[PROM_BLOCKS][PROM_BLOCK_SIZE];  // OTP ROM blocks

        bool operator ==(const PROMConfig &other) const;
        bool operator !=(const PROMConfig &other) const;
        uint8_t &operator [](size_t index);
        const uint8_t &operator [](size_t index) const;
    };

    struct SiliconVersion {
        uint8_t maj;  // Major read-only version
        uint8_t min;  // Minor read-only version

        bool operator ==(const SiliconVersion &other) const;
        bool operator !=(const SiliconVersion &other) const;
    };

    struct SPIDelays {
        bool cstglen;        // CS toggle enable
        bool prdasten;       // Pre-deassert delay enable
        bool pstasten;       // Post-assert delay enable
        bool itbyten;        // Inter-byte delay enable
        uint16_t prdastdly;  // Pre-deassert delay (10us units, big-endian)
        uint16_t pstastdly;  // Post-assert delay (10us units, big-endian)
        uint16_t itbytdly;   // Inter-byte delay (10us units, big-endian)

        bool operator ==(const SPIDelays &other) const;
        bool operator !=(const SPIDelays &other) const;
    };

    struct SPIMode {
        bool csmode;   // Chip select mode (false for open drain, true for push-pull)
        uint8_t cfrq;  // Clock frequency
        bool cpol;     // Clock polarity
        bool cpha;     // Clock phase

        bool operator ==(const SPIMode &other) const;
        bool operator !=(const SPIMode &other) const;
    };

    struct USBConfig {
        uint16_t vid;     // Vendor ID (little-endian)
        uint16_t pid;     // Product ID (little-endian)
        uint8_t majrel;   // Major release version
        uint8_t minrel;   // Minor release version
        uint8_t maxpow;   // Maximum consumption current (raw value in 2mA units)
        uint8_t powmode;  // Power mode
        uint8_t trfprio;  // Transfer priority

        bool operator ==(const USBConfig &other) const;
        bool operator !=(const USBConfig &other) const;
    };

    CP2130();
    ~CP2130();

    bool disconnected() const;
    bool isOpen() const;

    void bulkTransfer(uint8_t endpointAddr, unsigned char *data, int length, int *transferred, int &errcnt, std::string &errstr);
    void close();
    void configureGPIO(uint8_t pin, uint8_t mode, bool value, int &errcnt, std::string &errstr);
    void configureSPIDelays(uint8_t channel, const SPIDelays &delays, int &errcnt, std::string &errstr);
    void configureSPIMode(uint8_t channel, const SPIMode &mode, int &errcnt, std::string &errstr);
    void controlTransfer(uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, int &errcnt, std::string &errstr);
    void disableCS(uint8_t channel, int &errcnt, std::string &errstr);
    void disableSPIDelays(uint8_t channel, int &errcnt, std::string &errstr);
    void enableCS(uint8_t channel, int &errcnt, std::string &errstr);
    uint8_t getClockDivider(int &errcnt, std::string &errstr);
    bool getCS(uint8_t channel, int &errcnt, std::string &errstr);
    uint8_t getEndpointInAddr(int &errcnt, std::string &errstr);
    uint8_t getEndpointOutAddr(int &errcnt, std::string &errstr);
    EventCounter getEventCounter(int &errcnt, std::string &errstr);
    uint8_t getFIFOThreshold(int &errcnt, std::string &errstr);
    bool getGPIO0(int &errcnt, std::string &errstr);
    bool getGPIO1(int &errcnt, std::string &errstr);
    bool getGPIO2(int &errcnt, std::string &errstr);
    bool getGPIO3(int &errcnt, std::string &errstr);
    bool getGPIO4(int &errcnt, std::string &errstr);
    bool getGPIO5(int &errcnt, std::string &errstr);
    bool getGPIO6(int &errcnt, std::string &errstr);
    bool getGPIO7(int &errcnt, std::string &errstr);
    bool getGPIO8(int &errcnt, std::string &errstr);
    bool getGPIO9(int &errcnt, std::string &errstr);
    bool getGPIO10(int &errcnt, std::string &errstr);
    uint16_t getGPIOs(int &errcnt, std::string &errstr);
    uint16_t getLockWord(int &errcnt, std::string &errstr);
    std::u16string getManufacturerDesc(int &errcnt, std::string &errstr);
    PinConfig getPinConfig(int &errcnt, std::string &errstr);
    std::u16string getProductDesc(int &errcnt, std::string &errstr);
    PROMConfig getPROMConfig(int &errcnt, std::string &errstr);
    std::u16string getSerialDesc(int &errcnt, std::string &errstr);
    SiliconVersion getSiliconVersion(int &errcnt, std::string &errstr);
    SPIDelays getSPIDelays(uint8_t channel, int &errcnt, std::string &errstr);
    SPIMode getSPIMode(uint8_t channel, int &errcnt, std::string &errstr);
    uint8_t getTransferPriority(int &errcnt, std::string &errstr);
    USBConfig getUSBConfig(int &errcnt, std::string &errstr);
    bool isOTPBlank(int &errcnt, std::string &errstr);
    bool isOTPLocked(int &errcnt, std::string &errstr);
    bool isRTRActive(int &errcnt, std::string &errstr);
    void lockOTP(int &errcnt, std::string &errstr);
    int open(uint16_t vid, uint16_t pid, const std::string &serial = std::string());
    void reset(int &errcnt, std::string &errstr);
    void selectCS(uint8_t channel, int &errcnt, std::string &errstr);
    void setClockDivider(uint8_t value, int &errcnt, std::string &errstr);
    void setEventCounter(const EventCounter &evcntr, int &errcnt, std::string &errstr);
    void setFIFOThreshold(uint8_t threshold, int &errcnt, std::string &errstr);
    void setGPIO0(bool value, int &errcnt, std::string &errstr);
    void setGPIO1(bool value, int &errcnt, std::string &errstr);
    void setGPIO2(bool value, int &errcnt, std::string &errstr);
    void setGPIO3(bool value, int &errcnt, std::string &errstr);
    void setGPIO4(bool value, int &errcnt, std::string &errstr);
    void setGPIO5(bool value, int &errcnt, std::string &errstr);
    void setGPIO6(bool value, int &errcnt, std::string &errstr);
    void setGPIO7(bool value, int &errcnt, std::string &errstr);
    void setGPIO8(bool value, int &errcnt, std::string &errstr);
    void setGPIO9(bool value, int &errcnt, std::string &errstr);
    void setGPIO10(bool value, int &errcnt, std::string &errstr);
    void setGPIOs(uint16_t bmValues, uint16_t bmMask, int &errcnt, std::string &errstr);
    std::vector<uint8_t> spiRead(uint32_t bytesToRead, uint8_t endpointInAddr, uint8_t endpointOutAddr, int &errcnt, std::string &errstr);
    std::vector<uint8_t> spiRead(uint32_t bytesToRead, int &errcnt, std::string &errstr);
    void spiWrite(const std::vector<uint8_t> &data, uint8_t endpointOutAddr, int &errcnt, std::string &errstr);
    void spiWrite(const std::vector<uint8_t> &data, int &errcnt, std::string &errstr);
    std::vector<uint8_t> spiWriteRead(const std::vector<uint8_t> &data, uint8_t endpointInAddr, uint8_t endpointOutAddr, int &errcnt, std::string &errstr);
    std::vector<uint8_t> spiWriteRead(const std::vector<uint8_t> &data, int &errcnt, std::string &errstr);
    void stopRTR(int &errcnt, std::string &errstr);
    void writeLockWord(uint16_t word, int &errcnt, std::string &errstr);
    void writeManufacturerDesc(const std::u16string &manufacturer, int &errcnt, std::string &errstr);
    void writePinConfig(const PinConfig &config, int &errcnt, std::string &errstr);
    void writeProductDesc(const std::u16string &product, int &errcnt, std::string &errstr);
    void writePROMConfig(const PROMConfig &config, int &errcnt, std::string &errstr);
    void writeSerialDesc(const std::u16string &serial, int &errcnt, std::string &errstr);
    void writeUSBConfig(const USBConfig &config, uint8_t mask, int &errcnt, std::string &errstr);

    static std::list<std::string> listDevices(uint16_t vid, uint16_t pid, int &errcnt, std::string &errstr);
};

#endif  // CP2130_H
