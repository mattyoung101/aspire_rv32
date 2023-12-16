#include <stdio.h>
#include <stdint.h>

// Font Name: Caligraphy2
#define ASCII_ART \
"        ##                                                      \n" \
"     /####                              #                       \n" \
"    /  ###                             ###                      \n" \
"       /##                              #                       \n" \
"      /  ##                                                     \n" \
"      /  ##          /###      /###   ###   ###  /###     /##   \n" \
"     /    ##        / #### /  / ###  / ###   ###/ #### / / ###  \n" \
"     /    ##       ##  ###/  /   ###/   ##    ##   ###/ /   ### \n" \
"    /      ##     ####      ##    ##    ##    ##       ##    ###\n" \
"    /########       ###     ##    ##    ##    ##       ######## \n" \
"   /        ##        ###   ##    ##    ##    ##       #######  \n" \
"   #        ##          ### ##    ##    ##    ##       ##       \n" \
"  /####      ##    /###  ## ##    ##    ##    ##       ####    /\n" \
" /   ####    ## / / #### /  #######     ### / ###       ######/ \n" \
"/     ##      #/     ###/   ######       ##/   ###       #####  \n" \
"#                           ##                                  \n" \
" ##                         ##                                  \n" \
"                            ##                                  \n" \
"                             ##                                 \n" \
"                                                                \n" \
"                     ~ THE DESIRE TO SUCCEED ~                  \n" \
"																 \n" \
"																 \n" \
"      * (c) 2024, 2025 Matt Young. University of Queensland. *  \n" \
"      * Available under the (LICENCE HERE).                  *  \n" \

#define F_CPU (20000000) // CPU freq in Hz (estimated 20 MHz, TODO real frequency)

#define TIMEOUT (5) // Timeout in seconds to re-print message

// Based on https://github.com/ucb-bar/chipyard/issues/7#issuecomment-297475957
// and https://stackoverflow.com/a/2769598/5007892
// but for RV32I
uint64_t read_cycles(void) {
    uint32_t cycles_low;
	uint32_t cycles_high;
    asm volatile ("rdcycle %0" : "=r" (cycles_low)); // cycle[0:31]
    asm volatile ("rdcycleh %0" : "=r" (cycles_high)); // cycle[32:63]
	// https://stackoverflow.com/a/2769598/5007892
    return (uint64_t) cycles_high << 32 | cycles_low;
}

int main(void) {
	while (1) {
		printf(ASCII_ART);
		printf("\n\nAspire is a RISC-V RV32IC_Zmmul_Zicsr thesis project.\n");
		printf("This message will repeat in %d seconds.\n\n", TIMEOUT);
		
		uint64_t begin = read_cycles();
		// block while the difference between current time and begin time is
		// less than TIMEOUT seconds
		while (read_cycles() - begin <= (F_CPU * TIMEOUT));
	}
	return 0;
}
