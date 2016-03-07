/*
 * This sketch draws from the daphc example included in the WaveHC library for the Adafruit Arduino Waveshield, 
 * as well as the adavoice sketch for the Adafruit Microphone Amplifier Breakout.
 * 
 * This sketch detects microphone input to trigger WAV playback, and monitors input from 
 * an IR proximity sensor, manipulating WAV playback volume based on this analog input.
 * 
 * May Trump's life be short.
 */
#include <WaveHC.h>
#include <WaveUtil.h>

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

uint8_t dirLevel; // indent level for file/dir names    (for prettyprinting)
dir_t dirBuf;     // buffer for directory reads

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

const char *sounds[] = {
  "trump0" , "trump1" , "trump2" , "trump3" , "trump4" , "trump5" , "trump6" , "trump7"  }; 

int proximitySensorPin = 3; //analog pin 3

/*
 * Define macro to put error messages in flash memory
 */
#define error(msg) error_P(PSTR(msg))

// Function definitions (we define them here, but the code is below)
void play(FatReader &dir);

//////////////////////////////////// SETUP
void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps for debugging
  
  putstring_nl("\nWave test!");  // say we woke up!
  
  putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  Serial.println(FreeRam());

  //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    error("Card init. failed!");  // Something went wrong, lets print out why
  }
  
  // enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);
  
  // Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {   // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                           // we found one, lets bail
  }
  if (part == 5) {                     // if we ended up not finding one  :(
    error("No valid FAT partition!");  // Something went wrong, lets print out why
  }
  
  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(), DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    error("Can't open root dir!");      // Something went wrong,
  }
  
  // Whew! We got past the tough parts.
  putstring_nl("Files found (* = fragmented):");

  // Print out all of the files in all the directories.
  root.ls(LS_R | LS_FLAG_FRAGMENTED);
  //Play the startup sound, which is Donald Trump saying "Good morning"
  playFile(0);
}

//////////////////////////////////// LOOP
void loop() {

   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 3.3) / 1024;  // convert to volts
 
   Serial.println(volts);
  if (volts > 0.5){
    int fileIndex = random(1, 7);
    playFile(fileIndex);
  }
  //int proximityReading = analogRead(proximitySensorPin);
  //Serial.println(proximityReading);
  //delay(100);
}

/////////////////////////////////// HELPERS

void playFile(int index){
  FatReader file;
  char filename[14];
  (void)sprintf(filename,"%s.WAV", sounds[index]);

  if(!file.open(root, filename)) {
    PgmPrint("Couldn't open file ");
    Serial.print(filename);
    return;
  }
  if(!wave.create(file)) {
    PgmPrintln("Not a valid WAV");
    return;
  }
  wave.play();
  uint8_t n = 0;

  while (wave.isplaying) {// playing occurs in interrupts, so we print dots in realtime
    putstring(".");
    if (!(++n % 32))Serial.println();
    int proximityReading = analogRead(proximitySensorPin);
    if ( proximityReading > 200) {
      wave.volume = 2;
    }
    /*else if (false) {
      wave.volume = 1;
    }
    else if (false) {
      wave.volume = 2;
    }*/
    else {
      wave.volume = 0;
    }

    delay(100);
  }
}

/*
 * print error message and halt
 */
void error_P(const char *str) {
  PgmPrint("Error: ");
  SerialPrint_P(str);
  sdErrorCheck();
  while(1);
}
/*
 * print error message and halt if SD I/O error, great for debugging!
 */
void sdErrorCheck(void) {
  if (!card.errorCode()) return;
  PgmPrint("\r\nSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  PgmPrint(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}


