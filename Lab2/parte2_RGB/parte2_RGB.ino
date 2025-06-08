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

// Inicializar MPU
MPU9250_asukiaaa mpu;

// Sensor de color
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);

// Variables para sensor ultrasónico
long distanceCm;
const int NUM_READINGS = 5;
long ultrasonicReadings[NUM_READINGS];
int ultrasonicReadIndex = 0;
long ultrasonicTotal = 0;
long filteredDistanceCm = 0;

// Variables para MPU
float accelX, accelY, accelZ;
float gyroX, gyroY, gyroZ;

// Umbral para detección de obstáculos (en cm)
const int OBSTACLE_THRESHOLD_CM = 30;

// Prototipos de funciones
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

  // Configuración de pines ultrasónicos
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  // Configuración de pines de motores
  pinMode(MOTOR_EN_A, OUTPUT);
  pinMode(MOTOR_IN1_A, OUTPUT);
  pinMode(MOTOR_IN2_A, OUTPUT);
  pinMode(MOTOR_EN_B, OUTPUT);
  pinMode(MOTOR_IN3_B, OUTPUT);
  pinMode(MOTOR_IN4_B, OUTPUT);

  // Inicializar MPU
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

  // Inicializar sensor de color
  if (tcs.begin()) {
    Serial.println("Sensor TCS34725 detectado.");
  } else {
    Serial.println("No se encontró el sensor TCS34725.");
    while (1);
  }

  // Inicializar lectura filtrada del sensor ultrasónico
  for (int i = 0; i < NUM_READINGS; i++) {
    ultrasonicReadings[i] = 0;
  }

  stopMotors(); // Detener motores al inicio
}

void loop() {
  readUltrasonicSensor();
  readIMUSensor();
  readColorSensor();  // Nuevo

  if (filteredDistanceCm < OBSTACLE_THRESHOLD_CM && filteredDistanceCm > 0) {
    Serial.print("¡Obstáculo detectado a ");
    Serial.print(filteredDistanceCm);
    Serial.println(" cm! Deteniendo y girando.");
    stopMotors();
    delay(500);
    turnRight();
    delay(1500);
    stopMotors();
  } else {
    Serial.print("Camino despejado. Avanzando. Distancia: ");
    Serial.print(filteredDistanceCm);
    Serial.println(" cm.");
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

  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH, 30000); // Timeout 30ms

  if (duration == 0) {
    distanceCm = 999;
  } else {
    distanceCm = duration * 0.0343 / 2;
  }

  ultrasonicTotal -= ultrasonicReadings[ultrasonicReadIndex];
  ultrasonicReadings[ultrasonicReadIndex] = distanceCm;
  ultrasonicTotal += ultrasonicReadings[ultrasonicReadIndex];
  ultrasonicReadIndex = (ultrasonicReadIndex + 1) % NUM_READINGS;

  filteredDistanceCm = ultrasonicTotal / NUM_READINGS;

  Serial.print("Ultrasonico (RAW): ");
  Serial.print(distanceCm);
  Serial.print(" cm | Filtrado: ");
  Serial.print(filteredDistanceCm);
  Serial.println(" cm");
}

void readIMUSensor() {
  mpu.accelUpdate();
  mpu.gyroUpdate();

  accelX = mpu.accelX();
  accelY = mpu.accelY();
  accelZ = mpu.accelZ();

  gyroX = mpu.gyroX();
  gyroY = mpu.gyroY();
  gyroZ = mpu.gyroZ();

  Serial.print("Acel X: "); Serial.print(accelX);
  Serial.print(" | Y: "); Serial.print(accelY);
  Serial.print(" | Z: "); Serial.print(accelZ);
  Serial.println(" g");

  Serial.print("Gyro X: "); Serial.print(gyroX);
  Serial.print(" | Y: "); Serial.print(gyroY);
  Serial.print(" | Z: "); Serial.print(gyroZ);
  Serial.println(" deg/s");
}

void readColorSensor() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  float fr = (float)r / (float)c;
  float fg = (float)g / (float)c;
  float fb = (float)b / (float)c;

  Serial.print("Color (R,G,B): ");
  Serial.print(r); Serial.print(", ");
  Serial.print(g); Serial.print(", ");
  Serial.print(b); Serial.print(" | ");
  Serial.print("fr: "); Serial.print(fr, 2);
  Serial.print(" fg: "); Serial.print(fg, 2);
  Serial.print(" fb: "); Serial.print(fb, 2);

  if (c < 100) {
    Serial.println(" -> Negro");
  } else if (fr > 0.4 && fg < 0.3 && fb < 0.3) {
    Serial.println(" -> Rojo");
  } else if (fr < 0.3 && fg > 0.4 && fb < 0.3) {
    Serial.println(" -> Verde");
  } else if (fr < 0.3 && fg < 0.3 && fb > 0.4) {
    Serial.println(" -> Azul");
  } else if (fr > 0.3 && fg > 0.3 && fb > 0.3) {
    Serial.println(" -> Blanco");
  } else {
    Serial.println(" -> Color desconocido");
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
  digitalWrite(MOTOR_EN_A, LOW);
  digitalWrite(MOTOR_IN1_A, LOW);
  digitalWrite(MOTOR_IN2_A, LOW);

  digitalWrite(MOTOR_EN_B, LOW);
  digitalWrite(MOTOR_IN3_B, LOW);
  digitalWrite(MOTOR_IN4_B, LOW);
}
