#pragma once
#include <string>
using std::string;
enum class Label { Real = 0, Fake = 1, Ambiguous = 2 };
enum class Mode { CLS = 0, INF = 1};

struct JudgeRecord
{
	string model;
	string mode;
	int delta; //-3/0/+3
	int sample_id; //claim id
	int rep_id; //重复采样编号(用于估计C)
	int order_id; //证据/回答顺序编号(用于估计Fi;没有就填0)
	Label pred;
	Label truth;
	bool is_uncertain; //用于q，不确定样本
	double weight; //默认1.0
};

