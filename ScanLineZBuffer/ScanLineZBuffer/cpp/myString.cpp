//
//  myString.cpp
//  ScanLineZBuffer
//
//  Created by yy on 2017/12/25.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#include <iostream>
#include "myString.h"
#include <vector>

vector<string> Split(string& str, string c){
    vector<string> strArr;
    size_t begin = 0, end = 0;
    size_t len = str.length();
    
    string tmp;
    while(begin < len){
        if(begin == len - 1){
            tmp = str.substr(begin);
            strArr.push_back(tmp);
            break;
        }
        else{
            if(str.npos == str.find(c, begin)){
                end = len;
            }
            else{
                end = str.find(c, begin);
            }
            tmp = str.substr(begin, end - begin);
            strArr.push_back(tmp);
        }
        begin = end + 1;
    }
    return strArr;
}

