#include <iostream>
#include <string>
#include <map>
using namespace std;

struct InMemoryDB{
    map<string,int> userMap;
    bool inTransaction=false;
    pair<string,int> lastXfer = make_pair("",0);
    pair<string,int> curXfer;
    int transaction = 0;
    int* get(string key) {
        auto it = userMap.find(key);
        if(it == userMap.end()) {
            cout << "get: No value, returning nullptr " << endl;
            return nullptr;
        }
        cout << "userMap val: " << key << ":" << userMap[key] << endl;
        return &userMap[key];
    }
    void put (string key, int val){
        if(inTransaction) {
            auto it = userMap.find(key);
            if(it != userMap.end()) {
                lastXfer.first = key;
                lastXfer.second = userMap[key];

            }
            curXfer.first = key;
            curXfer.second = val;
            cout << "Putting: " << curXfer.first << " : " << curXfer.second << endl;
        } else {
            cout << "Put error! Not in transaction! " << endl;
        }
    }
    void begin_transaction() {
        inTransaction=true;
        cout << "Beginning Transaction " << transaction << endl;
    }
    void commit() {
        if(inTransaction) {
            userMap[curXfer.first]=curXfer.second;
            inTransaction=false;
            cout << "Committing transaction " << transaction++ << endl;
        } else {
            cout << "Commit error! No ongoing transaction" << endl;
        }
    }
    void rollback() {
        if(inTransaction) {
            cout << "Rolling transaction back... " << endl;
            userMap[lastXfer.first]=lastXfer.second;
        }
        else {
            cout << "Rollback Error! No ongoing transaction!" << endl;
        }
    }

};

int main() {
    InMemoryDB inmemoryDB;

    // should return null, because A doesn’t exist in the DB yet
    
    inmemoryDB.get("A");

    // should throw an error because a transaction is not in progress
    inmemoryDB.put("A", 5);

    // starts a new transaction
    inmemoryDB.begin_transaction();

    // // set’s value of A to 5, but its not committed yet
    inmemoryDB.put("A", 5);

    // // should return null, because updates to A are not committed yet
    inmemoryDB.get("A");

    // // update A’s value to 6 within the transaction
    inmemoryDB.put("A", 6);

    // // commits the open transaction
    inmemoryDB.commit();

    inmemoryDB.get("A");

    inmemoryDB.commit();

    inmemoryDB.rollback();

    inmemoryDB.get("B");

    inmemoryDB.begin_transaction();

    inmemoryDB.put("B",10);

    // inmemoryDB.put("B",10);

    inmemoryDB.rollback();

    inmemoryDB.get("B");

    // return 0;
}
