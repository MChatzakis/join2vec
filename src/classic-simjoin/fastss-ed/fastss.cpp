#include "../classic-simjoin-common/time.h"
#include "../classic-simjoin-common/type.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>

#include <cstdlib>

#include "../../common/timers.h"
#include "../../common/utilfuncs.h"

using namespace std;

int id;
int THRESHOLD;
long long res_num;
long long cand_num;

HashSet<int> checked_flag;
HashMap<unsigned long long, vector<pair<int, vector<int>>>> index;

vector<simjoin_res>results;

int check_ed(const vector<int> &p1, const vector<int> &p2)
{
	int i = 0, j = 0, updates = 0;
	while (i < (int)p1.size() && j < (int)p2.size())
	{
		if (p1[i] == p2[j])
		{
			updates++;
			j++;
			i++;
		} else {
			(p1[i] < p2[j] ? i++ : j++);
		}
	}
	return p1.size() + p2.size() - updates;
}

void insert_index(const string &result, const vector<int> &del_pos)
{
	static auto hash_function = hash<string>();
	auto &list = index[hash_function(result)];

	for (auto &p : list)
	{
		if (id == p.first) continue;
		if (checked_flag.find(p.first) != checked_flag.end()) continue;

		cand_num++;
		int ed = check_ed(p.second, del_pos);
		if (ed <= THRESHOLD)
		{
			checked_flag.insert(p.first);
			res_num++;

			//result_pairs.push_back(make_pair(id, p.first));
			
			simjoin_res res;
			res.id_1 = id;
			res.id_2 = p.first;
			res.score = ed;

			results.push_back(res);
		}
	}

	list.push_back(make_pair(id, del_pos));
}

void perform_deletion(string &&s, int k)
{
	static vector<int> del_pos;

	if (k == 0) {
		insert_index(s, del_pos);
	} else {
		for (int pos = (del_pos.empty() ? 0 : del_pos.back()); pos < (int)s.length(); pos++)
		{
			del_pos.push_back(pos);
			perform_deletion(s.substr(0, pos) + s.substr(pos + 1), k - 1);
			del_pos.pop_back();
		}
	}
}

int main(int argc, char **argv)
{
	if (argc < 3) return -1;

	TOTAL_TIME_START

	log_start();
	ifstream in(argv[1]);
	THRESHOLD = atoi(argv[2]);

	JOIN_TIME_START
	
	string line;
	while (getline(in, line))
	{
		checked_flag.clear();

		insert_index(line, vector<int>());
		
		for (int k = 1; k <= THRESHOLD; k++)
			perform_deletion(move(line), k);
		
		id++;
	}

	JOIN_TIME_END
	
	cout << "Candidates:	" << cand_num << endl;
	cout << "Result Pairs:	" << res_num << endl;

	TOTAL_TIME_END;

	log_timers();

	if(argc > 3){
		timers_to_file(argv[3]);
	}

	if(argc > 4){
		id_2_str(argv[1], results);
		results_to_file(argv[4], results);
	}

	return 0;
}

