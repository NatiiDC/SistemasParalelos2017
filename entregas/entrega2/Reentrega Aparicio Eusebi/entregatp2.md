# Entrega Nro. 2
## Programación con memoria compartida

#### Alumnos:
* Aparicio Natalia Elizabeth - 12667/7
* Eusebi Cirano - 12469/2

---

### Ejercicio 1

#### OpenMP

N | Secuencial | 2 THREADS | 4 THREADS
--- | --- | --- | ---
512 | 1.9203 | 1.0133 | 0.5615
1024 | 15.5681 | 8.3661 | 4,5790
2048 | 126,1329 | 66,7107 | 37,0839

**Speed Up**

N | 2 THREADS | 4 THREADS |
--- | --- | ---
512 | 1,8951 | 3,4198
1024 | 1,8608 | 3,3998
2048 | 1,8907 | 3,4012

**Eficiencia**  

N | 2 THREADS | 4 THREADS |
--- | --- | ---
512 | 0,9475 | 0,8549
1024 | 0,9304 | 0,8499
2048 | 0,9453 | 0,8503


#### PThread

N | Secuencial | 2 THREADS | 4 THREADS
--- | --- | --- | ---
512 | 1.9203 | 1.0094 | 0.5382
1024 | 15.5681 | 7.8027 | 4.2264
2048 | 126.1329 | 61.3400 | 32.5148

**Speed Up**

N | 2 THREADS | 4 THREADS |
--- | --- | ---
512 | 1.9023 | 3.5678
1024 | 1.9952 | 3.6835
2048 | 2.0562 | 3.8692

**Eficiencia**  

N | 2 THREADS | 4 THREADS |
--- | --- | ---
512 | 0.9511 | 0.8919
1024 | 0.9976 | 0.9208
2048 | 1.0281 | 0.9698

  ---

### Ejercicio 2

|N|Sequencial|2 THREADS|4 THREADS|
|---|---|---|---|
|2^25|6.778|3.753|2.159|
|2^26|13.786|7.608|4.353|
|2^27|28.568|15.923|8.989|

**Speed Up**

|N|Sequencial|2 THREADS|4 THREADS|
|---|---|---|---|
|2^25|1|1.806|3.139|
|2^26|1|1.812|3.167|
|2^27|1|1.794|3.178|

**Eficiencia**

|N|Sequencial|2 THREADS|4 THREADS|
|---|---|---|---|
|2^25|1|0.903|0.784|
|2^26|1|0.906|0.791|
|2^27|1|0.897|0.794|

---
### Ejercicio 2 Reentrega

|N|Sequencial|2 THREADS|4 THREADS|
|---|---|---|---|
|2^25|6.889977|3.697697|2.324992|
|2^26|14.698010|7.727129|4.747451|
|2^27|29.634855|15.699277|9.453237|

**Speed Up**

|N|Sequencial|2 THREADS|4 THREADS|
|---|---|---|---|
|2^25|1|1.86331|2.96344|
|2^26|1|1.90213|3.09597|
|2^27|1|1.88765|3.13488|

**Eficiencia**

|N|Sequencial|2 THREADS|4 THREADS|
|---|---|---|---|
|2^25|1|0.93165|0.74086|
|2^26|1|0.95106|0.77399|
|2^27|1|0.94382|0.78372|

---

### Ejercicio 3

|N|Sequencial|2 THREADS|4 THREADS|
|---|---|---|---|
|2^12|0.0565|0.0616|0.0344|
|2^13|0.2543|0.2562|0.1389|
|2^14|1.1361|1.2134|0.7158|

**Speed Up**

|N|Sequencial|2 THREADS|4 THREADS|
|---|---|---|---|
|2^12|1|0.9172|1.6424|
|2^13|1|0.9925|1.8308|
|2^14|1|0.9362|1.5871

**Eficiencia**

|N|Sequencial|2 THREADS|4 THREADS|
|---|---|---|---|
|2^12|1|0.4586|0.4106|
|2^13|1|0.4962|0.4577|
|2^14|1|0.4681|0.3979|

---
### Ejercicio 3 Reentrega

|N|Sequencial Backwards|2 THREADS Dynamic 1 Backwards|2 THREADS Dynamic 1 Forward|2 THREADS Guided Forward|4 THREADS Dynamic 1 Backwards|4 THREADS Dynamic 1 Forward|4 THREADS Guided Forward|
|---|---|---|---|---|---|---|---|---|
|2^12|0.146345|0.101148|0.084957|0.079655|0.045641|0.046350|0.048586|
|2^13|0.586539|0.394327|0.331745|0.325103|0.196426|0.196140|0.231349|
|2^14|2.598257|1.442356|1.404302|1.447472|0.842029|0.834679|1.014138|

**Speed Up**

|N|Sequencial Backwards|2 THREADS Dynamic 1 Backwards|2 THREADS Dynamic 1 Forward|2 THREADS Guided Forward|4 THREADS Dynamic 1 Backwards|4 THREADS Dynamic 1 Forward|4 THREADS Guided Forward|
|---|---|---|---|---|---|---|---|---|
|2^12|1|1.446840|1.722577|1.837223|3.206437|3.157389|3.012081|
|2^13|1|1.487443|1.768041|1.804163|2.986055|2.990499|2.535299|
|2^14|1|1.801397|1.850212|1.795030|3.085709|3.112881|2.562034|

**Eficiencia**

|N|Sequencial Backwards|2 THREADS Dynamic 1 Backwards|2 THREADS Dynamic 1 Forward|2 THREADS Guided Forward|4 THREADS Dynamic 1 Backwards|4 THREADS Dynamic 1 Forward|4 THREADS Guided Forward|
|---|---|---|---|---|---|---|---|---|
|2^12|1|0.723420|0.861288|0.918617|0.801609|0.789347|0.753020|
|2^13|1|0.743721|0.884020|0.902081|0.746513|0.747602|0.633824|
|2^14|1|0.900698|0.925106|0.897515|0.771427|0.778220|0.640587|

---

### Ejercicio 4

|N|Sequencial|2 THREADS|4 THREADS|
|---|---|---|---|
|2^25|0.0775|0.0300|0.0167|
|2^26|0.1528|0.0594|0.0326|
|2^27|0.3118|0.1138|0.0632|

**Speed Up**

|N|Sequencial|2 THREADS|4 THREADS|
|---|---|---|---|
|2^25|1|2.5833|4.6407
|2^26|1|2.5723|4.6871
|2^27|1|2.7398|4.9335

**Eficiencia**

|N|Sequencial|2 THREADS|4 THREADS|
|---|---|---|---|
|2^25|1|1.2916|1.1601
|2^26|1|1.2861|1.1717
|2^27|1|1.3699|1.23335
