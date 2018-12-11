

class ThreeAddressInstruction{
public:
    enum Type{
        LoadValue,
    };

    ThreeAddressInstruction(Type op, int dest, int src1, int src2);
private:
    Type op;
    int dest;
    int src1;
    int src2;
}