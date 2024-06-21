using Mba.Utility;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Emit;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Ast
{
    public enum AstKind
    {
        Const = 0,
        // For use in internal DSL generation code.
        WildCardConstant = 1,
        Var = 2,
        Ref = 3,

        Power = 4,
        Shl = 5,
        Add = 6,
        Mul = 7,
        And = 8,
        Or = 9,
        Xor = 10,
        Neg = 11,

        Ashr = 12, // arithmetic right shift
        Lshr = 13, // logical right shift
        Udiv = 14, // Unsigned division
        Urem = 15, // Unsigned remainder
    }

    public abstract class AstNode
    {
        private List<AstNode> operands;

        public List<AstNode> Children {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            { return operands; }
        }

        public AstKind Kind;

        public int index;

        public string Operator => AstFormatter.GetOperatorName(Kind);

        public uint BitSize { get; }

        protected abstract int OpCount { get; }

        public AstNode(AstKind kind, uint bitSize, params AstNode[] nodes)
        {
            Kind = kind;
            BitSize = bitSize;
            operands = new List<AstNode>(nodes.Length);
            for(int i = 0; i < nodes.Length; i++)
                operands.Add(nodes[i]);
        }

        public AstNode(AstKind kind, uint bitSize)
        {
            Kind = kind;
            BitSize = bitSize;
            operands = new();
        }

        public void SetOperand(int index, AstNode operand)
        {
            var last = operands[index];
            if (last != null && last.BitSize != operand.BitSize)
                throw new InvalidOperationException($"Cannot replace operand {last} with {operand}. The bit sizes do not match.");
            operands[index] = operand;
        }

        public void SetOperands(List<AstNode> operands)
        {
            this.operands = operands;
        }

        public void AddOperand(AstNode operand)
        {
            if (BitSize != operand.BitSize)
                throw new InvalidOperationException($"Cannot add operand {operand} to ast. The bit sizes do not match.");
            operands.Add(operand);
        }

        public void AddOperands(IEnumerable<AstNode> operands)
        {
            foreach (var op in operands)
                AddOperand(op);
        }

        public override bool Equals(object? obj)
        {
            if (obj is null)
                return false;
            if (ReferenceEquals(this, obj))
                return true;
            if (obj is not AstNode astNode)
                return false;
            if (BitSize != astNode.BitSize || Kind != astNode.Kind)
                return false;
            // Lazily use string equality instead of recursively comparing.
            return AstFormatter.FormatAst(this) == AstFormatter.FormatAst(astNode);
        }

        public override string ToString() => AstFormatter.FormatAst(this);

        public static AstNode Binop(AstKind kind, AstNode op0, AstNode op1)
        {
            return kind switch
            {
                AstKind.Add => new AddNode(op0, op1),
                AstKind.Mul => new MulNode(op0, op1),
                AstKind.And => new AndNode(op0, op1),
                AstKind.Or => new OrNode(op0, op1),
                AstKind.Xor => new XorNode(op0, op1),
                _ => throw new InvalidOperationException($"Unrecognized binop: {kind}")
            };
        }
    }
}
