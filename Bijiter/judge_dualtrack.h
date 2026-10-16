#pragma once
// judge_dualtrack.h
#pragma once
#include "judge_metrics.h"
#include <string>

struct DualTrack {
    int code; // 11/10/1/0 (注意 01 用整数 1 表示会歧义，建议存字符串)
    std::string code_str;
    std::string name;
};

inline DualTrack make_dualtrack(const CellStats& st, double c_thr = 0.8, double fi_thr = 0.2) {
    bool stable = (st.C >= c_thr);
    bool robust = (st.Fi <= fi_thr);

    DualTrack dt;
    if (stable && robust) { dt.code_str = "11"; dt.name = "TrustWorthy"; }
    else if (stable && !robust) { dt.code_str = "01"; dt.name = "ConfidentlyBiased"; }
    else if (!stable && robust) { dt.code_str = "10"; dt.name = "NoisyButFair"; }
    else { dt.code_str = "00"; dt.name = "Chaos"; }

    dt.code = std::stoi(dt.code_str); // 仅用于排序；展示请用 code_str
    return dt;
}
