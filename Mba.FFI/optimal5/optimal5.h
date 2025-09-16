#pragma once

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "flat_hash_map.hpp"

namespace opt5 {

    /* Swap two entries in the permutation vector */
    #define PERM_SWAP(x, y) perm[x] ^= perm[y]; perm[y] ^= perm[x]; perm[x] ^= perm[y]

    /* Hamiltonian tour of the vertices of the omnitruncated tesseract: */
    #define BITONIC4    3, 2, 1, 0, 1, 2, 3
    #define TRITONIC4   BITONIC4, 2, BITONIC4, 1, BITONIC4, 0, BITONIC4, 1, BITONIC4, 2, BITONIC4
    #define TETRONIC4   TRITONIC4, 1, TRITONIC4, 0, TRITONIC4, 1, TRITONIC4
    const static int8_t delta4[384] __attribute__((aligned(64))) = {-1, TETRONIC4, 0, TETRONIC4};

    uint16_t canonise_circuit_4(uint16_t orig, int16_t *element) {

        int16_t perm[6]; for (int j = 0; j < 6; j++) { perm[j] = j; }
        uint16_t t = orig; uint16_t tt = 0xffff; uint16_t d = 0;

        for (int j = 0; j < 384; j++) {
            switch (delta4[j]) {
                case 0: t = ((t & 0x00ff) << 8) + ((t & 0xff00) >> 8); perm[2] = -perm[2];
                // fall through
                case-1: if (t & 0x8000u) { t = ~t; perm[0] ^= 1; }
                break;
                case 1: d = (t ^ (t >> 4)) & 0x00f0;
                        t ^= d + (d << 4); PERM_SWAP(2, 3);
                break;
                case 2: d = (t ^ (t >> 2)) & 0x0c0c;
                        t ^= d + (d << 2); PERM_SWAP(3, 4);
                break;
                case 3: d = (t ^ (t >> 1)) & 0x2222; 
                        t ^= d + (d << 1); PERM_SWAP(4, 5);
                break;
            }
            if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
        }

        return tt;
    }

    std::vector<uint16_t> get_reps(uint16_t initial_t) {

        std::vector<uint16_t> x;

        uint16_t t = initial_t; uint16_t d = 0;

        for (int j = 0; j < 384; j++) {
            switch (delta4[j]) {
                case 0: t = ((t & 0x00ff) << 8) + ((t & 0xff00) >> 8);
                // fall through
                case-1: if (t & 0x8000u) { t = ~t; }
                break;
                case 1: d = (t ^ (t >> 4)) & 0x00f0;
                        t ^= d + (d << 4);
                break;
                case 2: d = (t ^ (t >> 2)) & 0x0c0c;
                        t ^= d + (d << 2);
                break;
                case 3: d = (t ^ (t >> 1)) & 0x2222; 
                        t ^= d + (d << 1);
                break;
            }
            x.push_back(t);
        }

        return x;
    }

    std::vector<std::vector<uint16_t> > get_cayley() {

        std::vector<std::vector<uint16_t> > cayley;
        cayley.push_back(get_reps(0x012d));

        for (int j = 1; j < 384; j++) {
            cayley.push_back(get_reps(cayley[0][j]));
        }

        return cayley;

    }

    std::vector<int> stabilise_circuit_4(uint16_t orig) {

        std::vector<int> x;

        uint16_t t = orig; uint16_t d = 0;

        for (int j = 0; j < 384; j++) {
            switch (delta4[j]) {
                case 0: t = ((t & 0x00ff) << 8) + ((t & 0xff00) >> 8);
                // fall through
                case-1: if (t & 0x8000u) { t = ~t; }
                break;
                case 1: d = (t ^ (t >> 4)) & 0x00f0;
                        t ^= d + (d << 4);
                break;
                case 2: d = (t ^ (t >> 2)) & 0x0c0c;
                        t ^= d + (d << 2);
                break;
                case 3: d = (t ^ (t >> 1)) & 0x2222; 
                        t ^= d + (d << 1);
                break;
            }
            if (t == orig) { x.push_back(j); }
        }

        return x;
    }

    #include "paths.h"

    uint32_t canonise_circuit_fast(uint32_t orig, int16_t *element, uint16_t *min4) {
        /*
        * This is intended to be much faster (amortized) than Knuth's
        * original circuit canonisation routine. This assumes orig
        * is strictly less than 0x80000000u, i.e. is normalised.
        */

        int16_t perm[6]; for (int j = 0; j < 6; j++) { perm[j] = j; }
        uint32_t t = orig; uint32_t d = 0;

        if ((t == 0) || (t == 0x69969669)) {
            // Deal with the highly symmetric truth tables
            memcpy(element, perm, 12); return t;
        }

        uint32_t frames[5];

        frames[0] = t;
        d = (t ^ (t >> 8)) & 0x0000ff00; t ^= d + (d << 8);
        frames[1] = t;
        d = (t ^ (t >> 12)) & 0x0000f0f0; t ^= d + (d << 12);
        frames[2] = t;
        d = (t ^ (t >> 14)) & 0x0000cccc; t ^= d + (d << 14);
        frames[3] = t;
        d = (t ^ (t >> 15)) & 0x0000aaaa; t ^= d + (d << 15);
        frames[4] = t;

        uint16_t smask = 0;
        uint16_t smallest = 0xffff;

        for (int i = 0; i < 10; i++) {

            uint16_t xx = ((uint16_t*) frames)[i ^ 1];
            if (xx & 0x8000u) { xx = ~xx; }
            xx = min4[xx];

            if (xx < smallest) {
                smallest = xx; smask = (1 << i);
            } else if (xx == smallest) {
                smask |= (1 << i);
            }
        }

        uint32_t tt = 0xffffffff;

        // for (int i = 0; i < 10; i++) {
        while (smask) {

            uint16_t powof2 = smask & (-smask);
            smask ^= powof2;
            int i = __builtin_ctz(powof2);

            // Restore frame
            t = frames[i / 2];

            perm[0] = 0;
            perm[1] = (i / 2) + 1;
            perm[2] = (i < 2) + 1;
            perm[3] = (i < 4) + 2;
            perm[4] = (i < 6) + 3;
            perm[5] = (i < 8) + 4;
            if (i & 1) { perm[1] = -perm[1]; t = (t << 16) | (t >> 16); }
            if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }

            if (smallest == 0) {

                hp_exhaust(t, tt, perm, element);
                return tt;

            } else {

                uint8_t *bytearray = ((uint8_t*) (min4 + 32768));
                int loc = hpaths[smallest % 1984];

                while (true) {
                    // follow sequence of directions

                    switch (bytearray[t >> 16]) {
                        case 0:  goto https;
                        case 1:  d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
                        break;
                        case 2:  d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
                        break;
                        case 3:  d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
                        break;
                        case 4:  d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
                        break;
                        case 5:  d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
                        break;
                        case 6:  d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
                        break;
                        case 7:  t = ~t; perm[0] ^= 1;
                        // fall through
                        case 8:  t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
                        break;
                        case 9:  t = ~t; perm[0] ^= 1;
                        // fall through
                        case 10: t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
                        break;
                        case 11: t = ~t; perm[0] ^= 1;
                        // fall through
                        case 12: t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
                        break;
                        case 13: t = ~t; perm[0] ^= 1;
                        // fall through
                        case 14: t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
                        break;
                    }
                }

                // We reduce modulo 1984 as a perfect hash function on the set of 222 canonical truth tables:
                https://en.wikipedia.org/wiki/Perfect_hash_function

                hpath function_pointer = fpointers[loc];
                if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
                function_pointer(t, tt, perm, element);

            }
        }

        return tt;
    }

    uint32_t apply_move(uint32_t x, int move) {

        uint32_t t = x; uint32_t d = 0;

        switch (move) {
            case 0:
            break;
            case 1:  d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4);
            break;
            case 2:  d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2);
            break;
            case 3:  d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1);
            break;
            case 4:  d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6);
            break;
            case 5:  d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3);
            break;
            case 6:  d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7);
            break;
            case 7:  t = ~t;
            // fall through
            case 8:  t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8);
            break;
            case 9:  t = ~t;
            // fall through
            case 10: t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4);
            break;
            case 11: t = ~t;
            // fall through
            case 12: t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2);
            break;
            case 13: t = ~t;
            // fall through
            case 14: t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1);
            break;
        }

        return t;

    }

    int forward_perm(int orig, int16_t *element) {

        if ((orig >=  1) && (orig <=  5)) { return  element[ orig]; }
        if ((orig >= -5) && (orig <= -1)) { return -element[-orig]; }
        return orig;

    }

    void reverse_perm(int16_t *x) {
        /*
        * Find the inverse of a NPN transformation.
        */

        int16_t y[6] = {0, 0, 0, 0, 0, 0};

        for (int j = 1; j <= 5; j++) { y[abs(x[j])] = j * (x[j] / abs(x[j])); }
        for (int j = 1; j <= 5; j++) { x[j] = y[j]; }

    }

    uint32_t apply_oper8r(uint32_t opa, char op, uint32_t opb) {

        uint32_t res = 0;
        if (op == '&' ) { res = opa & opb; }
        if (op == '|' ) { res = opa | opb; }
        if (op == '\\') { res = opa & (~opb); }
        if (op == '/' ) { res = (~opa) & opb; }
        if (op == '+' ) { res = opa ^ opb; }
        return res;

    }

    struct gatevec_fast {

        uint16_t gates[12]; // 24 bytes = 12 gates
        uint8_t n_gates;    // between 0 and 12, inclusive
        uint8_t output;

    };

    typedef ska::flat_hash_map<uint32_t, gatevec_fast> chainstore;

    void bytes2gv(std::vector<uint8_t> &bytes, chainstore &chains) {

        for (size_t j = 0; j < bytes.size(); j += 15) {

            gatevec_fast a;

            uint64_t g[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            uint64_t b[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            for (int i = 0; i < 15; i++) { b[i] = bytes[i+j]; }

            for (int i = 0; i < 3; i++) {
                uint64_t x = b[5*i] | (b[5*i+1] << 8) | (b[5*i+2] << 16) | (b[5*i+3] << 24) | (b[5*i+4] << 32);
                g[4*i] = x & 1023;
                g[4*i+1] = (x >> 10) & 1023;
                g[4*i+2] = (x >> 20) & 1023;
                g[4*i+3] = (x >> 30) & 1023;
            }

            std::vector<uint32_t> calcs;
            calcs.push_back(0x00000000); calcs.push_back(0x0000ffff);
            calcs.push_back(0x00ff00ff); calcs.push_back(0x0f0f0f0f);
            calcs.push_back(0x33333333); calcs.push_back(0x55555555);

            a.n_gates = 12;

            for (int i = 0; i < 12; i++) {
                if (g[i] == 0) { a.n_gates = i; break; }

                char op = "&|+\\"[g[i] >> 8];
                uint16_t gate = (0x5374 >> ((g[i] >> 6) & 0xc)) & 0xf;
                uint64_t i1 = (g[i] & 15) + 1;
                uint64_t i2 = ((g[i] >> 4) & 15) + 1;

                gate |= (i1 << 4) | (i2 << 10);
                a.gates[i] = gate;
                uint32_t opa = calcs[i1];
                uint32_t opb = calcs[i2];
                calcs.push_back(apply_oper8r(opa, op, opb));
            }

            a.output = (5 + a.n_gates) * 2;
            uint32_t w = calcs[calcs.size() - 1];
            chains[w] = a;

        }

    }

    void populate_perm(uint8_t *perm, int16_t *element) {
        for (int i = 1; i < 6; i++) {
            int16_t x = element[i];
            if (x < 0) { x = -x; perm[i] = 1; }
            perm[i] ^= (i ^ x) << 1;
        }
    }

    void apply_perm(gatevec_fast *gv, int16_t *element) {

        if (gv->n_gates == 0) {
            int16_t x = element[gv->output >> 1];
            if (x < 0) { x = -x; gv->output ^= 1; }
            gv->output = (x << 1) | (gv->output & 1);
        } else {
            uint64_t perm[2] = {0ull, 0ull};
            populate_perm((uint8_t*) perm, element);

            // std::cout << "Permutations: " << perm[0] << " " << perm[1] << std::endl;

            uint64_t y[4];
            uint64_t z[4];

            memcpy(z, gv->gates, 24);

            for (int i = 0; i < 3; i++) {
                y[i] = ((z[i] >> 4) & 0x001f001f001f001full) | ((z[i] >> 2) & 0x1f001f001f001f00ull);
            }

            y[3] = 0; z[3] = 0;

            asm (
            "movups (%0), %%xmm0 \n\t"
            "movups (%1), %%xmm2 \n\t"
            "movups 16(%1), %%xmm3 \n\t"
            "movdqa %%xmm0, %%xmm1 \n\t"
            "pshufb %%xmm2, %%xmm0 \n\t"
            "pshufb %%xmm3, %%xmm1 \n\t"
            "movups %%xmm0, (%1) \n\t"
            "movups %%xmm1, 16(%1) \n\t"
            : /* no output operands -- implicitly volatile */
            : "r" (perm), "r" (y)
            : "xmm0", "xmm1", "xmm2", "xmm3", "memory");

            for (int i = 0; i < 3; i++) {
                z[i] ^= ((y[i] & 0x003f003f003f003full) << 3) | ((y[i] & 0x3f003f003f003f00ull) << 1);
            }

            memcpy(gv->gates, z, 24);
        }

        gv->output ^= element[0];

    }

    std::string print_gatevec(gatevec_fast &gv) {

        std::ostringstream ss;

        uint16_t *x = gv.gates;

        for (int idx = 0; idx < gv.n_gates; idx++) {

            uint32_t a = x[idx];
            ss << "x" << (idx + 6) << " = ";

            uint32_t i1 = (a >>  4) & 31;
            uint32_t i2 = (a >> 10) & 31;

            switch (a & 7) {
                case 3: ss << "x" << i1 << " ^ x" << i2 << "; "; break;
                case 4: ss << "x" << i1 << " & x" << i2 << "; "; break;
                case 5: ss << "x" << i1 << " &~ x" << i2 << "; "; break;
                case 6: ss << "x" << i2 << " &~ x" << i1 << "; "; break;
                case 7: ss << "x" << i1 << " | x" << i2 << "; "; break;
            }

        }

        ss << "result = ";
        if (gv.output &  1) { ss << "~"; }
        if (gv.output >> 1) { ss << "x"; }
        ss << (gv.output >> 1) << ";";
        return ss.str();

    }

    uint32_t normalise_chain_fast(gatevec_fast * __restrict__ gv, uint16_t * __restrict__ tableau) {

        uint16_t *x = gv->gates;
        uint32_t negations = 0;

        uint32_t y[18];
        y[0] = 0x00000000; y[1] = 0x0000ffff; y[2] = 0x00ff00ff;
        y[3] = 0x0f0f0f0f; y[4] = 0x33333333; y[5] = 0x55555555;

        for (int idx = 0; idx < gv->n_gates; idx++) {

            uint32_t a = x[idx];
            a ^= (((negations >> ((a >> 4) & 31)) & 1) << 3) ^ (((negations >> ((a >> 10) & 31)) & 1) << 9);
            a ^= tableau[a & 0x020f];
            negations |= (a >> 15) << (idx + 6);
            x[idx] = a & 0x7fff;

            uint32_t i1 = y[(a >>  4) & 31];
            uint32_t i2 = y[(a >> 10) & 31];
            uint32_t res = (a & 4) ? (i1 & i2) : 0;
            res ^=         (a & 2) ? i2 : 0;
            res ^=         (a & 1) ? i1 : 0;
            y[idx + 6] = res;
        }

        uint8_t active_gate = gv->output >> 1;
        gv->output ^= ((negations >> active_gate) & 1);

        uint32_t res = y[active_gate];
        res ^= (gv->output & 1) ? 0xffffffffu : 0;
        return res;

    }

    gatevec_fast lookup_transform(uint32_t iv, chainstore &chains, uint16_t *min4) {
        /*
        * Retrieves a circuit from the hash table, abstracting away all of the
        * tedious mucking about with NPN equivalence classes.
        */

        if (iv == 0xffffffffu) {
            gatevec_fast a = chains[0];
            a.output = 1; return a;
        }

        int16_t element[6];
        uint32_t w;
        uint32_t v = iv;
        if (iv & 0x80000000u) { v = ~v; }
        w = canonise_circuit_fast(v, element, min4);
        if (iv & 0x80000000u) { element[0] ^= 1; }

        gatevec_fast a = chains[w];
        apply_perm(&a, element);
        uint32_t x = normalise_chain_fast(&a, min4 + 49152);

        if (iv != x) { std::cerr << "Major disaster! " << x << " != " << iv << " (canonised " << w << ")" << std::endl; exit(1); }

        return a;

    }

    void load_chains(chainstore &chains, std::string filename) {

        std::vector<uint8_t> bv(616124 * 15);

        std::ifstream infile(filename, std::ios::in | std::ios::binary); 
        infile.read(((char*) &bv[0]), bv.size());
        infile.close();

        // Initialise with primitive functions (constant and identity):
        chains[0x00000000].output = 0;
        chains[0x0000ffff].output = 2;

        bytes2gv(bv, chains);
    }

    void run_dijkstra(uint16_t *min4, uint8_t *bytearray) {

        uint16_t weights[32768];

        for (uint16_t i = 0; i < 32768; i++) { weights[i] = 999; bytearray[i] = 0; }
        for (uint16_t i = 0; i < 32768; i++) { weights[min4[i]] = 0; }

        int improvements;

        do {

            improvements = 0;

            for (uint32_t i = 0; i < 32768; i++) {

                for (int m = 1; m <= 14; m++) {
                    uint32_t j = apply_move(i << 16, m) >> 16;
                    if ((j < 32768) && (weights[j] + 1 < weights[i])) {
                        bytearray[i] = m; weights[i] = weights[j] + 1; improvements += 1;
                    }
                }

            }

        } while (improvements > 0);

    }

    void init_tableau(uint16_t *tableau) {

        for (int i = 0; i < 8; i++) {
            uint16_t j = i << 1;
            tableau[i] = 0; // (j >> 1) | (j << 15);
            j ^= ((j & 10) >> 1);
            tableau[i +   8] = ((j >> 1) | (j << 15)) ^ i;
            j ^= ((j & 12) >> 2);
            tableau[i + 520] = ((j >> 1) | (j << 15)) ^ i;
            j ^= ((j & 10) >> 1);
            tableau[i + 512] = ((j >> 1) | (j << 15)) ^ i;
        }

    }

    void prepare_table(uint16_t *min4) {

        for (uint16_t i = 0; i < 32768; i++) {

            int16_t element[6];
            min4[i] = canonise_circuit_4(i, element);

        }

        run_dijkstra(min4, (uint8_t*) (min4 + 32768));

        init_tableau(min4 + 49152);

    }

    class Optimiser {

        private:
        uint16_t* min4;

        public:
        chainstore chains;

        Optimiser(std::string filename) {
            load_chains(chains, filename);
            min4 = (uint16_t*) malloc(102400);
            prepare_table(min4);
        }

        ~Optimiser() { free(min4); }

        gatevec_fast lookup(uint32_t iv) {
            return lookup_transform(iv, chains, min4);
        }
    };

}
