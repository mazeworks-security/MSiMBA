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
        private static UInt128[] maskTable;

        static ModuloReducer()
        {
            maskTable = ComputeMaskTable(512);
        }

        /// <summary>
        /// For every bit size from 0 to maxBitcount, compute a bitwise mask that would truncate any integer
        /// to fit into the bit size. 
        /// E.g. 32 bit mask = 4,294,967,295 (aka uint max value)
        /// </summary>
        /// <param name="maxBitcount"></param>
        /// <returns></returns>
        private static UInt128[] ComputeMaskTable(uint maxBitcount)
        {
            var table = new UInt128[maxBitcount];
            for (int i = 0; i < 512; i++)
            {
                // For each bit index up to the current bitcount, set the bit to 1.
                UInt128 mask = 0;
                for (int bitIndex = 0; bitIndex < i; bitIndex++)
                    mask |= (((UInt128)1) << bitIndex);

                table[i] = mask;
            }

            return table;
        }

        public static UInt128 GetMask(uint bitCount)
        {
            return maskTable[bitCount];
        }

        public static UInt128 ReduceToModulo(UInt128 value, uint bitCount)
        {
            return value & maskTable[bitCount];
        }
    }
}
