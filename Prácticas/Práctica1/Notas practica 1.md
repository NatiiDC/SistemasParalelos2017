### Ejercicio 1

> Analizar el algoritmo matrices.c que resuelve la multiplicación de matrices
cuadradas de N*N, ¿dónde cree se producen demoras? ¿cómo se podría optimizar
el código? Optimizar el código y comparar los tiempos probando con diferentes
tamaños de matrices.

Codigo sin modificar:

*n = 100*  
Tiempo en segundos 0.048112  
Multiplicacion de matrices resultado correcto

*n = 1000*  
Tiempo en segundos 26.957387  
Multiplicacion de matrices resultado correcto


**1er cambio en el código:**
```c
for(i = 0; i < N; i++) {
  for(j = 0; j < N; j++) {
    aux = 0;
    for(k = 0; k < N; k++) {
      aux = aux + getValor(A,i,k,ORDENXFILAS)* getValor(B,k,j,ORDENXFILAS);
    }
    setValor(C,i,j,ORDENXFILAS,aux);
 }
}
```
Acumulamos en una variable auxiliar las sumas de las multiplicaciones para un elemento de la matriz resultante y se la asignamos al terminar las multiplicaciones.

*n = 100*  
Tiempo en segundos 0.025637  
Multiplicacion de matrices resultado correcto

*n = 1000*  
Tiempo en segundos 16.578740  
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

*n = 100*  
Tiempo en segundos 0.032066  
Multiplicacion de matrices resultado correcto

*n = 1000*  
Tiempo en segundos 12.428136  
Multiplicacion de matrices resultado correcto

**Conclusión**  
Ha mejorado notablemente la eficiencia haciendo menos llamadas a la matriz y cambiando de que forma guardamos la matriz.


### Ejercicio 2

> Analizar los algoritmo SumMulMatrices.c y SumMulMatricesOpt.c que resuelven la
siguiente operación (A \* B) + (C \* D) donde A, B, C y D son matrices cuadradas de N*N.  
Comparar los tiempos probando con diferentes tamaños de matrices, ¿Cuál
es más rápido? ¿Por qué?

*./SumMulMatrices 100*  
Tiempo en segundos 0.019713   
Multiplicacion de matrices resultado correcto  

*./SumMulMatricesOpt 100*  
Tiempo en segundos 0.018189  
Multiplicacion de matrices resultado correcto

*./SumMulMatrices 1000*  
Tiempo en segundos 23.726947   
Multiplicacion de matrices resultado correcto  

*./SumMulMatricesOpt 1000*  
Tiempo en segundos 15.577054  
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
Según el tamaño de las matrices y de bloque elegido
¿Cuál es más rápido? ¿Por qué? ¿Cuál sería el tamaño de bloque óptimo para un
determinado tamaño de matriz?
