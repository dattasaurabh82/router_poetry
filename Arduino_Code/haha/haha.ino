#include <Process.h>
#include <FileIO.h>

const int linkDataTranBtn = 2;
const int linkExtBtn = 3;
const int poemGenBtn = 4;
const int broadCastBtn = 5;

boolean linkExtState, linkDataTranState, poemGenState, broadCastState = LOW;
boolean prevLinkExtState, prevLinkDataTranState, prevPoemGenState, prevBroadCastState = LOW;

const int dataExtIndPin = 8;
const int linkExtIndPin = 9;
const int poemGenIndPin = 10;
const int poemRecitIndPin = 11;

//int pin = 2;

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
        Serial.println("Run Data extractor");
        dataExtractor();
        digitalWrite(dataExtIndPin, HIGH);
      } else {
        counter[0] = 1;
        blinkEndHigh(dataExtIndPin, 200);
      }
    }
    prevLinkDataTranState = linkDataTranState;

  }


  if (linkExtState != prevLinkExtState) {
    if (linkExtState == HIGH) {
      counter[1] += 1;
      if (counter[1] == 1 && counter[0] == 1 && counter[2] == 0 && counter[3] == 0) {
        Serial.println("Run link extractor");
        linkExtractor();
        digitalWrite(linkExtIndPin, HIGH);
      } else if (counter[1] >= 1 && counter[0] == 0 && counter[2] == 0 && counter[3] == 0) {
        blinkEndLow(linkExtIndPin, 200);
        counter[1] = 0;
      } else {
        counter[1] = 1;
        blinkEndHigh(linkExtIndPin, 200);
        Serial.println("Pressing same button again and again");
      }
      //ArrayRes();
    }
    prevLinkExtState = linkExtState;

  }

  if (poemGenState != prevPoemGenState) {
    if (poemGenState == HIGH) {
      counter[2] += 1;
      if (counter[2] == 1 && counter[0] == 1 && counter[1] == 1 && counter[3] == 0) {
        Serial.println("Run Poem generator");
        poemCreator();
        digitalWrite(poemGenIndPin, HIGH);
      } else if (counter[2] >= 1 && counter[0] == 0 && counter[1] == 0 && counter[3] == 0) {
        counter[2] = 0;
        blinkEndLow(poemGenIndPin, 200);
      } else {
        counter[2] = 1;
        blinkEndHigh(poemGenIndPin, 200);
        Serial.println("Pressing same button again and again");
      }
      //ArrayRes();
    }
    prevPoemGenState = poemGenState;

  }

  if (broadCastState != prevBroadCastState) {
    if (broadCastState == HIGH) {
      counter[3] += 1;
      if (counter[3] == 1 && counter[0] == 1 && counter[1] == 1 && counter[2] == 1) {
        Serial.println("Run Poet");
        poemReciter();
        digitalWrite(poemRecitIndPin, HIGH);
        //wait for 5 sec and reset
        delay(5000);
        reset();
      } else {
        blinkEndLow(poemRecitIndPin, 200);
        reset();
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

void blinkStat(int pinn, int delayy) {
  pinMode(pinn, OUTPUT);

  digitalWrite(pinn, LOW);
  delay(delayy);
  digitalWrite(pinn, HIGH);
  delay(delayy);
}

void dataExtractor() {
  Serial.println("Some bug fix needs to be done here");
  //  Serial.println("Extracting data from the Router");
  //  Process dataExtCmd;
  //  dataExtCmd.runShellCommandAsynchronously("cd /mnt/sda1/arduino/Poet && sh test.sh 30 192.168.8.237 192.168.8.179");
  //  while (dataExtCmd.running()) {
  //    blinkStat(dataExtIndPin, 100);
  //  }
  //
  //  Serial.println("All the data ported");

}

void linkExtractor() {
  Serial.println("Extracting Links from the captured data from router");
  Serial.println("As my processor is slow this might take some time");
  //
  Process linkExtCmd;
  linkExtCmd.runShellCommandAsynchronously("cd /mnt/sda1/arduino/Poet && python link_extractor.py");
  while (linkExtCmd.running())
    blinkStat(linkExtIndPin, 100);
  Serial.println("All the links extracted");
}

void poemCreator() {

  Serial.println("Getting the texts of the links by visiting them.. So hold on. It might take some time \n");
  //
  Process p;
  //
  p.runShellCommandAsynchronously("cd /mnt/sda1/arduino/Poet && python link_data_extractor.py");
  while (p.running())
    blinkStat(poemGenIndPin, 100);

  //Removing white spaces and tabs by running a terminal command
  p.runShellCommandAsynchronously("cd /mnt/sda1/arduino/Poet && sed -e 's/[\t]//g;/^$/d' combined_text.txt > combined.txt");
  while (p.running())  // Wait for the thing to finish
    blinkStat(poemGenIndPin, 100);

  //Combining into one line
  p.runShellCommandAsynchronously("cd /mnt/sda1/arduino/Poet && echo $(cat combined.txt) > corpus.txt");
  while (p.running())  // Wait for the thing to finish
    blinkStat(poemGenIndPin, 100);

  //removing the "now un - necessary" files
  p.runShellCommandAsynchronously("cd /mnt/sda1/arduino/Poet && rm combined.txt && rm combined_text.txt && rm data.txt");
  while (p.running())  // Wait for the thing to finish
    blinkStat(poemGenIndPin, 100);

  Serial.println("Corpus is ready now!!");

  delay(2000);
  //
  Serial.println("Firing Markov generator from corpus");
  //
  //Generating random arguments (order and length) for the markov script
  long order = random(2, 30);
  long poemLength = random(400, 2000);
  //Adding them as arguments to my mrkov.py which needs these arguments to generate psudo random texts
  String markovCmd = "cd /mnt/sda1/arduino/Poet && python markov1.py ";
  markovCmd.concat(order);
  markovCmd.concat(" ");
  markovCmd.concat(poemLength);
  Serial.println(markovCmd);
  //
  p.runShellCommandAsynchronously(markovCmd);
  while (p.running())  // Wait for the thing to finish
    blinkStat(poemGenIndPin, 100);

  //removing corpus text
  p.runShellCommandAsynchronously("cd /mnt/sda1/arduino/Poet && rm corpus.txt && rm links.txt");
  while (p.running())
    blinkStat(poemGenIndPin, 100);

  delay(2000);
  Serial.println("Poem created now");
}

void poemReciter() {
  Serial.println("Poet reciting!");
  Process recitor;
  recitor.runShellCommandAsynchronously("cd /mnt/sda1/arduino/Poet && espeak -a 20 \"$(cat poem.txt)\"");
  while (recitor.running())
    blinkStat(poemRecitIndPin, 100);
  recitor.runShellCommandAsynchronously("cd /mnt/sda1/arduino/Poet && rm poem.txt");
  while (recitor.running())
    blinkStat(poemRecitIndPin, 100);
  Serial.println(" Poem Done!!");
}

