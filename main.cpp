#include "tree.h"
#include "tree_util.h"
#include <iostream>
#include <chrono>
#include <string.h>

using namespace std;
using namespace std::chrono;

tree *generate_random_tree(const unsigned short int level)
{
	auto start = high_resolution_clock::now();

	cout << "Start generating of tree with " << level << " levels" << endl;
	
	unsigned long long sum = 0;
	unsigned int size = util::max_size_tree(level);
	unsigned int* nodes = new unsigned int[size]();
	
	for (unsigned int i = 0; i < size; i++)
	{
		const unsigned int rand_int = rand() + 1;
		nodes[i] = rand_int;
		sum += rand_int;
	}
	
	tree* root = new tree(size, nodes);

	auto stop = high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();
	cout << "Time taken by inserting " << size << " nodes to the tree: " << duration << " seconds" << endl;

	cout << "Sum of all generated random integer (any is less than 32767) - " << sum << endl;
	return root;
}

int main(int argc, char *argv[])
{
    if (argc < 2) 
    {
		cout << "Pass type of operation: \"sum\" or \"geom\"" << endl << endl;
        return -1;
    }
    if (argc < 3) 
    {
        cout << util::max_size_tree(26) << " nodes:" << endl;
		tree *btree = generate_random_tree(26);
		if (strcmp(argv[1], "sum") == 0) 
        {
            unsigned long long sum = btree->eval_sum();	
            cout << "Evaluated sum of all nodes in tree - " << sum << endl << endl;
        }
        if (strcmp(argv[1], "geom") == 0) {
            unsigned long long sum = btree->eval_geom_mean();	
            cout << "Evaluated sum of all nodes in tree - " << sum << endl << endl;
        }
		delete btree;
    }
    else 
    {
        unsigned short int tree_level = atoi(argv[2]);
        cout << util::max_size_tree(tree_level) << " nodes:" << endl;
        tree *btree = generate_random_tree(tree_level);
        if (argc == 3) {
            if (strcmp(argv[1], "sum") == 0) 
            {
                unsigned long long sum = btree->eval_sum();	
                cout << "Evaluated sum of all nodes in tree - " << sum << endl << endl;
            }
            if (strcmp(argv[1], "geom") == 0) {
                unsigned long long mean = btree->eval_geom_mean();	
                cout << "Evaluated geom mean of root in tree - " << mean << endl << endl;
            }        
        } 
        else
        {
            if (strcmp(argv[3], "pthread") == 0) 
            {
                unsigned short int pthread_level = atoi(argv[4]);
                if (strcmp(argv[1], "sum") == 0) 
                {
                    unsigned long long sum = btree->eval_sum_pthread(pthread_level);	
                    cout << "Evaluated sum of all nodes in tree - " << sum << endl << endl;
                }
                if (strcmp(argv[1], "geom") == 0) {
                    unsigned long long mean = btree->eval_geom_mean_pthread(pthread_level);	
                    cout << "Evaluated geom mean of root in tree - " << mean << endl << endl;
                }            
            }
            if (strcmp(argv[3], "openmp") == 0)
            {         
                unsigned short int pthread_level = atoi(argv[4]);       
                if (strcmp(argv[1], "sum") == 0) 
                {
                    unsigned long long sum = btree->eval_sum_openmp(pthread_level);	
                    cout << "Evaluated sum of all nodes in tree - " << sum << endl << endl;
                }
                if (strcmp(argv[1], "geom") == 0) {
                    unsigned long long mean = btree->eval_geom_mean_openmp(pthread_level);	
                    cout << "Evaluated geom mean of root in tree - " << mean << endl << endl;
                }        
            }
        }
        delete btree;
    }
}