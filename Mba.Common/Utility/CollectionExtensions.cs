using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Utility
{
    public static class CollectionExtensions
    {
        public static void AddRange<T>(this HashSet<T> me, IEnumerable<T> other)
        {
            foreach (var item in other)
                me.Add(item);
        }

        public static int IndexOf<T>(this IReadOnlyList<T> self, T elementToFind)
        {
            int i = 0;
            foreach (T element in self)
            {
                if (Equals(element, elementToFind))
                    return i;
                i++;
            }
            return -1;
        }
    }
}
