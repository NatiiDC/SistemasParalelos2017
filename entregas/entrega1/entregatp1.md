### Entrega Trabajo Practico 1

#### Realizar un algoritmo optimizado (sin bloques) que resuelva la expresión:  

𝑀 = 𝑑̅𝐴𝐵𝐶 + 𝑏̅𝐷𝐸𝐹  
Donde A, B, C, D, E y F son matrices de NxN.  

𝑑̅y 𝑏̅ son los promedios de los valores
de los elementos de las matrices D y B, respectivamente.

Está adjunto, es el `entregatp1a.c`.  
Correr para compilar: `gcc -o entregatp1a entregatpa.c -lm -O3`  
-lm es para que linkee la libreria de math.h  
-O3 para la optimizacion en la compilación

#### Describir brevemente cúales son las optimizaciones aplicadas
Al igual que en los ejercicios de la práctica, a la hora de hacer la operación de multiplicación de matrices, para cada elemento de la matriz resultante, la guardamos en una variable local y al finalizar las operaciones para ese elemento, lo guardamos en la matriz resultante.

Cuando sumamos las dos matrices resultantes de las multiplicaciones, reutilizamos la primer matriz para dejar los resultados.

El producto de Matriz por Escalar multiplica a la matriz pasada por el escalar pasado por parametro. No hubo diferencia entre hacerlo en una funcion a parte o ir calculandolo a medida que se recorria esa matriz (A o D) cuando se hacían las multiplicaciones.  


#### Comparar esta versión contra una versión del algoritmo implementada con bloques. Completar las siguientes tablas

>Indicar brevemente ¿Cuál es el tamaño de bloque
óptimo? ¿De qué depende?

Bloques de tamaño 64. Depende de la ram de la maquina.

> Tiempos (en segundos)

N | Optimizado | Por bloques (considerando el bloque óptimo)
--- | --- | ---
512 | 0.812397 | 0.716775
1024 | 7.691725 | 6.158147

> Indicar brevemente ¿Cuál es el algoritmo más
rápido y por qué?

A mayor cantidad de elementos, es más eficiente el de bloques. Debe aprovechar un poco mejor la cache y memoria para utilizar los datos.

> Describir brevemente las características de la arquitectura
utilizada (Procesador, memoria, niveles y tamaño de caché)

command: `lscpu`

```
Architecture:          x86_64
CPU(s):                4
NUMA node(s):          1
Model name:            Intel(R) Core(TM) i3-2350M CPU @ 2.30GHz
Stepping:              7
CPU MHz:               946.414
CPU max MHz:           2300,0000
CPU min MHz:           800,0000
BogoMIPS:              4589.57
L1d cache:             32K
L1i cache:             32K
L2 cache:              256K
L3 cache:              3072K
```
