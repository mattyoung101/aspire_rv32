#include <stdint.h>
#include "aspire.h"

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
"								                                 \n" \
"								                                 \n" \
"      * (c) 2024, 2025 Matt Young. University of Queensland. *  \n" \

#define TIMEOUT (5) // Timeout in seconds to re-print message

#define xstr(a) str(a)
#define str(a) #a

int main(void) {
    aspire_uart_reset();
    aspire_watchdog_set(false);

    while (1) {
        aspire_uart_puts(ASCII_ART);
        aspire_uart_puts("\n\nAspire is a RISC-V RV32IC_Zmmul_Zicsr CPU.\n");
        aspire_uart_puts("This message will repeat in " xstr(TIMEOUT) " seconds.\n\n");

        uint64_t begin = aspire_read_cycles();
        // block while the difference between current time and begin time is
        // less than TIMEOUT seconds
        while (aspire_read_cycles() - begin <= (F_CPU * TIMEOUT));
    }

    return 0;
}
