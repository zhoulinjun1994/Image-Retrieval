//
//  tag.cpp
//  
//
//  Created by Nikifor Zhao on 15/6/8.
//
//

#include <stdio.h>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <string>
#include <string.h>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iomanip>
using namespace std;

string fname[] = {"1","2","3","4","5","6","7","8","9","10"};

int filecnt[1<<20] = {0};
unordered_map<string, int> m[10000];
int sum[10000] = {0};
unordered_set<string> tag;
unordered_set<string> where[1<<20];
unordered_map<string, int> howmuch[10];
vector<string> vtag;
string filename[10000];
int idSum = 0;
int fSum[10];

double idf[1<<20];
int my_hash(string str) {
    int hash = 0;
    for (int i = 0; i < str.size(); i ++) {
        hash = hash * 81393 + str[i];
    }
    return hash & ((1<<20)-1);
}


int main() {
    /*
    ifstream fin("trainset_txt_img_cat.list");
    string s1, s2, s3;
    while (fin >> s1 >> s2 >> s3) {
        m[idSum].clear();
        filename[idSum] = s1;
        ifstream ft("./texts/"+s1+".xml");
        string str((istreambuf_iterator<char>(ft)), istreambuf_iterator<char>());
        ofstream fapp;
        fapp.open(s3.c_str(), ios::app);
        fapp << str;
        fapp.close();
        idSum ++;
    }
    for (int i = 0; i < 10; i ++) {
        ifstream ft(fname[i].c_str());
        string str((istreambuf_iterator<char>(ft)), istreambuf_iterator<char>());
        transform(str.begin(), str.end(), str.begin(), ::toupper);
        string temp = "";
        for (int p = 0; p < str.size(); p ++) {
            if (str[p] >= 'A' && str[p] <= 'Z') {
                temp += str[p];
            } else {
                if (temp == "") continue;
                sum[i] ++;
                int hash = my_hash(temp);
                if (m[i][temp] == 0) filecnt[hash] ++;
                m[i][temp] ++;
                temp = "";
            }
        }
        ft.close();
    }
    for (int i = 0; i < (1<<20); i ++) {
        idf[i] = log((double)10 / ((double)filecnt[i] + 1));
    }
    for (int i = 0; i < 10 ; i ++) {
        for (auto it = m[i].begin(); it != m[i].end(); it ++){
            double tf = (double)it->second / (double)sum[i];
            double tfidf = tf * (idf[my_hash(it->first)]);
            if (tfidf > 1e-4) {
                tag.insert(it->first);
                where[my_hash(it->first)].insert(fname[i]);
            }
        }
    }
    cout << tag.size() << endl;
    ofstream fout("tags2.txt");
    for (auto it = tag.begin(); it != tag.end(); it ++) {
        fout << *it;
        int hash = my_hash(*it);
//        for (auto it2 = where[hash].begin(); it2 != where[hash].end(); it2 ++) {
//            fout << "\t\t" << *it2;
//        }
        fout << endl;
    }
    fout.close();
    */
    ifstream ftag("tags2.txt");
    string st;
    while (ftag >> st) {
        vtag.push_back(st);
    }
    sort(vtag.begin(), vtag.end());
    ifstream fin("trainset_txt_img_cat.list");
    string s1, s2, s3;
    idSum = 0;
    while (fin >> s1 >> s2 >> s3) {
        m[idSum].clear();
        int t;
        for (t = 0; t < 10; t ++) {
            if (s3 == fname[t]) break;
        }
        fSum[t] ++;
        ifstream ft("./texts/"+s1+".xml");
        string str((istreambuf_iterator<char>(ft)), istreambuf_iterator<char>());
        transform(str.begin(), str.end(), str.begin(), ::toupper);
        string temp = "";
        for (int p = 0; p < str.size(); p ++) {
            if (str[p] >= 'A' && str[p] <= 'Z') {
                temp += str[p];
            } else {
                if (temp == "") continue;
                if (binary_search(vtag.begin(), vtag.end(), temp)) {
                    if (m[idSum][temp] == 0) {
                        int i;
                        for (i = 0; i < 10; i ++) {
                            if (s3 == fname[i]) break;
                        }
                        howmuch[i][temp] ++;
                        m[idSum][temp] = 1;
                    }
                }
                temp = "";
            }
        }
        ft.close();
        idSum ++;
    }
    ofstream fout("tags2_stat_2.txt");
    for (auto it = vtag.begin(); it != vtag.end(); it ++) {
        for (int i = 0; i < 10; i ++) {
            fout << setiosflags(ios::fixed) << setprecision(2) << (double)howmuch[i][*it] / fSum[i] << "\t";
        }
        fout << *it << "\t";
        fout << endl;
    }
    fout.close();
}
