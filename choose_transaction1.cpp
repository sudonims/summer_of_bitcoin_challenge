#include "transaction.h"
using namespace std;
#define MAX_WEIGHT 4000000

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
		transactions[a->getTrxId()] = a;
		transaction_ids.pb(a->getTrxId());
	}
	fin.close();

	unordered_map<string, vector<string>> waiting_child;

	sort(transaction_ids.begin(), transaction_ids.end(), [&](string a, string b) {
		return (float)transactions[a]->getFee() / (float)transactions[a]->getWeight() > (float)transactions[b]->getFee() / (float)transactions[b]->getWeight();
	});

	for(string i: transaction_ids) {
		auto parents = transactions[i]->getParents();
		auto weight = transactions[i]->getWeight();
		auto fee = transactions[i] -> getFee();

		if(weight > 33500) continue;
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
			} else {
				addTrxToBlock(i);
				transactions[i]->setAdd();
			}
		} else {
			addTrxToBlock(i);
			transactions[i]->setAdd();
		}
		
		for(auto it: waiting_child) {
			bool flag = false;
			if(transactions[it.first]->getAdded()) continue;
			for(string par_trx: it.second) {
				if(find(block.begin(), block.end(), par_trx) == block.end()) {
					flag = true;
					break;
				}
			}
			if(!flag) {
				if(addTrxToBlock(it.first)) {
					transactions[it.first]->setAdd();
				}
			} else {
				continue;
			}
		}
	}

	cout<<"Block Size:"<<block.size()<<endl<<"Block Weight:"<<curr_weight<<endl<<"Block Fee:"<<curr_fee<<endl;

	ofstream out("block.txt");
	for(string i: block) {
		out<<i<<endl;
	}

}