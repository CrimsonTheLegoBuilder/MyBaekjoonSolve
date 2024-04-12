//#include <Arduino_FreeRTOS.h>
//#include <semphr.h>
extern int __bss_end;
extern void* __brkval;

int memoryPrint() {
	int freeMemory;
	if ((int)__brkval == 0) freeMemory = ((int)&freeMemory) - ((int)&__bss_end);
	else freeMemory = ((int)&freeMemory) - ((int)&__brkval);
	return freeMemory;
}