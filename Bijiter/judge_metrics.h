#pragma once
#include "judge_types.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>

struct CellStats {
	double Acc = 0.0;
	double C = 0.0;
	double Fi = 0.0;
	double q = 0.5;
	double T = 0.0;
};

inline Label mode_label(const std::vector<Label>& v) {
	int cnt[3] = { 0,0,0 };
	for (auto x : v) cnt[static_cast<int>(x)]++;
	if (cnt[0] >= cnt[1] && cnt[0] >= cnt[2])return Label::Real;
	if (cnt[1] >= cnt[0] && cnt[1] >= cnt[2])return Label::Fake;
	return Label::Ambiguous;
}

inline CellStats compute_cell_stats(const std::vector<JudgeRecord>& records, double q_baseline = 0.5) {
	CellStats st;
	st.q = q_baseline;

	{
		std::unordered_map<int, const JudgeRecord*> first0;
		for (const auto& r : records) {
			if (r.order_id == 0 && !first0.count(r.sample_id)) first0[r.sample_id] = &r;
		}
		double s = 0.0, w = 0.0;
		for (auto& kv : first0) {
			auto* r = kv.second;
			s += (r->pred == r->truth ? 1.0 : 0.0) * r->weight;
			w += r->weight;
		}
		st.Acc = (w > 0) ? s / w : 0.0;
	}
	// C: 按(sample_id,order_id)分组，跨rep的众数占比
	{
		std::unordered_map<long long, std::vector<const JudgeRecord*>>g;
		for (const auto& r : records) {
			long long key = (long long)r.sample_id * 100000 + r.order_id;
			g[key].push_back(&r);
		}
		double s = 0.0, w = 0.0;
		for (auto& kv : g) {
			auto& vec = kv.second;
			int cnt[3] = { 0,0,0 };
			for (auto* r : vec)cnt[(int)r->pred]++;
			int best = std::max({ cnt[0],cnt[1],cnt[2] });
			double consistency = (double)best / vec.size();
			double ww = vec.front()->weight;
			s += consistency * ww;
			w += ww;
		}
		st.C = (w > 0) ? s / w : 0.0;
	}
	//Fi:同一sample_id下 order0 vs order1 的众数标签翻转
	{
		std::unordered_map<int, std::unordered_map<int, std::vector<Label>>>by_sample_order;
		for (const auto& r : records) {
			by_sample_order[r.sample_id][r.order_id].push_back(r.pred);
		}
		double s = 0.0, w = 0.0;
		for (auto& kv : by_sample_order) {
			auto& mp = kv.second;
			if (mp.count(0) && mp.count(1)) {
				Label p0 = mode_label(mp[0]);
				Label p1 = mode_label(mp[1]);
				double flip = (p0 != p1 ? 1.0 : 0.0);
				s += flip * 1.0;
				w += 1.0;
			}
		}
		st.Fi = (w > 0) ? s / w : 0.0;
	}
	st.T = st.C * (st.Acc - st.Fi * st.q);
	return st;
}