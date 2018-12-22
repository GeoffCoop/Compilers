#ifndef REGALLOCATION_HPP
#define REGALLOCATION_HPP

#define NUM_REG 8

class RegAlloc {
public:
    bool reg [NUM_REG];

    int getRegister(){
        static int num = 0;
        while (reg[num] != 0) { // potential infinite loop for full registers. I guess if all full, return -1?
            num++ % NUM_REG;
        }
        return num++ % NUM_REG;
    }

    void release(int num){
        reg[num] = 0;
        return;
    }

};

#endif
