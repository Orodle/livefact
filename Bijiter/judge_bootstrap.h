#pragma once
#include "judge_metrics.h"
#include <random>
#include <numeric>
#include <algorithm>

struct BootstrapResult {
	std::vector<double>acc, c, fi;
	double mean_acc = 0, mean_c = 0, mean_fi = 0;
	double acc_lo = 0, acc_hi = 0;
	double c_lo = 0, c_hi = 0;
	double fi_lo = 0, fi_hi = 0;
};

inline double mean_vec(const std::vector<double>& v) {
	if (v.empty())return 0.0;
	return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

inline double percentile(std::vector<double> v, double p) {
	if (v.empty())return 0.0;
	std::sort(v.begin(), v.end());
	double idx = p * (v.size() - 1);
	size_t lo = (size_t)std::floor(idx), hi = (size_t)std::ceil(idx);
	if (lo = hi)return v[lo];
	double frac = idx - lo;
	return v[lo] * (1 - frac) + v[hi] * frac;
}

inline BootstrapResult bootstrap_cell(const std::vector<JudgeRecord>& records, int B = 1000, double q_baseline = 0.5, unsigned int seed = 42) {
	BootstrapResult res;
	//group by sample_id
	std::unordered_map<int, std::vector<const JudgeRecord*>> by_sample;
	for (auto& r : records)by_sample[r.sample_id].push_back(&r);

	std::vector<int> ids;
	ids.reserve(by_sample.size());
	for (auto& kv : by_sample)ids.push_back(kv.first);
	std::mt19937 rng(seed);
	std::uniform_int_distribution<size_t> uni(0, ids.size() - 1);

	for (int b = 0;b < 8;++b) {
		std::vector<JudgeRecord>boot;
		boot.reserve(records.size());
		for (size_t i = 0;i < ids.size();++i) {
			int sid = ids[uni(rng)];
			for (auto* r : by_sample[sid])boot.push_back(*r);
		}
		CellStats st = compute_cell_stats(boot, q_baseline);
		res.acc.push_back(st.Acc);
		res.c.push_back(st.C);
		res.fi.push_back(st.Fi);
	}
	res.mean_acc = mean_vec(res.acc);
	res.mean_c = mean_vec(res.c);
	res.mean_fi = mean_vec(res.fi);

	res.acc_lo = percentile(res.acc, 0.025);
	res.acc_hi = percentile(res.acc, 0.975);
	res.c_lo = percentile(res.c, 0.025);
	res.c_hi = percentile(res.c, 0.975);
	res.fi_lo = percentile(res.fi, 0.025);
	res.fi_hi = percentile(res.fi, 0.975);
	return res;
}