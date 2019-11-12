# UART

## registers

### status register

  - bit 0 (Receive complete)
    value 1: byte
  - bit 2 (Transmit complete)
  - bit 3 (Transmit active)

### data received register
  
8-bit value of the last received byte from the serial port.

### data sending register

8-bit value of the next byte to send via uart. 
