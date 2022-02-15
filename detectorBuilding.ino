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
#define REDPIN A1
#define GREENPIN A2
#define BLUEPIN A3
#define SENSORPIN A0

//circuit parameters
#define R1 10000
#define V 5

void setup() {
  Serial.begin(9600); //Begin serial output
  //Set up pins
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(SENSORPIN, INPUT);
}

void loop() {
  float voltage = getVoltage(10, 20);
  float resistance = getResistance(voltage);
  float conductivity = getConductivity(resistance);
  float salinity = getSalinity(conductivity);
  lightLED(REDPIN, salinity, REDMIN, REDMAX);
  lightLED(GREENPIN, salinity, GREENMIN, GREENMAX);
  lightLED(BLUEPIN, salinity, BLUEMIN, BLUEMAX);
  printOutput(voltage, salinity);
}

float getVoltage(int readings, int timeDelay){
  // read the sensor {readings} times with {timeDelay} between them and return the voltage measured
  float avgReading = 0;
  for(int i = 0; i < readings; i++){
    avgReading += analogRead(SENSORPIN);
    delay(timeDelay);
  }
  avgReading /= readings;
  float voltage = avgReading / 1023.0; //10 bit number: range from 0 (0V) to 1023 (5V)
  return voltage;
}

float getResistance(float voltage){
  // return the resistance of R2 in the voltage divider given the {voltage} sensor value
  // voltage divider sensor = V * R2 / (R1 + R2), solving for R2: R2 = R1 * sensor / (V - sensor)
  float resistance = R1 * voltage / (V - voltage);
  return resistance;
}

float getConductivity(float resistance){
  // return the conductivity given the {resistance}
  //C = 1/R
  return 1 / resistance;
}

float getSalinity(float conductivity){
  //return the salinity in ppm given the least squares regression line for {conductivity}
  float ppm = (conductivity - 403.05) / 1582.6;
  return ppm;
}

void lightLED(int pin, float salinity, float minimum, float maximum){
  // turn on the led at {pin} if {salinity} is between {minimum} and {maximum}
  if(salinity >= minimum && salinity <= maximum){
    digitalWrite(pin, HIGH);
  }
  else{
    digitalWrite(pin, LOW);
  }
}

void printOutput(float voltage, float salinity){
  // print out the {voltage} and {salinity} with formatting
  Serial.println("Voltage: " + String(voltage) + "V  Salinity: " + String(salinity) + "ppm");
}
