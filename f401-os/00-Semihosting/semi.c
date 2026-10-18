#include <stdint.h>

static int semihost_call(int service, void *opaque){
        register int r0 asm("r0") = service;
        register void *r1 asm("r1") = opaque;
        register int result asm("r0");
        asm volatile("bkpt 0xab"
                     : "=r" (result) : "r" (r0), "r" (r1));
        return result;
}

enum SEMIHOST_SVC {
        SYS_WRITE = 0x05,
        SYS_READ = 0x06,
        SYS_READC = 0x07,
        SYS_WRITEC = 0x03
};

static void to_hex(uint32_t value, char *out){
          static const char hex[] = "0123456789ABCDEF";
          out[0] = '0';
          out[1] = 'x';
          for (int i = 0; i < 8; i++) {
                  int shift = 28 - 4 * i;              
                  out[2 + i] = hex[(value >> shift) & 0xF];
          }
  }


void main(void){
        char message[] = "Hello Mark!\n";
        uint32_t param[] = { 1, (uint32_t) message, sizeof(message) };
        semihost_call(SYS_WRITE, (void *) param);

        //write on terminal test
        // int len = 64;
        while(1){ 
                char buffer[64];
                uint32_t param2[] = { 0, (uint32_t) buffer, sizeof(buffer) };
                int res = sizeof(buffer) - semihost_call(SYS_READ, (void *) param2);

                char addr[12];
                to_hex((uint32_t)buffer, addr);
                uint32_t param3[] = { 1, (uint32_t) addr, sizeof(addr) };
                addr[10] = ':';
                addr[11] = ' ';
                semihost_call(SYS_WRITE, (void *) param3);

                uint32_t param4[] = { 1, (uint32_t) buffer, res };
                semihost_call(SYS_WRITE, (void *) param4);
        }
        
        while (1);
}