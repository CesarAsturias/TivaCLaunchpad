/**********************************************************************************
 *
 * TM4C123GHSPM.ld - Linker configuration for the Tiva C
 *
 ***********************************************************************************/

MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x00040000
	SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

SECTIONS
{
	/* CODE */
	.text :
	{
		_text = .;
		/* Ensure ISR vectors are not removed by linker */
		KEEP(*(.isr_vector))
		*(.text*)
		*(.rodata*)
		_etext = .;
	} > FLASH

	/* Static data */
	.data : AT(ADDR(.text) + SIZEOF(.text))
	{
		_data = .;
		*(vtable)
		*(.data*)
		_edata = .;
	} > SRAM

	/*  Static uninitialized data */
	.bss :
	{
		_bss = .;
		*(.bss*)
		*(COMMON)
		_ebss = .;
	} > SRAM
}
