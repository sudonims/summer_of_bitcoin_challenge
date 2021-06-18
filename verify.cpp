#include<bits/stdc++.h>
using namespace std;

#define pb push_back

class Transaction {
	string txn_id;
	int fee, weight;
	vector<string> parents;
	bool added;
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
		added = false;
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

	void setAdd() {
		added = true;
	}

	bool getAdded() {
		return added;
	}
};

int curr_weight = 0, curr_fee = 0;
vector<string> block;
unordered_map<string, Transaction*> transactions;

int main() {
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
	}
	fin.close();

	cout<<transactions.size()<<endl;

	int weight = 0, fee = 0;

	ifstream in;
	in.open("block_.txt");
	while(getline(in, line)) {
		// stringstream s(line);
		// cout<<line<<endl;
		weight += transactions[line]->getWeight();
		fee += transactions[line]->getFee();
	}

	cout<<weight<<" "<<fee<<endl;
}