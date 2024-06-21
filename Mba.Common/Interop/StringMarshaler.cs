using Mba.Common.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mba.Interop
{
    public static class StringMarshaler
    {
        public static unsafe string AcquireString(nint pStr) => AcquireString((sbyte*)pStr);

        public static unsafe string AcquireString(sbyte* pStr)
        {
            if (pStr == null)
                return String.Empty;

            var result = SpanExtensions.AsString(pStr);
            return result;
        }
    }
}
