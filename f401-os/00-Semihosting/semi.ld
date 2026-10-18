ENTRY(reset_handler)

/* describe how many mem the chip have*/
MEMORY{
    flash(rx) : ORIGIN = 0x08000000, LENGTH = 512K

}

/* decide where to place somethigs */
SECTIONS{
    /* .text is just a output segament name */
    .text : {
        /* why keep: vector_list is not used by any code so the linker */
        /* may consider vector_list is a trash so abandon it use keep to tell linker keep it */
        KEEP(*(.vector_list))
        /* to collect all .text segament of code */
        *(.text)
    } > flash
    /* place this output section in the flash */
}
/* why we need to place vector_list in the begin of flash */
/* since the cpu just need to read the mem in order */