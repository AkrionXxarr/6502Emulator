#ifndef _6502__PROCESSOR_
#define _6502__PROCESSOR_

#include <iostream>
#include <fstream>

#include <cstdint>

#define u8max 0xFF
#define u16max 0xFFFF
#define u32max 0xFFFFFFFF

typedef std::int8_t  i8;
typedef std::int16_t i16;
typedef std::int32_t i32;

typedef std::uint8_t  u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;

class RAM
{
public:
	RAM (u16 size) 
	{
		this->size = size;
		memory = new u8[size];
	}

	~RAM()
	{
		delete [] memory;
	}

	u8 Read(u16 addr)
	{
        if (addr > this->size) { return 0; }

        return this->memory[addr];
	}

	void Write(u16 addr, u8 val)
	{
        if (addr > this->size) { return; }

        this->memory[addr] = val;
	}

protected:
	u8* memory;
	u16 size;
};

class CPU
{
    enum STATUS_FLAGS
    {
        C = 0x01, // Carry
        Z = 0x02, // Zero
        I = 0x04, // Interrupt Disable
        D = 0x08, // Decimal Mode
        B = 0x10, // Break

        V = 0x40, // Overflow
        N = 0x80  // Negative
    };

public:
    CPU() 
    { 
        ram = new RAM(0xFFFF); // 64k RAM

        for (u16 i = 0; i < u16max; i++)
        {
            ram->Write(i, 0);
        }

        PC = 0x0200;
        SP = 0xFF; // Stack offset 256 bytes (0x0100 to 0x01FF), counts backwards.

        A = 0; X = 0; Y = 0;

        statusFlags = 0x20;
    }

    ~CPU() 
    { 
        delete ram;
    }

    void Run()
    {
        clockCount = 0;
        running = true;
        canExecute = true;

        while (running)
        {
            Tick();
            if (canExecute)
            {
                Execute(ram->Read(PC++));
                canExecute = false;
            }
        }
    }

    void Tick()
    {
        if (clockCount > 0) { clockCount--; }
        else { canExecute = true; }
    }

protected:
    void Execute(u8 opcode);

    void SetFlag(STATUS_FLAGS flag)   { this->statusFlags |= flag; }
    void ClearFlag(STATUS_FLAGS flag) { this->statusFlags &= ~flag; }
    bool CheckFlag(STATUS_FLAGS flag) { return (this->statusFlags & flag) != 0; }

    void Push(u8 val) { ram->Write((0x01 << 8) | SP--, val);  }
    u8 Pop()          { return ram->Read((0x01 << 8) | ++SP); }

    void SetZero(int x)       { if (x == 0)           { SetFlag(Z); } else { ClearFlag(Z); } }
    void SetNegative(int x)   { if ((x & 0x80) != 0) { SetFlag(N); } else { ClearFlag(N); } }
    void SetCarry(bool x)     { if (x)                { SetFlag(C); } else { ClearFlag(C); } }
    void SetOverflow(bool x)  { if (x)                { SetFlag(V); } else { ClearFlag(V); } }
    void SetInterrupt(bool x) { if (x)                { SetFlag(I); } else { ClearFlag(I); } }
    void SetBreak(bool x)     { if (x)                { SetFlag(B); } else { ClearFlag(B); } }
    void SetDecimal(bool x)   { if (x)                { SetFlag(D); } else { ClearFlag(D); } }

    bool Zero()      { return ((statusFlags & Z) != 0); }
    bool Negative()  { return ((statusFlags & N) != 0); }
    bool Carry()     { return ((statusFlags & C) != 0); }
    bool Overflow()  { return ((statusFlags & V) != 0); }
    bool Interrupt() { return ((statusFlags & I) != 0); }
    bool Break()     { return ((statusFlags & B) != 0); }
    bool Decimal()   { return ((statusFlags & D) != 0); }

    // Add instruction functions here that would be redundant to add to the switch
    void ADC(u8 val);
    void AND(u8 val);
    void ASL(u8& val);
    void BCC(u8 val);
    void BCS(u8 val);
    void BEQ(u8 val);
    void BIT(u8 val);
    void BMI(u8 val);
    void BNE(u8 val);
    void BPL(u8 val);
    void BRK();
    void BVC(u8 val);
    void BVS(u8 val);
    void CLC();
    void CLD();
    void CLI();
    void CLV();
    void CMP(u8 val);
    void CPX(u8 val);
    void CPY(u8 val);
    void DEC(u8& val);
    void DEX();
    void DEY();
    void EOR(u8 val);
    void INC(u8& val);
    void INX();
    void INY();
    void JMP(u8 lo, u8 hi);
    void JSR(u8 lo, u8 hi);
    void LDA(u8 val);
    void LDX(u8 val);
    void LDY(u8 val);
    void LSR(u8& val);
    void NOP();
    void ORA(u8 val);
    void PHA();
    void PHP();
    void PLA();
    void PLP();
    void ROL(u8& val);
    void ROR(u8& val);
    void RTI();
    void RTS();
    void SBC(u8 val);
    void SEC();
    void SED();
    void SEI();
    void STA(u8& val);
    void STX(u8& val);
    void STY(u8& val);
    void TAX();
    void TAY();
    void TSX();
    void TXA();
    void TXS();
    void TYA();

public:
    RAM* ram;

protected:
    u16 PC; // Program Counter register
    u8 SP; // Stack Pointer register
    u8 A, X, Y; // Accumulator, X, and Y registers
    u8 statusFlags; // CPU status register

    bool running;
    bool canExecute;
    u8 clockCount;
};
#endif
