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
#define R1 5000.0 //ohms
#define V 5.0 //volts
#define SENSORAREA 1.0 //cm2
#define SENSORLENGTH 4.0 //cm

void setup() {
  Serial.begin(9600); //Begin serial output
  //Set up pins
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(SENSORPIN, INPUT);
}

void loop() {
  double voltage = getVoltage(10, 20);
  double resistance = getResistance(voltage);
  double resistivity = getResistivity(resistance);
  double conductivity = getConductivity(resistivity);
  double salinity = getSalinity(conductivity);
  lightLED(REDPIN, salinity, REDMIN, REDMAX);
  lightLED(GREENPIN, salinity, GREENMIN, GREENMAX);
  lightLED(BLUEPIN, salinity, BLUEMIN, BLUEMAX);
  printOutput(voltage, salinity);
}

double getVoltage(int readings, int timeDelay){
  // read the sensor {readings} times with {timeDelay} between them and return the voltage measured in volts
  double avgReading = 0;
  for(int i = 0; i < readings; i++){
    avgReading += analogRead(SENSORPIN);
    delay(timeDelay);
  }
  avgReading /= readings;
  double voltage = avgReading / 1023.0; //10 bit number: range from 0 (0V) to 1023 (5V)
  return voltage;
}

double getResistance(double voltage){
  // return the resistance of R2 in the voltage divider given the {voltage} sensor value in ohms
  // voltage divider voltage = V * R2 / (R1 + R2)
  double resistance = R1 * voltage / (V - voltage);
  return resistance;
}

double getResistivity(double resistance){
  //returns the resistivity of the sensor area given the parameters of the sensor and the {resistance} of the sensor in ohm-centimeters
  // R = [rho] * l / A
  double resistivity = resistance * SENSORAREA / SENSORLENGTH;
  return resistivity;
}

double getConductivity(double resistivity){
  // return the conductivity given the {resistivity} in siemens/centimeter
  // [sigma] = 1 / [rho]
  return 1 / resistivity;
}

double getSalinity(double conductivity){
  //return the salinity in ppm given the least squares regression line for {conductivity}
  double ppm = ((conductivity * 1000000) - 403.05) / 1582.6; //least squares in uS/cm
  return ppm * 1000.0;
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
