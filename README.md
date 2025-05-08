# Laboratorio-Robotica-1
#LINK VIDEOS: https://drive.google.com/drive/folders/15AlvotKpdCCnD1rTmeP-yavsMe0CPimI?usp=sharing 

Descripción del proyecto

El proyecto consiste en la aplicación de conocimientos sobre robótica, permitiendo así profundizar en el área práctica de la robótica ensamblando un sistema compuesto de un microcontrolador, sensores, ruedas, entre otros elementos, fusionandolos para la elaboración de un robot móvil diferencial.

• Explicacion y conexion correcta de componentes.

Sensor ultrasónico HC-SR04: El robot móvil diferencial cuenta con la particularidad de responder de manera adecuada a las paredes u otros obstaculos que se interpongan en su camino. El sensor le permite calcular distancias hasta obstaculos en dirección recta hacia él.

sensor IMU-6050: El robot ensamblado cuenta con la habilidad de avanzar y girar gracias a esta conexión, que permite controlar sus movimientos. Estos movimientos los realiza mediante dos motores y dos ruedas conectadas al sistema.

Todos los elementos anteriormente mencionados se encuentran conectados en su funcionamiento mediante un microcontrolador Arduino Uno, el cual permite una mayor percepción por parte del robot diferencial.

• Implementacion del control de motores.

se usaron dos motores DC montados en una configuración diferencial. El control se logró combinando pines digitales, que determinan la dirección de giro, y señales PWM, que regulan la velocidad de cada motor. Se usaron cuatro pines digitales para indicar si cada motor debía girar hacia adelante o hacia atrás.
A través del PWM, se pudo controlar la velocidad de forma individual para cada lado del robot.
Se implementaron funciones como moverRobot(), detener(), retroceder() y girarDerecha(), lo que permitió manejar el movimiento del robot de manera sencilla y efectiva.
El comportamiento del robot se definió ajustando el duty cycle de las señales PWM según la acción deseada, y combinando eso con el estado de los pines de dirección. Esto le permitió al robot avanzar en línea recta, girar sobre su eje o retroceder según fuera necesario.

• Implementacion y prueba de control de velocidad por intervalos de
tiempo. 

La implementación del control de velocidad por intervalos de tiempo se realiza usando la función millis() para medir el tiempo transcurrido y dividir el movimiento del robot en fases (avanzar, girar, detenerse). En cada fase, la velocidad de los motores se ajusta según el tiempo transcurrido.

• Implementacion de codigo de cinematica y control de velocidad con el
IMU  		

El código implementa un sistema básico de cinemática diferencial y control de velocidad para un robot móvil, donde se estima la posición (x, y)
y la orientación θ del robot en el tiempo mediante una simulación basada en el modelo de ruedas diferenciales.
El movimiento se controla por fases (avance, giro y detención), y las velocidades de los motores se convierten en velocidades lineales usando un factor K_vel.

• Ajuste de direccion y correccion de inclinacion usando el IMU 

Este programa implementa un sistema de navegación recta automática para un robot móvil, usando un IMU MPU9250.
El enfoque se basa únicamente en lecturas del giroscopio (gyroZ) para detectar desviaciones laterales (giro en eje Z) y
realizar correcciones a través del control diferencial de los motores.
