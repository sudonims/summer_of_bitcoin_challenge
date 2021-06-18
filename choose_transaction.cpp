#include<bits/stdc++.h>
using namespace std;

#define pb push_back
#define MAX_WEIGHT 4000

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

vector<vector<int>> dp(5215, vector<int>(MAX_WEIGHT+1, -1));
// int **dp;
// vector<Transaction*> block;
vector<Transaction*> transactions;
map<string, int> m; 


int sol(vector<Transaction *> t, int weight, int n) {
	if(n==0 || weight==0) {
		return 0;
	} 
	if(dp[n][weight] != -1) return dp[n][weight];
	else if(t[n-1]->getWeight() <= weight) {
		int a = sol(t, weight - t[n-1]->getWeight(), n-1) + t[n-1]->getFee();
		int b = sol(t, weight, n-1);
		
		return dp[n][weight] = max(a, b);
		// if(a > b) {
		// 	dp[n][weight] = a;
		// 	//check Parent
		// } else {
		// 	dp[n][weight] = b;
		// }
	} else if(t[n-1]->getWeight()> weight) {
		return dp[n][weight] = sol(t, weight, n-1);
		// return dp[n][weight] ;
	}
}

int getDP(int row, int col) {
	return dp[row][col];
}

int setDP(int row, int col, int val) {
	dp[row][col] = val;
}

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
		transactions.pb(a);
	}
	fin.close();

	cout<<transactions.size()<<endl;

	
	// memset(dp, -1, sizeof(dp));

	// dp = (int **)malloc(sizeof(int*)*(transactions.size() + 1));

	cout<<"INIT DONE"<<endl;
	for(int i=0;i<transactions.size()+1;i++) {
		setDP(i, 0, 0);
	}

	for(int j=1;j<MAX_WEIGHT+1;j++) {
		setDP(0, j, 0);
	}

	cout<<"INIT DONE"<<endl;
	// int k = 0;
	for(int i=1;i<transactions.size()+1;i++) {
		// dp[i] = (int *)malloc(sizeof(int) * (MAX_WEIGHT+1));
		for(int j=1;j<MAX_WEIGHT+1;j++) {
			if(transactions[i-1]->getWeight() <= j) {

				if(transactions[i-1]->getFee() + dp[i-1][j-transactions[i-1]->getWeight()] > dp[i-1][j-1]) {
					dp[i][j] = transactions[i-1]->getFee() + dp[i-1][j-transactions[i-1]->getWeight()];
					// cout<<transactions[i-1]->getTrxId()<<" "<<k++<<endl;
					// m[transactions[i-1]->getTrxId()] = 1;
				} else {
					dp[i][j] = dp[i-1][j-1];
				}
				// cout<<i<<" "<<j<<endl;				// dp[i][j] = max(transactions[i-1]->getFee() + dp[i-1][j-transactions[i-1]->getWeight()], dp[i-1][j-1]);

			} else {
				dp[i][j] = dp[i-1][j-1];
			}
		}
	}
	cout<<"DOne\n";
	int i = transactions.size(), j = MAX_WEIGHT;

	// cout<<sol(transactions, j, i)<<endl;

	while (i > 0 && j > 0) {
    if(dp[i][j] != dp[i-1][j]) {
      cout<<dp[i][j]<<" "<<i<<" "<<transactions[i]->getTrxId()<<" "<<transactions[i]->getFee()<<" "<<transactions[i]->getWeight()<<endl;
      j = j-transactions[i]->getWeight();
      i = i-1;
    }
    else {
      i = i-1;
    }
  }
	cout<<endl<<dp[transactions.size()][MAX_WEIGHT]<<endl;

	// cout<<sol(transactions, MAX_WEIGHT, transactions.size())<<endl;
}

