/*
 * Codigo muestra para controlar un mp3 serial
 * Fuentes:
 * - https://github.com/cefaloide/ArduinoSerialMP3Player
 * - http://geekmatic.in.ua/pdf/Catalex_MP3_board.pdf
 * -
 * Nombres archivos para que funcione
 |
 |-01
 |  |- 001xxx.mp3
 |  |- 002xxx.mp3
 |-02
 |  |- 001xxx.mp3
 |  |- 002xxx.mp3
 |  |- 005xxx.mp3
 */

#include "Arduino.h"

#ifndef SERIAL_MP3_H
#define SERIAL_MP3_H

/************ Command byte **************************/
#define CMD_NEXT_SONG           0X01 // Play next song.
#define CMD_PREV_SONG           0X02 // Play previous song.
#define CMD_PLAY_W_INDEX        0X03 // Play with index. Dat: 0x0001, (1st song), 0x0002 (2nd song)
#define CMD_VOLUME_UP           0X04 // Increase Volume by one
#define CMD_VOLUME_DOWN         0X05 // Decrese volumne by one
#define CMD_SET_VOLUME          0X06 // Set volume from 0 (0x0) to 30 (0x1E)

#define CMD_SNG_CYCL_PLAY       0X08 // Single Cycle Play.
#define CMD_SEL_DEV             0X09 // Select device
#define CMD_SLEEP_MODE          0X0A //
#define CMD_WAKE_UP             0X0B
#define CMD_RESET               0X0C
#define CMD_PLAY                0X0D // Resume playback
#define CMD_PAUSE               0X0E // Pasue playback
#define CMD_PLAY_FOLDER_FILE    0X0F // Play song by folder.
// Dat: 0x0101 Play song /01/001xxx.mp3
// Dat: 0x0102 Play song /01/002xxx.mp3

#define CMD_STOP_PLAY            0X16
#define CMD_FOLDER_CYCLE         0X17 // Cycle with folder name (dat:0x01 folder cycle 01)
#define CMD_SHUFFLE_PLAY         0x18 //
#define CMD_SET_SNGL_CYCL        0X19 // Set single cycle.

#define CMD_SET_DAC              0X1A
#define DAC_ON                   0X00
#define DAC_OFF                  0X01

#define CMD_PLAY_W_VOL           0X22
#define CMD_PLAYING_N            0x4C
#define CMD_QUERY_STATUS         0x42
#define CMD_QUERY_VOLUME         0x43
#define CMD_QUERY_FLDR_TRACKS    0x4e
#define CMD_QUERY_TOT_TRACKS     0x48
#define CMD_QUERY_FLDR_COUNT     0x4f

/************ Opitons **************************/
#define DEV_TF                   0X02

#define TIMEOUT_MP3_SERIAL       2000


class SerialMp3
{
public:
  String mp3Answer;                  // Answer from the MP3.
  Stream *mp3;
  bool   stream_obj_set_;

  SerialMp3();

  bool init(Stream&);

  /* --------------------------------------------------------------------------
   * Function: Send command to the MP3
   * Parameter: - int8_t command
   * Parameter: - int16_t dat         parameter for the command
   * Parameter:- uint8_t *Send_buf    (optional) Send buffer array
   * -------------------------------------------------------------------------- */
  void sendCommand(int8_t command, int16_t dat, int8_t *Send_buf);
  void sendCommand(int8_t command, int16_t dat);
  void sendCommand(int8_t command);

  /* --------------------------------------------------------------------------
   * Function: playFromFolder. Plays file from folder
   * Parameter:- uint8_t folder       Numero Carpeta
   * Parameter:- uint8_t song_number  Numero Cancion
   * Parameter:- uint8_t *Send_buf    (optional) Send buffer array
   *  -------------------------------------------------------------------------- */
  void playFromFolder(uint8_t folder, uint8_t song_number, int8_t *Send_buf);
  void playFromFolder(uint8_t folder, uint8_t song_number);


  void setVolume(int16_t volume); // 0-30
  void pause();
  void resume();
  void stop();

  /* --------------------------------------------------------------------------
   * Function decodeMP3Answer: Decode MP3 answer.
   * Parameter: String&            byte_resp      Stores byte response in hex
   * Parameter: String&            msg            Stores interpreted msg
   * Return:    Did it read anything?
   *  -------------------------------------------------------------------------- */
  bool decodeMP3Answer(String& msg);
  bool decodeMP3Answer(String& msg, String& hex_resp_string);
  bool decodeMP3Answer(String& msg, uint8_t *hex_response);


  /* --------------------------------------------------------------------------
   * Function: sanswer. Returns a String answer from mp3 UART module.
   * Parameter: - HardwareSerial&  mp3 serial
   *            - uint8_t *hex_response  array of 10 elements for storing mp3 response
   * Return: String. If the answer is well formated answer.
   *  -------------------------------------------------------------------------- */
  bool sanswer(uint8_t *hex_response);
};

#endif
