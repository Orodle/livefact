#pragma once
#include "judge_types.h"
#include <map>
#include <vector>
#include <tuple>
#include <string>

using CellKey = std::tuple<std::string, std::string, int>;

inline std::map < CellKey, std::vector<JudgeRecord>> group_by_cell(const std::vector<JudgeRecord>& recs) {
	std::map<CellKey, std::vector<JudgeRecord>> groups;
	for (const auto& r : recs) {
		CellKey key = std::make_tuple(r.model, r.mode, r.delta);
		groups[key].push_back(r);
	}
	return groups;
}
