#include "transaction.h"
using namespace std;

vector<string> block;
unordered_map<string, Transaction*> transactions;

bool verifyBlock() {
	for(int i=0;i<block.size();i++) {
		auto parents = transactions[block[i]]->getParents();
		if(parents.size() > 0) {
			for(string par_trx: parents) {
				if(find(block.begin(), block.end(), par_trx) > find(block.begin(), block.end(), block[i])) {
					return false;
				}
			}
		}
	}
	return true;
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
		transactions[a->getTrxId()] = a;
	}
	fin.close();

	int weight = 0, fee = 0;

	ifstream in;
	in.open("block_.txt");
	while(getline(in, line)) {
		block.pb(line);
		weight += transactions[line]->getWeight();
		fee += transactions[line]->getFee();
	}

	printf("Block Weight: %d\nFee: %d\nBlock Validity:%d\n", weight, fee, verifyBlock());
}