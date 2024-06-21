using Mba.Ast;
using Iced.Intel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.Intrinsics.Arm;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using static Iced.Intel.AssemblerRegisters;
using ApInt = System.UInt64;
using Mba.Common.MSiMBA;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace Mba.Common.MSiMBA
{
    public static class JitCompatibility
    {
        public static readonly bool IsSupported;

        static JitCompatibility()
        {
            IsSupported = IsWinX64();
        }

        private static bool IsWinX64()
        {
            var handle = Process.GetCurrentProcess().Handle;
            IsWow64Process2(handle, out var processMachine, out var nativeMachine);
            return nativeMachine == 0x8664;
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool IsWow64Process2(
            IntPtr process,
            out ushort processMachine,
            out ushort nativeMachine
        );
    }

    public unsafe class AstJit
    {
        // Args:
        // 8 bit register because it's only used as a shift
        static AssemblerRegister8 bitIdxRegister = cl;
        static AssemblerRegister64 combIdxRegister = rdx;

        // Spills
        static AssemblerRegister64 returnRegister = rax;
        static AssemblerRegister64 rvIdxRegister = rbx;
        static AssemblerRegister64 scratch1 = rsi;
        static AssemblerRegister64 scratch2 = rdi;

        // Start of the mutable executable page
        private byte* start;
        // Current starting point
        private byte* pagePtr;

        private readonly AstNode ast;
        private readonly ulong mask;
        private readonly IReadOnlyList<VarNode> variables;
        private readonly bool multiBit;
        private readonly ulong numCombinations;

        public AstJit(byte* pagePtr, ulong mask, IReadOnlyList<VarNode> variables, AstNode ast, bool multiBit, ulong numCombinations)
        {
            this.start = pagePtr;
            this.pagePtr = pagePtr;
            this.ast = ast;
            this.mask = mask;
            this.variables = variables;
            this.multiBit = multiBit;
            this.numCombinations = numCombinations;
        }

        public ApInt[] BuildResultVector()
        {
            ApInt varCount = (ApInt)variables.Count;
            var capacity = (int)(numCombinations * (multiBit ? ast.BitSize : 1));
            var resultVec = new ApInt[capacity];
            var numBitIterations = MultibitSiMBA.GetNumBitIterations(multiBit, ast.BitSize);

            // Push all clobbered registers
            PushReg(rvIdxRegister);
            PushReg(scratch1);
            PushReg(scratch2);

            // Jit the ast
            Jit(ast);

            // Pop the evaluation result
            PopReg(returnRegister);

            // Mask off bits that we don't care about
            // mov rsi, mask
            WriteBytes(0x48, 0xBE);
            *(ulong*)pagePtr = mask;
            pagePtr += 8;
            // and rax, rsi
            WriteBytes(0x48, 0x21, 0xF0);

            // Shift the value back down to bit index zero,
            // varValue = varValue >> (ushort)v
            // shr rax, bitIdxRegister
            WriteBytes(0x48, 0xD3, 0xE8);

            // Restore the clobbered registers.
            PopReg(scratch2);
            PopReg(scratch1);
            PopReg(rvIdxRegister);

            // Return
            WriteBytes(0xC3);

            var fptr = (delegate* unmanaged[SuppressGCTransition]<ushort, ulong, ulong>)start;
            var casted = (ulong)numCombinations;
            int arrIdx = 0;
            for (ushort bitIndex = 0; bitIndex < numBitIterations; bitIndex++)
            {
                for (ulong i = 0; i < casted; i++)
                {
                    var result = fptr(bitIndex, i);
                    resultVec[arrIdx] = result;
                    arrIdx++;
                }
            }

            return resultVec;
        }

        private unsafe void PushReg(AssemblerRegister64 obj)
        {
            var reg = obj.Value;
            byte encoding = reg switch
            {
                Register.RAX => 0x50,
                Register.RBX => 0x53,
                Register.RSI => 0x56,
                Register.RDI => 0x57,
                _ => throw new InvalidOperationException(),
            };

            *(ushort*)pagePtr = encoding;
            pagePtr++;
        }

        private unsafe void PopReg(AssemblerRegister64 obj)
        {
            var reg = obj.Value;
            byte encoding = reg switch
            {
                Register.RAX => 0x58,
                Register.RBX => 0x5B,
                Register.RSI => 0x5E,
                Register.RDI => 0x5F,
                _ => throw new InvalidOperationException(),
            };

            *(ushort*)pagePtr = encoding;
            pagePtr++;
        }


        private unsafe void PushConst(ulong constant)
        {
            // mov rax, constant
            *(byte*)pagePtr = 0x48;
            pagePtr++;
            *(byte*)pagePtr = 0xB8;
            pagePtr++;

            *(ulong*)pagePtr = constant;
            pagePtr += 8;

            // Push rax
            PushReg(rax);
        }

        private void Jit(AstNode node)
        {
            var op0 = () => Jit(node.Children[0]);
            var op1 = () => Jit(node.Children[1]);

            var binop = (Action apply) =>
            {
                // Jit both operations.
                op0();
                op1();

                // Pop the operands.
                PopReg(scratch2);
                PopReg(scratch1);

                // Apply the function.
                apply();

                // Push the result.
                PushReg(scratch1);
            };

            switch(node.Kind)
            {
                // Push a constant
                case AstKind.Const:
                    var constant = (node as ConstNode).Value;
                    PushConst((ulong)constant);
                    break;
                // Push a variable
                case AstKind.Var:
                    var varNode = (node as VarNode);
                    var varIdx = varNode.index;

                    // varMask = 1 << varIdx
                    // mov rsi, 1
                    WriteBytes(0x48, 0xC7, 0xC6, 0x01, 0x00, 0x00, 0x00);
                    // shl rsi, varidx
                    WriteBytes(0x48, 0xc1, 0xE6, (byte)varIdx);

                    // // varValue = i & varMask
                    // mov rdi, combIdxRegister
                    WriteBytes(0x48, 0x89, 0xD7);
                    // and rdi, rsi
                    WriteBytes(0x48, 0x21, 0xF7);

                    // Shift the value back down to bit index zero,
                    // varValue = varValue >> (ushort)v
                    // shr rdi, varIdx
                    WriteBytes(0x48, 0xC1, 0xEF, (byte)varIdx);

                    // Shift the variable value(which is either zero or one) up to the current bitIndex.
                    // varValue <<= bitIndex;
                    // shl rdi, bitIdxRegister
                    WriteBytes(0x48, 0xD3, 0xE7);

                    // Push the result.
                    PushReg(rdi);
                    break;
                case AstKind.Mul:
                    binop(Mul);
                    break;
                case AstKind.Add:
                    binop(Add);
                    break;
                case AstKind.And:
                    binop(And);
                    break;
                case AstKind.Or:
                    binop(Or);
                    break;
                case AstKind.Xor:
                    binop(Xor);
                    break;
                case AstKind.Neg:
                    op0();
                    PopReg(rsi);
                    Neg();
                    PushReg(rsi);
                    break;
                default:
                    throw new InvalidOperationException();
            }
        }

        // imul rsi, rdi
        private void Mul() => WriteBytes(0x48, 0x0F, 0xAF, 0xF7);
        // add rsi, rdi
        private void Add() => WriteBytes(0x48, 0x01, 0xFE);
        // and rsi, rdi
        private void And() => WriteBytes(0x48, 0x21, 0xFE);
        // or rsi, rdi
        private void Or() => WriteBytes(0x48, 0x09, 0xFE);
        // xor rsi, rdi
        private void Xor() => WriteBytes(0x48, 0x31, 0xFE);
        // not rsi
        private void Neg() => WriteBytes(0x48, 0xF7, 0xD6);

        private void WriteBytes(params byte[] bytes)
        {
            for(int i = 0; i < bytes.Length; i++)
            {
                var b = bytes[i];
                *pagePtr = b;
                pagePtr++;
            }
        }
    }
}
