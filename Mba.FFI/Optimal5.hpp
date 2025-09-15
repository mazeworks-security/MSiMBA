#pragma once

#include "optimal5/optimal5.h"
#include "common.hpp"

FFI_EXPORT opt5::Optimiser* GetOptimal5Db(const char* database)
{
	return new opt5::Optimiser(database);
}

FFI_EXPORT void LookupOptimal5(opt5::Optimiser* opt, uint32_t truthTable, opt5::gatevec_fast* output)
{
	*output = opt->lookup(truthTable);
}

FFI_EXPORT void PrintGatevec(opt5::gatevec_fast* pGv) {

    auto gv = *pGv;
    std::ostringstream ss;

    uint16_t* x = gv.gates;

    for (int idx = 0; idx < gv.n_gates; idx++) {

        uint32_t a = x[idx];
        ss << "x" << (idx + 6) << " = ";

        uint32_t i1 = (a >> 4) & 31;
        uint32_t i2 = (a >> 10) & 31;

        std::cout << "idx: " << idx << ", a: " << a << ", i1 " << i1 << ", i2 " << i2 << std::endl;

        switch (a & 7) {
        case 3: ss << "x" << i1 << " ^ x" << i2 << "; "; break;
        case 4: ss << "x" << i1 << " & x" << i2 << "; "; break;
        case 5: ss << "x" << i1 << " &~ x" << i2 << "; "; break;
        case 6: ss << "x" << i2 << " &~ x" << i1 << "; "; break;
        case 7: ss << "x" << i1 << " | x" << i2 << "; "; break;
        }

    }

    ss << "result = ";
    if (gv.output & 1) { ss << "~"; }
    if (gv.output >> 1) { ss << "x"; }
    ss << (gv.output >> 1) << ";";
    std::cout << ss.str().c_str() << std::endl;
}