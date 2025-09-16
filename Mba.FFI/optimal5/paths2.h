#pragma once

void hp_path2(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path3(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path4(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path5(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path6(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path7(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path8(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path9(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path10(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path11(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path12(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path13(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path14(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path15(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path16(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path17(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path18(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path19(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path20(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path21(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path22(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path23(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path24(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path25(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path26(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path27(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path28(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path29(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path30(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path31(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path32(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path33(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path34(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path35(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path36(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path37(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path38(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path39(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path40(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path41(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path42(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path43(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path44(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path45(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path46(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path47(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path48(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path49(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path50(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path51(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path52(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path53(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path54(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path55(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path56(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path57(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path58(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path59(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path60(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path61(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path62(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path63(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path64(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 7)) & 0x00aa00aa; t ^= d + (d << 7); PERM_SWAP(2, 5);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path65(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path66(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path67(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path68(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path69(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path70(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path71(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path72(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path73(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path74(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 6)) & 0x00cc00cc; t ^= d + (d << 6); PERM_SWAP(2, 4);
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    d = (t ^ (t >> 3)) & 0x0a0a0a0a; t ^= d + (d << 3); PERM_SWAP(3, 5);
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path75(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 2)) & 0x0c0c0c0c; t ^= d + (d << 2); PERM_SWAP(3, 4);
    t = ((t & 0x55555555u) << 1) + ((t & 0xaaaaaaaau) >> 1); perm[5] = -perm[5];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    t = ((t & 0x33333333u) << 2) + ((t & 0xccccccccu) >> 2); perm[4] = -perm[4];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

void hp_path76(uint32_t &t, uint32_t &tt, int16_t *perm, int16_t *element) {
    uint32_t d = 0;
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x00ff00ffu) << 8) + ((t & 0xff00ff00u) >> 8); perm[2] = -perm[2];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    t = ((t & 0x0f0f0f0fu) << 4) + ((t & 0xf0f0f0f0u) >> 4); perm[3] = -perm[3];
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 1)) & 0x22222222; t ^= d + (d << 1); PERM_SWAP(4, 5);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
    d = (t ^ (t >> 4)) & 0x00f000f0; t ^= d + (d << 4); PERM_SWAP(2, 3);
    if (t & 0x80000000u) { t = ~t; perm[0] ^= 1; }
    if (t < tt) { memcpy(element, perm, 6 * sizeof(uint16_t)); tt = t; }
}

const static uint8_t hpaths[1984] = {
    0, 2, 0, 3, 0, 0, 4, 5, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 7, 7, 8, 9, 0, 10, 0, 0, 9,
    9, 55, 9, 23, 56, 0, 0, 22, 0, 0, 22, 0, 0, 0, 0, 0, 0, 57, 58, 59, 0, 0, 0, 0, 0, 60, 0, 0, 0, 11, 12,
    2, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 68, 0, 0, 0, 0, 0, 0, 0, 17, 0, 9, 0, 69, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 15, 70, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 17, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 5, 5, 18, 18, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 1, 0, 19, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 12, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 12, 12, 0, 0, 9, 9, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 20, 0, 7, 21, 12, 0, 0, 22, 9, 0, 22, 0, 23, 0, 0, 0, 22, 0, 0, 0,
    0, 0, 0, 7, 7, 9, 9, 1, 1, 0, 0, 9, 9, 0, 0, 0, 0, 0, 0, 0, 72, 24, 12, 24, 12, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 12, 12, 0, 0, 9, 26,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 73, 0, 74, 0, 0, 60, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 28, 0, 29, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 30, 31, 31, 1, 1, 0, 0, 31, 32, 0, 0, 0, 0,
    0, 0, 0, 0, 12, 12, 12, 12, 1, 1, 1, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 34,
    12, 0, 13, 0, 35, 36, 1, 0, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 12, 12, 12, 12, 10, 1, 0, 10, 1, 38, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 39, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 41, 18, 0, 0, 4, 5, 0, 4, 0,
    41, 0, 0, 0, 42, 0, 0, 43, 43, 0, 0, 9, 0, 9, 9, 1, 44, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 45, 9, 0, 19, 0, 0, 16, 9, 0, 0, 0, 0, 0, 0, 0, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 47, 1, 48, 1, 48, 1, 48, 49, 0, 48, 0, 0, 0, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 28, 61, 0, 0, 5, 0, 50, 9, 48, 0, 0, 0, 51, 0, 0, 52, 0, 0, 0, 0, 7, 0, 0, 62, 0, 24, 0, 0,
    58, 9, 0, 63, 0, 64, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 7, 20, 65, 9, 1, 33, 0, 0, 26, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 24, 0, 0, 66, 67, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 5, 5, 18, 0, 0, 0, 5, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    76, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 53, 1, 0, 0, 1, 54, 1, 0, 0, 0, 0, 0, 19, 0, 0, 0};

const static hpath fpointers[] = {hp_exhaust, hp_null, hp_path2, hp_path3, hp_path4, hp_path5, hp_path6, hp_path7, hp_path8, hp_path9, hp_path10, hp_path11, hp_path12, hp_path13, hp_path14, hp_path15, hp_path16, hp_path17, hp_path18, hp_path19, hp_path20, hp_path21, hp_path22, hp_path23, hp_path24, hp_path25, hp_path26, hp_path27, hp_path28, hp_path29, hp_path30, hp_path31, hp_path32, hp_path33, hp_path34, hp_path35, hp_path36, hp_path37, hp_path38, hp_path39, hp_path40, hp_path41, hp_path42, hp_path43, hp_path44, hp_path45, hp_path46, hp_path47, hp_path48, hp_path49, hp_path50, hp_path51, hp_path52, hp_path53, hp_path54, hp_path55, hp_path56, hp_path57, hp_path58, hp_path59, hp_path60, hp_path61, hp_path62, hp_path63, hp_path64, hp_path65, hp_path66, hp_path67, hp_path68, hp_path69, hp_path70, hp_path71, hp_path72, hp_path73, hp_path74, hp_path75, hp_path76};

