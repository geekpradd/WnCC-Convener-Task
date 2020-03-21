#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
using namespace std;

int main(){
    ifstream fin("timestat.txt");
    string inp;
    vector< vector<double> > timevalues; // timevalues will be a 2D vector. Each row will contain three values corresponding to real, user, sys times
    vector<double> temp; // temporary vector to store current row
    
    while (getline(fin, inp)){
        if (inp.size()==0)
            continue; // if line is the empty line, continue onto next line

        int i; // we will iterate from the back of the line via iteration index i
        int delta = 0; // as we will use the value of i to get the place value of the current digit, we need delta to account for the . that comes up
        int secondsscaled = 0; // seconds scaled is the value of the time stored as integer without the decimal

        int dot_index; // we need the index where the dot came up to convert to decimal as then only we know the number of decimal places
        
        for (i = inp.size()-2; i>=0; --i){
            if (inp[i]=='m')
                break; // reached minutes zone, break
            if (inp[i]=='.'){
                delta++; // increment delta, needs to be used below for the place value. It effectively compensates the presence of . by reducing the exponent in the power of 10
                dot_index = i; // index where . appears used to calculate number of decimal places
                continue;
            }
            secondsscaled= secondsscaled + pow(10, inp.size()-2-i-delta)*(inp[i]-'0');
            // update secondsscaled by adding in the new digit multiplied by the power of 10 corresponding to it's place value
        }

        double seconds = secondsscaled/pow(10, inp.size()-2-dot_index); // get seconds by dividing by 10^(number of decimals)
        int minutes = 0;

        i--; // i breaked when we got m so decrement it so that we start from the digits of m
        int first_i = i; // first index corresponding to a digit of m
        for (; i>=0; --i){
            if (inp[i]==' ')
                break; // whitespace encoutnered. break.
            minutes = minutes + pow(10, first_i - i)*(inp[i]-'0'); // update minutes
        }
        seconds = 60*minutes + seconds; // get total seconds value

        temp.push_back(seconds); // push back to tem
        if (temp.size()==3){
            timevalues.push_back(temp); // if three elements have been added to temp that means current batch is over. so push it and continue on with the next batch
            temp.clear();
        }
    }
    int n = timevalues.size();
    double real_mean = 0, user_mean = 0, sys_mean = 0;
    for (int i=0; i<n; ++i){
        real_mean += timevalues[i][0]; user_mean += timevalues[i][1]; sys_mean += timevalues[i][2];
    }
    real_mean /= n; user_mean /= n; sys_mean /= n; // compute mean
    double real_std = 0, user_std = 0, sys_std = 0;

    for (int i=0; i<n; ++i){
        real_std += (timevalues[i][0]-real_mean)*(timevalues[i][0]-real_mean);
        user_std += (timevalues[i][1]-user_mean)*(timevalues[i][1]-user_mean);
        sys_std += (timevalues[i][2]-sys_mean)*(timevalues[i][2]-sys_mean);
    }
    real_std /= n; user_std /= n; sys_std /= n; // first store variance then take sqaure root
    real_std = sqrt(real_std); user_std = sqrt(user_std); sys_std = sqrt(sys_std);

    int real_count = 0, user_count = 0, sys_count = 0;
    for (int i=0; i<n; ++i){
        if (timevalues[i][0] > real_mean - real_std && timevalues[i][0] < real_mean + real_std)
            real_count++;
        if (timevalues[i][1] > user_mean - user_std && timevalues[i][1] < user_mean + user_std)
            user_count++;
        if (timevalues[i][2] > sys_mean - sys_std && timevalues[i][2] < sys_mean + sys_std)
            sys_count++;
    } // self explanatory

    cout << "Number of runs: " << n << endl;
    cout << "Average time statistics" << endl;
    cout << "real " << real_mean << " user " << user_mean << " sys " << sys_mean << endl;
    cout << "Standard deviation of Time statistics" << endl;
    cout << "real " << real_std << " user " << user_std << " sys " << sys_std << endl;
    cout << "Number of runs within (average - standard deviation) to (average + standard deviation)" << endl;
    cout << "real " << real_count << " user " << user_count << " sys " << sys_count << endl;

    return 0;
}