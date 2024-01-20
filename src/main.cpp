#include <Arduino.h>
#include "gui/gui.h"

// select I2C sound library
#undef ESP8266_Audio
#define ESP32_audioI2S

#ifdef  ESP8266_Audio
  /* I2S Sound PIN definitions. According to print on PCB board:
  I018 = LRCLK  Word Select WS  (Frame Clock = LRCLK)
  I042 = BCLK   SerClk SCK   (Bit Clock=BCLK)
  I017 = SDIN   SerData SD
  */
  #define LRCLK 18  
  #define I2S_BCLK 42
  #define I2S_SDIN 17

  #include "AudioGeneratorAAC.h"
  #include "AudioOutputI2S.h"
  #include "AudioFileSourcePROGMEM.h"
  #include "sampleaac.h"

  AudioFileSourcePROGMEM *in;
  AudioGeneratorAAC *aac;
  AudioOutputI2S *out;
#endif //  ESP8266_Audio  

#ifdef ESP32_audioI2S
  #include "WiFiMulti.h"
  #include "Audio.h"
  #include "SD.h"
  #include "FS.h"

  #include "Credentials.h"

  // Pins fro I2S
  #define I2S_DOUT      17
  #define I2S_BCLK      42
  #define I2S_LRC       18

  // Pins for SD Card. For Elecrow display from schematic: ESP32_Display_7.0inch-V1.1-20230407.pdf
  #define SD_CS         10
  #define SPI_MOSI      11
  #define SPI_MISO      13
  #define SPI_SCK       12

  Audio audio;
  WiFiMulti wifiMulti;
#endif // ESP32_audioI2S

char printstring[200];
int noTitles = 11;
char title[15][50]={
  "demo01.mp3",
  "demo02.mp3",
  "demo03.mp3",
  "demo04.mp3",
  "demo05.mp3",
  "demo06.mp3",
  "demo07.mp3",
  "demo08.mp3",
  "demo09.mp3",
  "demo10.mp3",
  "demo11.mp3",
  NULL
};

//---------------- setup() ------------------
void setup() {
  Serial.begin(115200);
  gui_start();
  drawExamples();

  // audio stuff
  #ifdef ESP8266_Audio   
    // https://diyi0t.com/i2s-sound-tutorial-for-esp32/
    in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac));
    aac = new AudioGeneratorAAC();
    out = new AudioOutputI2S();
    //out -> SetGain(0.125);
    out -> SetPinout(I2S_BCLK,LRCLK,I2S_SDIN);//SetPinout(int bclk, int wclk, int dout)
  
    aac->begin(in, out);
  #endif //  ESP8266_Audio  

  #ifdef ESP32_audioI2S
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(21); // default 0...21

    //--- Wifi connection
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(default_ssid, default_pass);
    wifiMulti.run();
    if(WiFi.status() != WL_CONNECTED){
        WiFi.disconnect(true);
        wifiMulti.run();
    }

    //--- SD Card
    pinMode(SD_CS, OUTPUT);      
    digitalWrite(SD_CS, HIGH);
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    SD.begin(SD_CS);

    //--- audio
    audio.forceMono(true);  // we have only one speaker, force mono.

    //  *** radio streams ***
    // getestete Steams MP
    // funktioniert:
    // audio.connecttohost("https://dispatcher.rndfnk.com/hr/hrinfo/live/mp3/high"); // HR Info
    //    audio.connecttohost("http://metafiles.gl-systemhaus.de/hr/hr1_2.m3u"); // HR1
    //    audio.connecttohost("http://www.dradio.de/streaming/dlf.m3u"); // Deutschlandfunk


    //    audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u");
    //    audio.connecttohost("http://somafm.com/wma128/missioncontrol.asx"); //  asx
    //    audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.aac"); //  128k aac

    // audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.mp3"); //  128k mp3

    // audio.connecttohost("http://stream.antennethueringen.de/live/aac-64/stream.antennethueringen.de/"); // aac
    //  audio.connecttohost("http://mcrscast.mcr.iol.pt/cidadefm");                                         // mp3
    //  audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u");                                // m3u
    //  audio.connecttohost("https://stream.srg-ssr.ch/rsp/aacp_48.asx");                                   // asx
    //  audio.connecttohost("http://tuner.classical102.com/listen.pls");                                    // pls
    //  audio.connecttohost("http://stream.radioparadise.com/flac");                                        // flac
    //  audio.connecttohost("http://stream.sing-sing-bis.org:8000/singsingFlac");                           // flac (ogg)
    //  audio.connecttohost("http://s1.knixx.fm:5347/dein_webradio_vbr.opus");                              // opus (ogg)
    //  audio.connecttohost("http://stream2.dancewave.online:8080/dance.ogg");                              // vorbis (ogg)
    //  audio.connecttohost("http://26373.live.streamtheworld.com:3690/XHQQ_FMAAC/HLSTS/playlist.m3u8");    // HLS
    //  audio.connecttohost("http://eldoradolive02.akamaized.net/hls/live/2043453/eldorado/master.m3u8");   // HLS (ts)
    //  *** web files ***

    // funktiniert definitv - MP
    // audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Pink-Panther.wav");        // wav

    //  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Santiano-Wellerman.flac"); // flac
    // audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Olsen-Banden.mp3");        // mp3
    //  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Miss-Marple.m4a");         // m4a (aac)
    //  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Collide.ogg");             // vorbis
    //  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/sample.opus");             // opus
    //  *** local files ***

    // funktiniert definitv - MP
      audio.connecttoFS(SD, "/test.wav");     // SD
    
    //  audio.connecttoFS(SD, "/sherylcrow_noonesaiditwouldbeeasy.mp3");     // SD  
    //  audio.connecttoFS(SD_MMC, "/test.wav"); // SD_MMC
    //  audio.connecttoFS(SPIFFS, "/test.wav"); // SPIFFS

    //  audio.connecttospeech("Wenn die Hunde schlafen, kann der Wolf gut Schafe stehlen.", "de"); // Google TTS

  #endif // ESP32_audioI2S
}

#ifdef ESP8266_Audio   
  // test i2S Sound
  void soundTest()
  {
    if (aac->isRunning()) {
      aac->loop();
    } else {
      aac -> stop();
      Serial.printf("Sound Generator\n");
      delay(1000);
    }
  }
#endif //  ESP8266_Audio 


void loop() {
  #ifdef ESP8266_Audio   
    soundTest();
  #endif  
  //drawInLoop();
  touchpadTest();
  //delay(20);

  #ifdef ESP32_audioI2S
    audio.loop();
    
    if(Serial.available()){ // put streamURL in serial monitor
        audio.stopSong();
        String r=Serial.readString(); r.trim();
        if(r.length()>5) audio.connecttohost(r.c_str());
        log_i("free heap=%i", ESP.getFreeHeap());
    }
    
  #endif   
  //Serial.print(".");
}

//--------------- audio event callback functions

// end of audio file
void audio_eof_mp3(const char *info)
{
  static int count = 0;
  sprintf(printstring,"audio_eof_mp3 - %s", info);
  Serial.print(printstring);
  audio.connecttoFS(SD, title[count]);     // SD  
  count++;
  if(count>=noTitles)
    count = 0;
}

void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}

void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}
void audio_eof_speech(const char *info){
    Serial.print("eof_speech  ");Serial.println(info);
}
