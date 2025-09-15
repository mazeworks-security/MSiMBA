#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>
#include <functional>

uint64_t count_colourings(int m, int n, bool increasing, uint64_t verboten, std::function<void(uint8_t*)> consumer) {
    /*
    * Enumerates the ways to assign inputs to a circuit by means of a
    * backtracking algorithm.
    *
    * m := number of inputs to circuit
    * n := size of set of distinct potential inputs
    * increasing := whether to count colourings up to permutation of inputs
    * verboten := bitmask asserting relations between inputs
    *
    * Note: this is intended if you wish to actually iterate through
    * the solutions, rather than merely count them. (Otherwise, there
    * are more efficient methods using enumerative combinatorics.) As
    * such, we provide a final argument:
    *
    * consumer := function to call on the current colouring
    */

    uint8_t currstack[16];
    uint8_t  maxstack[16];

    maxstack[0] = increasing ? 0 : (n - 1);

    uint64_t count = 0;

    for (int initial = 0; initial <= maxstack[0]; initial++) {

        int focus = 1;
        currstack[0] = initial;
        currstack[1] = 0;

        while (focus) {

            uint8_t limit = maxstack[focus - 1] + 1;
            limit = (limit >= n) ? (n - 1) : limit;

            if (currstack[focus] > limit) {
                focus -= 1;
            } else {
                uint64_t vb = verboten;
                if (focus >= 2) { vb = vb >> ((focus * (focus + 1)) / 2 - 2); }

                if ((vb & 1) && (currstack[focus - 1] >= currstack[focus])) { goto inccol; }

                for (int lb = 1; lb <= focus; lb++) {
                    if (((vb >> lb) & 1) && (currstack[focus - lb] == currstack[focus])) { goto inccol; }
                }

                maxstack[focus] = maxstack[focus - 1];
                if (maxstack[focus] < currstack[focus]) { maxstack[focus] = currstack[focus]; }
                if (focus < (m - 1)) {
                    focus += 1;
                    currstack[focus] = -1;
                } else {
                    /* valid complete colouring */
                    consumer(currstack);
                    count += 1;
                }
            }

            inccol:
            currstack[focus] += 1; /* increment colouring */
        }

    }

    return count;

}

std::vector<uint16_t> parse_circuit(std::string circuit) {

    std::vector<uint16_t> vec;

    uint16_t w = 1;

    for (int i = 0; i < circuit.length(); i++) {
        char c = circuit[i];

        if ((c == '#') || (c == '\n')) { break; }
        if (c == '*') { vec.push_back(w); w += 2; }
        if ((c >= '0') && (c <= '9')) { vec.push_back((c - '0') * 2); }
    }

    return vec;

}

uint64_t get_verboten(std::vector<uint16_t> &circuit) {

    uint64_t verboten = 1;

    for (uint64_t i = 2; i < circuit.size(); i += 2) {

        if (circuit[i] & 1) {
            int n = (circuit[i] >> 1);
            verboten |= ((1ull) << (((n + 1) * (n + 2)) / 2 - 2));
        } else if (circuit[i+1] & 1) {
            int n = (circuit[i+1] >> 1);
            for (int j = 0; j < 2; j++) {
                uint16_t c = circuit[circuit[i] + j];
                if (c & 1) {
                    int m = c >> 1;
                    verboten |= ((1ull) << ((n * (n + 1)) / 2 - 2 + (n - m)));
                }
            }
        }
    }

    return verboten;

} 

int main() {

    auto useless = [](uint8_t* cs){(void) cs;};

    std::string line;

    while (std::getline(std::cin, line)) {

        auto circuit = parse_circuit(line);
        int i = 0;
        for (auto it = circuit.begin(); it != circuit.end(); ++it) {
            i += ((*it) & 1);
        }
        auto verboten = get_verboten(circuit);
        auto ncols = count_colourings(i, 4, true, verboten, useless);
        std::cout << line << " : " << ncols << std::endl;

    }

    return 0;

}
