#ifndef REGALLOCATION_HPP
#define REGALLOCATION_HPP

#define NUM_REG 8

class RegAlloc {
public:

    bool reg [NUM_REG];

    static std::shared_ptr<RegAlloc> r;
    static std::shared_ptr<RegAlloc> instance() {
        if(r == nullptr) {
            r = std::make_shared<RegAlloc>();
        }
        return r;
    }
    
    int getRegister(){
        static int num = 0;
        while (reg[num] != 0) { // potential infinite loop for full registers. I guess if all full, return -1?
            num = (num+1) % NUM_REG;
        }
        reg[num] = 1;
        return num++ % NUM_REG;
    }

    void release(int num){
        reg[num] = 0;
        return;
    }

};

#endif
