using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.Interop
{
    internal class UnmanagedAnalyses
    {
        [DllImport("Mba.FFI")]
        [SuppressGCTransition]
        [return: MarshalAs(UnmanagedType.U1)]
        public unsafe static extern bool CanChangeCoeff(ulong old_coeff, ulong new_coeff, ulong in_mask, ulong out_mask);

        [DllImport("Mba.FFI")]
        [SuppressGCTransition]
        [return: MarshalAs(UnmanagedType.U1)]
        public unsafe static extern bool CanRemoveMask(ulong coeff, ulong and_mask, ulong out_mask);

        [DllImport("Mba.FFI")]
        [SuppressGCTransition]
        public unsafe static extern void SimplifyDisjointSumMultiply(void* coeffsWithMasks, int len, ulong out_mask);
    }
}
