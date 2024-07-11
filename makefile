# compiler
CC 		= gcc
LDFLAGS = -I inc -lbcm_host
OPTI 	= -Ofast

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

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(OPTI)

$(OBJ_DIR)/%.o: $(ASM_DIR)/%.s
	$(CC) -o $@ -c $^

$(ASM_DIR)/%.s: $(SRC_DIR)/%.c
	$(CC) -S $@ -c $^ $(LDFLAGS) $(OPTI)

# CLEAR
clear: clear_asm clear_obj
	rm -f piSMI

clear_obj:
	rm -rf $(OBJ_DIR)/*

clear_asm:
	rm -rf $(ASM_DIR)/*