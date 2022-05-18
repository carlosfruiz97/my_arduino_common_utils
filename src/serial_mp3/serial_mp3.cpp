#include "serial_mp3.h"
#include <my_arduino_common_utils.h>

SerialMp3::SerialMp3()
{
  stream_obj_set_ = false;
}


bool SerialMp3::init(Stream& inputStream)
{
  this->mp3             = &inputStream;
  this->stream_obj_set_ = true;

  // Set volume to max
  this->setVolume(30);
}


/* --------------------------------------------------------------------------
 * Function: Send command to the MP3
 * Parameter: - HardwareSerial&  mp3 serial
 * Parameter: - int8_t command
 * Parameter: - int16_t dat         parameter for the command
 * Parameter:- uint8_t *Send_buf    (optional) Send buffer array
 * -------------------------------------------------------------------------- */
void SerialMp3::sendCommand(int8_t command, int16_t dat, int8_t *Send_buf)
{
  delay(20);
  Send_buf[0] = 0x7e;               //
  Send_buf[1] = 0xff;               //
  Send_buf[2] = 0x06;               // Len
  Send_buf[3] = command;            //
  Send_buf[4] = 0x01;               // 0x00 NO, 0x01 feedback
  Send_buf[5] = (int8_t)(dat >> 8); //datah
  Send_buf[6] = (int8_t)(dat);      //datal
  Send_buf[7] = 0xef;               //

  // LOG("Sending: ");
  for (uint8_t i = 0; i < 8; i++)
  {
    this->mp3->write(Send_buf[i]);
    // LOG(sbyte2hex(Send_buf[i]));
  }
  // LOGN("");
}


void SerialMp3::sendCommand(int8_t command, int16_t dat)
{
  uint8_t send_buffer[8];

  this->sendCommand(command, dat, send_buffer);
}


void SerialMp3::sendCommand(int8_t command)
{
  this->sendCommand(command, 0);
}


/* --------------------------------------------------------------------------
 * Function: playFromFolder. Plays file from folder
 * Parameter:- uint8_t folder       Numero Carpeta
 * Parameter:- uint8_t song_number  Numero Cancion
 * Parameter:- uint8_t *Send_buf    (optional) Send buffer array
 *  -------------------------------------------------------------------------- */
void SerialMp3::playFromFolder(uint8_t folder, uint8_t song_number, int8_t *Send_buf)
{
  uint16_t dat = 0;

  dat |= folder << 8;
  dat |= song_number;
  this->sendCommand(CMD_PLAY_FOLDER_FILE, dat, Send_buf);
}


void SerialMp3::playFromFolder(uint8_t folder, uint8_t song_number)
{
  uint8_t send_buffer[8];

  this->playFromFolder(folder, song_number, send_buffer);
}


/* --------------------------------------------------------------------------
 *  UTILITY FUNCTIONS
 *  -------------------------------------------------------------------------- */
void SerialMp3::setVolume(int16_t volume)
{
  if (volume > 30)
  {
    volume = 30;
  }
  this->sendCommand(CMD_SET_VOLUME, volume);
}


void SerialMp3::pause()
{
  this->sendCommand(CMD_PAUSE);
}


void SerialMp3::resume()
{
  this->sendCommand(CMD_PLAY);
}


void SerialMp3::stop()
{
  this->sendCommand(CMD_STOP_PLAY);
}


/* --------------------------------------------------------------------------
 * Function decodeMP3Answer: Decode MP3 answer.
 * Parameter: String&            msg            Stores interpreted msg
 * Parameter: uint8_t *          hex_response   Array response
 * Return:    Did it read anything?
 *  -------------------------------------------------------------------------- */
bool SerialMp3::decodeMP3Answer(String& msg)
{
  uint8_t hex_response[10];

  bool decoded = this->decodeMP3Answer(msg, hex_response);

  return decoded;
}


bool SerialMp3::decodeMP3Answer(String& msg, String& hex_resp_string)
{
  uint8_t hex_response[10];

  bool decoded = this->decodeMP3Answer(msg, hex_response);

  /* Convert to String */
  if (decoded)
  {
    hex_resp_string = byte_array_to_string(hex_response, 10);
  }

  return decoded;
}


bool SerialMp3::decodeMP3Answer(String& msg, uint8_t *hex_response)
{
  if (mp3->available())
  {
    bool response_valid = false;
    msg            = "";
    response_valid = this->sanswer(hex_response);
    if (!response_valid)
    {
      msg += "Invalid Response";
    }
    else
    {
      switch (hex_response[3])
      {
      case 0x3A:
        msg += "Memory card inserted";
        break;

      case 0x3B:
        msg += "Memory card removed";
        break;

      case 0x3D:
        msg += "Completed play num " + String(hex_response[6], DEC);
        break;

      case 0x40:
        msg += "Error";
        break;

      case 0x41:
        msg += "Data recived correctly";
        break;

      case 0x42:
        msg += "Status playing: " + String(hex_response[6], DEC);
        break;

      case 0x48:
        msg += "File count: " + String(hex_response[6], DEC);
        break;

      case 0x4C:
        msg += "Playing: " + String(hex_response[6], DEC);
        break;

      case 0x4E:
        msg += "Folder file count: " + String(hex_response[6], DEC);
        break;

      case 0x4F:
        msg += "Folder count: " + String(hex_response[6], DEC);
        break;

      default:
        msg += "??? Unknown response";
        break;
      }
    }

    return true;
  }
  return false;
}


/* --------------------------------------------------------------------------
 * Function: sanswer. Returns a String answer from mp3 UART module.
 * Parameter: - HardwareSerial&  mp3 serial
 *            - uint8_t *hex_response  array of 10 elements for storing mp3 response
 * Return: String. If the answer is well formated answer.
 *  -------------------------------------------------------------------------- */
bool SerialMp3::sanswer(uint8_t *hex_response)
{
  uint8_t i = 0;

  // Get only 10 Bytes
  uint32_t start_time = millis();
  bool     esperar_serial = true;
  bool     time_out, ultimo_byte;


  while (esperar_serial)
  {
    if (mp3->available())
    {
      uint8_t b = mp3->read();
      hex_response[i] = b;
      i++;
    }


    // Check si contiuar
    time_out       = (millis() - start_time) > TIMEOUT_MP3_SERIAL;
    ultimo_byte    = i == 10;
    esperar_serial = !time_out && !ultimo_byte;
  }

  bool primer_check = (hex_response[0] & 0xFF) == 0x7E;
  bool ultimo_check = (hex_response[9] & 0xFF) == 0xEF;

  // if the answer format is correct.
  if (primer_check && ultimo_check)
  {
    return true;
  }

  return false;
}
