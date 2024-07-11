#include "gpio.h"
#include "smi.h"
#include "main.h"

int main(int argc, char *argv[]){
	if(!init_gpio()){
		printf("init Gpio failed\r\n");
		return EXIT_FAILURE;
	}
	if(!init_smi_clk()){
		printf("init SMI et Clocks failed\r\n");
		return EXIT_FAILURE;
	}
	
	if (!setup_smi()){ 
		printf("setup_smi failed\r\n");
		return EXIT_FAILURE;
	}

	uint16_t count[0xFF];
	for(uint8_t i = 0; i<0xFF;i++) {
		count[i] = i;
	}
	
	while(1){
		set_smi_timing(0, 0, 2, 2, 2, 1);
		smi_block_write(0, 0xFF, count, 0xFF, 500);
		// sleep(1);
	}

	// clear warning in unused case of args
	(void) argc;
	(void) argv;
	return EXIT_SUCCESS;
}

// Get virtual memory segment for peripheral regs or physical mem
void *map_segment(void *addr, uint32_t size){
    int fd;
    void *mem;

	fd = open("/dev/mem", O_RDWR|O_SYNC|O_CLOEXEC);
    if (fd < 0){
		printf("Error: can't open /dev/mem, run using sudo\n");
		return NULL;
	}

    mem = (uint32_t*) mmap(0, size, PROT_WRITE|PROT_READ, MAP_SHARED, fd, (uint32_t)addr);
	if(mem == MAP_FAILED){
		printf("mmap Failed");
		return NULL;
	}

    close(fd);
    return(mem);
}