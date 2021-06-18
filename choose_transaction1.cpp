#include<bits/stdc++.h>

using namespace std;

#define pb push_back
#define MAX_WEIGHT 4000000

class Transaction {
	string txn_id;
	int fee, weight;
	vector<string> parents;

	public: 
	Transaction(vector<string> str) {
		txn_id = str[0];
		fee = stoi(str[1]);
		weight = stoi(str[2]);
		if(str.size() > 3) {
			string last = str[3], word;
			// cout<<last<<endl;
			stringstream s(last);
			while(getline(s, word, ';')) {
				parents.pb(word);
			}
		}
	}

	string getTrxId() {
		return txn_id;
	}

	int getFee() {
		return fee;
	}

	int getWeight() {
		return weight;
	}

	vector<string> getParents() {
		return parents;
	}
};

int curr_weight = 0, curr_fee = 0;
vector<string> block;
unordered_map<string, Transaction*> transactions;

bool addTrxToBlock (string trxId) {
	if(curr_weight + transactions[trxId]->getWeight() <= MAX_WEIGHT) {
		block.pb(trxId);
		curr_weight += transactions[trxId]->getWeight();
		curr_fee += transactions[trxId]->getFee();
		return true;
	} else {
		return false;
	}
}

int main() {
	vector<string> transaction_ids, child_transactions, undependent_transactions;
	ifstream fin("mempool.csv");

	string line, word;
	vector<string> singleTransaction;
	getline(fin, line);
	while(getline(fin, line)) {
		singleTransaction.clear();
		stringstream s(line);
		while(getline(s, word, ',')) {
			singleTransaction.pb(word);
		}
		Transaction *a = new Transaction(singleTransaction);
		// transaction_ids.pb(a->getTrxId());
		// if(a->getParents().size() > 0) {
		// 	child_transactions.pb(a->getTrxId());
		// } else {
		// 	undependent_transactions.pb(a->getTrxId());
		// }
		transactions[a->getTrxId()] = a;
		transaction_ids.pb(a->getTrxId());
	}
	fin.close();

	cout<<transactions.size()<<endl;

	// multimap<int, pair<string, vector<string>>> waiting_child;
	unordered_map<string, vector<string>> waiting_child;
	unordered_map<string, vector<string>>::iterator it;
	sort(transaction_ids.begin(), transaction_ids.end(), [&](string a, string b) {
		return (float)transactions[a]->getFee() / (float)transactions[a]->getWeight() > (float)transactions[b]->getFee() / (float)transactions[b]->getWeight();
	});



	cout<<transaction_ids[0]<<" INIT "<<transaction_ids.back()<<endl;
	int b = 0;
	for(string i: transaction_ids) {
		auto parents = transactions[i]->getParents();
		auto weight = transactions[i]->getWeight();
		auto fee = transactions[i] -> getFee();
		cout<<b++<<" ";
		if(parents.size() > 0) {
			bool flag = false;
			for(string par_trx: parents) {
				if(find(block.begin(), block.end(), par_trx) == block.end()) {
					flag = true;
					break;
				}
			}
			if(flag) {
				waiting_child[i] = parents;
				// waiting_child.insert(make_pair(fee,make_pair(i, parents)));
			} else {
				addTrxToBlock(i);
			}
		} else {
			addTrxToBlock(i);
			// if(!addTrxToBlock(i)) {
			// 	continue;
			// }
		}
		
		for(it=waiting_child.begin();it != waiting_child.end(); it++) {
			bool flag = false;
			for(string par_trx: it->second) {
				if(find(block.begin(), block.end(), par_trx) == block.end()) {
					flag = true;
					break;
				}
			}
			if(!flag) {
				if(addTrxToBlock(it->first)) {
					waiting_child.erase(it);
				}
			} else {
				continue;
			}
		}
	}


	// for(auto i: child_transactions) {
	// 	auto parents = transactions[i]->getParents();
	// 	auto weight = transactions[i]->getWeight();
	// 	auto fee = transactions[i] -> getFee();
	// 	if(parents.size() > 0) {
	// 		bool flag = false;
	// 		for(auto par_trx: parents) {
	// 			if(!addTrxToBlock(par_trx)) {
	// 				flag = true;
	// 				break;
	// 			}
	// 		}
	// 		if(flag) {
	// 			for(auto par_trx: parents) {
	// 				block.erase(std::remove(block.begin(), block.end(), par_trx), block.end());
	// 			}
	// 		} else {
	// 			addTrxToBlock(i);
	// 		}
	// 	} else {
	// 		if(!addTrxToBlock(transactions[i]->getTrxId())) {
	// 			continue;
	// 		}
	// 	}
	// }

	cout<<curr_fee<<" "<<curr_weight<<" "<<block.size()<<endl;
}