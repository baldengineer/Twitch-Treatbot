#include "treatbot_globals.h"

// Spark Fun Qwiic MP3
// http://librarymanager/All#SparkFun_MP3_Trigger
MP3TRIGGER mp3;
uint8_t mp3_volume = 18;

void setup_mp3() {
  print_message("MP3");
  if (mp3.begin() == false) {
    print_message("MP3 Fail");
  } else {
    mp3.setVolume(mp3_volume);
    mp3_play(1);
  }
}

void mp3_play(int track) {
  /* F001.mp3 - Dialing
     F002.mp3 - Connecting
     F003.mp3 - AddOhms VTR */

    char msg[10];
    sprintf(msg, "Track %d", track);
    print_message(msg);
    mp3.playFile(track);
    mp3_status();
}

void mp3_status() {
  Serial.print("Status: "); Serial.println(mp3.getStatus());
  int stat = mp3.getStatus();
  switch (stat) {
    case 0:
      Serial.println("OK");
      break;
    case 1:
      Serial.println("Fail");
      break;
    case 2:
      Serial.println("NO File");
      break;
    case 5:
      Serial.println("SD Error");
      break;
    case 255:
      Serial.println("255 ERROR!");
      break;
    default:
      Serial.print("Unknown MP3 error: ");
      Serial.println(stat);
  }
}