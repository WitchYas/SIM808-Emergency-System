# SIM808 Emergency System

A firmware project for the STM32F407 microcontroller that creates an emergency response system using a SIM808 GSM/GPRS module with integrated GPS capability.

## Project Overview

This embedded system is designed to:
- **Make emergency calls** to a predefined phone number
- **Send SMS messages** with GPS coordinates
- **Retrieve GPS location** data from SIM808's integrated GPS receiver
- **Provide visual feedback** via LED indicators

## Hardware Requirements

- **Microcontroller**: STM32F407 (ARM Cortex-M4)
- **GSM/GPS Module**: SIM808 or compatible
- **Communication**: UART/USART2 (PA2=TX, PA3=RX)
- **LED**: Connected to PD12 for status indication
- **Frequency**: 16 MHz system clock (configurable)
- **Baud Rate**: 9600 bps (UART communication)

## Project Structure

```
SIM808 emergency/
├── main.c              # Main application entry point
├── sim808.c/h          # SIM808 GSM/GPS module driver
├── uart.c/h            # USART2 communication driver
├── gpio.c/h            # GPIO configuration and control
├── delay.c/h           # Timing and delay utilities
└── README.md           # This file
```

## Module Descriptions

### 1. **main.c** - Application Logic
Contains the `main()` function that orchestrates the emergency sequence:
1. Initialize GPIO, UART, and SIM808
2. Turn on LED to indicate system active
3. Make emergency call
4. Retrieve GPS coordinates
5. Send SMS with location link
6. Turn off LED

**Emergency Number**: `26367129` (configurable)
**Message Format**: Google Maps link with latitude and longitude

### 2. **sim808.c/h** - GSM/GPS Driver
Implements high-level interface to SIM808 module:

#### Functions:
- `configSIM808()` - Initialize SIM808 settings
- `GSM_MakeCall(phoneNumber)` - Initiate a call
- `GSM_SendSMS(phoneNumber, message)` - Send text message
- `GSM_GetGPS(lat, lon)` - Retrieve GPS coordinates

#### Key AT Commands Used:
| Command | Purpose |
|---------|---------|
| `AT+CLIP=1` | Enable calling line identification |
| `AT+CNMI=2,1,0,0,0` | Configure SMS notifications |
| `AT+CMGF=1` | Set SMS text mode |
| `AT&W` | Save settings to SIM808 memory |
| `ATD<number>;` | Make a call |
| `AT+CMGS="<number>"` | Send SMS |
| `AT+CGNSPWR=1` | Power on GPS |
| `AT+CGNSINF=0` | Request GPS information |

### 3. **uart.c/h** - Serial Communication Driver
Manages USART2 communication with SIM808 module:

#### Functions:
- `USART2_Init()` - Initialize USART2 at 9600 baud
- `UART2_SendChar(ch)` - Send single character
- `USART2_SendString(str)` - Send null-terminated string
- `uart2_available()` - Check if data is available to read
- `UART2_GetChar()` - Receive single character (blocking)

**Pin Configuration**:
- **PA2**: TX (Transmit)
- **PA3**: RX (Receive)
- **Baud Rate**: 9600
- **Data Bits**: 8
- **Stop Bits**: 1
- **Parity**: None

### 4. **gpio.c/h** - GPIO Control
Manages GPIO pin configuration and LED control:

#### Functions:
- `GPIO_init()` - Initialize GPIO pins
- `LED_On()` - Turn LED on (PD12 HIGH)
- `LED_Off()` - Turn LED off (PD12 LOW)
- `LED_Toggle()` - Toggle LED state

**LED Configuration**:
- **Port**: GPIOD
- **Pin**: PD12
- **Mode**: Output
- **Speed**: Low
- **Pull-up**: Enabled

### 5. **delay.c/h** - Timing Utility
Provides blocking delay functionality:

#### Functions:
- `delay(milliseconds)` - Blocking delay in milliseconds

**Note**: Uses busy-wait loop; for production, consider SysTick timer for better timing accuracy.

## Operating Sequence

### 1. Initialization Phase
```c
GPIO_init();      // Configure PD12 LED
USART2_Init();    // Setup UART at 9600 baud
configSIM808();   // Configure SIM808 module
```

### 2. Emergency Activation
```c
LED_On();                           // Visual feedback: system active
GSM_MakeCall("26367129");           // Place call (3 seconds)
GSM_GetGPS(lat, lon);              // Retrieve location
GSM_SendSMS("26367129", msg);      // Send SMS with coordinates
LED_Off();                          // Indicate completion
```

### 3. Communication Protocol
All SIM808 communication uses AT commands:
1. Command sent via UART2
2. 500ms - 3000ms delay (SIM808 processing time)
3. Response received from UART2 buffer
4. Response parsed for required data

## AT Command Reference

### Call Control
- `ATD<number>;` - Make a call (semicolon is required)
- `ATH` - Hang up current call

### SMS Operations
- `AT+CMGF=1` - Set text mode
- `AT+CMGF=0` - Set PDU mode
- `AT+CMGS="<number>"` - Send SMS (text mode)
- `AT+CMGR=<id>` - Read SMS

### GPS/Location
- `AT+CGNSPWR=1` - Power on GPS
- `AT+CGNSPWR=0` - Power off GPS
- `AT+CGNSINF=0` - Get GPS information

### Configuration
- `AT+CLIP=1` - Enable calling line identification
- `AT+CNMI=2,1,0,0,0` - Configure SMS notifications
- `AT&W` - Save settings
- `AT&F` - Factory reset

## Timing Information

### Typical Delays
- SMS Setup: 500ms per AT command
- Call Duration: 3000ms (configurable)
- GPS Acquisition: 3000ms minimum (can be 30+ seconds on cold start)
- SMS Transmission: 2000ms

### Critical Timings
- **Cold GPS Start**: 30-60 seconds (first lock after power-on)
- **Warm GPS Start**: 5-10 seconds
- **Hot GPS Start**: 1-5 seconds
- **SMS Send**: 2-5 seconds

## Pin Configuration Summary

### UART2 (Communication with SIM808)
| Pin | Function | Configuration |
|-----|----------|----------------|
| PA2 | TX | Alternate Function AF7 |
| PA3 | RX | Alternate Function AF7 |

### GPIO (LED Control)
| Pin | Function | Configuration |
|-----|----------|----------------|
| PD12| LED Output | Push-Pull Output, Low Speed |

## Limitations and Notes

1. **Blocking Operations**: All delays and UART reads are blocking (busy-wait)
2. **Buffer Sizes**: GPS buffer limited to 256 bytes; increase if needed
3. **Error Handling**: Minimal error checking; expand for production use
4. **GPS Accuracy**: Depends on SIM808 GPS reception (outdoor recommended)
5. **SMS Length**: Limited to 160 characters for standard SMS
6. **Timing**: Delay function is calibrated for 16 MHz clock; adjust if different

## Future Enhancements

- [ ] Implement interrupt-driven UART communication
- [ ] Add SysTick timer-based delay function
- [ ] Implement response validation and error checking
- [ ] Add support for multiple emergency numbers
- [ ] Integrate low-power modes
- [ ] Add battery level monitoring
- [ ] Implement SMS received handler
- [ ] Add web-based tracking integration


