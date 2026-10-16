#pragma once
#include "judge_types.h"
#include "judge_utils.h"
#include <fstream>
#include <sstream>
#include <vector>

inline std::vector<JudgeRecord> read_csv(const string& path) {
	std::ifstream fin(path);
	if (!fin) throw std::runtime_error("Cannot open file: " + path);

	std::vector<JudgeRecord> out;
	string line;
	std::getline(fin, line);

	while (std::getline(fin, line)) {
		if (line.empty()) continue;
		std::stringstream ss(line);
		string token;

		JudgeRecord r;
		std::getline(ss, r.model, ',');
		std::getline(ss, token, ','); r.mode = token;
		std::getline(ss, token, ','); r.delta = std::stoi(token);
		std::getline(ss, token, ','); r.sample_id = std::stoi(token);
		std::getline(ss, token, ','); r.rep_id = std::stoi(token);
		std::getline(ss, token, ','); r.order_id = std::stoi(token);
		std::getline(ss, token, ','); r.pred = label_from_string(token);
		std::getline(ss, token, ','); r.truth = label_from_string(token);
		std::getline(ss, token, ','); r.is_uncertain = (std::stoi(token) != 0);
		if (std::getline(ss, token, ',')) r.weight = std::stod(token);
		else r.weight = 1.0;

		out.push_back(r);
	}
	return out;
}