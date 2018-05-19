/*
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Contributors: Alex Tereschenko <alext.mkrs@gmail.com>
 * Contributors: Manivannan Sadhasivam <manivannan.sadhasivam@linaro.org>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Example usage: Toggles GPIO's 23 and 24 recursively. Press Ctrl+C to exit
 *
 */


/* standard headers */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* mraa header */
#include "mraa/gpio.h"



volatile sig_atomic_t flag = 1;

void
sig_handler(int signum)
{
    if (signum == SIGINT) {
        fprintf(stdout, "Exiting...");
        flag = 0;
    }
}

int blink(int pin)
{
    mraa_result_t status = MRAA_SUCCESS;
    mraa_gpio_context gpio_1, gpio_2;

    /* install signal handler */
    signal(SIGINT, sig_handler);


    /* initialize mraa for the platform (not needed most of the times) */
    mraa_init();

    gpio_1 = mraa_gpio_init(pin);
    if (gpio_1 == NULL) {
       // fprintf(stderr, "Failed to initialize GPIO %d", GPIO_PIN_1);
        mraa_deinit();
        return EXIT_FAILURE;
    }

    /* set GPIO to output */
    status = mraa_gpio_dir(gpio_1, MRAA_GPIO_OUT);
    if (status != MRAA_SUCCESS) {
        goto err_exit;
    }

    while (flag) {
        status = mraa_gpio_write(gpio_1, 1);
        if (status != MRAA_SUCCESS) {
            goto err_exit;
        }

       
        sleep(1);

        status = mraa_gpio_write(gpio_1, 0);
        if (status != MRAA_SUCCESS) {
            goto err_exit;
        }

        
        sleep(1);
    }

    status = mraa_gpio_close(gpio_1);
    if (status != MRAA_SUCCESS) {
        goto err_exit;
    }

    mraa_deinit();

    return EXIT_SUCCESS;

    err_exit:
    mraa_result_print(status);

    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();

    return EXIT_FAILURE;

}

int main(void)
{
} 
