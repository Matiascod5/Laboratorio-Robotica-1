#include <Wire.h>
#include <MPU9250_asukiaaa.h>

MPU9250_asukiaaa mpu;

float gyroBiasZ = 0;
float x = 0, y = 0, theta = 0;
unsigned long lastUpdateTime = 0;

const int N = 500;
const float L = 0.12;
const float K_vel = 0.0005;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(4, OUTPUT); // IN1 motor derecho
  pinMode(5, OUTPUT); // IN2 motor derecho
  pinMode(6, OUTPUT); // ENA

  pinMode(9, OUTPUT); // IN3 motor izquierdo
  pinMode(7, OUTPUT); // IN4 motor izquierdo
  pinMode(3, OUTPUT); // ENB

  pinMode(10, OUTPUT); // Trigger ultrasonido
  pinMode(11, INPUT);  // Echo ultrasonido

  mpu.setWire(&Wire);
  mpu.beginAccel();
  mpu.beginGyro();
  delay(1000);

  Serial.println("Calibrando el sensor...");
  for (int i = 0; i < N; i++) {
    mpu.gyroUpdate();
    gyroBiasZ += mpu.gyroZ();
    delay(5);
  }
  gyroBiasZ /= N;
  Serial.print("Bias Gyro Z: ");
  Serial.println(gyroBiasZ);

  lastUpdateTime = millis();
}

void loop() {
  moverRectoCorrigiendo(50); // movimiento con corrección (más lento)
  delay(2000);

  moverRectoSimple(40); // movimiento sin corrección (más lento)
  delay(1500);
  detener();

  girarDerecha(50); // giro (más lento)
  delay(1000);
  detener();

  retroceder(40); // retroceso (más lento)
  delay(2000);
  detener();

  // Medir distancia
  long distancia = medirDistancia();
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia < 20) {
    Serial.println("¡Obstáculo detectado! Cambiando de dirección...");
    detener();
    delay(500);
    girarDerecha(50); // también más lento aquí
    delay(1000);
    detener();
    delay(500);
  }

  // Mostrar posición
  Serial.print("x: "); Serial.print(x, 2);
  Serial.print(" m | y: "); Serial.print(y, 2);
  Serial.print(" m | θ: "); Serial.print(theta, 2);
  Serial.println(" rad");

  // --- Medir inclinación ---
  mpu.accelUpdate();
  float inclinacionX = atan2(mpu.accelY(), mpu.accelZ()) * 180 / PI;
  float inclinacionY = atan2(mpu.accelX(), mpu.accelZ()) * 180 / PI;

  Serial.print("Inclinación X: ");
  Serial.print(inclinacionX);
  Serial.print("° | Inclinación Y: ");
  Serial.print(inclinacionY);
  Serial.println("°");

  delay(1000);
}

void moverRectoCorrigiendo(int velocidadPWM) {
  mpu.gyroUpdate();
  float gyroZ = mpu.gyroZ() - gyroBiasZ;

  unsigned long currentTime = millis();
  float dt = (currentTime - lastUpdateTime) / 1000.0;
  lastUpdateTime = currentTime;

  theta += gyroZ * dt;

  float vL = velocidadPWM * K_vel;
  float vR = velocidadPWM * K_vel;
  float v = (vR + vL) / 2;

  float Kp = 1.0;
  int ajuste = Kp * theta;

  int pwmDer = constrain(velocidadPWM - ajuste, 0, 255);
  int pwmIzq = constrain(velocidadPWM + ajuste, 0, 255);

  analogWrite(6, pwmDer);
  analogWrite(3, pwmIzq);

  digitalWrite(4, HIGH); digitalWrite(5, LOW);
  digitalWrite(9, HIGH); digitalWrite(7, LOW);

  x += v * cos(theta) * dt;
  y += v * sin(theta) * dt;

  Serial.print("PWM Der: ");
  Serial.print(pwmDer);
  Serial.print(" | PWM Izq: ");
  Serial.println(pwmIzq);
}

void moverRectoSimple(int velocidadPWM) {
  digitalWrite(4, HIGH); digitalWrite(5, LOW);
  digitalWrite(9, HIGH); digitalWrite(7, LOW);
  analogWrite(6, velocidadPWM);
  analogWrite(3, velocidadPWM);
}

void retroceder(int velocidad) {
  digitalWrite(4, LOW); digitalWrite(5, HIGH);
  digitalWrite(9, LOW); digitalWrite(7, HIGH);
  analogWrite(6, velocidad);
  analogWrite(3, velocidad);
}

void girarDerecha(int velocidad) {
  digitalWrite(4, HIGH); digitalWrite(5, LOW);
  digitalWrite(9, LOW); digitalWrite(7, HIGH);
  analogWrite(6, velocidad);
  analogWrite(3, velocidad);
}

void detener() {
  analogWrite(6, 0);
  analogWrite(3, 0);
}

long medirDistancia() {
  digitalWrite(10, LOW);
  delayMicroseconds(2);
  digitalWrite(10, HIGH);
  delayMicroseconds(10);
  digitalWrite(10, LOW);
  long duracion = pulseIn(11, HIGH);
  return (duracion / 2) / 29.1;
}
