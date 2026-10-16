#pragma once
#include <vector>
#include <cmath>

//参考分布1:C的理想分布(质量集中在0.8~1.0)
inline std::vector<double> ref_hist_c(int bins) {
	std::vector<double>h(bins, 0.0);
	for (int i = 0;i < bins;++i) {
		double x = (i + 0.5) / bins;
		h[i] = std::exp(10.0 * (x - 1.0));
	}
	return h;
}

//参考分布2:Fi的理想分布(质量集中在0)
inline std::vector<double>ref_hist_fi(int bins) {
	std::vector<double>h(bins, 0.0);
	for (int i = 0;i < bins; ++i) {
		double x = (i + 0.5) / bins;
		h[i] = std::exp(-10.0 * x);
	}
	return h;
}