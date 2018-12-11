#ifndef THREEADDRESSINSTRUCTION_HPP
#define THREEADDRESSINSTRUCTION_HPP

class ThreeAddressInstruction{
public:
    enum Type{
        LoadValue,
    };

    ThreeAddressInstruction(Type op, int dest, int src1, int src2);
    
    Type op;
    int dest;
    int src1;
    int src2;
};

#endif
