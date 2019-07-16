/**
* @filename main.c
* @author Martin
* @brief Application Entry source file
* @date 2019-07-09
* @version 0.1
* @note initial draft
*/

#include "Task.h"
#include "Init.h"

/**
 * main function for application entry
 * @return application execute status.
 * @note The function will never return if the application runs normally.
 */
int main(void)
{
	Init();
	Task_MainCycle();
	return 0;
}
