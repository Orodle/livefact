#pragma once
#include <vector>
#include <cmath>
#include <numeric>
#include <stdexcept>

inline std::vector<double>histogram(const std::vector<double>& samples, int bins, double minv, double maxv) {
	std::vector<double>h(bins, 0.0);
	if (samples.empty())return h;
	double range = maxv - minv;
	if (range <= 0)range = 1e-9;
	for (double x : samples) {
		int b = (int)((x - minv) / range * bins);
		if (b < 0)b = 0;
		if (b >= bins)b = bins - 1;
		h[b] += 1.0;
	}
	return h;
}

inline double tvd(const std::vector<double>& p, const std::vector<double>& q) {
	if (p.size() != q.size()) throw std::runtime_error("TVD size mismatch");
	double sp = std::accumulate(p.begin(), p.end(), 0.0);
	double sq = std::accumulate(q.begin(), q.end(), 0.0);
	if (sp <= 0 || sq <= 0)return 0.0;
	double d = 0.0;
	for (size_t i = 0;i < p.size();++i) {
		double pi = p[i] / sp;
		double qi = q[i] / sq;
		d += std::abs(pi - qi);
	}
	return 0.5 * d;
}