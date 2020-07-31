# Temporizador para uso de mascarillas

Con la venida de la pandemia originada por el coronavirus o COVID-19, el uso de la mascarilla se ha vuelto algo indispensable para poder llevar una vida "normal", como antes de que todo esto ocurriese. Por ello, esta utilidad para Arduino ayuda a saber cuanto uso se le ha dado a una mascarilla y así saber cuando ha terminado su vida útil. Por defecto, viene configurada para una FPP2 de ocho hora de duración, pero es perfectamente configurable para cada caso. 


# Requisitos y caracterísitcas principales

 - Arduino UNO o equivalente, basado en el microcontrolador ATMEGA328 en cualquiera de sus variantes.
 - Librerías necesarias: [u8g2](https://github.com/olikraus/u8g2), [Chrono](https://github.com/SofaPirate/Chrono), [Button](https://github.com/madleech/Button)
 -  Pantalla OLED con controlador SSD1306, SH1106 o cualquiera compatible con *u8g2*.
 - Tres pulsadores.
 

 

## Características

 - Funciones de inicio, parada y reinicio a cero con entradas de pulsadores. 
 - Almacena el tiempo transcurrido en la EEPROM del Arduino. 
 - Muestra el tiempo de uso en horas y el porcentaje de la mascarilla. 
 - Avisa cuando es necesario el cambio de la misma. 

## Funcionamiento

En un primer momento, el tiempo transcurrido está en cero. Al pulsar el botón de inicio, comenzará a avanzar el tiempo y con ello una barra de progreso para hacer más visual el transcurso de éste. Cuando pulsamos el botón de pausa,  se detendrá el conteo de tiempo y almacenará en la EEPROM de Arduino el último valor de éste para así, tras apagar y encender, mantenga memoria del uso real de la mascarilla. El botón de puesta a cero, reiniciará el contador al instante de *"fábrica"* y escribirá en la memoria un valor nulo de tiempo de uso. Cuando el valor de la variable `duracion` se alcance, se detendrá la cuenta del tiempo y mostrará un triángulo de advertencia recordando al usuario que debe de desechar la mascarilla y sustituirla por otra nueva. 

## Capturas de la interfaz

![Interfaz inicial](https://i.ibb.co/JC5zb28/IMG-20200801-010627.jpg)
![Reemplazar mascarilla](https://i.ibb.co/K2WjcT6/IMG-20200801-010644.jpg)



# Características pendientes

Se irá actualizando:

 - Redimensionar automáticamente la barra de progreso dependiendo del valor de `duracion` especificado.
 - Añadir duraciones predefinidas para otras mascarillas, como FPP3 o quirúrgicas sin tener que escribirlas en el código.
 - Añadir un sistema para usar varias mascarillas a la vez.

