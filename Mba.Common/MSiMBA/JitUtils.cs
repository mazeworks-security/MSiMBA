using Iced.Intel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.MSiMBA
{
    [Flags]
    public enum VIRTUAL_ALLOCATION_TYPE : uint
    {
        MEM_COMMIT = 0x00001000,
        MEM_RESERVE = 0x00002000,
        MEM_RESET = 0x00080000,
        MEM_RESET_UNDO = 0x01000000,
        MEM_REPLACE_PLACEHOLDER = 0x00004000,
        MEM_LARGE_PAGES = 0x20000000,
        MEM_RESERVE_PLACEHOLDER = 0x00040000,
        MEM_FREE = 0x00010000,
    }

    [Flags]
    public enum PAGE_PROTECTION_FLAGS : uint
    {
        PAGE_NOACCESS = 0x00000001,
        PAGE_READONLY = 0x00000002,
        PAGE_READWRITE = 0x00000004,
        PAGE_WRITECOPY = 0x00000008,
        PAGE_EXECUTE = 0x00000010,
        PAGE_EXECUTE_READ = 0x00000020,
        PAGE_EXECUTE_READWRITE = 0x00000040,
        PAGE_EXECUTE_WRITECOPY = 0x00000080,
        PAGE_GUARD = 0x00000100,
        PAGE_NOCACHE = 0x00000200,
        PAGE_WRITECOMBINE = 0x00000400,
        PAGE_GRAPHICS_NOACCESS = 0x00000800,
        PAGE_GRAPHICS_READONLY = 0x00001000,
        PAGE_GRAPHICS_READWRITE = 0x00002000,
        PAGE_GRAPHICS_EXECUTE = 0x00004000,
        PAGE_GRAPHICS_EXECUTE_READ = 0x00008000,
        PAGE_GRAPHICS_EXECUTE_READWRITE = 0x00010000,
        PAGE_GRAPHICS_COHERENT = 0x00020000,
        PAGE_GRAPHICS_NOCACHE = 0x00040000,
        PAGE_ENCLAVE_THREAD_CONTROL = 0x80000000,
        PAGE_REVERT_TO_FILE_MAP = 0x80000000,
        PAGE_TARGETS_NO_UPDATE = 0x40000000,
        PAGE_TARGETS_INVALID = 0x40000000,
        PAGE_ENCLAVE_UNVALIDATED = 0x20000000,
        PAGE_ENCLAVE_MASK = 0x10000000,
        PAGE_ENCLAVE_DECOMMIT = 0x10000000,
        PAGE_ENCLAVE_SS_FIRST = 0x10000001,
        PAGE_ENCLAVE_SS_REST = 0x10000002,
        SEC_PARTITION_OWNER_HANDLE = 0x00040000,
        SEC_64K_PAGES = 0x00080000,
        SEC_FILE = 0x00800000,
        SEC_IMAGE = 0x01000000,
        SEC_PROTECTED_IMAGE = 0x02000000,
        SEC_RESERVE = 0x04000000,
        SEC_COMMIT = 0x08000000,
        SEC_NOCACHE = 0x10000000,
        SEC_WRITECOMBINE = 0x40000000,
        SEC_LARGE_PAGES = 0x80000000,
        SEC_IMAGE_NO_EXECUTE = 0x11000000,
    }

    public static class JitUtils
    {
        public static byte[] EncodeInstructions(IList<Instruction> instructions, ulong sourceRIP, out ulong endRIP)
        {
            // Allocate a new encoder.
            var stream = new MemoryStream();
            StreamCodeWriter compiledWriter = new StreamCodeWriter(stream);
            var encoder = Iced.Intel.Encoder.Create(64, compiledWriter);
            ulong rip = 0;

            // Relocate the instructions. TODO: Refactor this to remove the redundant decoding.
            var relocatedInstructions = RelocateInstructions(instructions, sourceRIP);
            foreach (var insn in relocatedInstructions)
            {
                var result = encoder.Encode(insn, rip + sourceRIP);
                var codeReader = new ByteArrayCodeReader(stream.GetBuffer(), (int)rip, (int)result);
                var decoder = Iced.Intel.Decoder.Create(64, codeReader);
                decoder.IP = sourceRIP + rip;
                rip += (ulong)result;
            }

            endRIP = sourceRIP + rip;
            return stream.GetBuffer().Take((int)rip).ToArray();
        }

        public static IList<Instruction> RelocateInstructions(IList<Instruction> instructions, ulong rip)
        {
            // Attempt to relocate the instructions to the target rip.
            var codeWriter = new CodeWriterImpl();
            var block = new InstructionBlock(codeWriter, instructions, rip);
            bool success = BlockEncoder.TryEncode(64, block, out var errorMsg, out BlockEncoderResult result);
            if (!success)
                throw new Exception(errorMsg);

            // Initialize a decoder.
            var bytes = codeWriter.ToArray();
            var codeReader = new ByteArrayCodeReader(bytes);
            var decoder = Iced.Intel.Decoder.Create(64, codeReader);
            decoder.IP = rip;

            // Decode the newly relocated instructions.
            List<Instruction> output = new List<Instruction>();
            int decodedLength = 0;
            while (decodedLength != bytes.Length)
            {
                var instruction = decoder.Decode();
                decodedLength += instruction.Length;
                output.Add(instruction);
            }

            return output;
        }

        sealed class CodeWriterImpl : CodeWriter
        {
            readonly List<byte> AllBytes = new List<byte>();

            public byte[] ToArray() => AllBytes.ToArray();

            public override void WriteByte(byte value) => AllBytes.Add(value);
        }

        public static unsafe nint AllocateExecutablePage(int numPages)
        {
            // Allocate a large RWX section of memory.
            numPages += 1;
            var numBytes = 4096 * numPages;
            nint ptr = (nint)VirtualAlloc(null, (uint)numBytes, VIRTUAL_ALLOCATION_TYPE.MEM_COMMIT, PAGE_PROTECTION_FLAGS.PAGE_EXECUTE_READWRITE);

            // Make the last page readonly, so that we can elide bound checks within our jit.
            var catchPtr = ptr + (4096 * (numPages - 1));
            PAGE_PROTECTION_FLAGS old;
            VirtualProtect((void*)catchPtr, 4096, PAGE_PROTECTION_FLAGS.PAGE_READONLY, &old);

            return ptr;
        }

        public static unsafe void WriteBytes(nint ptr, byte[] bytes)
        {
            var baseAddr = (ulong)ptr;
            for(uint i = 0; i < bytes.Length; i++)
            {
                var addr = (byte*)(baseAddr + i);
                *addr = bytes[i];
            }
        }

        [DllImport("KERNEL32.dll", ExactSpelling = true, SetLastError = true)]
        [DefaultDllImportSearchPaths(DllImportSearchPath.System32)]
        public static extern unsafe void* VirtualAlloc(
        [Optional] void* lpAddress,
        nuint dwSize,
        VIRTUAL_ALLOCATION_TYPE flAllocationType,
        PAGE_PROTECTION_FLAGS flProtect);

        [DllImport("KERNEL32.dll", ExactSpelling = true, SetLastError = true)]
        [DefaultDllImportSearchPaths(DllImportSearchPath.System32)]
        public static extern unsafe byte VirtualProtect(
        void* lpAddress,
        nuint dwSize,
        PAGE_PROTECTION_FLAGS flNewProtect,
        PAGE_PROTECTION_FLAGS* lpflOldProtect);
    }
}
