# Ticket System

Sistema de registro de tickets de reclamación diseñado en `C` puro. Demuestra un uso robusto de diferentes conceptos avanzados y arquitectónicos como manejo de memoria dinámica, punteros, manejo de archivos, compilación con _Makefile_, y una modularización clara y escalable.

## 1. Estructura del proyecto

El proyecto ha sido estructurado bajo convenciones de un sistema profesional separando la parte del _encabezado/declaración_ (`include/`), la _lógica e implementación_ (`src/`), y una zona de persistencia y exportación en disco (`assets/`). El código se divide en una lógica de negocio clara sobre los tipos de operaciones, la modularidad auxiliar (utilidades) y el punto de entrada principal (`main.c`).

```text
ticket_system/
│
├── src/
│   ├── main.c           (Lógica principal y punto de entrada)
│   ├── ticket/
│   │   └── ticket.c     (Implementación de la lógica de negocio y archivos)
│   └── utils/
│       └── utils.c      (Implementación de validaciones y de I/O)
│
├── include/
│   ├── ticket/
│   │   └── ticket.h     (Declaración de objeto Ticket e interfaz)
│   └── utils/
│       └── utils.h      (Declaraciones de ayudantes o utils)
│
├── assets/              (Directorio donde se almacenarán en disco los tickets generados)
│
├── Makefile             (Archivo con las reglas automáticas de compilación segura)
└── README.md            (Toda la documentación base técnica del proyecto)
```

## 2. Cómo compilar el programa

Para compilar el programa se incluyó un **Makefile** a nivel de la raíz del proyecto. Este usa `gcc` y flags rigurosas de desarrollo `(-Wall -Wextra -std=c11)` para confirmar que no se tienen _warnings_ ni _bugs_.

Ejecute en consola (estando dentro del área `ticket_system`):
```bash
make
```
o explícitamente:
```bash
make all
```

> **Nota:** Con esto se compilarán los correspondientes `.c` como objetos intermedios dento de la carpeta oculta `obj/` y generará un binario final llamado `ticket_app`.

## 3. Cómo ejecutarlo

Una vez se ha compilado exitosamente, para arrancar la utilidad interactiva CLI, puede llamar al target predefinido de ejecución dentro del **Makefile**:

```bash
make run
```
O de manera directa sobre el ejecutable binario que se formó de la previa compilación:
```bash
./ticket_app
```

## 4. Cómo limpiar los archivos

Dado la cantidad de código intermedio y artefactos formados con la compilación (.o dentro de un árbol interno de obj/), se proporciona un _target_ en el **Makefile** que erradica por completo la estructura temporal de construcción e inclusive elimina el aplicativo, dejando virgen nuevamente el proyecto:

```bash
make clean
```

---

## Explicación Técnica (Detallada)

### Uso de Punteros
El núcleo usa punteros exhaustivamente para optimizar envíos de estructuras `Ticket` a las distintas sub-funciones de validación, escritura y captura usando un único modelo (paso de argumentos por referencia). Para llenar un ticket, el contexto principal invoca `fill_ticket_data(Ticket *ticket)`, previniendo copias completas de los datos cada vez, de esta manera el ticket que instanciamos es modificado "in-place" o directamente. El tipo string (un arreglo de `char`) en C decae de por sí a punteros, y es por esto que implementamos `read_string` con argumentos apuntando hacia `char *buffer` que interactúan de manera confiable con la función `fgets`.

### Manejo de Memoria
El proyecto no aparta la instancia sobre memoria automática (el stack). En su lugar, pide espacio en la zona global (el _heap_) usando:
```c
Ticket *new_ticket = (Ticket *)malloc(sizeof(Ticket));
```
Esto genera que los datos perduren la scope de su entorno o contexto e inicialmente requiere mayor responsabilidad con el uso de memoria; para este modelo se valida la no nulidad que indica una carencia de memoria libre por del sistema (`malloc != NULL`). Finalmente, nos hacemos cargo de reciclar correctamente ese recurso reservado a través de una función _destroy()_ del mismo módulo invocando un llamado a `free()` para esquivar los _memory leaks_ (fugas de memoria) e ineficiencias persistentes de RAM antes de concluir.

### Generación del Radicado (Casting Explícito)
Se produce la mezcla de un sello de tiempo único sumándole algo del azar, transformándolo finalmente con "casting explícito" requerido (`long`) para alinear al tipo del radicado:
```c
long timestamp = (long)time(NULL);
long random_offset = (long)rand() % 10000;
ticket->radicado = timestamp + random_offset;
```
Este diseño minimiza choques si se abren ventanas múltiples en el mismo minuto y permite una trazabilidad limpia guardando todo en el struct base.

### Manejo de Errores
El software posee defensas rigurosas a fallos:
- **Reserva fallida (Malloc):** Si el apuntador recibido deviene NULL, el programa aborta ordenadamente con un `EXIT_FAILURE` sin intentar desreferenciar lo que arrojaría un fatal `Segmentation Fault`.
- **Lectura e Inputs Inválida:** No se usa `gets` ante los riesgos de _buffer overflow_. El programa recurre segurizado a `fgets` e incluye funciones que limpian el buffer fantasma para un ingreso impecable. En caso de cadenas nulas o con mala sintaxis del `@` para email, emite el `fprintf(stderr)` correspondiente en consola y sale educadamente.
- **Fallos en escritura/Disco:** Usando la API de archivos se examina si la apertura a escritura falla (`fopen == NULL`). Si los permisos sobre `assets/` merman la escritura o el _file descriptor_ topa, no rompe el programa abrubtamente, reportándose con mensaje en consola advirtiendo al usuario del escenario adverso en su disco.

### Funcionamiento del Makefile
Un **Makefile** permite la automatización fluida del ciclo de desarrollo de _compilación -> empaquetado -> limpieza._ 

El diseño se conforma por dependencias donde el target general `all` va a exigir que se complete `assets_dir` (para cerciorar la existencia de la carpeta que guarda los ficheros del registro) y de compilar el binario `$(TARGET)`. A su vez, el `$(TARGET)` exige las depedenvcias de `$(OBJS)`, que son todos ficheros `.o` que internamente recurren a construir basándose en las contrapartes `.c`. Además inyecta la cabecera general por la flag `-I`.
Se incluyen targets utilitarios y todos etiquetados debajo del `.PHONY` para advertirle a GNU Make que `all`, `run`, o `clean` son verbos/órdenes conceptuales a ejecutar, evadiendo fallas o falsos en caso que alguien genere a propósito un fichero local con tales nombres.