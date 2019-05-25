#include "conf_general.h"
#include "ch.h"
#include "hal.h"
#include "mc_interface.h"
#include "hw.h"
#include "timeout.h"


#ifdef APP_CUSTOM_TO_USE
#include APP_CUSTOM_TO_USE
#endif

// Initialize a new thread
static THD_FUNCTION(ps_thread, arg);
static THD_WORKING_AREA(ps_thread_wa, 4096); // 4kb stack

void powersteering_init(void) {
    // Set I/O Pins here
    // Example. Set the UART TX pin as an input with pulldown
	// palSetPadMode(HW_UART_TX_PORT, HW_UART_TX_PIN, PAL_MODE_INPUT_PULLDOWN);

    // Start the thread
    chThdCreateStatic(ps_thread_wa, sizeof(ps_thread_wa), NORMALPRIO, ps_thread, NULL);
}

void LuenbergerInit(){

}

void LuenbergerStep(){

}

static THD_FUNCTION(ps_thread, arg){
    (void) arg;

    chRegSetThreadName("PS_CONTROL");

    // Init variables here
    // eg. Init observer matrices, initial states, etc..
    LuenbergerInit();

    for(;;){
        // Infinite loop
        // Control here

        // Example ADC read
        // float value = (float)ADC_Value[ADC_IND_EXT];
        LuenbergerStep();

        // Set current
        // mc_interface_set_current(2.0);

        // Run this loop at 500Hz
        // Probably we won't need this delay
		chThdSleepMilliseconds(2);
 
		// Reset the timeout
		timeout_reset();

    }
}
