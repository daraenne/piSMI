all:
	gcc -o bareGpio src/*.c -I inc -lbcm_host -Ofast

asm:
	gcc -S src/main.c -o out/main.s -I inc -lbcm_host
	gcc -S src/dma.c -o out/dma.s -I inc -lbcm_host
	gcc -S src/gpio.c -o out/gpio.s -I inc -lbcm_host
	gcc -S src/smi.c -o out/smi.s -I inc -lbcm_host

clear:
	rm -rf bareGpio *.s