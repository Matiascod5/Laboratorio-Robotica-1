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

  - **¿Qué es la percepción en robótica y por qué es fundamental en los sistemas autónomos?**
    La percepción en robótica es la capacidad de un robot para obtener, interpretar y comprender información del entorno a través de sensores.
    Estainformación le permite tomar decisiones y actuar de manera adecuada frente a diferentes situaciones u obstáculos.
    Es fundamental en los sistemas autónomos porque les permite captar e interpretar información del entorno para tomar decisiones adecuadas. Sin
    percepción, un robot no podría detectar obstáculos, reconocer objetos ni adaptarse a cambios en su entorno, limitando su capacidad de “independencia

  - **En el sensor ultrasónico HC-SR04 ¿ Qué parámetro se mide para calcular la distancia? **
    El sensor ultrasónico HC-SR04 mide el tiempo que tarda una onda de sonido en ir desde el emisor, rebotar en un objeto y regresar al receptor.
    A este tiempo se le conoce comúnmente como "Tiempo de Vuelo"
    una vez tengamos el tiempo, podemos calcular la distancia con esta fórmula: Distancia = (Tiempo de vuelo - Velocidad del sonido en el aire)/2
    Se divide por 2 porque el tiempo medido es el de ida y vuelta de la onda sonora.

  - **¿Cómo influye el ruido en las mediciones del sensor ultrasónico y cómo podría reducirse?**
    El ruido influye en las mediciones haciéndolas inexactas de la distancia del robot con objetos y, en general, el entorno sobre el que navega un robot. Esto puede afectar o
    poner en riesgo el rendimiento del robot, su navegación o integridad.El ruido se puede reducir aplicando filtros a las mediciones como el filtro pasa bajos, filtro de Kalman o filtro media móvil que permitan mejorar la confiabilidad de los datos medidos.
     De la misma manera, también se pueden integrar más sensores de distancia y proximidad para mejorar la confiabilidad y precisión de las mediciones, ya sea mediante  métodos de redundancia, sinergía o complementariedad.



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

  - **Si el robot detecta el color rojo en el suelo ¿ Qué acción debería tomar? ¿ Por qué?  **
    Si el robot equipado con Arduino detecta el color rojo mediante un sensor RGB (como el TCS34725 o similares), debería ejecutarse inmediatamente la acción de detenerse

  - **Si el sensor ultrasónico detecta valores erráticos ¿Qué estrategias podría aplicar para mejorar la precisión? **
    Dependiendo de la naturaleza de estos valores erráticos en las mediciones, se pueden tomar diferentes medidas:
    En casos de mediciones aleatoriamente altas, se podrían aplicar filtros como el filtro pasa bajos, filtro de Kalman o filtro media móvil.
    En caso de mediciones erráticas con el robot en estado estático, se podría considerar calibrar las mediciones mediante la fórmula de calibración con los valores de Offset y
     Ganancia. V_real = V_medición * Ganancia + Offset. En caso de mediciones erráticas luego de aplicar filtros y calibración a las mediciones, se puede considerar la integración de nuevos sensores para brindar robustez
    , confiabilidad y precisión a las mediciones del robot mediante la sinergia, complementariedad o redundancia.


  - **Si tuvieras que integrar un nuevo sensor para mejorar la navegación del robot ¿Cuál elegirías y por qué? ** 
    Si tuviera que integrar un nuevo sensor para mejorar la navegación del robot, elegiría un sensor LiDAR como el VL53L0X o el TFmini LiDAR, debido a que proporciona una medición precisa, estable y rápida de la distancia.
    Además, tiene un rango efectivo amplio, ofrece mayor inmunidad frente a interferencias ambientales en comparación con sensores ultrasónicos, y permite detectar y anticipar obstáculos con mayor confiabilidad, mejorando así notablemente la navegación autónoma del robot basado en Arduino.

  - **¿Cual es el tiempo de respuesta del robot al detectar un cambio de color? **
    Según el codigo lo configuramos para que el robot detecte un cambio de color en un delay de aproximadamente 50 milisegundos aunque el resultado va variando a la hora de la          prueba.

---


