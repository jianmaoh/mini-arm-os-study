# f401-os: Bare-metal STM32F401 Self-study

A self-study port of [jserv/mini-arm-os](https://github.com/jserv/mini-arm-os) to a real
**STM32F401RE Nucleo** board, building up the Cortex-M boot process from scratch.
Work in progress.

The original mini-arm-os mainly targets an STM32F103 emulated in QEMU. This repo ports it
to the STM32F401 and adds my own modifications and notes to each chapter.

## Hardware

  | Item | Spec |
  |---|---|
  | Board | NUCLEO-F401RE |
  | MCU | STM32F401RE, Cortex-M4 (r0p1) |
  | Flash | 512 KB, starting at `0x08000000` |
  | SRAM | 96 KB, `0x20000000`–`0x20017FFF` |

## Progress

- [x] **00-Semihosting**: vector table, Reset_Handler, linker script, semihosting I/O

## 00-Semihosting

Written without HAL or the C runtime (`-nostartfiles`):

- **Vector table** (`startup.c`): entry 0 holds the initial SP (`0x20018000`, top of SRAM),
  entry 1 holds the Reset_Handler address
- **Linker script** (`semi.ld`): places the vector table at the start of flash, `0x08000000`
- **Semihosting** (`semi.c`): uses inline assembly and `bkpt 0xAB` to invoke the debugger's
  `SYS_READ` / `SYS_WRITE`, implementing a terminal echo that also prints the buffer's
  address in SRAM

### Changes from the original

| Item | Original (QEMU STM32F103) | This port (STM32F401RE) |
|---|---|---|
| CPU | Cortex-M3 | Cortex-M4 |
| Flash origin | `0x00000000` | `0x08000000` |
| Initial SP | — | `0x20018000` (top of 96 KB SRAM) |
| Function | Prints "Hello World" | Terminal echo + prints SRAM address |

### Verification

Stack frame derived from the `objdump` disassembly:

```
reset:          SP = 0x20018000   (vector table [0])
reset_handler:  push {r7, lr}      → 0x20017FF8
main:           push {r4, r7, lr}  → 0x20017FEC
                sub  sp, #148      → 0x20017F58   ← buffer
```

This matches the program's actual output, `0x20017F58: mark`.

## Build & Flash

Requires `arm-none-eabi-gcc` and OpenOCD:

```bash
cd f401-os/00-Semihosting
make
openocd -f board/st_nucleo_f4.cfg \
    -c "init" \
    -c "program semi.elf verify" \
    -c "arm semihosting enable" \
    -c "reset run"
```

## Credits

Based on [jserv/mini-arm-os](https://github.com/jserv/mini-arm-os),
Copyright (C) 2015-2018 National Cheng Kung University, Taiwan.
Released under the BSD 2-Clause License; see [`f401-os/LICENSE`](f401-os/LICENSE).
