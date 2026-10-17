The reason for each one specifically, and whether it can fire in our first blink program:

Exception	What it is	What triggers it	Can it fire in our blink program?
NMI	Non-maskable interrupt: software can’t switch it off On STM32, mainly the clock security system detecting an external oscillator failure Only if you enable that feature, so no
HardFault	The catch-all fatal error	Bad memory access, invalid instruction, or any other fault that has no handler enabled	Yes: any bug can cause it
MemManage	Memory-protection violation	The optional memory protection unit (MPU) catching a forbidden access	No: it’s off by default, and a fault here escalates to HardFault
BusFault	A bus access that failed	Reading or writing an address where nothing exists	Off by default, so it escalates to HardFault
UsageFault	Programmer error	Undefined instruction, or jumping to an address with the Thumb bit cleared (remember the odd reset address)	Off by default, so it escalates to HardFault
SVC	Supervisor call	The svc instruction, used by operating systems to request services	Only if code executes that instruction, so no
DebugMon	Debug monitor event	Only used for a specific “monitor mode” of debugging	No: OpenOCD uses hardware debug instead
PendSV	Pended system call	Software setting it pending, used by operating systems to switch tasks	No, not without an RTOS
SysTick	The core’s built-in timer	The timer counting down to zero	Not until you configure it, which we’ll do for delays later


Summary: “zeros first, real values later”

The idea in one sentence: the compiler can only write down what it can know about one file, so it leaves a blank plus a note, and the linker fills the blank once it can see everything.

Phase	Tool	What it knows	What happens to the table
1. Compile	gcc -c	One file only. Sizes and positions inside it (slot 1 is at offset 4)	Writes zeros as placeholders and adds a relocation record (“offset 4 needs the address of Reset_Handler”)
2. Layout	linker	All files plus linker.ld	Places each section in order from ORIGIN, so every function and variable gets a final address
3. Patch	linker	Final addresses	Reads each relocation record and overwrites the zeros with the real number (setting the Thumb bit for functions)

Phase 3 can only follow phase 2, because the addresses don’t exist before the layout is done. The zeros in your objdump -s output (0030 00000000 00000000 00000000) are phase 1: exactly what we expect to see in a .o file.

This isn’t limited to the table. Every function call to something in another file works the same way. Our main(); call in the reset handler will appear as a relocation record too.


Reset Handler, what it does: The addresses are already decided. Before main, I make sure the bytes at those addresses hold what C promises: the declared initial values for .data, and zero for .bss.”variables defined” isn’t quite the right idea. A variable isn’t defined at runtime, it’s defined at compile time. The startup code only makes sure the memory behind it has the correct starting value.


