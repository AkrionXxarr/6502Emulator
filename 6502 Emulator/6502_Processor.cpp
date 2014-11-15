// Temp
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;
// --

#include "6502_Processor.hpp"

void CPU::Execute(u8 opcode)
{
#define CLK(x) clockCount += x

#define R(addr) ram->Read(addr)
#define W(addr, v) ram->Write(addr, v)

#define ZeroPage(addr) ram->Read(0x0000 | (ram->Read(addr)))
#define ZeroPage_X(addr) ram->Read(0x0000 | (ram->Read(addr) + X))
#define ZeroPage_Y(addr) ram->Read(0x0000 | (ram->Read(addr) + Y))

    u8 hi, lo;
    u8 val;
    u16 tempAddr;

    // Temp variables
    ofstream outFile;

    // Instruction concept:
    // Address mode specific stuff is handled here
    // Instruction generic stuff is handled in a unique function
    switch (opcode)
    {
        // Temp instruction - Print accumulator
    case 0x07:
        cout << u32(A) << endl;
        _sleep(500);
        break;

        // Temp instruction - Dump RAM to file
    case 0x0B:
        cout << "Creating RamDump.txt" << endl;
        outFile.open("RamDump.txt");
        outFile << "--- Zero Page (0x0000 - 0x00FF) ---\n";
        for (u32 i = 0, col = 0; i <= 0x00FF; i++, col++)
        {
            if (col != 0 && ((col % 20) == 0)) { outFile << "\n"; }
            outFile << setw(2) << setfill('0') << hex << u32(ram->Read(i)) << " ";
        }
        outFile << "\n\n--- Stack (0x0100 - 0x01FF) ---\n";
        for (u32 i = 0x0100, col = 0; i <= 0x01FF; i++, col++)
        {
            if (col != 0 && ((col % 20) == 0)) { outFile << "\n"; }
            outFile << setw(2) << setfill('0') << hex << u32(ram->Read(i)) << " ";
        }
        outFile << "\n\n--- RAM (0x0200 - 0xFFF9) ---\n";
        for (u32 i = 0x0200, col = 0; i <= 0xFFF9; i++, col++)
        {
            if (col != 0 && ((col % 20) == 0)) { outFile << "\n"; }
            outFile << setw(2) << setfill('0') << hex << u32(ram->Read(i)) << " ";
        }
        outFile << "\n\n--- Interrupt table (0xFFFA - 0xFFFF) ---\n";
        for (u32 i = 0xFFFA, col = 0; i <= 0xFFFF; i++, col++)
        {
            if (col != 0 && ((col % 20) == 0)) { outFile << "\n"; }
            outFile << setw(2) << setfill('0') << hex << u32(ram->Read(i)) << " ";
        }
        outFile.close();
        break;

        // Temp instruction - Stop execution
    case 0x0C:
        running = false;
        break;

        // ADC - Add with Carry
    case 0x69: // Immediate
        val = R(PC++);
        ADC(val);
        CLK(2);
        break;
    case 0x65: // Zero Page
        val = ZeroPage(PC++);
        ADC(val);
        CLK(3);
        break;
    case 0x75: // Zero Page, X
        val = ZeroPage_X(PC++);
        ADC(val);
        CLK(4);
        break;
    case 0x6D: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        val = R(lo | (hi << 8));
        ADC(val);
        CLK(4);
        break;
    case 0x7D: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + X);
        if (lo + X > 0xFF) { CLK(1); }
        ADC(val);
        CLK(4);
        break;
    case 0x79: // Absolute, Y
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(1); }
        ADC(val);
        CLK(4);
        break;
    case 0x61: // (Indirect, X)
        val = ZeroPage_X(PC++);
        lo = R(val);
        hi = R(val + 1);
        val = R(lo | (hi << 8));
        ADC(val);
        CLK(6);
        break;
    case 0x71: // (Indirect), Y
        val = R(PC++);
        lo = R(val);
        hi = R(val + 1);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(1); }
        ADC(val);
        CLK(5);
        break;


        // AND - Logical AND
    case 0x29: // Immediate
        val = R(PC++);
        AND(val);
        CLK(2);
        break;
    case 0x25: // Zero Page
        val = ZeroPage(PC++);
        AND(val);
        CLK(3);
        break;
    case 0x35: // Zero Page, X
        val = ZeroPage_X(PC++);
        AND(val);
        CLK(4);
        break;
    case 0x2D: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        val = R(lo | (hi << 8));
        AND(val);
        CLK(4);
        break;
    case 0x3D: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + X);
        if (lo + X > 0xFF) { CLK(1); }
        AND(val);
        CLK(4);
        break;
    case 0x39: // Absolute, Y
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(1); }
        AND(val);
        CLK(4);
        break;
    case 0x21: // (Indirect, X)
        val = ZeroPage_X(PC++);
        lo = R(val);
        hi = R(val + 1);
        val = R(lo | (hi << 8));
        AND(val);
        CLK(6);
        break;
    case 0x31: // (Indirect), Y
        val = R(PC++);
        lo = R(val);
        hi = R(val + 1);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(1); }
        AND(val);
        CLK(5);
        break;

        
        // ASL - Arithmetic Shift Left
    case 0x0A: // Accumulator
        ASL(A);
        CLK(2);
        break;
    case 0x06: // Zero Page
        tempAddr = PC++;
        val = ZeroPage(tempAddr);
        ASL(val);
        W(tempAddr, val);
        CLK(5);
        break;
    case 0x16: // Zero Page, X
        tempAddr = PC++;
        val = ZeroPage_X(tempAddr);
        ASL(val);
        W(tempAddr, val);
        CLK(6);
        break;
    case 0x0E: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = (lo | (hi << 8));
        val = R(tempAddr);
        ASL(val);
        W(tempAddr, val);
        CLK(6);
        break;
    case 0x1E: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = (lo | (hi << 8)) + X;
        val = R(tempAddr);
        ASL(val);
        W(tempAddr, val);
        CLK(7);
        break;
        

        // BCC - Branch if Carry Clear
    case 0x90: // Relative
        val = R(PC++);
        BCC(val);
        CLK(2);
        break;
        

        // BCS - Branch if Carry Set
    case 0xB0: // Relative
        val = R(PC++);
        BCS(val);
        CLK(2);
        break;
        

        // BEQ - Branch if Equal
    case 0xF0: // Relative
        val = R(PC++);
        BEQ(val);
        CLK(2);
        break;
        

        // BIT - Bit Test
    case 0x24: // Zero Page
        val = ZeroPage(PC++);
        BIT(val);
        CLK(3);
        break;
    case 0x2C: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        val = R(lo | (hi << 8));
        BIT(val);
        CLK(4);
        break;
        

        // BMI - Branch if Minus
    case 0x30: // Relative
        val = R(PC++);
        BMI(val);
        CLK(2);
        break;
        

        // BNE - Branch if Not Equal
    case 0xD0: // Relative
        val = R(PC++);
        BNE(val);
        CLK(2);
        break;
        

        // BPL - Branch if Positive
    case 0x10: // Relative
        val = R(PC++);
        BPL(val);
        CLK(2);
        break;
        

        // BRK - Force Interrupt
    case 0x00: // Implied
        BRK();
        CLK(7);
        break;
        
        // BVC - Branch if Overflow Clear
    case 0x50: // Relative
        val = R(PC++);
        BVC(val);
        CLK(2);
        break;
        

        // BVS - Branch if Overflow Set
    case 0x70: // Relative
        val = R(PC++);
        BVS(val);
        CLK(2);
        break;
        

        // CLC - Clear Carry Flag
    case 0x18: // Implied
        CLC();
        CLK(2);
        break;
        

        // CLD - Clear Decimal Mode
    case 0xD8: // Implied
        CLD();
        CLK(2);
        break;
        

        // CLI - Clear Interrupt Disable
    case 0x58: // Implied
        CLI();
        CLK(2);
        break;
        

        // CLV - Clear Overflow Flag
    case 0xB8: // Implied
        CLV();
        CLK(2);
        break;
        

        // CMP - Compare
    case 0xC9: // Immediate
        val = R(PC++);
        CMP(val);
        CLK(2);
        break;
    case 0xC5: // Zero Page
        val = ZeroPage(PC++);
        CMP(val);
        CLK(3);
        break;
    case 0xD5: // Zero Page, X
        val = ZeroPage_X(PC++);
        CMP(val);
        CLK(4);
        break;
    case 0xCD: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        val = R(lo | (hi << 8));
        CMP(val);
        CLK(4);
        break;
    case 0xDD: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + X);
        if (lo + X > 0xFF) { CLK(1); }
        CMP(val);
        CLK(4);
        break;
    case 0xD9: // Absolute, Y
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(1); }
        CMP(val);
        CLK(4);
        break;
    case 0xC1: // (Indirect, X)
        val = ZeroPage_X(PC++);
        lo = R(val);
        hi = R(val + 1);
        val = R(lo | (hi << 8));
        CMP(val);
        CLK(6);
        break;
    case 0xD1: // (Indirect), Y
        val = R(PC++);
        lo = R(val);
        hi = R(val + 1);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(1); }
        CMP(val);
        CLK(5);
        break;
        

        // CPX - Compare X Register
    case 0xE0: // Immediate
        val = R(PC++);
        CPX(val);
        CLK(2);
        break;
    case 0xE4: // Zero Page
        val = ZeroPage(PC++);
        CPX(val);
        CLK(3);
        break;
    case 0xEC: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        val = R(lo | (hi << 8));
        CPX(val);
        CLK(4);
        break;
        

        // CPY - Compare Y Register
    case 0xC0: // Immediate
        val = R(PC++);
        CPY(val);
        CLK(2);
        break;
    case 0xC4: // Zero Page
        val = ZeroPage(PC++);
        CPY(val);
        CLK(3);
        break;
    case 0xCC: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        val = R(lo | (hi << 8));
        CPY(val);
        CLK(4);
        break;
        

        // DEC - Decrement Memory
    case 0xC6: // Zero Page
        tempAddr = PC++;
        val = ZeroPage(tempAddr);
        DEC(val);
        W(tempAddr, val);
        CLK(5);
        break;
    case 0xD6: // Zero Page, X
        tempAddr = PC++;
        val = ZeroPage_X(tempAddr);
        DEC(val);
        W(tempAddr, val);
        CLK(6);
        break;
    case 0xCE: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = (lo | (hi << 8));
        val = R(tempAddr);
        DEC(val);
        W(tempAddr, val);
        CLK(6);
        break;
    case 0xDE: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = (lo | (hi << 8)) + X;
        val = R(tempAddr);
        DEC(val);
        W(tempAddr, val);
        CLK(7);
        break;
        

        // DEX - Decrement X Register
    case 0xCA: // Implied
        DEX();
        CLK(2);
        break;
        

        // DEY - Decrement Y Register
    case 0x88: // Implied
        DEY();
        CLK(2);
        break;
        

        // EOR - Exclusive OR
    case 0x49: // Immediate
        val = R(PC++);
        EOR(val);
        CLK(2);
        break;
    case 0x45: // Zero Page
        val = ZeroPage(PC++);
        EOR(val);
        CLK(3);
        break;
    case 0x55: // Zero Page, X
        val = ZeroPage_X(PC++);
        EOR(val);
        CLK(4);
        break;
    case 0x4D: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        val = R(lo | (hi << 8));
        EOR(val);
        CLK(4);
        break;
    case 0x5D: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + X);
        if (lo + X > 0xFF) { CLK(1); }
        EOR(val);
        CLK(4);
        break;
    case 0x59: // Absolute, Y
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(1); }
        EOR(val);
        CLK(4);
        break;
    case 0x41: // (Indirect, X)
        val = ZeroPage_X(PC++);
        lo = R(val);
        hi = R(val + 1);
        val = R(lo | (hi << 8));
        EOR(val);
        CLK(6);
        break;
    case 0x51: // (Indirect), Y
        val = R(PC++);
        lo = R(val);
        hi = R(val + 1);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(5); }
        EOR(val);
        CLK(5);
        break;
        

        // INC - Increment Memory
    case 0xE6: // Zero Page
        tempAddr = PC++;
        val = ZeroPage(tempAddr);
        INC(val);
        W(tempAddr, val);
        CLK(5);
        break;
    case 0xF6: // Zero Page, X
        tempAddr = PC++;
        val = ZeroPage_X(tempAddr);
        INC(val);
        W(tempAddr, val);
        CLK(6);
        break;
    case 0xEE: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = (lo | (hi << 8));
        val = R(tempAddr);
        INC(val);
        W(tempAddr, val);
        CLK(6);
        break;
    case 0xFE: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = ((lo | (hi << 8)) + X);
        val = R(tempAddr);
        INC(val);
        W(tempAddr, val);
        CLK(7);
        break;
        

        // INX - Increment X Register
    case 0xE8: // Implied
        INX();
        CLK(2);
        break;
        

        // INY - Increment Y Register
    case 0xC8: // Implied
        INY();
        CLK(2);
        break;
        

        // JMP - Jump
    case 0x4C: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        JMP(lo, hi);
        CLK(3);
        break;
    case 0x6C: // Indirect
        lo = R(PC++);
        hi = R(PC++);
        JMP(R(lo | (hi << 8)), R(lo | (hi << 8)) + 1);
        CLK(5);
        break;
        

        // JSR - Jump to Subroutine
    case 0x20: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        JSR(lo, hi);
        CLK(6);
        break;
        

        // LDA - Load Accumulator
    case 0xA9: // Immediate
        val = R(PC++);
        LDA(val);
        CLK(2);
        break;
    case 0xA5: // Zero Page
        val = ZeroPage(PC++);
        LDA(val);
        CLK(3);
        break;
    case 0xB5: // Zero Page, X
        val = ZeroPage_X(PC++);
        LDA(val);
        CLK(4);
        break;
    case 0xAD: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        val = R(lo | (hi << 8));
        LDA(val);
        CLK(4);
        break;
    case 0xBD: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + X);
        if (lo + X > 0xFF) { CLK(1); }
        LDA(val);
        CLK(4);
        break;
    case 0xB9: // Absolute, Y
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(1); }
        LDA(val);
        CLK(4);
        break;
    case 0xA1: // (Indirect, X)
        val = ZeroPage_X(PC++);
        lo = R(val);
        hi = R(val + 1);
        val = R(lo | (hi << 8));
        LDA(val);
        CLK(6);
        break;
    case 0xB1: // (Indirect), Y
        val = R(PC++);
        lo = R(val);
        hi = R(val + 1);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(1); }
        LDA(val);
        CLK(5);
        break;
        

        // LDX - Load X Register
    case 0xA2: // Immediate
        val = R(PC++);
        LDX(val);
        CLK(2);
        break;
    case 0xA6: // Zero Page
        val = ZeroPage(PC++);
        LDX(val);
        CLK(3);
        break;
    case 0xB6: // Zero Page, Y
        val = ZeroPage_Y(PC++);
        LDX(val);
        CLK(4);
        break;
    case 0xAE: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        val = R(lo | (hi << 8));
        LDX(val);
        CLK(4);
        break;
    case 0xBE: // Absolute, Y
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(1); }
        LDX(val);
        CLK(4);
        break;
        

        // LDY - Load Y Register
    case 0xA0: // Immediate
        val = R(PC++);
        LDY(val);
        CLK(2);
        break;
    case 0xA4: // Zero Page
        val = ZeroPage(PC++);
        LDY(val);
        CLK(3);
        break;
    case 0xB4: // Zero Page, X
        val = ZeroPage_X(PC++);
        LDY(val);
        CLK(4);
        break;
    case 0xAC: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        val = R(lo | (hi << 8));
        LDY(val);
        CLK(4);
        break;
    case 0xBC: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + X);
        if (lo + X > 0xFF) { CLK(1); }
        LDY(val);
        CLK(4);
        break;
        

        // LSR - Logical Shift Right
    case 0x4A: // Accumulator
        LSR(A);
        CLK(2);
        break; 
    case 0x46: // Zero Page
        tempAddr = PC++;
        val = ZeroPage(tempAddr);
        LSR(val);
        W(tempAddr, val);
        CLK(5);
        break;
    case 0x56: // Zero Page, X
        tempAddr = PC++;
        val = ZeroPage_X(tempAddr);
        LSR(val);
        W(tempAddr, val);
        CLK(6);
        break;
    case 0x4E: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = (lo | (hi << 8));
        val = R(tempAddr);
        LSR(val);
        W(tempAddr, val);
        CLK(6);
        break;
    case 0x5E: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = (lo | (hi << 8)) + X;
        val = R(tempAddr);
        LSR(val);
        W(tempAddr, val);
        CLK(7);
        break;
        

        // NOP - No Operation
    case 0xEA: // Implied
        // Does nothing! :D
        CLK(2);
        break;
        

        // ORA - Logical Inclusive OR
    case 0x09: // Immediate
        val = R(PC++);
        ORA(val);
        CLK(2);
        break;
    case 0x05: // Zero Page
        val = ZeroPage(PC++);
        ORA(val);
        CLK(3);
        break;
    case 0x15: // Zero Page, X
        val = ZeroPage_X(PC++);
        ORA(val);
        CLK(4);
        break;
    case 0x0D: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        val = R(lo | (hi << 8));
        ORA(val);
        CLK(4);
        break;
    case 0x1D: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + X);
        if (lo + X > 0xFF) { CLK(1); }
        ORA(val);
        CLK(4);
        break;
    case 0x19: // Absolute, Y
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(1); }
        ORA(val);
        CLK(4);
        break;
    case 0x01: // (Indirect, X)
        val = ZeroPage_X(PC++);
        lo = R(val);
        hi = R(val + 1);
        val = R(lo | (hi << 8));
        ORA(val);
        CLK(6);
        break;
    case 0x11: // (Indirect), Y
        val = R(PC++);
        lo = R(val);
        hi = R(val + 1);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(1); }
        ORA(val);
        CLK(5);
        break;
        

        // PHA - Push Accumulator
    case 0x48: // Implied
        PHA();
        CLK(3);
        break;
        

        // PHP - Push Processor Status
    case 0x08: // Implied
        PHP();
        CLK(3);
        break;
        

        // PLA - Pull Accumulator
    case 0x68: // Implied
        PLA();
        CLK(4);
        break;
        

        // PLP - Pull Processor Status
    case 0x28: // Implied
        PLP();
        CLK(4);
        break;


        // ROL - Rotate Left
    case 0x2A: // Accumulator
        ROL(A);
        CLK(2);
        break;
    case 0x26: // Zero Page
        tempAddr = PC++;
        val = ZeroPage(tempAddr);
        ROL(val);
        W(tempAddr, val);
        CLK(5);
        break;
    case 0x36: // Zero Page, X
        tempAddr = PC++;
        val = ZeroPage_X(tempAddr);
        ROL(val);
        W(tempAddr, val);
        CLK(6);
        break;
    case 0x2E: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = (lo | (hi << 8));
        val = R(tempAddr);
        ROL(val);
        W(tempAddr, val);
        CLK(6);
        break;
    case 0x3E: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = (lo | (hi << 8)) + X;
        val = R(tempAddr);
        ROL(val);
        W(tempAddr, val);
        CLK(7);
        break;
        

        // ROR - Rotate Right
    case 0x6A: // Accumulator
        ROR(A);
        CLK(2);
        break;
    case 0x66: // Zero Page
        tempAddr = PC++;
        val = ZeroPage(tempAddr);
        ROR(val);
        W(tempAddr, val);
        CLK(5);
        break;
    case 0x76: // Zero Page, X
        tempAddr = PC++;
        val = ZeroPage_X(tempAddr);
        ROR(val);
        W(tempAddr, val);
        CLK(6);
        break;
    case 0x6E: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = (lo | (hi << 8));
        val = R(tempAddr);
        ROR(val);
        W(tempAddr, val);
        CLK(6);
        break;
    case 0x7E: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = (lo | (hi << 8)) + X;
        val = R(tempAddr);
        ROR(val);
        W(tempAddr, val);
        CLK(7);
        break;
        

        // RTI - Return from Interrupt
    case 0x40: // Implied
        RTI();
        CLK(6);
        break;
        

        // RTS - Return from Subroutine
    case 0x60: // Implied
        RTS();
        CLK(6);
        break;
        

        // SBC - Subtract with Carry
    case 0xE9: // Immediate
        val = R(PC++);
        SBC(val);
        CLK(2);
        break;
    case 0xE5: // Zero Page
        val = ZeroPage(PC++);
        SBC(val);
        CLK(3);
        break;
    case 0xF5: // Zero Page, X
        val = ZeroPage_X(PC++);
        SBC(val);
        CLK(4);
        break;
    case 0xED: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        val = R(lo | (hi << 8));
        SBC(val);
        CLK(4);
        break;
    case 0xFD: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + X);
        if (lo + X > 0xFF) { CLK(1); }
        SBC(val);
        CLK(4);
        break;
    case 0xF9: // Absolute, Y
        lo = R(PC++);
        hi = R(PC++);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(1); }
        SBC(val);
        CLK(4);
        break;
    case 0xE1: // (Indirect, X)
        val = ZeroPage_X(PC++);
        lo = R(val);
        hi = R(val + 1);
        val = R(lo | (hi << 8));
        SBC(val);
        CLK(6);
        break;
    case 0xF1: // (Indirect), Y
        val = R(PC++);
        lo = R(val);
        hi = R(val + 1);
        val = R((lo | (hi << 8)) + Y);
        if (lo + Y > 0xFF) { CLK(1); }
        SBC(val);
        CLK(5);
        break;
        

        // SEC - Set Carry Flag
    case 0x38: // Implied
        SEC();
        CLK(2);
        break;
        

        // SED - Set Decimal Flag
    case 0xF8: // Implied
        SED();
        CLK(2);
        break;
        

        // SEI - Set Interrupt Disable
    case 0x78: // Implied
        SEI();
        CLK(2);
        break;
        

        // STA - Store Accumulator
    case 0x85: // Zero Page
        tempAddr = ZeroPage(PC++);
        STA(val);
        W(tempAddr, val);
        CLK(3);
        break;
    case 0x95: // Zero Page, X
        tempAddr = ZeroPage_X(PC++);
        STA(val);
        W(tempAddr, val);
        CLK(4);
        break;
    case 0x8D: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = lo | (hi << 8);
        STA(val);
        W(tempAddr, val);
        CLK(4);
        break;
    case 0x9D: // Absolute, X
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = (lo | (hi << 8)) + X;
        STA(val);
        W(tempAddr, val);
        CLK(5);
        break;
    case 0x99: // Absolute, Y
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = (lo | (hi << 8)) + Y;
        STA(val);
        W(tempAddr, val);
        CLK(5);
        break;
    case 0x81: // (Indirect, X)
        val = ZeroPage_X(PC++);
        lo = R(val);
        hi = R(val + 1);
        tempAddr = lo | (hi << 8);
        STA(val);
        W(tempAddr, val);
        CLK(6);
        break;
    case 0x91: // (Indirect), Y
        val = R(PC++);
        lo = R(val);
        hi = R(val + 1);
        tempAddr = (lo | (hi << 8)) + Y;
        STA(val);
        W(tempAddr, val);
        CLK(6);
        break;
        

        // STX - Store X Register
    case 0x86: // Zero Page
        tempAddr = ZeroPage(PC++);
        STX(val);
        W(tempAddr, val);
        CLK(3);
        break;
    case 0x96: // Zero Page, Y
        tempAddr = ZeroPage_Y(PC++);
        STX(val);
        W(tempAddr, val);
        CLK(4);
        break;
    case 0x8E: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = lo | (hi << 8);
        STX(val);
        W(tempAddr, val);
        CLK(4);
        break;
        

        // STY - Store Y Register
    case 0x84: // Zero Page
        tempAddr = ZeroPage(PC++);
        STY(val);
        W(tempAddr, val);
        CLK(3);
        break;
    case 0x94: // Zero Page, X
        tempAddr = ZeroPage_X(PC++);
        STY(val);
        W(tempAddr, val);
        CLK(4);
        break;
    case 0x8C: // Absolute
        lo = R(PC++);
        hi = R(PC++);
        tempAddr = lo | (hi << 8);
        STY(val);
        W(tempAddr, val);
        CLK(4);
        break;
        

        // TAX - Transfer Accumulator to X
    case 0xAA: // Implied
        TAX();
        CLK(2);
        break;
        

        // TAY - Transfer Accumulator to Y
    case 0xA8: // Implied
        TAY();
        CLK(2);
        break;
        

        // TSX - Transfer Stack Pointer to X
    case 0xBA: // Implied
        TSX();
        CLK(2);
        break;
        

        // TXA - Transfer X to Accumulator
    case 0x8A: // Implied
        TXA();
        CLK(2);
        break;
        

        // TXS - Transfer X to Stack Pointer
    case 0x9A: // Implied
        TXS();
        CLK(2);
        break;
        

        // TYA - Transfer Y to Accumulator
    case 0x98: // Implied
        TYA();
        CLK(2);
        break;


    default:
        running = false;
        break;
    }
}