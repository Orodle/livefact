#pragma once
#include "judge_types.h"
#include  <stdexcept>

inline Label label_from_string(const string& s) {
	if (s == "Real" || s == "REAL" || s == "0")return Label::Real;
	if (s == "Fake" || s == "FAKE" || s == "1")return Label::Fake;
	if (s == "Ambiguous" || s == "AMBIGUOUS" || s == "2")return Label::Ambiguous;
	throw std::runtime_error("Unknown Label: " + s);
}

inline Mode mode_from_string(const string& s) {
	if (s == "CLS")return Mode::CLS;
	if (s == "INF")return Mode::INF;
	throw std::runtime_error("Unknown Mode: " + s);
}

inline Label label_from_int(int x) {
	switch (x) {
	case 0: return Label::Real;
	case 1: return Label::Fake;
	case 2: return Label::Ambiguous;
	default: throw std::runtime_error("Invalid label int");
	}
}
