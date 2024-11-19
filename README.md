# Desarrollo de una Shell Personalizada en el Refugio Postapocalíptico

## Introducción

En un mundo devastado por la pandemia del Cordyceps, donde la tecnología es un recurso escaso pero vital para la supervivencia, las comunidades refugiadas dependen de sistemas heredados para mantener sus infraestructuras esenciales. La comunicación, el monitoreo de sistemas y la gestión de recursos se han vuelto más críticos que nunca. En este contexto, desarrollar herramientas que nos permitan interactuar y controlar nuestros sistemas es fundamental.

<p align="center">
  <img src=".img/img.png"/>
</p>

En este segundo trabajo, tu misión es diseñar e implementar una **shell personalizada** al estilo Bourne Shell, adaptada a las necesidades del refugio. Esta shell no solo nos permitirá ejecutar y controlar programas esenciales, sino que también se integrará con el sistema de monitoreo desarrollado previamente, proporcionando una interfaz unificada para gestionar nuestros recursos tecnológicos.

## Objetivos del proyecto

1. **Utilizar mecanismos de creación de procesos**: Implementar la creación y gestión de procesos para ejecutar comandos y programas.

2. **Utilizar mecanismos de concurrencia e IPC**: Implementar comunicación entre procesos para coordinar tareas y compartir información crucial.

3. **Diseñar un intérprete de línea de comandos al estilo Bourne Shell**: Crear una shell personalizada que pueda interpretar y ejecutar comandos, incluyendo características avanzadas.

4. **Ampliar las características de la shell**: Añadir funcionalidades como ejecución en segundo plano, pipes, redirección de entrada/salida y manejo de scripts.

5. **Comprender el uso de la API de señales del SO**: Implementar el manejo de señales para controlar procesos y responder a eventos del sistema.

6. **Aplicar CMake para la gestión del proyecto**: Utilizar CMake para organizar y compilar el proyecto de manera eficiente.

7. **Integrar la shell con el sistema de monitoreo**: Conectar la shell con el programa de monitoreo del laboratorio anterior, permitiendo su control y configuración desde la línea de comandos.

8. **Utilizar cJSON para el manejo de datos**: Implementar el uso de cJSON para leer y escribir configuraciones en formato JSON.

9. **Alcanzar al menos un 5% de cobertura de testing**: Escribir pruebas que cubran al menos el 5% del código, asegurando la calidad y fiabilidad de la shell.

## Actividades

A continuación, se detallan las tareas que debes realizar para completar el laboratorio.

### 1. Command Line Prompt

Implementa un **prompt de línea de comandos** que muestre el camino al directorio actual. Dado que la gestión de los directorios es esencial en el refugio, el prompt debe proporcionar información clara sobre la ubicación en el sistema de archivos.

- Formato del prompt: `usuario@hostname:directorio_actual$`
- Ejemplo para el directorio home: `survivor@refugio:~$`

### 2. Comandos Internos

La shell debe soportar los siguientes **comandos internos**:

- **`cd <directorio>`**: Cambia el directorio actual a `<directorio>`. Si `<directorio>` no está presente, muestra el directorio actual. Si el directorio no existe, se debe imprimir un error apropiado. Este comando debe actualizar las variables de entorno `PWD` y `OLDPWD`, y soportar `cd -` para volver al último directorio de trabajo.

- **`clr`**: Limpia la pantalla, proporcionando una vista despejada para nuevas tareas.

- **`echo <comentario|$env_var>`**: Muestra `<comentario>` en la pantalla seguido de una nueva línea. Soporta variables de entorno utilizando `$`.

- **`quit`**: Cierra la shell, permitiendo al usuario salir de manera controlada. Los recursos deben liberarse adecuadamente.

### 3. Invocación de Programas

La shell debe interpretar las entradas del usuario que no son comandos internos como la **invocación de programas externos**.

- Utiliza `fork()` y `execvp()` para crear un proceso hijo y ejecutar el programa.
- Soporta rutas relativas y absolutas.
- Maneja errores si el programa no existe o no puede ejecutarse.

### 4. Archivo de Comandos (Batch File)

La shell debe ser capaz de tomar comandos desde un **archivo de comandos** (batch file).

- Si la shell se invoca con un argumento (e.g., `./myshell comandos.txt`), debe leer y ejecutar los comandos del archivo.
- Al alcanzar el fin de archivo (EOF), la shell debe cerrarse.
- Si se ejecuta sin argumentos (`./myshell`), la shell muestra el prompt y espera comandos del usuario vía stdin.

### 5. Ejecución en Segundo Plano

Implementa la **ejecución de procesos en segundo plano** utilizando `&` al final de la línea de comando.

- La shell debe retornar al prompt inmediatamente después de lanzar el proceso en segundo plano.
- Al iniciar un trabajo en segundo plano, se debe imprimir un mensaje indicando su **ID de trabajo** y **ID de proceso**:
  ```
  [<job id>] <process id>
  ```
- Ejemplo:
  ```
  survivor@refugio:~$ echo 'hola' &
  [1] 10506
  hola
  ```

### 6. Manejo de Señales

Implementa el **manejo de señales** para controlar procesos en ejecución:

- Las señales resultantes de combinaciones como `CTRL-C`, `CTRL-Z` o `CTRL-\` (`SIGINT`, `SIGTSTP`, `SIGQUIT`) deben enviarse al proceso en primer plano en ejecución, no a la shell.
- Si no hay un proceso en ejecución en primer plano, la shell debe ignorar la señal y continuar esperando comandos.

### 7. Pipes

Implementa la funcionalidad de **pipes** utilizando el operador `|`.

- Conecta la salida estándar del primer comando con la entrada estándar del segundo comando.
- Soporta múltiples pipes en una sola línea de comando.
- Ejemplos:
  - `survivor@refugio:~$ last | wc -l`
  - `survivor@refugio:~$ ps aux | grep firefox`
  - `survivor@refugio:~$ grep 'error' log.txt | sort | uniq -c`

### 8. Redirección de Entrada/Salida

Implementa la **redirección de entrada y salida** utilizando los operadores `<` y `>`.

- Permite redirigir `stdin` desde un archivo y `stdout` hacia un archivo.
- Ejemplo:
  - `programa arg1 arg2 < entrada.txt > salida.txt`
- La redirección debe funcionar también para comandos internos como `echo`.

### 9. Integración con el Sistema de Monitoreo

Conecta la shell con el programa de monitoreo desarrollado en el laboratorio anterior:

- Agrega comandos internos para controlar el monitoreo:
  - **`start_monitor`**: Inicia el programa de monitoreo en un proceso hijo.
  - **`stop_monitor`**: Detiene el programa de monitoreo.
  - **`status_monitor`**: Muestra el estado actual del monitoreo.
- Utiliza **IPC** (por ejemplo, señales, pipes nombrados o sockets Unix) para la comunicación entre la shell y el programa de monitoreo.
- Asegura que el programa de monitoreo pueda recibir configuraciones y enviar información relevante.

### 10. Uso de cJSON para Configuración

Implementa el uso de **cJSON** para manejar configuraciones en formato JSON:

- Crea un archivo `config.json` para almacenar configuraciones del programa de monitoreo (e.g., intervalos de muestreo, métricas a recolectar).
- El programa de monitoreo debe leer y aplicar estas configuraciones al iniciarse.
- Permite que la shell modifique configuraciones a través de comandos internos, actualizando el archivo JSON.

### 11. Gestión del Proyecto con CMake

Utiliza **CMake** para organizar y compilar el proyecto:

- Configura `CMakeLists.txt` para compilar la shell y el programa de monitoreo, gestionando las dependencias.
- Organiza el código en módulos o bibliotecas según su funcionalidad.
- Asegura que las librerías externas (e.g., `prometheus-client-c`, `cJSON`, `libmicrohttpd`) se encuentren y enlacen correctamente.

### 12. Cobertura de Testing

Implementa pruebas que cubran al menos **el 5% del código**:

- Escribe tests unitarios para funciones críticas de la shell y del programa de monitoreo.
- Utiliza herramientas como `gcov` y `lcov` para medir la cobertura de código.
- Genera reportes de cobertura y analiza áreas de mejora.

## Entregables

- Código fuente bien organizado y documentado, siguiendo buenas prácticas de programación.
- Archivo `CMakeLists.txt` para la compilación del proyecto.
- Archivo `config.json` con las configuraciones iniciales.
- Reportes de cobertura de testing.
- Documentación generada con Doxygen.
- Un archivo `INSTALL.md` con instrucciones claras para compilar, ejecutar y utilizar la shell y el programa de monitoreo.
- Informe que responda a las siguientes preguntas

```console
¿Dónde se encuentran los pipes en el sistema de archivos y qué atributos tienen?

¿Qué mecanismos de IPC existen en Linux y en qué situaciones es apropiado usar cada uno?

¿Cómo funcionan los pipes con nombre (named pipes) y en qué se diferencian de los pipes anónimos?

¿Cuál es el rol de CMake en la gestión de proyectos y cómo facilita la compilación en múltiples plataformas?

¿Cómo se pueden especificar y gestionar las dependencias externas en un proyecto usando CMake?

¿Qué es el framework de testing Unity en C y cómo se integra en un proyecto para realizar pruebas unitarias?

¿Cómo se mide y mejora la cobertura de código en pruebas unitarias utilizando herramientas como gcov y lcov junto con Unity?
```

## Evaluación

La evaluación se basará en:

- **Funcionalidad**: Cumplimiento de todos los requisitos especificados.
- **Calidad del Código**: Legibilidad, organización y adherencia a buenas prácticas.
- **Integración**: Correcta interacción entre la shell y el programa de monitoreo.
- **Uso de Herramientas**: Implementación efectiva de CMake y cJSON.
- **Testing**: Cobertura de pruebas y calidad de los tests.
- **Documentación**: Calidad y claridad de la documentación generada.
- **Creatividad**: Valoración de funcionalidades adicionales o mejoras implementadas.

## Recursos Adicionales

- **Documentación de Sistemas Operativos**:

  - Páginas de manual (`man`) para llamadas al sistema como `fork`, `execvp`, `pipe`, `signal`, etc.
  - Tutoriales sobre IPC y manejo de señales en Linux.

- **CMake**:

  - [Documentación oficial de CMake](https://cmake.org/documentation/).
  - Ejemplos y guías para configurar proyectos en C.
  - [Video seminario sobre CMake y buenas prácticas.](https://youtu.be/EQ5H6Uc5WPA)

- **cJSON**:

  - [Repositorio oficial de cJSON](https://github.com/DaveGamble/cJSON) con documentación y ejemplos.

- **Herramientas de Testing**:
  - `gcov` y `lcov` para análisis de cobertura de código.
  - [Unity Testing Framework para pruebas unitarias en C](https://github.com/ThrowTheSwitch/Unity).

## Consideraciones Finales

En el refugio, cada línea de código y cada herramienta desarrollada puede marcar la diferencia entre la supervivencia y el caos. Este proyecto es una contribución real a la fortaleza y resiliencia de nuestra comunidad.

A medida que avances, mantén un registro detallado de tus decisiones y aprendizajes. Comparte tus hallazgos con tus compañeros y fomenta la colaboración. Recuerda que, en estos tiempos, el conocimiento compartido es un recurso invaluable.

---

<center>
    <strong>¡Buena suerte en tu misión, developer del refugio! Tu trabajo es crucial para mantener la llama de la civilización encendida.</strong>
</center>
