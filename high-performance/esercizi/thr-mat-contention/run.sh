#!/bin/bash

# Output CSV file
output_csv="output.csv"

# Header for the CSV file
echo "n,m,c,clock_cycles" > "$output_csv"

# Define the inputs you want to test
inputs=(
    "128 128 1"
    "256 256 1"
    "512 512 1"
    "1024 1024 1"
    "2048 2048 1"
    "4096 4096 1"
    "8192 8192 1"
    "128 128 0"
    "256 256 0"
    "512 512 0"
    "1024 1024 0"
    "2048 2048 0"
    "4096 4096 0"
    "8192 8192 0"
)

# Number of times to run the binary
num_runs=5

# Path to your 
binary="./a.out"

# Loop through the inputs
for input in "${inputs[@]}"; do
    nmc=($input)
    n="${nmc[0]}"
    m="${nmc[1]}"
    c="${nmc[2]}"
    
    for ((i=1; i<=$num_runs; i++)); do
        # Run the binary and capture the clock cycles
        clock_cycles=$("$binary" "$n" "$m" "$c")
        
        # Append the results to the CSV file
        echo "$n,$m,$c,$clock_cycles" >> "$output_csv"
    done
done

echo "Data collection complete. Results are stored in $output_csv"
