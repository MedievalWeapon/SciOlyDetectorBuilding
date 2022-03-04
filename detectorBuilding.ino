//red min max
#define REDMIN 0
#define REDMAX 0

//green min max
#define GREENMIN 0
#define GREENMAX 0

//blue min max
#define BLUEMIN 0
#define BLUEMAX 0

//pin definitions
#define REDPIN A2
#define GREENPIN A1
#define BLUEPIN A0
#define SENSORPIN A3

//circuit parameters
#define V 5.0 //volts

void setup() {
  Serial.begin(9600); //Begin serial output
  //Set up pins
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(SENSORPIN, INPUT);
}

void loop() {
  double voltage = getVoltage(1000, 2);
  double salinity = getSalinity(voltage);
  lightLED(REDPIN, salinity, REDMIN, REDMAX);
  lightLED(GREENPIN, salinity, GREENMIN, GREENMAX);
  lightLED(BLUEPIN, salinity, BLUEMIN, BLUEMAX);
  printOutput(voltage, salinity);
}

double getVoltage(int readings, int timeDelay){
  // read the sensor {readings} times with {timeDelay} between them and return the voltage measured in volts
  double avgReading = 0.0;
  for(int i = 0; i < readings; i++){
    avgReading += analogRead(SENSORPIN);
    delay(timeDelay);
  }
  avgReading /= readings;
  double voltage = V * avgReading / 1023.0; //10 bit number: range from 0 (0V) to 1023 (5V)
  return voltage;
}

double getSalinity(double voltage){
  //return the salinity in ppm given the best fit line for {voltage}
  double ppm = 1150000.0 * exp(-7.96 * voltage);
  return ppm;
}

void lightLED(int pin, double salinity, double minimum, double maximum){
  // turn on the led at {pin} if {salinity} is between {minimum} and {maximum}
  if(salinity >= minimum && salinity <= maximum){
    digitalWrite(pin, HIGH);
  }
  else{
    digitalWrite(pin, LOW);
  }
}

void printOutput(double voltage, double salinity){
  // print out the {voltage} and {salinity} with formatting
  Serial.println("Voltage: " + String(voltage) + "V  Salinity: " + String(salinity) + "ppm");
}
