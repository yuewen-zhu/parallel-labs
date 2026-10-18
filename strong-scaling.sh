cd ~/hpc-week1/simple/07_performance
g++ -std=c++17 -O2 -Wall -Wextra -pthread matrix.cpp -o matrix
(
    set -e
    for p in 0 1 2 4; do
        echo "=== warmup: workers=$p ==="
        ./matrix 4096 "$p" 256

        for r in 1 2 3; do
            echo "=== measured: workers=$p run=$r ==="
            ./matrix 4096 "$p" 256
        done
    done
) | tee 07_strong_scaling.txt