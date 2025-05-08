void setup() {
  pinMode(4, OUTPUT); // IN1 - motor derecho
  pinMode(5, OUTPUT); // IN2 - motor derecho
  pinMode(9, OUTPUT); // IN3 - motor izquierdo
  pinMode(7, OUTPUT); // IN4 - motor izquierdo
  pinMode(6, OUTPUT); // ENA - habilitación motor derecho
  pinMode(3, OUTPUT); // ENB - habilitación motor izquierdo
}

void loop() {
  int velocidad = 1; // valor entre 0 y 255

  // Adelante
  analogWrite(6, velocidad); // ENA
  analogWrite(3, velocidad); // ENB
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(7, LOW);
  delay(2000);

  // Atrás
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  digitalWrite(9, LOW);
  digitalWrite(7, HIGH);
  delay(2000);

  // Giro a la derecha
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(7, LOW);
  delay(2000);

  // Giro a la izquierda
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(9, LOW);
  digitalWrite(7, LOW);
  delay(2000);

  // Detener
  analogWrite(6, 0);
  analogWrite(3, 0);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(9, LOW);
  digitalWrite(7, LOW);
  delay(2000);
}
