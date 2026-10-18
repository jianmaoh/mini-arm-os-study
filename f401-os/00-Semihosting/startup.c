#include <stdint.h>
extern void main(void);

void reset_handler(void)
{
    main();
}

__attribute__((section(".vector_list")))
uint32_t vector_list[] = {  
    0x20018000,
    (uint32_t)reset_handler 
    // liker place addr here which is 80000054
    // &vector_list[1] is 0x08000004 vector_list[1] = 0x08000055
};

/*開機時 CPU 從 0x00000000 讀第 0 格 因為 BOOT0 = 0 這個位址映射到 flash 開頭 0x08000000
所以讀到的是我放在 vector_list[0] 的 0x20018000
CPU 把這個值放進 SP 也就是 SRAM 的尾端
接著 CPU 讀 0x00000004 拿到 0x08000055 放進 PC 跳到 0x08000054 執行 reset_handler。
之後函式呼叫、區域變數、保存的暫存器都放在 stack 上 從 0x20018000 往低位址長*/