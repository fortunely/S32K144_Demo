/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


#include "S32K144.h" /* include peripheral declarations S32K144 */

int main(void)
{
#define COUNTER_LIMIT 100

        int counter = 0;

        for(;;) {       
            counter++;

            if(counter > COUNTER_LIMIT) {
                counter = 0;
            }
        }

	return 0;
}
