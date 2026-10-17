What a linker script is, and why we need one

After compiling, each .c file becomes an object file with named chunks: code, constants, initialized variables, zero-initialized variables. The linker merges those chunks into one program, but it doesn’t know your hardware. The script answers three questions:

So the reset handler is the first code that runs, but the CPU finds it through the table. The handler is just a function, and it doesn’t contain information about where flash or RAM are. Look at what it actually does: it copies .data and zeroes .bss, using five symbols (_sidata, _sdata, _edata, _sbss, _ebss) that the linker script supplies. Those are the only layout facts it needs.

t. ENTRY(Reset_Handler) writes the name of the starting function into the .elf file’s header. That’s used by the linker (as a “never throw this away” marker) and by tools like GDB. The chip never reads it.


: it’s more precise to call _estack the top of the stack, or the initial stack pointer value, than the “end of the stack”. The stack starts at the top of RAM and grows downward, so the stack’s growing end is the opposite direction. ST’s naming (“end of stack”) means “end of the RAM region”.

The CPU reads word 0 (stack pointer) and word 1 (reset handler address) from the vector table at the start of flash. Reset_Handler runs the C setup and calls main(). ENTRY is only a hint to tools, not used by the CPU. MEMORY tells the linker what regions physically exist. _estack is the top of RAM, computed from MEMORY, where the stack starts and grows downward. SECTIONS uses wildcard patterns to collect input sections, assigns addresses, and exports symbols (_sidata, _sdata, ...) the reset handler needs. ALIGN(4) keeps section boundaries word-aligned.

Where are flash and RAM on this chip?
Which chunk goes in which memory?
Which addresses should the startup code know about, such as where RAM variables begin and end?

`Stack vs Stack pointer` 

`Exception` 

`Handler` 

`Vector table`

`Reset handler` First code that ever runs on your chip, address in ARM is `0x00000004`

A CPU is not magic: when it comes out of reset it has to find its first instruction somewhere. The Cortex-M4 does this the same way every time, in hardware:

It reads the 32-bit number stored at address 0x00000000 and loads it into the stack pointer (SP).
It reads the 32-bit number at address 0x00000004 and treats it as an address.
It jumps to that address and starts executing.


1. Flash stores the entire program, not just information about it. You said flash holds “where to look for RAM, the stack pointer address, all that”. That’s part of it, but flash holds everything the chip needs to run, permanently:

the vector table (the stack pointer value and the handler addresses)
your compiled code (.text)
constants like string literals (.rodata)
the starting values of your initialized variables (the stored 5 in int counter = 5;)


Volatile memory loses all stored data when the power turns off, while non-volatile memory keeps its data permanently even without power

so flash is non volatile and RAM is volatile



r	read-only sections may go here
w	writable (read/write) sections may go here
x	executable code may go here
a	allocated sections
i or l	initialized sections
!	invert the letters after it


SECTIONS command describes how to assemble the output from the inputs, in the order written.