#### Ejercicio 1  

Al sacarle la variable private(i) que recibía como parámetro, empezó a funcionar correctamente. El otro parametro no influye en el resultado share(A).

#### Ejercicio 2  

El problema está en que en cada iteración, se usa el resultado de la operación anterior para calcularlo.
Se necesita respetar ese orden. Solo se puede paralelizar una cuenta independiente.

#### Ejercicio 3  

En mi máquina pareciera ser más lento por columnas. En el aula de posgrado me fijo.

#### Ejercicio 4  
