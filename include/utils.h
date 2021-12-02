#ifndef UTILS
#define UTILS

#include <string>
#include <vector>

unsigned getFileLines(std::string name);
unsigned getFileLineLength(std::string name);
bool fileExists(std::string name);

double max(double x, double y);

unsigned mod(unsigned x, unsigned y);
void clearContents(std::string filename);

bool question(std::string q);
void printStr(char *str);

void print_header(void);
void print_footer(void);
void print_avg_divergence(double div);

void print_total_time(double time);
void print_structs_created(double time);

int hamming_distance(int a, int b);
unsigned convert_bit_array_to_decimal(std::vector<unsigned> bit_array);
unsigned generate_bit_from_h_key(unsigned h_key);

double dot_product(std::vector<int> x, std::vector<double> y);
std::vector<int> sum_vectors(std::vector<int> *x, std::vector<int> *y);
std::vector<int> div_vector(std::vector<int> *x, unsigned d);
double sum_vector_elems(std::vector<double> *v);
bool index_exists(unsigned index, std::vector<unsigned> *v);

bool positive_less_than(double x, double y);

#endif
