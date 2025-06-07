const float L = 0.12; // distancia entre ruedas en metros
const float K_vel = 0.0005; // factor de conversión PWM -> m/s (ajústalo a tu robot)

float x = 0, y = 0, theta = 0; // posición y orientación
unsigned long lastTime = 0;
unsigned long startTime = 0; // variable para medir el tiempo

int pwmDer = 0; // velocidad inicial del motor derecho
int pwmIzq = 0; // velocidad inicial del motor izquierdo
boolean movimientoIniciado = false; // bandera para controlar el inicio del movimiento

// Función para mover el robot
void moverRobot(int derecho, int izquierdo) {
  analogWrite(6, abs(derecho)); // usar valor absoluto para PWM
  analogWrite(3, abs(izquierdo));
  
  if (derecho > 0) {
    digitalWrite(4, HIGH); digitalWrite(5, LOW); // motor derecho adelante
  } else if (derecho < 0) {
    digitalWrite(4, LOW); digitalWrite(5, HIGH); // motor derecho atrás
  } else {
    digitalWrite(4, LOW); digitalWrite(5, LOW); // motor derecho detenido
  }

  if (izquierdo > 0) {
    digitalWrite(9, HIGH); digitalWrite(7, LOW); // motor izquierdo adelante
  } else if (izquierdo < 0) {
    digitalWrite(9, LOW); digitalWrite(7, HIGH); // motor izquierdo atrás
  } else {
    digitalWrite(9, LOW); digitalWrite(7, LOW); // motor izquierdo detenido
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT); pinMode(5, OUTPUT); // motor derecho
  pinMode(9, OUTPUT); pinMode(7, OUTPUT); // motor izquierdo
  pinMode(6, OUTPUT); // ENA
  pinMode(3, OUTPUT); // ENB
  
  // Asegurarse que los motores estén detenidos al inicio
  moverRobot(0, 0);
  
  // Reiniciar todas las variables de posición
  x = 0;
  y = 0;
  theta = 0;
  
  Serial.println("Sistema iniciado, esperando para comenzar movimiento");
  Serial.println("x: 0.000 | y: 0.000 | θ: 0.000");
  
  // No iniciar el cronómetro todavía
  movimientoIniciado = false;
}

void loop() {
  unsigned long currentTime = millis();
  
  // Botón o condición para iniciar el movimiento (puedes adaptarlo)
  // Por ejemplo, podrías usar un botón en un pin específico
  // o iniciar después de un tiempo fijo
  
  // Para pruebas, iniciaremos el movimiento después de 5 segundos
  if (!movimientoIniciado && currentTime > 5000) {
    movimientoIniciado = true;
    startTime = currentTime;
    lastTime = currentTime;
    Serial.println("¡Iniciando movimiento!");
  }
  
  // Solo realizar cálculos si el movimiento ha sido iniciado
  if (movimientoIniciado) {
    float dt = (currentTime - lastTime) / 1000.0; // calcular el tiempo transcurrido en segundos
    lastTime = currentTime;
    
    // Tiempo de avance en milisegundos
    unsigned long tiempoAvance = 3000;  // 3 segundos para avanzar
    unsigned long tiempoGiro = 3000;    // 3 segundos para girar
    unsigned long tiempoTotal = tiempoAvance + tiempoGiro;
    
    // Determinar fase del movimiento
    if (currentTime - startTime < tiempoAvance) {
      // Fase 1: Avanzar
      pwmDer = 100;
      pwmIzq = 100;
    } 
    else if (currentTime - startTime < tiempoTotal) {
      // Fase 2: Girar
      pwmDer = 0;
      pwmIzq = 100;
    } 
    else {
      // Fase 3: Detener
      pwmDer = 0;
      pwmIzq = 0;
    }
    
    // Aplicar el movimiento a los motores
    moverRobot(pwmDer, pwmIzq);
    
    // Calcular velocidades y actualizar posición solo si algún motor está en movimiento
    if (pwmDer != 0 || pwmIzq != 0) {
      // Velocidades lineales aproximadas
      float vR = pwmDer * K_vel; // velocidad del motor derecho
      float vL = pwmIzq * K_vel; // velocidad del motor izquierdo
  
      // Cinemática diferencial
      float v = (vR + vL) / 2.0; // velocidad lineal promedio
      float w = (vR - vL) / L;   // velocidad angular
  
      // Actualizar la posición y orientación
      x += v * cos(theta) * dt;  // actualiza la posición en x
      y += v * sin(theta) * dt;  // actualiza la posición en y
      theta += w * dt;           // actualiza la orientación (θ)
      
      // Normalizar theta entre -PI y PI
      while (theta > PI) theta -= 2*PI;
      while (theta < -PI) theta += 2*PI;
    }
    
    // Mostrar los resultados
    Serial.print("t: "); Serial.print((currentTime - startTime)/1000.0, 1);
    Serial.print(" | x: "); Serial.print(x, 3);
    Serial.print(" | y: "); Serial.print(y, 3);
    Serial.print(" | θ: "); Serial.println(theta, 3);
  }

  delay(100); // actualizar cada 100 ms
}