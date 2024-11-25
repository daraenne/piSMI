# compiler
CC 		= gcc
CFLAGS 	= -I inc -Wall
LDFLAGS = -lbcm_host
OPTI 	= -Ofast
ARGS	= $(CFLAGS) $(OPTI)

# dossiers et liste des fichiers
SRC_DIR = src
ASM_DIR = asm
# .PRECIOUS: $(ASM_DIR)/%.s # decommenter cette ligne si on veut garder les fichiers asm
OBJ_DIR = obj
SRC 	= $(wildcard $(SRC_DIR)/*.c)
ASM 	= $(patsubst $(SRC_DIR)/%.c, $(ASM_DIR)/%.s, $(SRC))
OBJ 	= $(patsubst $(ASM_DIR)/%.s, $(OBJ_DIR)/%.o, $(ASM))

# executable
EXEC 	= piSMI

all: $(EXEC)
	@echo "compilation succesful"

$(EXEC): $(OBJ)
	$(CC) $^ -o $@ $(ARGS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(ASM_DIR)/%.s | $(OBJ_DIR)
	$(CC) -c $^ -o $@ $(ARGS)

$(ASM_DIR)/%.s: $(SRC_DIR)/%.c | $(ASM_DIR)
	$(CC) -S -c $^ -o $@ $(ARGS)

# Création des répertoires si nécessaire
$(OBJ_DIR) $(ASM_DIR):
	mkdir -p $@

# CLEAR
clean: clear

clear: clear_asm clear_obj
	rm -f $(EXEC)

clear_obj:
	rm -rf $(OBJ_DIR)/*

clear_asm:
	rm -rf $(ASM_DIR)/*
