#include <Wire.h> 
#include <MPU9250_asukiaaa.h>

MPU9250_asukiaaa mpu;

// Pines de motor
const int ENA = 6, IN1 = 4, IN2 = 5; // Motor derecho
const int ENB = 3, IN3 = 9, IN4 = 7; // Motor izquierdo

// Parámetros de velocidad
int pwmBase = 90;
int pwmDer = pwmBase;
int pwmIzq = pwmBase;

// Variables de corrección
float gyroBiasZ = 0;
float accelBiasX = 0;
const int N = 500; // Muestras para calibración

bool robotSeMovio = false;

void moverRecto(int pwmDer, int pwmIzq) {
  analogWrite(ENA, pwmDer);
  analogWrite(ENB, pwmIzq);

  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void detenerMotores() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void calibrarIMU() {
  Serial.println("Calibrando IMU...");
  for (int i = 0; i < N; i++) {
    mpu.accelUpdate();
    mpu.gyroUpdate();
    gyroBiasZ += mpu.gyroZ();
    accelBiasX += mpu.accelX();
    delay(5);
  }
  gyroBiasZ /= N;
  accelBiasX /= N;
  Serial.println("Calibración completa.");
  Serial.print("Bias Gyro Z: "); Serial.println(gyroBiasZ);
  Serial.print("Bias Accel X: "); Serial.println(accelBiasX);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  mpu.setWire(&Wire);
  mpu.beginAccel();
  mpu.beginGyro();
  delay(1000);

  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  calibrarIMU();
  detenerMotores();
}

void loop() {
  mpu.accelUpdate();
  mpu.gyroUpdate();

  float gyroZ_corr = mpu.gyroZ() - gyroBiasZ;
  float accelX_corr = mpu.accelX() - accelBiasX;

  if (!robotSeMovio && abs(accelX_corr) > 0.02) {
    robotSeMovio = true;
    Serial.println("✔ Movimiento detectado, iniciando motores...");
    moverRecto(pwmBase, pwmBase);
  }

  if (robotSeMovio) {
    // Corrección de rumbo usando giro en eje Z
    if (gyroZ_corr > 0.8) {
      pwmDer = pwmBase - 20;
      pwmIzq = pwmBase + 10;
      Serial.println("↪ Corrección: Girando a la izquierda");
    } else if (gyroZ_corr < -0.8) {
      pwmDer = pwmBase + 10;
      pwmIzq = pwmBase - 20;
      Serial.println("↩ Corrección: Girando a la derecha");
    } else {
      pwmDer = pwmBase;
      pwmIzq = pwmBase;
      Serial.println("⬆ Manteniendo línea recta");
    }
    moverRecto(pwmDer, pwmIzq);
  }

  // Mostrar datos para análisis colocar en texto si solo se quiere observar corrección
  //Serial.print("Gyro Z corregido (°/s): "); Serial.print(gyroZ_corr);
  //Serial.print(" | Aceleración X corregida (g): "); Serial.println(accelX_corr);

  delay(30);
}
