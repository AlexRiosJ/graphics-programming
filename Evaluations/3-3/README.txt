Evaluación aúlica 2

Autor: Alejandro Rios Jasso
Fecha: Abril 20, 2020

Para esta evaluación aúlica se entrega 1 solo programa "main.c".

En este se encuentra un compilado de los ejercicios 1, 2, 4, 5, 6

Para el ejercicio 1:
    lineas 386-389, 260-265, archivos cylinder.c y .h
    Se observa un cilindro girando sobre su eje en un lugar del cuarto.
Para el ejercicio 2: 
    lineas 373-384, 273-277, archivos sphere.c y .h
    Se observan 3 esferas de diferente color.
Para el ejercicio 4:
    lineas 279-303, (initShadow() linea 92)
    Se observan 3 esferas rebotando con su sombra en un cuarto.
Para el ejercicio 5:
    funcion move() linea 187
    Se puede navegar hacia adelante y atras, derecha e izquierda con las teclas [W, A, S, D] y se respeta el PITCH y YAW, y que no se pueden atravesar los muros.
Para el ejercicio 6:
    funcion rotateCamera() linea 341
    Se mantiene el mouse en el centro y se puede mirar hacia cualquier dirección con el mouse.

Al comienzo de la ejecución el movimiento de las esferas estará pausado, pulse ENTER para reanudar.
Se puede pulsar ENTER en cualquier momento para detener / reanudar su movimiento.

Le dejo los siguientes archivos:
/
main.c
README.txt
utils/
    cylinder.c
    cylinder.h
    sphere.c
    sphere.h
    utils.c
    utils.h
    mat4.c
    mat4.h
    transforms.c
    transforms.h
shaders/
    color.fsh
    projection-gouraud.vsh

Nota: 
El rebote no se maneja con cualquier plano inclinado para mejorar la calidad de los demás entregables.
(Es lo único que considero estar 100% correcto ya que el rebote se calcula de una manera más sencilla).