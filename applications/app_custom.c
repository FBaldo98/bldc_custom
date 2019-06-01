#include "conf_general.h"
#include "ch.h"
#include "hal.h"
#include "mc_interface.h"
#include "hw.h"
#include "timeout.h"
#include "matrix.h"
#include "luenberger_observer.h"

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

static THD_FUNCTION(ps_thread, arg){
    (void) arg;

    chRegSetThreadName("PS_CONTROL");

    luenberger_matrices_t* matrices = (luenberger_matrices_t*)malloc(sizeof(luenberger_matrices_t));

    InitLuembergerMatrices(matrices);

    // Defines the max difference between the 2 position sensors
    float max_pos_diff = 100;

    // Init  input matrix
    // Define size
    matrix_t* input;
    InitMatrix(input, 2, 1, 0.0);

    // Init variables here
    // eg. Init observer matrices, initial states, etc..
    LuenbergerInit();

    for(;;){
        // Infinite loop
        // Control here

        // ADC read
        float steering_pos1 = (float)ADC_Value[ADC_IND_EXT];
        float steering_pos2 = (float)ADC_Value[ADC_IND_EXT2];

        if(abs(steering_pos1 - steering_pos2) >= max_pos_diff){
            // Call fault function
            break;
        }

        // Read extensimeter from CAN
        float extensimeter = 0.0;

        // Create matrices for the observer input

        // Luenberger step here

        // Set current
        // mc_interface_set_current(2.0);

        // Run this loop at 500Hz
        // Probably we won't need this delay
		chThdSleepMilliseconds(2);
 
        // Send data via can
        // comm_can_send_buffer(uint8_t controller_id, uint8_t *data, unsigned int len, uint8_t send);

		// Reset the timeout
		timeout_reset();

    }
}
