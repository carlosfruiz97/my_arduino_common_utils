#include "my_arduino_common_utils.h"

/* --------------------------------------------------------------------------
 * Function: byte_array_to_string. Convert byte array to string
 * Parameter:- uint8_t   *byte_array    input array
 *           - uint8_t    arry_length   array size
 * Returns: String array converted to string
 *  -------------------------------------------------------------------------- */
String byte_array_to_string(uint8_t *byte_array, uint8_t arry_length)
{
  String  output = "";
  uint8_t b;

  for (uint8_t i = 0; i < arry_length; i++)
  {
    b = byte_array[i];

    if (b < 16)
    {
      output += "0";
    }
    output += String(b, HEX);
    output += " ";
  }
  output.trim();
  return output;
}


/* --------------------------------------------------------------------------
 * Function: sbyte2hex. Returns a byte data in HEX format.
 * Parameter:- uint8_t b. Byte to convert to HEX.
 *  -------------------------------------------------------------------------- */
String sbyte2hex(uint8_t b)
{
  String shex;

  shex = "0X";

  if (b < 16)
  {
    shex += "0";
  }
  shex += String(b, HEX);
  shex += " ";
  return shex;
}
