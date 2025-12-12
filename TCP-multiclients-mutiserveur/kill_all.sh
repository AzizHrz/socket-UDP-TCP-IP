#!/bin/bash

if [ ! -f server_pids.txt ]; then
    echo "No PID file found (server_pids.txt). Nothing to kill."
    exit 1
fi

echo "Stopping servers..."
while read pid; do
    if kill $pid 2>/dev/null; then
        echo "Killed PID $pid"
    else
        echo "PID $pid already stopped"
    fi
done < server_pids.txt

rm -f server_pids.txt

echo "All services terminated."
