#!/bin/bash

BIN=./bin

tmux new-session -d  "$BIN/main_server" \; \
     split-window -h "$BIN/lists_dir_server" \; \
     split-window -v "$BIN/cat_dir_server" \; \
     select-pane -t 0 \; \
     split-window -v "$BIN/elapsed_server" \; \
     select-pane -t 3 \; \
     split-window -v "$BIN/datetime_server" \; \
     select-pane -t 5 \; \
     split-window -v "$BIN/client" \; \
     attach
