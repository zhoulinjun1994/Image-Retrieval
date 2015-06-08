//
//  TE.cpp
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

vector<string> vtag;
unordered_map<string, int> m;
int main() {
    ifstream ftag("tags2.txt");
    string st;
    while (ftag >> st) {
        vtag.push_back(st);
    }
    for (int i = 0; i < vtag.size(); i ++) {
        m[vtag[i]] = i + 1;
    }
    vector<int> cnt;
    ifstream fin("testset_txt_img_cat.list");
    ofstream fout("testset_tag.txt");
    string s1, s2, s3;
    while (fin >> s1 >> s2 >> s3) {
        int sum = 0;
        cnt.clear();
        cnt.resize(vtag.size());
        ifstream ft("./texts/"+s1+".xml");
        string str((istreambuf_iterator<char>(ft)), istreambuf_iterator<char>());
        transform(str.begin(), str.end(), str.begin(), ::toupper);
        string temp = "";
        for (int p = 0; p < str.size(); p ++) {
            if (str[p] >= 'A' && str[p] <= 'Z') {
                temp += str[p];
            } else {
                if (temp == "") continue;
                sum ++;
                if (m[temp] > 0) {
                    cnt[m[temp]-1] ++;
                }
                temp = "";
            }
        }
        ft.close();
        for (int i = 0; i < vtag.size(); i ++) {
            fout << (double)cnt[i] * 1000 / sum << " ";
        }
        fout << endl;
    }
}