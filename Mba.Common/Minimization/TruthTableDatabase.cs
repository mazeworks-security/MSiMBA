using Mba.Ast;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Minimization
{
    public class TruthTableDatabase
    {
        private IReadOnlyList<byte[]> Tables { get; }

        public static readonly TruthTableDatabase Instance = new();

        private TruthTableDatabase()
        {
            Tables = new List<byte[]>()
            {
                LoadTruthTableBinary(1),
                LoadTruthTableBinary(2),
                LoadTruthTableBinary(3),
                LoadTruthTableBinary(4),
            };
        }

        private unsafe byte[] LoadTruthTableBinary(int numVars)
        {
            // Fetch the serialized truth table from our embedded resources.
            var path = $"{numVars}variable_truthtable.bc";
            var name = Assembly.GetExecutingAssembly().GetManifestResourceNames().Single(x => x.Contains(path));
            var stream = Assembly.GetExecutingAssembly().GetManifestResourceStream(name);
            var bytes = new byte[stream.Length];
            stream.Read(bytes, 0, bytes.Length);
            return bytes;
        }

        public unsafe AstNode GetTableEntry(IReadOnlyList<VarNode> vars, int index)
        {
            // Fetch the bytecode index for the entry.
            var bytes = Tables[vars.Count - 1];
            var offsetIdx = 4 * index;
            var valueBytes = new byte[] { bytes[offsetIdx], bytes[offsetIdx + 1], bytes[offsetIdx + 2], bytes[offsetIdx + 3] };
            var offsetValue = BitConverter.ToInt32(valueBytes, 0);

            // Construct an AST from the bytecode.
            fixed (byte* ptr = &bytes[offsetValue])
            {
                uint i = 0;
                return ParseBinaryBooleanFunc(vars, ptr, ref i);
            }
        }

        private static unsafe AstNode ParseBinaryBooleanFunc(IReadOnlyList<VarNode> vars, byte* bytes, ref uint i)
        {
            byte opcode = bytes[i];
            i += 1;

            var binop = (AstKind opcode, ref uint i)
                => AstNode.Binop(opcode, ParseBinaryBooleanFunc(vars, bytes, ref i), ParseBinaryBooleanFunc(vars, bytes, ref i));

            switch (opcode)
            {
                case 0:
                    ulong constant = *(ulong*)&bytes[i];
                    return new ConstNode(constant, vars[0].BitSize);
                case 2:
                    byte idx = bytes[i];
                    i += 1;
                    return vars[idx];
                case 8:
                    return binop(AstKind.And, ref i);
                case 9:
                    return binop(AstKind.Or, ref i);
                case 10:
                    return binop(AstKind.Xor, ref i);
                case 11:
                    var a = ParseBinaryBooleanFunc(vars, bytes, ref i);
                    return new NegNode(a);
                // Other operators (add, mul, pow) will not be present in serialized binary truth tables.
                default:
                    throw new InvalidOperationException($"Unrecognized opcode: {opcode}");
            }
        }
    }
}
