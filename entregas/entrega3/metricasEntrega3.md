# Entrega Nro. 3
## Programación con memoria distribuida

#### Alumnos:
* Aparicio Natalia Elizabeth - 12667/7
* Eusebi Cirano - 12469/2

---

### Ejercicio 1

Para la solución se generaron las 6 matrices en el nodo raíz.  
Se comunicaron las matrices B, C, E y F completas vía **Broadcast**.
Se comunicaron las matrices A y D via **Scatter**.
Se calcularon los promedios en paralelo y se comunicó la suma total local con un **All Reduce**.  
Cada proceso generó su parte de matriz resultante y multiplicó ABCd + DEFb de su parte.  
Finalmente el nodo raíz recibió el resultado a travez de **Gather**.

(comunicaciones)  

N | 1M- 4 procesos | 2M - 4 procesos | 2M - 8 procesos | 4 PThreads | 4 OpenMP | Secuencial
| --- | --- | --- | --- | --- | --- | --- |
512  | 0.463537 (0.010813) | 0.528271 (0.117815) | 0.529454 (0.305401) | 0.5382 | 0.5615 | 1.920353 |
1024 | 3.556329 (0.031752) | 3.790304 (0.431785) | 2.193876 (0.436301) | 4.2264 | 4,5790 |15.568164 | 
2048 | 28.567744 (0.101481) | 28.261122 (1.708869) | 15.541894 (1.714496) | 32.5148 | 37,0839 |126.132920 |


**Speed Up**

(Speed up y eficiencia sin comunicaciones)

N | 1M- 4 procesos | 2M - 4 procesos | 2M - 8 procesos | 4 PThreads | 4 OpenMP | Secuencial
| --- | --- | --- | --- | --- | --- | --- |
512  | 4.1428 (4.2417) | 3.6351 (4.6785) | 3.6270 (8.5709) | 3.5678 | 3,4198 | 1 |
1024 | 4.3775 (4.4170) | 4.1073 (4.6354) | 7.0961 (8.8577) | 3.6835 | 3,3998 | 1 |
2048 | 4.4152 (4.4309) | 4.4631 (4.7503) | 8.1156 (9.1219) | 3.8692 | 3,4012 | 1 |

**Eficiencia**  

N | 1M- 4 procesos | 2M - 4 procesos | 2M - 8 procesos | 4 PThreads | 4 OpenMP | Secuencial
| --- | --- | --- | --- | --- | --- | --- |
512  | 1.0357 (1.0604) | 0.9087 (1.1696) | 0.5433 (1.0713) | 0.8919 | 0,8549 | 1 |
1024 | 1.0943 (1.1042) | 1.0268 (1.1588) | 0.8870 (1.1072) | 0.9208 | 0,8499 | 1 |
2048 | 1.1038 (1.1077) | 1.1157 (1.1875) | 1.0144 (1.1402) | 0.9698 | 0,8503 | 1 |

Creemos que la superlinealidad de la mayoría de las métricas tiene que ver con que el algoritmo realizado durante la práctica 1 no era lo suficientemente eficiente.

### Ejercicio 2

Para la solución se generó el arreglo a ordenar en el nodo raíz.  
Se dividió el arreglo en P partes y se comunicó con **Scatter**.  
Se ordenaron las partes localmente con qsort (glibc).  
Para el merge se utilizó comunicación uno a uno con los procesos de a pares.  
**No** se utilizaron sincronizaciones por niveles.

(comunicaciones)  

N | 1M- 4 procesos | 2M - 4 procesos | 2M - 8 procesos | 4PThread |Secuencial
--- | --- | --- | --- | --- | --- |
2^25 | 3.546283 (0.053461) | 4.491897 (1.165484) | 3.564420 (1.187326) | 2.324992 | 6.778 |
2^26 | 7.209091 (0.120798) | 9.126683 (2.324794) | 7.189370 (2.347179) | 4.747451 | 13.786 |
2^27 | 14.995244 (0.445046) | 18.602658 (4.632169) | 14.542401 (4.674381) | 9.453237 | 28.568 |


**Speed Up**

N | 1M- 4 procesos | 2M - 4 procesos | 2M - 8 procesos | 4 PThread |Secuencial
--- | --- | --- | --- | ---|---|
2^25 | 1.9112 (1.9405) | 1.5089 (2.0376) | 1.9015 (2.3770) | 2.96344 | 1 |
2^26 | 1.9123 (1.9448) | 1.5100 (2.0267) | 1.9175 (2.8470) | 3.09597 | 1 |
2^27 | 1.9051 (1.9634) | 1.5356 (2.0448) | 1.9644 (2.8950) | 3.13488 | 1 |

**Eficiencia**  

N | 1M- 4 procesos | 2M - 4 procesos | 2M - 8 procesos | 4 PThread |Secuencial
--- | --- | --- | --- | --- | --- |
2^25 | 0.4778 (0.4851) | 0.3772 (0.5094) | 0.2376 (0.3564) | 0.74086 | 1 |
2^26 | 0.4780 (0.4862) | 0.3776 (0.5066) | 0.2396 (0.3558) | 0.77399 | 1 |
2^27 | 0.4762 (0.4908) | 0.3839 (0.5112) | 0.2455 (0.3618) | 0.78372 | 1 |

### Ejercicio 3


(comunicaciones)  

N | 1M- 4 procesos | 2M - 4 procesos | 2M - 8 procesos | Secuencial
--- | --- | --- | --- | --- |
2^25 | 4.759722 (0.151055) | 7.142621 (2.750442) | 5.802915 (2.778128) | 11.580526
2^26 | 9.915964 (0.242992) | 14.508592 (5.491159) | 11.738293 (5.539755) | 24.503229
2^27 | 20.281398 (0.530355) | 29.511291 (10.957680) | 23.745724 (11.044880) | 50.802676


**Speed Up**

N | 1M- 4 procesos | 2M - 4 procesos | 2M - 8 procesos | Secuencial
--- | --- | --- | --- | ---
2^25 | --- | --- | --- | ---
2^26 | --- | --- | --- | ---
2^27 | --- | --- | --- | ---


**Eficiencia**  

N | 1M- 4 procesos | 2M - 4 procesos | 2M - 8 procesos | Secuencial
--- | --- | --- | --- | --- |
2^25 | --- | --- | --- | ---
2^26 | --- | --- | --- | ---
2^27 | --- | --- | --- | ---
