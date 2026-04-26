#!/usr/bin/env bash

echo "compilando..."
g++ main.cpp -o pinza #compila el main.cpp y genera un ejecutable llamado pinza
echo "ejecutando..." #mensaje
./pinza & #ejecuta el programa en segundo plano(el & permite seguir ejecutando el script)
PID=$! #guarda el ID del proceso que se ha lanzado en segundo plano
echo "PID: $PID" #muestra el identificador del proceso en pantalla
wait $PID #espera que el proceso termine antes de continuar con el script
if ps -p $PID > /dev/null #comprueba si el proceso sigue activo (ps lista procesos y -p busca por PID)
then
    echo "sigue activo, terminando..."
    kill $PID #finaliza el proceso manualmente con PID
else
    echo "finalizado correctamente"
fi
#&:ejecuta en segundo plano
#$!: guarda el PID del ultimo proceso lanzado
#wait: espera a que termine
#ps -p: comprueba si sigue activo
#kill finaliza el proceso
