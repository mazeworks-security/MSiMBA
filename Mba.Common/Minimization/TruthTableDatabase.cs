﻿using Mba.Ast;
using Microsoft.Z3;
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
            var buffer = Tables[vars.Count - 1];
            var offset = 8 * index;

            var start = DecodeUint(buffer, (int)offset);
            return Deserialize(buffer, vars, (int)start);
        }

        private static AstNode Deserialize(byte[] buffer, IReadOnlyList<VarNode> variables, int offset)
        {
            var id = buffer[offset];
            offset += 4;

            switch(id)
            {
                case 2:
                    var symbolIdx = buffer[offset];
                    return variables[symbolIdx];

                case 8:
                case 9:
                case 10:
                    var aOffset = DecodeUint(buffer, offset);
                    offset += 4;
                    var bOffset = DecodeUint(buffer, offset);

                    var a = Deserialize(buffer, variables, (int)aOffset);

                    var b = Deserialize(buffer, variables, (int)bOffset);

                    var opcode = id switch
                    {
                        8 => AstKind.And,
                        9 => AstKind.Or,
                        10 => AstKind.Xor,
                    };

                    return AstNode.Binop(opcode, a, b);

                case 11:
                    var srcOffset = DecodeUint(buffer, offset);
                    var src = Deserialize(buffer, variables, (int)srcOffset);
                    return new NegNode(src);

                default:
                    throw new InvalidOperationException($"Cannot deserialize opcode {id}");
            }
        }

        private static uint DecodeUint(byte[] buffer, int start)
        {
            return BitConverter.ToUInt32(buffer.Skip(start).Take(4).ToArray());
        }
    }
}
