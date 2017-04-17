### Ejercicio 1

> Analizar el algoritmo matrices.c que resuelve la multiplicación de matrices
cuadradas de N*N, ¿dónde cree se producen demoras? ¿cómo se podría optimizar
el código? Optimizar el código y comparar los tiempos probando con diferentes
tamaños de matrices.k

Codigo sin modificar:

*n = 128*  
Tiempo en segundos 0.089556  
Multiplicacion de matrices resultado correcto

*n = 512*  
Tiempo en segundos 6.426406
Multiplicacion de matrices resultado correcto

*n = 1024*  
Tiempo en segundos 54.319362  
Multiplicacion de matrices resultado correcto


**1er cambio en el código:**
```c
for(i = 0; i < N; i++) {
  for(j = 0; j < N; j++) {
    int aux = 0;
    for(k = 0; k < N; k++) {
      aux = aux + getValor(A,i,k,ORDENXFILAS)* getValor(B,k,j,ORDENXFILAS);
    }
    setValor(C,i,j,ORDENXFILAS,aux);
 }
}
```
**Acumulamos en una variable auxiliar las sumas de las multiplicaciones para un elemento de la matriz resultante y se la asignamos al terminar las multiplicaciones.**

*n = 128*  
Tiempo en segundos 0.069374    
Multiplicacion de matrices resultado correcto

*n = 512*  
Tiempo en segundos 3.400423  
Multiplicacion de matrices resultado correcto

*n = 1024*  
Tiempo en segundos 35.799943    
Multiplicacion de matrices resultado correcto


**2do cambion en el codigo:**
```c
//Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
 for(i=0;i<N;i++){
   for(j=0;j<N;j++){
     setValor(A,i,j,ORDENXFILAS,1);
     setValor(B,i,j,ORDENXCOLUMNAS,1);
  }
 }
```
**A la hora de crear las matrices, a una la ordenamos por filas y la otra por columnas, según como la recorramos**

*n = 128*  
Tiempo en segundos 0.050814  
Multiplicacion de matrices resultado correcto

*n = 512*  
Tiempo en segundos 1.763525  
Multiplicacion de matrices resultado correcto

*n = 1024*  
Tiempo en segundos 14.957786  
Multiplicacion de matrices resultado correcto

3ra mejora: a los getter y setter le sacamos el parametro de orden y creamos un set y get por cada orden. Entonces se remueve el if. Para el n = 1024 se nota la mejora.

./matrices 1024  
Tiempo en segundos 12.209155  
Multiplicacion de matrices resultado correcto  

4ta y ultima mejora: no usamos getters y setter, accedemos directo a la matriz.

./matrices 1024  
Tiempo en segundos 5.289600  
Multiplicacion de matrices resultado correcto  

**Conclusión**  
Ha mejorado notablemente la eficiencia haciendo menos llamadas a la matriz y cambiando de que forma la guardamos.


### Ejercicio 2

> Analizar los algoritmo SumMulMatrices.c y SumMulMatricesOpt.c que resuelven la
siguiente operación (A \* B) + (C \* D) donde A, B, C y D son matrices cuadradas de N*N.  
Comparar los tiempos probando con diferentes tamaños de matrices, ¿Cuál
es más rápido? ¿Por qué?

*./SumMulMatrices 100*  
Tiempo en segundos 0.081049   
Multiplicacion de matrices resultado correcto  

*./SumMulMatricesOpt 100*  
Tiempo en segundos 0.069649  
Multiplicacion de matrices resultado correcto

*./SumMulMatrices 512*  
Tiempo en segundos 3.108495  
Multiplicacion de matrices resultado correcto  

*./SumMulMatricesOpt 512*  
Tiempo en segundos 2.067240  
Multiplicacion de matrices resultado correcto  

*./SumMulMatrices 1024*  
Tiempo en segundos 23.224687  
Multiplicacion de matrices resultado correcto  

*./SumMulMatricesOpt 1024*  
Tiempo en segundos 16.640072  
Multiplicacion de matrices resultado correcto  


**Conclusión**  
En un primer momento parecía que por hacer menos recorridos del for en el *SumMulMatrices*, resulta que *SumMulMatricesOpt* es por mucho más eficiente.  
Allí se ve como la cache entra en juego para el acceso a la matriz. Aunque el segundo algoritmo hace más recorridos, aprovecha de mejor forma la localidad espacial de los elementos.


### Ejercicio 3

> Describir brevemente cómo funciona el algoritmo multBloques.c que resuelve la
multiplicación de matrices cuadradas de N*N utilizando la técnica por bloques.  
Ejecutar el algoritmo utilizando distintos tamaños de matrices y distintos tamaño
de bloques, comparar los tiempos con respecto a la multiplicación de matrices
optimizada del ejercicio 1.  

> Según el tamaño de las matrices y de bloque elegido
¿Cuál es más rápido? ¿Por qué? ¿Cuál sería el tamaño de bloque óptimo para un
determinado tamaño de matriz?

Organiza a las matrices en la memoria por bloques. Dados un N (cantidad de bloques del lado de la matriz) y un r (cantidad de elementos por lado del bloque), un bloque es una submatriz de rxr elementos, y hay NxN bloques en la matriz.

En general, el tamaño de bloque que mejores resultados da, es del de 64.

### Ejercicio 5

> El algoritmo fib.c resuelve la serie de Fibonacci para un numero N dado utilizando dos métodos, el método recursivo y el método iterativo. Analizar los tiempos de ambos métodos ¿Cuál es más rápido? ¿Por qué?

Es más rápido de la forma iterativa. El Recursivo, a medida que aumenta el N, va repitiendo las funciones con el mismo parametro. Es poco eficiente.


### Ejercicio 7

> El algoritmo instrucciones.c compara el tiempo de ejecución de las operaciones básicas suma (+), resta (-), multiplicación ( * ) y división (/) para dos operandos dados x e y. ¿Qué análisis se puede hacer de cada operación? ¿Qué ocurre si x e y son potencias de 2?

Para la suma, resta y la multiplicacion fueron los mismos tiempos. Dado que la multiplicacion en numeros binarios es un corrimiento de los bits, no genera mucho mas procesamiento. Si llevó más tiempo la división, lo cual es razonable.

Cuando la división es con numeros de potencias de 2, es reduce significativamente el tiempo. Debe ser que es más sencillo de procesar las potencias de dos en el sistema binario.

### Ejercicio 8

> En función del ejercicio 7 analizar el algoritmo instrucciones2.c que resuelve la una operación binaria con dos operaciones distintas.

Mientras no sean potencias de 2, la división será mas lenta de la multiplicación.
