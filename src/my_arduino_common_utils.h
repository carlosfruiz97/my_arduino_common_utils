/*
 * Main header function for the library. Loads utility functions used throught the
 * library.
 *
 * To include a submodule do the following:
 #include<my_arduino_common_utils.h>
 #include<serial_mp3/serial_mp3.h>
 */

#ifndef MY_ARDUINO_COMMON_UTILS_H
#define MY_ARDUINO_COMMON_UTILS_H

#include "Arduino.h"

// class stuff {};

/* --------------------------------------------------------------------------
 * Function: byte_array_to_string. Convert byte array to string
 * Parameter:- uint8_t   *byte_array    input array
 *           - uint8_t    arry_length   array size
 * Returns: String array converted to string
 *  -------------------------------------------------------------------------- */
String byte_array_to_string(uint8_t *byte_array, uint8_t arry_length);

/* --------------------------------------------------------------------------
 * Function: sbyte2hex. Returns a byte data in HEX format.
 * Parameter:- uint8_t b. Byte to convert to HEX.
 *  -------------------------------------------------------------------------- */
String sbyte2hex(uint8_t b);

#endif
