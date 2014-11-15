#include "6502_Processor.hpp"

void CPU::ADC(u8 val)
{
    u32 temp = val + A + (Carry() ? 1 : 0);
    SetZero(temp & 0xFF);
    if (Decimal())
    {
        if (((A & 0xF) + (val & 0xf) + (Carry() ? 1 : 0)) > 9) temp += 6;
        SetNegative(temp);
        SetOverflow(!((A ^ val) & 0x80) && ((A ^ temp) & 0x80));
        if (temp > 0x99) temp += 96;
        SetCarry(temp > 0x99);
    }
    else
    {
        SetNegative(temp);
        SetOverflow(!((A ^ val) & 0x80) && ((A ^ temp) & 0x80));
        SetCarry(temp > 0xFF);
    }
    A = u8(temp);
}

void CPU::AND(u8 val)
{
    val &= A;
    SetNegative(val);
    SetZero(val);
    A = val;
}

void CPU::ASL(u8& val)
{
    SetCarry((val & 0x80) != 0);
    val <<= 1;
    SetNegative(val);
    SetZero(val);
}

void CPU::BCC(u8 val)
{
    if (!Carry())
    {
        clockCount += ((PC & 0xFF00) != ((PC + val) & 0xFF00) ? 2 : 1);
        PC += val;
    }
}

void CPU::BCS(u8 val)
{
    if (Carry())
    {
        clockCount += ((PC & 0xFF00) != ((PC + val) & 0xFF00) ? 2 : 1);
        PC += val;
    }
}

void CPU::BEQ(u8 val)
{
    if (Zero())
    {
        clockCount += ((PC & 0xFF00) != ((PC + val) & 0xFF00) ? 2 : 1);
        PC += val;
    }
}

void CPU::BIT(u8 val)
{
    SetOverflow((val & 0x40) != 0);
    SetNegative(val & 0x80);
    SetZero(val & A);
}

void CPU::BMI(u8 val)
{
    if (Negative())
    {
        clockCount += ((PC & 0xFF00) != ((PC + val) & 0xFF00) ? 2 : 1);
        PC += val;
    }
}

void CPU::BNE(u8 val)
{
    if (!Zero())
    {
        clockCount += ((PC & 0xFF00) != ((PC + val) & 0xFF00) ? 2 : 1);
        PC += val;
    }
}

void CPU::BPL(u8 val)
{
    if (!Negative())
    {
        clockCount += ((PC & 0xFF00) != ((PC + val) & 0xFF00) ? 2 : 1);
        PC += val;
    }
}

void CPU::BRK()
{
    PC++;

    Push(PC >> 8);   // Push PC hi on stack
    Push(PC & 0xFF); // Push PC lo on stack

    Push(statusFlags | B);

    SetInterrupt(true);

    PC = (ram->Read(0xFFFF) << 8) | ram->Read(0xFFFE);
}

void CPU::BVC(u8 val)
{
    if (Overflow())
    {
        clockCount += ((PC & 0xFF00) != ((PC + val) & 0xFF00) ? 2 : 1);
        PC += val;
    }
}

void CPU::BVS(u8 val)
{
    if (!Overflow())
    {
        clockCount += ((PC & 0xFF00) != ((PC + val) & 0xFF00) ? 2 : 1);
        PC += val;
    }
}

void CPU::CLC()
{
    SetCarry(0);
}

void CPU::CLD()
{
    SetDecimal(0);
}

void CPU::CLI()
{
    SetInterrupt(0);
}

void CPU::CLV()
{
    SetOverflow(0);
}

void CPU::CMP(u8 val)
{
    SetCarry((A - val) < 0x100);
    SetZero(((A - val) & 0xFF) == 0);
    SetNegative(A - val);
}

void CPU::CPX(u8 val)
{
    SetCarry((X - val) < 0x100);
    SetZero(((X - val) & 0xFF) == 0);
    SetNegative(X - val);
}

void CPU::CPY(u8 val)
{
    SetCarry((Y - val) < 0x100);
    SetZero(((Y - val) & 0xFF) == 0);
    SetNegative(Y - val);
}

void CPU::DEC(u8& val)
{
    val--;
    SetZero(val);
    SetNegative(val);
}

void CPU::DEX()
{
    X--;
    SetZero(X);
    SetNegative(X);
}

void CPU::DEY()
{
    Y--;
    SetZero(Y);
    SetNegative(Y);
}

void CPU::EOR(u8 val)
{
    A ^= val;
    SetZero(A);
    SetNegative(A);
}

void CPU::INC(u8& val)
{
    val++;
    SetZero(val);
    SetNegative(val);
}

void CPU::INX()
{
    X++;
    SetZero(X);
    SetNegative(X);
}

void CPU::INY()
{
    Y++;
    SetZero(Y);
    SetNegative(Y);
}

void CPU::JMP(u8 lo, u8 hi)
{
    PC = (hi << 8) | lo;
}

void CPU::JSR(u8 lo, u8 hi)
{
    PC--;
    Push(PC >> 8);
    Push(PC & 0xFF);
    PC = (hi << 8) | lo;
}

void CPU::LDA(u8 val)
{
    A = val;
    SetZero(A);
    SetNegative(A);
}

void CPU::LDX(u8 val)
{
    X = val;
    SetZero(X);
    SetNegative(X);
}

void CPU::LDY(u8 val)
{
    Y = val;
    SetZero(Y);
    SetNegative(Y);
}

void CPU::LSR(u8& val)
{
    SetCarry((val & 0x1) != 0);
    val >>= 1;
    SetZero(val);
    SetNegative(val);
}

void CPU::NOP()
{
}

void CPU::ORA(u8 val)
{
    A |= val;
    SetZero(A);
    SetNegative(A);
}

void CPU::PHA()
{
    Push(A);
}

void CPU::PHP()
{
    Push(statusFlags);
}

void CPU::PLA()
{
    A = Pop();
    SetZero(A);
    SetNegative(A);
}

void CPU::PLP()
{
    statusFlags = Pop();
}

void CPU::ROL(u8& val)
{
    u8 temp;
    temp = Carry();
    SetCarry((val & 0x80) != 0);
    val <<= 1;
    val |= temp;
    SetZero(val);
    SetNegative(val);
}

void CPU::ROR(u8& val)
{
    u8 temp;
    temp = Carry();
    SetCarry((val & 0x1) != 0);
    val >>= 1;
    val |= (temp << 7);
    SetZero(val);
    SetNegative(val);
}

void CPU::RTI()
{
    statusFlags = Pop();
    u8 temp = Pop();
    PC = temp | (Pop() << 8);
}

void CPU::RTS()
{
    u8 temp = Pop();
    PC = temp | (Pop() << 8);
    PC++;
}

void CPU::SBC(u8 val)
{
    u32 temp = A - val - (Carry() ? 0 : 1);
    SetNegative(temp);
    SetZero(temp & 0xFF);
    SetOverflow(((A ^ temp) & 0x80) && ((A ^ val) & 0x80));
    if (Decimal())
    {
        if (((A & 0xf) - (Carry() ? 0 : 1)) < (val & 0xf)) temp -= 6;
        if (temp > 0x99) temp -= 96;
    }
    SetCarry(temp < 0x100);
    A = (temp & 0xFF);
}

void CPU::SEC()
{
    SetCarry(1);
}

void CPU::SED()
{
    SetDecimal(1);
}

void CPU::SEI()
{
    SetInterrupt(1);
}

void CPU::STA(u8& val)
{
    val = A;
}

void CPU::STX(u8& val)
{
    val = X;
}

void CPU::STY(u8& val)
{
    val = Y;
}

void CPU::TAX()
{
    X = A;
    SetZero(X);
    SetNegative(X);
}

void CPU::TAY()
{
    Y = A;
    SetZero(Y);
    SetNegative(Y);
}

void CPU::TSX()
{
    X = SP;
    SetZero(X);
    SetNegative(X);
}

void CPU::TXA()
{
    A = X;
    SetZero(A);
    SetNegative(A);
}

void CPU::TXS()
{
    SP = X;
}

void CPU::TYA()
{
    A = Y;
    SetZero(A);
    SetNegative(A);
}
