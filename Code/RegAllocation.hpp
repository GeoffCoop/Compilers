

#define NUM_REG 8

int getRegister(){
    static int reg = 0;
    return reg++ % NUM_REG;
}