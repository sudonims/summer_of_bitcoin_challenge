#ifndef TRANSACTION_H
#define TRANSACTION_H

#include<bits/stdc++.h>
#define pb push_back

class Transaction {
	std::string txn_id;
	int fee, weight;
	std::vector<std::string> parents;
	bool added;

	public:
	Transaction(std::vector<std::string> str) {
		txn_id = str[0];
		fee = stoi(str[1]);
		weight = stoi(str[2]);
		if(str.size() > 3) {
			std::string last = str[3], word;
			// cout<<last<<endl;
			std::stringstream s(last);
			while(getline(s, word, ';')) {
				parents.pb(word);
			}
		}
		added = false;
	}

	std::string getTrxId() {
		return txn_id;
	}

	int getFee() {
		return fee;
	}

	int getWeight() {
		return weight;
	}

	std::vector<std::string> getParents() {
		return parents;
	}

	void setAdd() {
		added = true;
	}

	bool getAdded() {
		return added;
	}
};


#endif