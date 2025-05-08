// Pines para motor derecho
const int ENA = 6;  // PWM
const int IN1 = 4;
const int IN2 = 5;

// Pines para motor izquierdo
const int ENB = 3;  // PWM
const int IN3 = 9;
const int IN4 = 7;

void setup() {
  // Configurar pines como salida
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Velocidad baja
  moverAdelante(80, 80);
  Serial.println("Velocidad baja");
  delay(3000);

  // Velocidad media
  moverAdelante(140, 140);
  Serial.println("Velocidad media");
  delay(3000);

  // Velocidad alta
  moverAdelante(200, 200);
  Serial.println("Velocidad alta");
  delay(3000);

  // Detenerse
  detener();
  Serial.println("Detenido");
  delay(3000);
}

void moverAdelante(int pwmDer, int pwmIzq) {
  analogWrite(ENA, pwmDer);
  analogWrite(ENB, pwmIzq);

  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  // Motor derecho adelante
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);  // Motor izquierdo adelante
}

void detener() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}
