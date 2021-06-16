#include<bits/stdc++.h>
using namespace std;

#define pb push_back
#define MAX_WEIGHT 400000

vector<vector<int>> dp(5215, vector<int>(400001, -1));

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

// int sol(vector<Transaction *> t, int weight, int n) {
// 	if(n==0 || weight==0) return 0;

// 	if(t[n-1]->getWeight() <= weight) {
// 		return max(sol(t, weight - t[n-1]->getWeight(), n-1) + t[n-1]->getFee(), sol(t, weight, n-1));
// 	} else {
// 		return sol(t, weight, n-1);
// 	}
// }

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

	cout<<transactions.size()<<endl;

	
	// memset(dp, -1, sizeof(dp));

	// dp = new int*[transactions.size()+1];

	cout<<"INIT DONE"<<endl;
	// for(int i=0;i<transactions.size()+1;i++) {
	// 	dp[i] = new int[MAX_WEIGHT+1];
	// }

	// cout<<"INIT DONE"<<endl;

	for(int i=1;i<transactions.size()+1;i++) {
		for(int j=1;j<MAX_WEIGHT+1;j++) {
			if(i==0 || j==0) {
				dp[i][j] = 0;
			}
			if(transactions[i-1]->getWeight() <= j) {
				dp[i][j] = max(transactions[i-1]->getFee() + dp[i-1][j-transactions[i-1]->getWeight()], dp[i-1][j-1]);
				// cout<<"A";
			} else {
				dp[i][j] = dp[i-1][j-1];
			}
		}
	}
	cout<<"DOne\n";

	cout<<dp[transactions.size()][MAX_WEIGHT]<<endl;

	// cout<<sol(transactions, MAX_WEIGHT, transactions.size())<<endl;
}

