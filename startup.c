#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sidata, _sdata, _edata;
extern uint32_t _sbss, _ebss;

//C/C++ has external linkage on function declarations by default
int main (void);
void Reset_Handler(void);

// 
void Default_Handler(void){
    while(1){
    }
};

void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak,alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak,alias("Default_Handler")));
void SVCall_Handler(void) __attribute__((weak,alias("Default_Handler")));
void DebugMonitor_Handler(void) __attribute__((weak,alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak,alias("Default_Handler")));
void Systick_Handler(void) __attribute__((weak,alias("Default_Handler")));

//points to the address of the stack pointer (the top of the stack) and handler functions
__attribute__ ((section(".isr_vector"),used))
const uint32_t vector_table[] = {
    (uint32_t) &_estack,
    (uint32_t) Reset_Handler,
    (uint32_t) NMI_Handler,
    (uint32_t) HardFault_Handler,
    (uint32_t) MemManage_Handler,
    (uint32_t) BusFault_Handler,
    (uint32_t) UsageFault_Handler,
    0, 0, 0, 0,
    (uint32_t) SVCall_Handler,
    (uint32_t) DebugMonitor_Handler,
    0,
    (uint32_t) PendSV_Handler,
    (uint32_t) Systick_Handler
};

void Reset_Handler(void){
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata){
        *dst = *src;
        dst++;
        src++;
    }

    dst  = &_sbss;
    
    while (dst < &_ebss){
        *dst = 0;
        dst++;
    }

    main();

    // main should never return; if it does, spin here so the failure is visible
    while (1){}
}




