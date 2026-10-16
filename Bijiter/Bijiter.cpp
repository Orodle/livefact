#include "csv_readera.h"
#include "judge_group.h"
#include "judge_overconfidence.h"
#include <iostream>
#include <fstream>

int main(int argc,char** argv) {
	if (argc < 3) {
		std::cerr << "Usage: " << argv[0] << " input.csv output.csv\n";
		return 1;
	}
	std::string in_path = argv[1];
	std::string out_path = argv[2];

	auto recs = read_csv(in_path);
	auto groups = group_by_cell(recs);

	std::ofstream fout(out_path);
	fout << "model,mode,delta,Acc,C,Fi,q,T,code,name,"
		<< "bootC_mean,bootC_lo,bootC_hi,bootFi_mean,bootFi_lo,bootFi_hi,"
		<< "tvd_c,tvd_fi,tvd_joint,is_true_overconfidence,is_false_overconfidence\n";

	for (auto& kv : groups) {
		auto key = kv.first;
		auto& vec = kv.second;
		auto res = analyze_overconfidence(vec, /*B=*/1000, /*q=*/0.5,/*c_thr=*/0.8, /*fi_thr=*/0.2,/*tvd_threshold=*/0.2, /*seed=*/42);
		//bootstrap结果需要再算一次或者把analyze_overconfidence返回里加boot
		auto boot = bootstrap_cell(vec, 1000, 0.5, 42);
		fout << std::get<0>(key) << ","
			<< std::get<1>(key) << ","
			<< std::get<2>(key) << ",";

		CellStats st = compute_cell_stats(vec, 0.5);
		fout << st.Acc << "," << st.C << "," << st.Fi << "," << st.q << "," << st.T << ","
			<< res.dt.code_str << "," << res.dt.name << ","
			<< boot.mean_c << "," << boot.c_lo << "," << boot.c_hi << ","
			<< boot.mean_fi << "," << boot.fi_lo << "," << boot.fi_hi << ","
			<< res.tvd_c << "," << res.tvd_fi << "," << res.tvd_joint << ","
			<< (res.is_true_overconfidence ? 1 : 0) << ","
			<< (res.is_false_overconfidence ? 1 : 0) << "\n";
	}
	return 0;
}