#include<bits/stdc++.h>
using namespace std;

#define pb push_back

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


int main() {
	vector<Transaction*> transactions;
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
		transactions.pb(a);
	}
	fin.close();

}