using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Common.MSiMBA
{
    public static class RangeUtil
    {
        public static List<int> Get(int count) => Enumerable.Range(0, count).ToList();

        public static List<int> Get(long count) => Enumerable.Range(0, (int)count).ToList();

        public static List<int> Get(int start, int stop)
        {
            var foo1 = Enumerable.Range(start, stop - start).ToList(); ;
            return foo1;
        }

        public static List<int> Get(int start, long stop) => Enumerable.Range(start, (int)stop - (int)start).ToList();

        public static List<int> Get(int start, int stop, int step)
        {
            var output = new List<int>();

            if (start == stop)
                return new List<int>() { };

            if (start > stop)
            {
                for (int i = start; i > stop; i += step)
                {
                    output.Add(i);
                }
            }

            else
            {

                for (int i = start; i < stop; i += step)
                {
                    output.Add(i);
                }

            }
            return output;

        }

        private static IEnumerable<int> GetWithStep(int min, int max, int step)
        {
            for (int i = min; i <= max; i = checked(i + step))
                yield return i;
        }
    }
}
