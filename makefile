# compiler
CC 		= gcc
CFLAGS 	= -I inc
LDFLAGS = -lbcm_host
OPTI 	= -Ofast
ARGS	= $(CFLAGS) $(LDFLAGS) $(OPTI)

# dossiers et liste des fichiers
SRC_DIR = src
ASM_DIR = asm
OBJ_DIR = obj
SRC 	= $(wildcard $(SRC_DIR)/*.c)
ASM 	= $(patsubst $(SRC_DIR)/%.c, $(ASM_DIR)/%.s, $(SRC))
OBJ 	= $(patsubst $(ASM_DIR)/%.s, $(OBJ_DIR)/%.o, $(ASM))

# executable
EXEC 	= piSMI

all: $(EXEC)
	@echo "compilation succesful"

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(ARGS)

$(OBJ_DIR)/%.o: $(ASM_DIR)/%.s
	$(CC) -C -o $@ -c $^ $(ARGS)

$(ASM_DIR)/%.s: $(SRC_DIR)/%.c
	$(CC) -S -o $@ -c $^ $(ARGS)

# CLEAR
clear: clear_asm clear_obj
	rm -f $(EXEC)

clear_obj:
	rm -rf $(OBJ_DIR)/*

clear_asm:
	rm -rf $(ASM_DIR)/*