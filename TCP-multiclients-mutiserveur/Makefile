# ===============================
#   MULTISERVICE TCP SYSTEM
# ===============================

CC      = gcc
CFLAGS  = -Wall -Wextra -O2
LDFLAGS = -pthread

SRC_DIR = .
BIN_DIR = bin

# Programs
MAIN_SERVER    = $(BIN_DIR)/main_server
CLIENT         = $(BIN_DIR)/client

LISTS_DIR_SRV  = $(BIN_DIR)/lists_dir_server
CAT_DIR_SRV    = $(BIN_DIR)/cat_dir_server
ELAPSED_SRV    = $(BIN_DIR)/elapsed_server
DATETIME_SRV   = $(BIN_DIR)/datetime_server

# Source files
MAIN_SERVER_SRC    = main_server.c
CLIENT_SRC         = client.c

LISTS_DIR_SRC      = lists_dir_server.c
CAT_DIR_SRC        = cat_dir_server.c
ELAPSED_SRC        = elapsed_server.c
DATETIME_SRC       = datetime_server.c

# ===============================
#  Targets
# ===============================

all: prep $(MAIN_SERVER) \
           $(LISTS_DIR_SRV) \
           $(CAT_DIR_SRV) \
           $(ELAPSED_SRV) \
           $(DATETIME_SRV) \
           $(CLIENT)

prep:
	@mkdir -p $(BIN_DIR)

$(MAIN_SERVER): $(MAIN_SERVER_SRC) ports.h network.h
	$(CC) $(CFLAGS) $(MAIN_SERVER_SRC) -o $(MAIN_SERVER) $(LDFLAGS)

$(CLIENT): $(CLIENT_SRC) ports.h network.h
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o $(CLIENT) $(LDFLAGS)

$(LISTS_DIR_SRV): $(LISTS_DIR_SRC) ports.h network.h
	$(CC) $(CFLAGS) $(LISTS_DIR_SRC) -o $(LISTS_DIR_SRV)

$(CAT_DIR_SRV): $(CAT_DIR_SRC) ports.h network.h
	$(CC) $(CFLAGS) $(CAT_DIR_SRC) -o $(CAT_DIR_SRV)

$(ELAPSED_SRV): $(ELAPSED_SRC) ports.h network.h
	$(CC) $(CFLAGS) $(ELAPSED_SRC) -o $(ELAPSED_SRV)

$(DATETIME_SRV): $(DATETIME_SRC) ports.h network.h
	$(CC) $(CFLAGS) $(DATETIME_SRC) -o $(DATETIME_SRV)

# ===============================
#  Clean
# ===============================
clean:
	rm -rf $(BIN_DIR)

# ===============================
#  Run system (optional helpers)
# ===============================

run-main:
	$(MAIN_SERVER)

run-client:
	$(CLIENT)

run-listdir:
	$(LISTS_DIR_SRV)

run-cat:
	$(CAT_DIR_SRV)

run-elapsed:
	$(ELAPSED_SRV)

run-datetime:
	$(DATETIME_SRV)
