#!/bin/bash 
 
# Specifica il nome del file CSV di destinazione 
csv_file=$1 
 
# Intestazione del file CSV
headers=$(awk -F'[%:]' '{print $1}' /proc/meminfo | paste -sd ",")
filter=$(awk -F'[%:]' '{print $1}' /proc/meminfo | paste -sd "|")

echo "Timestamp,"$headers > $csv_file

for i in {1..16}
do
    ./nbody/launch_nbody.sh -r 1000 -n 500000 > "data/${i}" &
done

# Loop infinito per leggere i dati ogni 2 secondi 
for i in {1..600} 
do 
    timestamp=$(date +%s) 
    meminfo=$(cat /proc/meminfo | grep -E $filter | awk '{print $2}') 
     
    # Salva i dati nel file CSV 
    echo "$timestamp,$meminfo" | tr '\n' ',' | sed 's/,$/\n/' >> "$csv_file" 
     
    # Attendiamo 1 secondo
    sleep 1
done

killall nbodySim
