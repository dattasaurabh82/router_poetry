#include <Process.h>
#include <FileIO.h>

const int linkDataTranBtn = 4;
const int linkExtBtn = 6;
const int poemGenBtn = 5;
const int broadCastBtn = 7;

boolean linkExtState, linkDataTranState, poemGenState, broadCastState = LOW;
boolean prevLinkExtState, prevLinkDataTranState, prevPoemGenState, prevBroadCastState = LOW;

const int dataExtIndPin = 10;
const int linkExtIndPin = 11;
const int poemGenIndPin = 13;
const int poemRecitIndPin = 12;

int pin = 2;

void setup() {

  pinMode(linkDataTranBtn, INPUT);
  pinMode(linkExtBtn, INPUT);
  pinMode(poemGenBtn, INPUT);
  pinMode(broadCastBtn, INPUT);

  pinMode(dataExtIndPin, OUTPUT);
  pinMode(linkExtIndPin, OUTPUT);
  pinMode(poemGenIndPin, OUTPUT);
  pinMode(poemRecitIndPin, OUTPUT);

  Bridge.begin();
  Serial.begin(9600);
  FileSystem.begin();

  // Wait until a Serial Monitor is connected.
  while (!Serial);
  Serial.println("Serial has been established \n \n");
}

int counter[4] = {0, 0, 0, 0};

void loop() {
  linkDataTranState = digitalRead(linkDataTranBtn);
  linkExtState = digitalRead(linkExtBtn);
  poemGenState = digitalRead(poemGenBtn);
  broadCastState = digitalRead(broadCastBtn);


  if (linkDataTranState != prevLinkDataTranState) {
    if (linkDataTranState == HIGH) {
      counter[0] += 1;
      if (counter[0] == 1 && counter[1] == 0 && counter[2] == 0 && counter[3] == 0) {
        //Serial.println("Run Data extractor");
        dataExtractor();
        digitalWrite(dataExtIndPin, HIGH);
      } else {
        counter[0] = 1;
        blinkEndHigh(dataExtIndPin, 200);
      }
      //ArrayRes();
    }
    prevLinkDataTranState = linkDataTranState;

  }


  if (linkExtState != prevLinkExtState) {
    if (linkExtState == HIGH) {
      counter[1] += 1;
      if (counter[1] == 1 && counter[0] == 1 && counter[2] == 0 && counter[3] == 0) {
        //Serial.println("Run link extractor");
        linkExtractor();
        digitalWrite(linkExtIndPin, HIGH);
      } else if (counter[1] >= 1 && counter[0] == 0 && counter[2] == 0 && counter[3] == 0) {
        blinkEndLow(linkExtIndPin, 200);
        counter[1] = 0;
      } else {
        counter[1] = 1;
        blinkEndHigh(linkExtIndPin, 200);
        //Serial.println("Pressing same button again and again");
      }
      //ArrayRes();
    }
    prevLinkExtState = linkExtState;

  }

  if (poemGenState != prevPoemGenState) {
    if (poemGenState == HIGH) {
      counter[2] += 1;
      if (counter[2] == 1 && counter[0] == 1 && counter[1] == 1 && counter[3] == 0) {
        digitalWrite(poemGenIndPin, HIGH);
        //Serial.println("Run Poem generator");
        poemCreator();
      } else if (counter[2] >= 1 && counter[0] == 0 && counter[1] == 0 && counter[3] == 0) {
        counter[2] = 0;
        blinkEndLow(poemGenIndPin, 200);
      } else {
        counter[2] = 1;
        blinkEndHigh(poemGenIndPin, 200);
        //Serial.println("Pressing same button again and again");
      }
      //ArrayRes();
    }
    prevPoemGenState = poemGenState;

  }

  if (broadCastState != prevBroadCastState) {
    if (broadCastState == HIGH) {
      counter[3] += 1;
      if (counter[3] == 1 && counter[0] == 1 && counter[1] == 1 && counter[2] == 1) {
        digitalWrite(poemRecitIndPin, HIGH);
        Serial.println("Run Poet");
        poemReciter();
        //wait for 5 sec and reset
        delay(5000);
        reset();
      } else {
        blinkEndLow(poemRecitIndPin, 200);
      }
      //ArrayRes();
    }
    prevBroadCastState = broadCastState;
  }
}

void ArrayRes() {
  for (int i = 0; i <= 3; i++) {
    Serial.println(counter[i]);
  }
  Serial.println(" ------------- \n");
}

void reset() {
  for (int i = 0; i <= 3; i++) {
    counter[i] = 0;
  }
  digitalWrite(poemRecitIndPin, LOW);
  digitalWrite(poemGenIndPin, LOW);
  digitalWrite(linkExtIndPin, LOW);
  digitalWrite(dataExtIndPin, LOW);

}

void blinkEndHigh(int pinn, int delayy) {

  pinMode(pinn, OUTPUT);
  //
  digitalWrite(pinn, LOW);
  delay(delayy);
  digitalWrite(pinn, HIGH);
  delay(delayy);
  digitalWrite(pinn, LOW);
  delay(delayy);
  digitalWrite(pinn, HIGH);
  delay(delayy);
  digitalWrite(pinn, LOW);
  delay(delayy);
  digitalWrite(pinn, HIGH);
  delay(delayy);
  digitalWrite(pinn, LOW);
  delay(delayy);
  digitalWrite(pinn, HIGH);
  delay(delayy);
  digitalWrite(pinn, LOW);
  delay(delayy);
  digitalWrite(pinn, HIGH);
}

void blinkEndLow(int pinn, int delayy) {
  pinMode(pinn, OUTPUT);

  digitalWrite(pinn, LOW);
  delay(delayy);
  digitalWrite(pinn, HIGH);
  delay(delayy);
  digitalWrite(pinn, LOW);
  delay(delayy);
  digitalWrite(pinn, HIGH);
  delay(delayy);
  digitalWrite(pinn, LOW);
  delay(delayy);
  digitalWrite(pinn, HIGH);
  delay(delayy);
  digitalWrite(pinn, LOW);
  delay(delayy);
  digitalWrite(pinn, HIGH);
  delay(delayy);
  digitalWrite(pinn, LOW);
}

void dataExtractor() {
  // --TO DO --
  Serial.println("To Do extraction of new data");
}

void linkExtractor() {

  Serial.println("Extracting Links from the captured data from router");
  Serial.println("As my processor is slow this might take some time");

  Process extractorCmd;
  extractorCmd.runShellCommand("cd /mnt/sda1/arduino/Poet && python link_extractor.py");
  //while (extractorCmd.running());
  /**/
  while (extractorCmd.running()) {
    digitalWrite(dataExtIndPin, HIGH);
    delay(100);
    digitalWrite(dataExtIndPin, LOW);
    delay(100);
  }
  /**/
  while (extractorCmd.running());
  Serial.println("All the links extracted");
}

void poemCreator() {

  Serial.println("Getting the texts of the links by visiting them.. So hold on. It might take some time \n");

  Process p;
  p.runShellCommand("cd /mnt/sda1/arduino/Poet && python link_data_extractor.py");
  //while (p.running()); // Wait for the thing to finish
  while (p.running()) {
    digitalWrite(dataExtIndPin, HIGH);
    delay(100);
    digitalWrite(dataExtIndPin, LOW);
    delay(100);
  } // Wait for the thing to finish
  // Removing white spaces and tabs by running a terminal command
  p.runShellCommand("cd /mnt/sda1/arduino/Poet && sed -e 's/[\t]//g;/^$/d' combined_text.txt > combined.txt");
  while (p.running());  // Wait for the thing to finish
  //Combining into one line
  p.runShellCommand("cd /mnt/sda1/arduino/Poet && echo $(cat combined.txt) > corpus.txt");
  while (p.running());  // Wait for the thing to finish
  //removing the "now un - necessary" files
  p.runShellCommand("cd /mnt/sda1/arduino/Poet && rm combined.txt && rm combined_text.txt && rm links.txt");
  while (p.running());  // Wait for the thing to finish
  Serial.println("Corpus is ready now!!");
  delay(2000);

  Serial.println("Firing Markov generator from corpus");

  //Generating random arguments (order and length) for the markov script
  long order = random(2, 30);
  long poemLength = random(400, 2000);
  //Adding them as arguments to my mrkov.py which needs these arguments to generate psudo random texts
  String markovCmd = "cd /mnt/sda1/arduino/Poet && python markov1.py ";
  markovCmd.concat(order);
  markovCmd.concat(" ");
  markovCmd.concat(poemLength);
  Serial.println(markovCmd);

  p.runShellCommand(markovCmd);
  while (p.running());  // Wait for the thing to finish

  //removing corpus text
  p.runShellCommand("cd /mnt/sda1/arduino/Poet && rm corpus.txt");
  while (p.running());
  delay(2000);

  Serial.println("Poem created now");
}

void poemReciter() {

  //  Process recitor;
  //  recitor.runShellCommand("cd /mnt/sda1/arduino/Poet && espeak -a 20 \"$(cat poem.txt)\"");
  //  while (recitor.running());
  //  recitor.runShellCommand("cd /mnt/sda1/arduino/Poet && rm poem.txt");
  //  while (recitor.running());
  Serial.println(" Poem Done!!");
}



