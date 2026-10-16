#pragma once
#include "judge_bootstrap.h"
#include "judge_tvd.h"
#include "judge_ref.h"
#include "judge_dualtrack.h"

struct OverconfidenceResult {
	DualTrack dt;
	double tvd_c = 0.0;
	double tvd_fi = 0.0;
	double tvd_joint = 0.0;
	bool is_true_overconfidence = false;
	bool is_false_overconfidence = false;
};

inline OverconfidenceResult analyze_overconfidence(const std::vector<JudgeRecord>& records, int B = 1000, double q_baseline = 0.5, double c_thr = 0.8, double fi_thr = 0.2, double tvd_threshold = 0.2, unsigned int seed = 42) {
	//1.点估计
	CellStats st = compute_cell_stats(records, q_baseline);
	DualTrack dt = make_dualtrack(st, c_thr, fi_thr);

	//2.Bootstrap
	auto boot = bootstrap_cell(records, B, q_baseline, seed);

	//3.TVD
	int bins = 20;
	auto h_c = histogram(boot.c, bins, 0.0, 1.0);
	auto h_fi = histogram(boot.fi, bins, 0.0, 1.0);
	auto r_c = ref_hist_c(bins);
	auto r_fi = ref_hist_fi(bins);

	OverconfidenceResult res;
	res.dt = dt;
	res.tvd_c = tvd(h_c, r_c);
	res.tvd_fi = tvd(h_fi, r_fi);
	res.tvd_joint = 0.5 * (res.tvd_c + res.tvd_fi);

	//4.真/伪过度自信
	if (dt.code_str == "01" && res.tvd_joint >= tvd_threshold) {
		res.is_true_overconfidence = true;
	}
	if (dt.code_str == "00" || (dt.code_str == "01" && res.tvd_joint < tvd_threshold)) {
		res.is_false_overconfidence = true;
	}
	return res;
}