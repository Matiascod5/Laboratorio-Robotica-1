# Laboratorio 2 - Robótica y Sistemas Autónomos

**Curso:** ICI4150  
**Profesor:** Sandra Cano  
**Tema:** Sensores, Percepción y Planificación Básica con procesamiento de datos en Robótica Móvil  
**Integrantes del grupo:** 
- Adolfo Cordero
- Ruben Carvajal
- Matias Castro
- Carlos Da silva
- Sebastian Gatica

#LINK VIDEOS: https://drive.google.com/drive/folders/1lcxLrsuCurR6A5LsiarIUT4XW50D7ADl?usp=sharing
---
### Descripción del proyecto
El Laboratorio 2 se enfocó en desarrollar un sistema de percepción y planificación básica para un robot móvil autónomo utilizando Arduino. El proyecto consistió en conectar sensores como el HC-SR04 (ultrasónico) y el sensor RGB, procesar sus datos mediante técnicas de filtrado y umbralización, y luego implementar una lógica de navegación reactiva basada en la detección de obstáculos y colores. El robot fue programado para detenerse ante ciertos colores y evitar obstáculos, integrando así percepción sensorial y control de movimiento de manera autónoma.
## 🔧 Parte 1: Configuración del Hardware y pruebas iniciales
---
### Evaluaciones

- **Conexión correcta de componentes:**  
  Se conectó exitosamente el sensor ultrasónico HC-SR04 y el sensor RGB al Arduino UNO. Se validó su funcionamiento en el video `PARTE1.mp4`.
  El HC-SR04 fue configurado para medir distancias con sus pines TRIG y ECHO, y el RGB para detectar colores a través de sus canales. Se verificó su funcionamiento en el video PARTE1.mp4, donde ambos sensores    muestran lecturas activas por monitor serie, confirmando que la conexión fue exitosa

- **Código funcional:**  
  El archivo `Parte1_Percep.ino` muestra correctamente por monitor serie los datos de distancia y color.

- **Análisis de variaciones en las lecturas:**  
  Se observó cómo influye la iluminación, la superficie y la distancia en la precisión de las mediciones.

- **Respuestas teóricas:**

  - ¿Qué es la percepción en robótica?  
    Es la capacidad del robot para interpretar su entorno usando sensores. Es esencial en sistemas autónomos porque permite la toma de decisiones.

  - ¿Qué mide el sensor ultrasónico?  
    El tiempo de vuelo de la onda sonora (ida y vuelta), con lo cual se calcula la distancia.

  - ¿Cómo influye el ruido y cómo reducirlo?  
    Puede generar errores en la lectura. Se puede aplicar filtrado (media móvil, Kalman, pasa bajos) o usar sensores adicionales para mejorar precisión.

---

## Parte 2: Procesamiento de datos y Algoritmo de Navegación

### Evaluaciones

- **Algoritmo de planificación reactiva:**  
  Implementado en `parte2_RGB.ino`. El robot se detiene con rojo y actúa con otros colores. Video de prueba: `PARTE2.mp4`.

- **Umbralización:**  
  Se definieron umbrales para la distancia (10 cm) y colores (R, G, B) para tomar decisiones.

- **Pruebas y ajustes reales:**  
  El sistema fue probado sobre superficies con colores y obstáculos. Ajustes se evidencian en video `PARTE2.mp4`.

- **Reflexión y respuestas teóricas:**

  - ¿Qué debe hacer si detecta rojo?  
    Detenerse por seguridad o cambio de contexto.

  - ¿Cómo enfrentar lecturas erráticas del ultrasónico?  
    Filtros, calibración (offset y ganancia) y/o incorporación de nuevos sensores.

  - ¿Qué nuevo sensor añadirías?  
    Sensor LiDAR como el VL53L0X por su precisión, velocidad y resistencia al ruido.

  - ¿Cuál es el tiempo de respuesta al cambio de color?  
    Aproximadamente 50 milisegundos (configurado con `delay(50)`).

---


