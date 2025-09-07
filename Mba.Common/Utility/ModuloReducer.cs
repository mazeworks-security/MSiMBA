using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Utility
{
    public static class ModuloReducer
    {
        public static ulong GetMask(uint bitCount)
        {
            return ulong.MaxValue >> ((ushort)64 - (ushort)bitCount);
            //return maskTable[bitCount];
        }

        public static ulong ReduceToModulo(ulong value, uint bitCount)
        {
            return value & GetMask(bitCount);
        }
    }
}
