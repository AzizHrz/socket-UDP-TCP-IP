#!/bin/bash

BIN=./bin

echo "======================================="
echo "  STARTING MULTI-SERVICE TCP SERVERS"
echo "======================================="

# Run each server in background with logs
$BIN/lists_dir_server   > logs_lists_dir.txt   2>&1 &
PID_LIST=$!

$BIN/cat_dir_server     > logs_cat_dir.txt     2>&1 &
PID_CAT=$!

$BIN/elapsed_server     > logs_elapsed.txt     2>&1 &
PID_ELAPSED=$!

$BIN/datetime_server    > logs_datetime.txt    2>&1 &
PID_DATETIME=$!

$BIN/main_server        > logs_main.txt        2>&1 &
PID_MAIN=$!

echo "Servers running with PIDs:"
echo "  LIST_DIR      : $PID_LIST"
echo "  CAT_DIR       : $PID_CAT"
echo "  ELAPSED_TIME  : $PID_ELAPSED"
echo "  DATETIME      : $PID_DATETIME"
echo "  MAIN_SERVER   : $PID_MAIN"

echo "Logs available in local directory."

# Save PIDs for safe shutdown
echo $PID_LIST     >  server_pids.txt
echo $PID_CAT      >> server_pids.txt
echo $PID_ELAPSED  >> server_pids.txt
echo $PID_DATETIME >> server_pids.txt
echo $PID_MAIN     >> server_pids.txt

echo "server_pids.txt created."
