#include "transaction.h"
using namespace std;
#define MAX_WEIGHT 4000000

/*
Some global variables to be used in the program
*/
int curr_weight = 0, curr_fee = 0;
vector<string> block;
unordered_map<string, Transaction*> transactions;


/*
 This function adds a transaction to the block and return true or false based on success and failure respectively
*/
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
	vector<string> transaction_ids;
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

		/* map of all transactions with trx_id as key and Transaction class as value */
		transactions[a->getTrxId()] = a;

		/* Just vector of trx_ids */
		transaction_ids.pb(a->getTrxId());
	}
	fin.close();

	/* Stores the child transactions whose parents are yet to be included in the block */
	unordered_map<string, vector<string>> waiting_child;

	/* sorts the transactions in descending order based on the fee to weight ration for better optimal solution */
	sort(transaction_ids.begin(), transaction_ids.end(), [&](string a, string b) {
		return (float)transactions[a]->getFee() / (float)transactions[a]->getWeight() > (float)transactions[b]->getFee() / (float)transactions[b]->getWeight();
	});

	for(string i: transaction_ids) {
		auto parents = transactions[i]->getParents();
		auto weight = transactions[i]->getWeight();
		auto fee = transactions[i] -> getFee();


		/* checks for parents of current trx */
		if(parents.size() > 0) {
			waiting_child[i] = parents;
		} else {
			if(addTrxToBlock(i))
				transactions[i]->setAdd();
		}
		
		/* Run a loop through waiting pool */
		for(auto it: waiting_child) {
			bool flag = false;

			/* continue if trx already present in block */
			if(transactions[it.first]->getAdded()) continue;

			/* Check if parents are in the block */
			for(string par_trx: it.second) {
				if(find(block.begin(), block.end(), par_trx) == block.end()) {
					flag = true;
					break;
				}
			}

			/* Add trx to block if parents are in the block */
			if(!flag) {
				if(addTrxToBlock(it.first)) {
					transactions[it.first]->setAdd();
				}
			} else {
				continue;
			}
		}
	}

	/* Final outputs */
	cout<<"Transactions read:"<<transactions.size()<<endl;
	cout<<"Block Size:"<<block.size()<<endl<<"Block Weight:"<<curr_weight<<endl<<"Block Fee:"<<curr_fee<<endl;

	ofstream out("block.txt");
	for(string i: block) {
		out<<i<<endl;
	}

}