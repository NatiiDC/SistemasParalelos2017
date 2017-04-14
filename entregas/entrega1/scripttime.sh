#!/bin/bash

echo "Normal"
echo "./entregatp1a 256 - 50 iteraciones"
echo `./entregatp1a 256 50`

echo
echo "En bloque"
echo "./entregatp1b 16 16 - (256) 50 iteraciones"
echo `./entregatp1b 16 16 50`

echo
echo "./entregatp1b 8 32 - (256) 50 iteraciones"
echo `./entregatp1b 8 32 50`

echo
echo "./entregatp1b 32 8 - (256) 50 iteraciones"
echo `./entregatp1b 32 8 50`

echo
echo "Normal"
echo "./entregatp1a 512 - 50 iteraciones"
echo `./entregatp1a 512 50`

echo
echo "En bloque"
echo "./entregatp1b 32 16 - (512) 50 iteraciones"
echo `./entregatp1b 32 16 50`

echo
echo "./entregatp1b 16 32 - (512) 50 iteraciones"
echo `./entregatp1b 16 32 50`

echo
echo "./entregatp1b 8 64 - (512) 50 iteraciones"
echo `./entregatp1b 8 64 50`

echo
echo "Normal"
echo "./entregatp1a 1024 - 10 iteraciones"
echo `./entregatp1a 1024 10`

echo
echo "En bloque"
echo "./entregatp1b 32 32 - (1024) 10 iteraciones"
echo `./entregatp1b 32 32 10`

echo
echo "./entregatp1b 16 64 - (1024) 10 iteraciones"
echo `./entregatp1b 16 64 10`

echo
echo "./entregatp1b 64 16 - (1024) 10 iteraciones"
echo `./entregatp1b 64 16 10`

#
# Normal
# ./entregatp1a 256 - 50 iteraciones
# Tiempo promedio para 50 iteracion/es: 0.092156
#
# En bloque
# ./entregatp1b 16 16 - (256) 50 iteraciones
# Tiempo promedio para 50 iteracion/es: 0.101275
#
# ./entregatp1b 8 32 - (256) 50 iteraciones
# Tiempo promedio para 50 iteracion/es: 0.104119
#
# ./entregatp1b 32 8 - (256) 50 iteraciones
# Tiempo promedio para 50 iteracion/es: 0.118518
#
# Normal
# ./entregatp1a 512 - 50 iteraciones
# Tiempo promedio para 50 iteracion/es: 0.812397
#
# En bloque
# ./entregatp1b 32 16 - (512) 50 iteraciones
# Tiempo promedio para 50 iteracion/es: 0.828914
#
# ./entregatp1b 16 32 - (512) 50 iteraciones
# Tiempo promedio para 50 iteracion/es: 0.833180
# 
# ./entregatp1b 8 64 50 - (512) 50 iteraciones
# Tiempo promedio para 50 iteracion/es: 0.716775
#
#
# Normal
# ./entregatp1a 1024 - 10 iteraciones
# Tiempo promedio para 10 iteracion/es: 7.691725
#
# En bloque
# ./entregatp1b 32 32 - (1024) 10 iteraciones
# Tiempo promedio para 10 iteracion/es: 6.533485
#
# ./entregatp1b 16 64 - (1024) 10 iteraciones
# Tiempo promedio para 10 iteracion/es: 6.158147
#
# ./entregatp1b 64 16 - (1024) 10 iteraciones
# Tiempo promedio para 10 iteracion/es: 6.611950
