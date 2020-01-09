#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <ratio>
//#include <../GoogleORtools/knapsack_solver.h>
#include <stdio.h>
#define problemsize 320

using namespace std;
//file reader func definition
//input : filename
//output : weights, capacity, profit
void ReadFile(
    string filename,
    vector<int> &capacities,
    vector<int> &profits,
    vector<vector<int>> &weights);

//file exporter
//input : string
//output : results.txt
void FileWriter(string txt);

//filename generator to run all 320 problems
//input : path
//output : filenames
vector<string> FileNames(string path);

//Implementation 0-1 knapsack like GoogleOrTools
int MyKnapsack(
    vector<vector<int>> &weights,
    vector<int> &capacities,
    vector<int> &profits);

 //maximum function  
 int maximum(int a, int b) { return (a > b) ? a : b; }

int MyKnapsack(
    vector<vector<int>> &weights,
    vector<int> &capacities,
    vector<int> &profits)
{
    int i, w;
    vector<vector<int>> K;
    vector<int> temp;
    K.reserve(profits.size() + 1);
    temp.reserve(capacities[0] + 1);
    for (i = 0; i <= capacities[0] + 1; i++)
    {
        temp.push_back(0);
    }

    for (i = 0; i <= profits.size() + 1; i++)
    {
        K.push_back(temp);
    }

    for (i = 0; i <= profits.size(); i++)
    {
        for (w = 0; w <= capacities[0]; w++)
        {
            if (i == 0 || w == 0)
                K[i][w] = 0;
            else if (weights[0][i - 1] <= w)
                K[i][w] = maximum(profits[i - 1] + K[i - 1][w - weights[0][i - 1]], K[i - 1][w]);
            else
                K[i][w] = K[i - 1][w];
        }
    }

    return K[profits.size()][capacities[0]];
}

vector<string> FileNames(string path)
{
    vector<string> filenames;
    vector<int> n = {10, 50, 100, 500};
    vector<int> r = {50, 100, 500, 1000};
    vector<int> type = {1, 2, 3, 4};
    string filename;
    filenames.reserve(problemsize);

    filename += path + "problem_";
    //read n
    for (auto n : n)
    {
        //read r
        for (auto r : r)
        {
            //read type
            for (auto type : type)
            {
                
                for (int instance = 1; instance < 6; instance++)
                {
                    filename = path + "problem_" + to_string(n) + "_" +
                               to_string(r) + "_" + to_string(type) + "_" +
                               to_string(instance) + "_5.txt";
                    filenames.push_back(filename);
                }
            }
        }
    }
    return filenames;
}

void ReadFile(
    string filename,
    vector<int> &capacities,
    vector<int> &profits,
    vector<vector<int>> &weights)
{
    long lines = 0;
    long dump = 0;
    string line;
    fstream input;
    stringstream sstream;
    input.open(filename, ios::in);

    if (input.is_open())
    {
        long tempprofit, tempweight, tempcapacity = 0;
        vector<int> tempweightvec;
        int linecount = 1;

        getline(input, line);
        sstream << line;
        sstream >> lines;
        sstream.clear();
        profits.reserve(lines);
        tempweightvec.reserve(lines); 
        do
        {
            getline(input, line);
            sstream << line;
            sstream >> dump >> tempprofit >> tempweight;
            sstream.clear();
            profits.push_back(tempprofit);
            tempweightvec.push_back(tempweight);
            linecount++;
        } while (linecount <= lines);
        getline(input, line);
        sstream << line;
        sstream >> tempcapacity;
        sstream.clear();
        capacities.push_back(tempcapacity);
        weights.push_back(tempweightvec);
        input.close();
    }
    else
    {
        cout << "error opening file " << filename << endl;
        exit(-1);
    }
}

void WriteFile(string txt)
{
    fstream output;
    output.open("knapsack_results.csv", ios::out | ios::app);
    if (output.is_open())
    {
        output << txt;
    }
    output.close();
};


int main()
{
    int problemNo = 1;
    vector<string> filenames;

    remove("..\\knapsack_code\\knapsack_results.csv");

    //Filename generator
    filenames = FileNames("..\\generator");

    FileWriter("Problem Number,TimePersonal(ms)\n");
    for (auto filename : filenames)
    {
        int PValue = 0;
        vector<int> profits, capacities;
        vector<vector<int>> weights;

        //Prepares the input from the file
        ReadFile(filename, capacities, profits, weights);

        FileWriter(to_string(problemNo) + ",");

        //My knapsack implementation 
        //chrono
        auto tbegin = chrono::high_resolution_clock::now();
        PValue = MyKnapsack(weights, capacities, profits);
        auto tend = chrono::high_resolution_clock::now();

        chrono::duration<double, milli> t2 = tend - tbegin;

        FileWriter(to_string(PValue) );

        //On to the next problem
        FileWriter("\n");
        problemNo++;
    }
    return 0;
}
