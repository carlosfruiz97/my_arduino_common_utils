#define DEBUG    1
#if DEBUG == 1
#define LOGN(x)    Serial.println(x)
#define LOG(x)     Serial.print(x)
#else
#define LOG(x)
#define LOGN(x)
#endif


// Import Utils library and serial_mp3 module
#include <my_arduino_common_utils.h>
#include <serial_mp3/serial_mp3.h>

// Initiate Serial mp3 module
SerialMp3 mp3_player;


void setup()
{
  Serial.begin(9600);
  Serial.println("Starting Debug Serial");

  // Initate Stream class that will comunicate with mp3
  Serial1.begin(9600);
  mp3_player.init(Serial1);

  // PLay from folder
  Serial.println("Play 01/01");
//  mp3_player.playFromFolder(1, 1);
}


String msg, hex_resp_string;

void loop()
{
  // Mp3 State
  if (mp3_player.decodeMP3Answer(msg, hex_resp_string))
  {
    LOG("\nMp3 Response:    ");
    LOGN(msg);
    LOG("  -  Hex Arry:   ");
    LOG(hex_resp_string);
    LOGN("");
  }

  // Serial Monitor comands
  if (Serial.available())
  {
    char    c = Serial.read();
    uint8_t send_buffer[8];
    switch (c)
    {
    case 'p':
      // Pause!
      mp3_player.pause();
      break;

    case 'r':
      // Resume!
      mp3_player.resume();
      break;

    case 'q':
      // Stop!
      mp3_player.stop();
      break;


    case 'v':
      // Volume!
      mp3_player.setVolume(15);
      break;

    case 'V':
      // Volume!
      mp3_player.setVolume(30);
      break;


    case 'f':
      // PLay from folder 01/002xxx.mp3!
      mp3_player.playFromFolder(1, 2);
      break;

    case 'F':
      // PLay from folder 03/003xxx.mp3!
      mp3_player.playFromFolder(3, 3);
      break;



    case 'c':
      // Cicle folder 02!
      mp3_player.sendCommand(CMD_FOLDER_CYCLE, 2);
      break;
    }
  }
}
