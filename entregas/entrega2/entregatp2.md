### Ejercicio 1

promedio de 20 iteraciones con 512 y 1024

#### OpenMP

N | 2 THREADS | 4 THREADS | Secuencial
--- | --- | --- | ---
. | segundos | segundos |
512 | 1.013316 | 0.561529 | 1.920353
1024 | 8.366157 | 4,579080 | 15.568164
2048 | 66,710775 | 37,083982 | 126,132920

**Speed Up**

N | 2 THREADS | 4 THREADS |
--- | --- | ---
512 | 1,895117614 | 3,419864335
1024 | 1,860850089 | 3,399845384
2048 | 1,890742837 | 3,401277673

**Eficiencia**  

N | 2 THREADS | 4 THREADS |
--- | --- | ---
512 | 0,947558807 | 0,854966084
1024 | 0,930425045 | 0,849961346
2048 | 0,945371418 | 0,850319418


#### PThread

N | 2 THREADS | 4 THREADS | Secuencial
--- | --- | --- | ---
. | segundos | segundos |
512 | x | x | 1.920353
1024 | x | x | 15.568164
2048 | x | x | 126,132920

**Speed Up**

N | 2 THREADS | 4 THREADS |
--- | --- | ---
512 | x | x
1024 | x | x
2048 | x | x

**Eficiencia**  

N | 2 THREADS | 4 THREADS |
--- | --- | ---
512 | x | x
1024 | x | x
2048 | x | x


### Ejercicio 2
