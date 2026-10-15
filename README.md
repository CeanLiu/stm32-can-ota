# stm32-can-ota
Over-the-wire firmware updates for an STM32 ECU: an ESP32 TCU fetches new firmware over WiFi and flashes it across CAN bus via a custom bootloader.


openocd /usr/share/openocd/scripts/board/st_nucleo_f4.cfg


openocd -f board/st_nucleo_f4.cfg \
        -c "init" \
        -c "reset halt" \
        -c "flash probe 0" \
        -c "flash info 0" \
        -c "exit"

`openocd -f board/st_nucleo_f4.cfg`: start OpenOCD with the Nucleo config, as before.
`\` at the end of a line: tells bash “the command continues on the next line”. It’s only there for readability.
`-c "..."`: run one OpenOCD command. You can chain several -c flags and they run in order.
`init`: finish setup and actually connect to the chip. Config files only describe things, and init is what acts on them.
`reset halt`: reset the chip and stop the CPU before it runs any code. We do it because flash operations are safest when the processor isn’t running something that could be using the flash.
`flash probe 0`: ask OpenOCD to identify the flash. 0 is the bank number (the chip’s main flash is bank 0).
`flash info 0`: print the flash layout of bank 0: its start address and its list of sectors.
`exit`: quit, so it doesn’t stay running as a server.    


`probe` 


`xPSR: 0x01000000`	Bit 24 (the T bit) is set, meaning the CPU is in Thumb mode. It’s the reason the -mthumb flag is mandatory: this core can’t run anything else.




