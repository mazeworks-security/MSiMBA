#pragma once

#include "polyfuncs.h"

template <typename PolyType>
std::vector<PolyType> autoreduce(const std::vector<PolyType>& F) {
    std::vector<PolyType> G(F);
    std::vector<PolyType> P;

    while (!G.empty()) {
        PolyType h = G.back();
        G.pop_back();
        h = normalform(h, P);
        if (!h.isZero()) {
            auto itP = P.begin();
            while ( itP != P.end() ) {
                if ((*itP).lm().isdivisible(h.lm())) {
                    G.push_back(*itP);
                    P.erase(itP);
                } else {
                    ++itP;
                }
            }
            P.push_back(h);
        }
    }

    int PSize = P.size();
    for (int i=0; i<PSize; ++i) {
        PolyType h = P.front();
        P.erase(P.begin());
        h = normalform(h, P);
        if (h.isZero()) {
            --PSize;
        } else {
            P.push_back(h);
        }
    }
    G = std::move(P);
    return G;
}


template <typename PolyType>
bool criteria(int i, int j, PolyType& p, PolyType &q,
              std::vector<std::vector<int>> &M, std::vector<PolyType> &G) {
    if (p.lm().isrelativelyprime(q.lm())) {
        return true;
    }
    int G_size = G.size();
    for (int k = 0; k<G_size; ++k) {
        if (M[i][k] && M[k][j] && G[k].lm().isdivisible(p.lm() * q.lm())) {
            return true;
        }
    }
    return false;
}

template <typename PolyType>
std::vector<PolyType> buchberger(const std::vector<PolyType>& F, const int num_vars) {
    std::vector<PolyType> G;
    std::vector<std::tuple<int, int>> pairs;

    G = autoreduce(F);
    int k = G.size();
    // Matrix M with treated pairs
    std::vector<std::vector<int>> M(k, std::vector<int>(k, 0));


    for (int i=-num_vars; i<k; ++i) {
        for (int j=0; j<k; ++j) {
            if (i<j) {
                pairs.push_back(std::make_tuple(i, j));
            }
        }
    }

    while (!pairs.empty()) {
        PolyType s, h;
        int i, j;

        std::tie(i, j) = pairs.front();
        pairs.erase(pairs.begin());

        if (i < 0) {
            auto Gj_lm = G[j].lm();

            typename PolyType::MonomType xi(std::abs(i));
            if (Gj_lm.isrelativelyprime(xi))
                continue;

            s = G[j] * xi;
        } else {
            M[i][j] = 1;
            PolyType p = G[i], q = G[j];
            // if (p.lm().isrelativelyprime(q.lm())) {
            //     continue;
            // }
            if (criteria(i, j, p, q, M, G)) {
                continue;
            }
            s = spoly(p, q);
        }

        h = normalform(s, G);
        if (!h.isZero()) {
            G.push_back(h);
            for (int i=-num_vars; i<k; ++i) {
                pairs.push_back(std::make_tuple(i, k));
            }

            ++k;
            // Enlarge M for new h entry
            for (auto& row: M) {
                row.push_back(0);
            }
            M.push_back(std::vector<int>(k, 0));

        }
    }

    G = autoreduce(G);

    return G;
}
