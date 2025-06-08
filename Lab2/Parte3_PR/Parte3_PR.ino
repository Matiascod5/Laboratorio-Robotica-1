#include <Wire.h>
#include <MPU9250_asukiaaa.h>
#include <Adafruit_TCS34725.h>

// Pines del sensor ultrasónico
#define ULTRASONIC_TRIG_PIN 12
#define ULTRASONIC_ECHO_PIN 11

// Pines del motor A
#define MOTOR_EN_A 10
#define MOTOR_IN1_A 7
#define MOTOR_IN2_A 6

// Pines del motor B
#define MOTOR_EN_B 9
#define MOTOR_IN3_B 5
#define MOTOR_IN4_B 4

// Inicializar sensores
MPU9250_asukiaaa mpu;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

// Variables ultrasónico
long distanceCm = 0;
float filteredDistanceCm = 0;
const float SMOOTHING_FACTOR = 0.6;

const int OBSTACLE_THRESHOLD_CM = 10;
String detectedColor = "ninguno";

// Prototipos
void readUltrasonicSensor();
void readIMUSensor();
void readColorSensor();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMotors();

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  pinMode(MOTOR_EN_A, OUTPUT);
  pinMode(MOTOR_IN1_A, OUTPUT);
  pinMode(MOTOR_IN2_A, OUTPUT);
  pinMode(MOTOR_EN_B, OUTPUT);
  pinMode(MOTOR_IN3_B, OUTPUT);
  pinMode(MOTOR_IN4_B, OUTPUT);

  mpu.setWire(&Wire);
  mpu.beginAccel();
  mpu.beginGyro();
  delay(1000);

  Serial.println("Inicializando MPU9250...");
  if (!mpu.accelUpdate() || !mpu.gyroUpdate()) {
    Serial.println("¡Error al detectar el MPU9250!");
  } else {
    Serial.println("MPU9250 inicializado correctamente.");
  }

  if (tcs.begin()) {
    Serial.println("Sensor TCS34725 detectado.");
  } else {
    Serial.println("No se encontró el sensor TCS34725.");
    while (1);
  }

  filteredDistanceCm = 100;
  stopMotors();
}

void loop() {
  readUltrasonicSensor();
  readIMUSensor();
  readColorSensor();

  Serial.print("Distancia: ");
  Serial.print(filteredDistanceCm);
  Serial.print(" cm | Color detectado: ");
  Serial.println(detectedColor);

  bool accionEjecutada = false;

  // Acción por obstáculo
  if (filteredDistanceCm < OBSTACLE_THRESHOLD_CM && filteredDistanceCm > 0) {
    Serial.println("¡Obstáculo cerca! Deteniendo y girando.");
    stopMotors();
    delay(500);
    turnRight();
    delay(500);
    stopMotors();
    accionEjecutada = true;
  }

  // Acción por color
  if (detectedColor == "rojo") {
    Serial.println("Color rojo detectado: Retrocediendo.");
    moveBackward();
    delay(1000);
    stopMotors();
    accionEjecutada = true;
  } else if (detectedColor == "verde") {
    Serial.println("Color verde detectado: Girando a la izquierda.");
    turnLeft();
    delay(1000);
    stopMotors();
    accionEjecutada = true;
  } else if (detectedColor == "azul") {
    Serial.println("Color azul detectado: Girando a la derecha.");
    turnRight();
    delay(1000);
    stopMotors();
    accionEjecutada = true;
  }

  // Si no hay ninguna acción especial, avanza
  if (!accionEjecutada) {
    moveForward();
  }

  delay(100);
}

void readUltrasonicSensor() {
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH, 30000);
  if (duration == 0) {
    distanceCm = 999;
  } else {
    distanceCm = duration * 0.0343 / 2;
  }

  filteredDistanceCm = (SMOOTHING_FACTOR * distanceCm) + (1.0 - SMOOTHING_FACTOR) * filteredDistanceCm;
}

void readIMUSensor() {
  mpu.accelUpdate();
  mpu.gyroUpdate();

  Serial.print("Acel X: "); Serial.print(mpu.accelX());
  Serial.print(" | Y: "); Serial.print(mpu.accelY());
  Serial.print(" | Z: "); Serial.println(mpu.accelZ());

  Serial.print("Gyro X: "); Serial.print(mpu.gyroX());
  Serial.print(" | Y: "); Serial.print(mpu.gyroY());
  Serial.print(" | Z: "); Serial.println(mpu.gyroZ());
}

void readColorSensor() {
  uint32_t rTotal = 0, gTotal = 0, bTotal = 0, cTotal = 0;
  const int numReadings = 10;

  for (int i = 0; i < numReadings; i++) {
    uint16_t r, g, b, c;
    tcs.getRawData(&r, &g, &b, &c);
    rTotal += r;
    gTotal += g;
    bTotal += b;
    cTotal += c;
    delay(5);
  }

  float rAvg = rTotal / numReadings;
  float gAvg = gTotal / numReadings;
  float bAvg = bTotal / numReadings;
  float cAvg = cTotal / numReadings;

  float fr = rAvg / cAvg;
  float fg = gAvg / cAvg;
  float fb = bAvg / cAvg;

  // Clasificación mejorada
  if (fr > 0.4 && fg < 0.35 && fb < 0.35) {
    detectedColor = "rojo";
  } else if (fr < 0.35 && fg > 0.4 && fb < 0.35) {
    detectedColor = "verde";
  } else if (fr < 0.35 && fg < 0.35 && fb > 0.4) {
    detectedColor = "azul";
  } else if (fr > 0.3 && fg > 0.3 && fb > 0.3) {
    detectedColor = "blanco";
  } else if (cAvg < 100) {
    detectedColor = "negro";
  } else {
    detectedColor = "desconocido";
  }
}


void moveForward() {
  analogWrite(MOTOR_EN_A, 200);
  digitalWrite(MOTOR_IN1_A, HIGH);
  digitalWrite(MOTOR_IN2_A, LOW);

  analogWrite(MOTOR_EN_B, 200);
  digitalWrite(MOTOR_IN3_B, HIGH);
  digitalWrite(MOTOR_IN4_B, LOW);
}

void moveBackward() {
  analogWrite(MOTOR_EN_A, 200);
  digitalWrite(MOTOR_IN1_A, LOW);
  digitalWrite(MOTOR_IN2_A, HIGH);

  analogWrite(MOTOR_EN_B, 200);
  digitalWrite(MOTOR_IN3_B, LOW);
  digitalWrite(MOTOR_IN4_B, HIGH);
}

void turnLeft() {
  analogWrite(MOTOR_EN_A, 180);
  digitalWrite(MOTOR_IN1_A, LOW);
  digitalWrite(MOTOR_IN2_A, HIGH);

  analogWrite(MOTOR_EN_B, 180);
  digitalWrite(MOTOR_IN3_B, HIGH);
  digitalWrite(MOTOR_IN4_B, LOW);
}

void turnRight() {
  analogWrite(MOTOR_EN_A, 180);
  digitalWrite(MOTOR_IN1_A, HIGH);
  digitalWrite(MOTOR_IN2_A, LOW);

  analogWrite(MOTOR_EN_B, 180);
  digitalWrite(MOTOR_IN3_B, LOW);
  digitalWrite(MOTOR_IN4_B, HIGH);
}

void stopMotors() {
  analogWrite(MOTOR_EN_A, 0);
  analogWrite(MOTOR_EN_B, 0);
  digitalWrite(MOTOR_IN1_A, LOW);
  digitalWrite(MOTOR_IN2_A, LOW);
  digitalWrite(MOTOR_IN3_B, LOW);
  digitalWrite(MOTOR_IN4_B, LOW);
}
