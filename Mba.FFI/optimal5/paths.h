#pragma once

    /* Function pointer for a Hamiltonian path through the hyperoctahedral group */
    typedef void (*hpath)(uint32_t&, uint32_t&, int16_t*, int16_t*);

    /* Empty Hamiltonian path */
    void hp_null(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {

        (void) t; (void) tt; (void) perm; (void) element;

    }

    /* Full Hamiltonian path which visits all 384 elements of the group */
    void hp_exhaust(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {

        uint32_t d = 0;

        for (int j = 0; j < 384; j++) {
            switch (delta4[j]) {
                case 0: t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
                // fall through
                case-1: if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
                break;
                case 1: d = (t ^ (t >> 4)) & 0x00f000f0;
                        t ^= d + (d << 4); PERM_SWAP(2, 3);
                break;
                case 2: d = (t ^ (t >> 2)) & 0x0c0c0c0c;
                        t ^= d + (d << 2); PERM_SWAP(3, 4);
                break;
                case 3: d = (t ^ (t >> 1)) & 0x22222222; 
                        t ^= d + (d << 1); PERM_SWAP(4, 5);
                break;
            }
            if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
        }

    }

    /* Other Hamiltonian paths, computed using the Held-Karp algorithm */
    #include "paths2.h"

