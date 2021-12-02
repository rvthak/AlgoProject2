#include "utils.h"
#include <vector>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

// Get the line count of the given file by counting the newline characters
unsigned getFileLines(string name){
	ifstream file(name);
  	return count(istreambuf_iterator<char>(file),
                 istreambuf_iterator<char>(),'\n');
} // Returns Zero in case the file does not exits

// Get the file line width, based on the width of the first line
unsigned getFileLineLength(string name){
	ifstream file(name);
	string firstline;
	int tmp;
	unsigned count=0;

	// Get the first line of the file and return its length
	getline(file, firstline);

	// Turn the line into a stream for easier reading
	istringstream line_stream(firstline);

	// loop to count all the elements
	while( line_stream >> tmp ){
		count++;
	}

	return count;
} // Returns Zero in case the file does not exits

// Check if a given file exists
bool fileExists(string name){
    ifstream file(name.c_str());
    return file.good();
}

// Get a binary answer to the given question
bool question(string q){
	cout << "\033[36;1m (?) Question : \033[0m" << q << " : \033[33;1m(y/n)\033[0m" << endl;

	string str;
	while(1){
		cout << "     ";
		cin >> str;

		if (cin.fail()){
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\033[36;1m (?) Invalid input :\033[0m Try again : \033[33;1m(y/n)\033[0m" << endl;
		}
		else if(str=="y"){
			cin.get();
			return true;
		}
		else if(str=="n"){
			cin.get();
			return false;
		}
		else{
			cout << "\033[36;1m (?) Invalid input :\033[0m Try again : \033[33;1m(y/n)\033[0m" << endl;
		}
	}
}

// Return max 
double max(double x, double y){
	if( x > y ){ return x; }
	return y;
}

// Modulo : Always positive
unsigned mod(unsigned x, unsigned y){
	if( x<0 ){ x*=(-1); }
	if( y<0 ){ y*=(-1); }
	return x%y;
}

// Open the given if (if it exists) and clear all of its existing contents
void clearContents(string filename){
	std::ofstream file;
	file.open(filename, std::ofstream::out | std::ofstream::trunc);
	file.close();
}

// Prints the chars of the string independently
void printStr(char *str){
	int i=0;
	while( str[i] != 0){
		printf("%c", str[i]);
		i++;
	} printf("\n");
}

// Print the "UI header"
void print_header(void){
	cout << "\033[33;1m _____________________________________________________________________________________ " << endl;
	cout << "|                                                                                     |" << endl;
	cout << "|\033[0m                    \033[33;1mSoftware Development for Algorithmic Problems\033[0m                    \033[33;1m|" << endl;
	cout << "|\033[0m                             \033[33;1m1st Assignment 2021-2021\033[0m                                \033[33;1m|" << endl;
	cout << "|\033[0m                      \033[36;1mRovithakis Ioannis - Christos Tsouchlaris\033[0m                      \033[33;1m|" << endl;
	cout << "|\033[0m                              \033[36;1msdi1800164 - sdi1800204\033[0m                                \033[33;1m|" << endl;
	cout << "|_____________________________________________________________________________________|\033[0m" << endl;
	cout << endl;
}

// Print the "UI foooter"
void print_footer(void){
	cout << "\033[33;1m _____________________________________________________________________________________ " << endl;
	cout << "|                                                                                     |" << endl;
	cout << "|\033[0m                                \033[36;1m Shutting Down\033[0m                                       \033[33;1m|" << endl;
	cout << "|_____________________________________________________________________________________|\033[0m" << endl;
	cout << endl;
}

// Print the Distance Divergence
void print_avg_divergence(double div){
	std::cout << "\033[33;1m (i) Average Distance Divergence: \033[0m" << div << std::endl;
}

// Print the total time
void print_total_time(double time){
	std::cout << "\033[33;1m (i) Total Execution Time: \033[0m" << time << " \033[33;1msec\033[0m" << std::endl << std::endl;
}

// Print the Amount of time to create the structs and load the data
void print_structs_created(double time){
	std::cout << "\033[36;1m (T) \033[33;1mLoading Completed: \033[0m" << time << " \033[33;1msec\033[0m" << std::endl << std::endl;
}

// Calculates the hamming distance between two given integers
int hamming_distance(int a, int b)
{
	int x = a ^ b;
	int distance = 0;

	while (x > 0)
	{
		distance += x & 1;
		x >>= 1;
  }

	return distance;
}


// We get a bit array and we convert it to it's decimal value
// f.e. { 0, 1, 1, 0, 1 } == 20
unsigned convert_bit_array_to_decimal(std::vector<unsigned> bit_array)
{
  int decimal;

  for (long unsigned i = 0; i < bit_array.size(); i++)
  {
    int bit = bit_array[i];
    int bit_value = bit * pow(2, i);
    decimal += bit_value;
    i++;
  }

  return decimal;
}

// When we get a hash key from an H hash function, we generate a bit
unsigned generate_bit_from_h_key(unsigned h_key)
{
	unsigned bit = (h_key % 2 == 0) ? 1 : 0;
	return bit;
}

// Calculate the dot product between the two given vectors
double dot_product(std::vector<int> x, std::vector<double> y){
	double sum = 0;
	for(unsigned i=0; i<(x.size()); i++){
		sum += x[i] * y[i];
	}
	return sum;
}

// Add the two vectors element to element
std::vector<int> sum_vectors(std::vector<int> *x, std::vector<int> *y){
	std::vector<int> sum;
	for(unsigned i=0; i<(x->size()); i++){
		sum.push_back( (*x)[i] + (*y)[i] );
	}
	return sum;
}

// Divide all the elements of 'x' by 'd'
std::vector<int> div_vector(std::vector<int> *x, unsigned d){
	std::vector<int> div;
	for(unsigned i=0; i<(x->size()); i++){
		div.push_back( (*x)[i] / d );
	}
	return div;
}

// Sum the elements of the given array
double sum_vector_elems(std::vector<double> *v){
	double sum=0;
	for(unsigned i=0; i<(v->size()); i++){
		sum += (*v)[i];
	}
	return sum;
}

// Return true if the given index is stored in the vector
bool index_exists(unsigned index, std::vector<unsigned> *v){
	for(unsigned i=0; i<(v->size()); i++){
		if( (*v)[i] == index ){ return true; }
	}
	return false;
}

// return true if x is less than y, but consider negative numbers == +inf
// True if x < y
bool positive_less_than(double x, double y){
	// Different signs
	if( x<0 && y>0 ){
		return false;
	}
	else if( x>0 && y<0 ){
		return true;
	}
	// Both are negative
	else if( x<0 && y<0 ){
		return false;
	}
	// Both are positive
	if(x<y){ return true; }
	return false;
}
