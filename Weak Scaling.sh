cd ~/hpc-week1/simple/07_performance
(
    set -e
    for p in 1 2 4; do
        rows=$((4096 * p))
        echo "=== warmup: workers=$p rows=$rows ==="
        ./matrix "$rows" "$p" 256

        for r in 1 2 3; do
            echo "=== measured: workers=$p rows=$rows run=$r ==="
            ./matrix "$rows" "$p" 256
        done
    done
) | tee 07_weak_scaling.txt