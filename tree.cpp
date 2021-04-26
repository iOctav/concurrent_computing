#include "tree.h"
#include "tree_util.h"
#include <chrono>
#include <iostream>
#include <cmath>
#include <pthread.h>
#include <omp.h>

using namespace std;
using namespace std::chrono;
using namespace util;

tree::tree()
{
	root_ = nullptr;
}

tree::tree(unsigned int size, unsigned int *arr)
{
	if (size > 0 && arr[0] != 0)
	{
		root_ = deserialize(0, size, arr);
	}
	else
	{
		root_ = nullptr;
	}
}

tree::~tree()
{
	destroy_tree();
}

unsigned int tree::get_root_value()
{
	return root_->value;
}

unsigned long long tree::eval_sum()
{
	auto start = high_resolution_clock::now();
	
	unsigned long long sum = 0;
	if (root_ != nullptr)
	{
		sum = eval_sum_func(root_);		
	}
	else
	{
		sum = 0;
	}

	auto stop = high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();
	cout << "Time taken by sequential algorithm: " << duration << " seconds" << endl;

	return sum;
}

unsigned long long tree::eval_sum_pthread(unsigned int level)
{
	auto start = high_resolution_clock::now();

	unsigned long long sum = 0;
	if (root_ != nullptr)
	{
		struct pthread_node pnode;
		pnode.leaf = root_;
		pnode.level = level;
		eval_sum_pthread_func((void*) &pnode);
		sum = pnode.leaf->tree_sum;
	}
	else
	{
		sum = 0;
	}

	auto stop = high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();
	cout << "Time taken by pthread algorithm with " << pthread_count(level) << " threads: " << duration << " seconds" << endl;

	return sum;	
}

unsigned long long tree::eval_sum_openmp(unsigned int level)
{
	auto start = high_resolution_clock::now();
	//omp_set_max_active_levels(level - 1);
	unsigned long long sum = 0;
	if (root_ != nullptr)
	{
		struct pthread_node pnode;
		pnode.leaf = root_;
		pnode.level = level;
		eval_sum_pthread_func((void*) &pnode);
		sum = pnode.leaf->tree_sum;
	}
	else
	{
		sum = 0;
	}

	auto stop = high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();
	cout << "Time taken by pthread algorithm with " << pthread_count(level) << " threads: " << duration << " seconds" << endl;

	return sum;	
}

long double tree::eval_geom_mean()
{
	auto start = high_resolution_clock::now();
	
	long double sum = 0;
	if (root_ != nullptr)
	{
		sum = eval_geom_mean_func(root_);		
	}
	else
	{
		sum = 0;
	}

	auto stop = high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();
	cout << "Time taken by sequential algorithm: " << duration << " seconds" << endl;

	return sum;
}

long double tree::eval_geom_mean_pthread(unsigned int level)
{
	auto start = high_resolution_clock::now();

	long double sum = 0;
	if (root_ != nullptr)
	{
		struct pthread_node pnode;
		pnode.leaf = root_;
		pnode.level = level;
		eval_geom_mean_pthread_func((void*) &pnode);
		sum = pnode.leaf->tree_geometric_mean;
	}
	else
	{
		sum = 0;
	}

	auto stop = high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();
	cout << "Time taken by pthread algorithm with " << pthread_count(level) << " threads: " << duration << " seconds" << endl;

	return sum;	
}

long double tree::eval_geom_mean_openmp(unsigned int level)
{
	auto start = high_resolution_clock::now();
	//omp_set_max_active_levels(level - 1);
	long double sum = 0;
	if (root_ != nullptr)
	{
		struct pthread_node pnode;
		pnode.leaf = root_;
		pnode.level = level;
		eval_geom_mean_pthread_func((void*) &pnode);
		sum = pnode.leaf->tree_geometric_mean;
	}
	else
	{
		sum = 0;
	}

	auto stop = high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();
	cout << "Time taken by pthread algorithm with " << pthread_count(level) << " threads: " << duration << " seconds" << endl;

	return sum;	
}

unsigned int* tree::serialize()
{
	if (root_ != nullptr)
	{
		unsigned int max_size = max_size_tree(height(root_));
		unsigned int* arr = new unsigned int[max_size]();
		arr[0] = root_->value;
		serialize(root_, 0, arr);
	}
	return nullptr;
}

unsigned int tree::height()
{
	if (root_ != nullptr)
	{
		return height(root_);
	}
	return 0;
}


void tree::destroy_tree()
{
	destroy_tree(root_);
}

void tree::serialize(node* leaf, unsigned int parent_index, unsigned int* arr)
{
	if (leaf->left != nullptr)
	{
		arr[parent_index * 2 + 1] = leaf->left->value;
		serialize(leaf->left, parent_index * 2 + 1, arr);
	}
	if (leaf->right != nullptr)
	{
		arr[parent_index * 2 + 2] = leaf->right->value;
		serialize(leaf->right, parent_index * 2 + 2, arr);
	}
}

node* tree::deserialize(unsigned parent_index, unsigned int size, unsigned int* arr)
{
	node *parent = new node;
	parent->value = arr[parent_index];

	if (parent_index*2 + 2 >= size)
	{
		return parent;
	}
	
	if (arr[parent_index * 2 + 1] != 0)
	{
		parent->left = deserialize(parent_index * 2 + 1, size, arr);
	}
	if (arr[parent_index * 2 + 2] != 0)
	{
		parent->right = deserialize(parent_index * 2 + 2, size, arr);
	}
	return parent;
}

unsigned int tree::height(node* leaf)
{
	unsigned int left_height = 0;
	unsigned int right_height = 0;
	if (leaf->left != nullptr)
	{
		left_height = height(leaf->left);
	}
	if (leaf->right != nullptr)
	{
		right_height = height(leaf->right);
	}
	
	if (left_height > right_height)
	{
		return left_height + 1;
	}
	return right_height + 1;
}



void tree::destroy_tree(node *leaf)
{
	if(leaf != nullptr)
    {
		destroy_tree(leaf->left);
		destroy_tree(leaf->right);
		delete leaf;
	}
}

unsigned long long eval_sum_func(node* leaf)
{
	if (leaf->left != nullptr)
	{
		leaf->tree_sum += eval_sum_func(leaf->left);
	}
	if (leaf->right != nullptr)
	{
		leaf->tree_sum += eval_sum_func(leaf->right);
	}
	leaf->tree_sum += leaf->value;
	return leaf->tree_sum;
}

void *eval_sum_pthread_func(void *args)
{
	struct pthread_node *pnode = (struct pthread_node *)args;
	if (pnode->level > 0)
	{
		pthread_t thread1, thread2;
		int iret1, iret2;
		if (pnode->leaf->left != nullptr)
		{
			struct pthread_node left_pnode;
			left_pnode.leaf = pnode->leaf->left;
			left_pnode.level = pnode->level - 1;
			iret1 = pthread_create(&thread1, NULL, eval_sum_pthread_func, (void*) &left_pnode);
		}
		if (pnode->leaf->right != nullptr)
		{
			struct pthread_node right_pnode;
			right_pnode.leaf = pnode->leaf->right;
			right_pnode.level = pnode->level - 1;
			iret2 = pthread_create(&thread2, NULL, eval_sum_pthread_func, (void*) &right_pnode);
		}

		if (pnode->leaf->left != nullptr)
		{
			pthread_join(thread1, NULL);
			if (iret1 == 0)
			{
				pnode->leaf->tree_sum += pnode->leaf->left->tree_sum;
			} 
			else 
			{
				cerr << "Could not join thread " << iret1 << endl;
				exit(-2);
			}
		}

		if (pnode->leaf->right != nullptr)
		{
			pthread_join(thread2, NULL);
			if (iret2 == 0)
			{
				pnode->leaf->tree_sum += pnode->leaf->right->tree_sum;
			}
			else 
			{
				cerr << "Could not join thread. Status: " << iret2 << endl;
				exit(-2);
			}
		}

		pnode->leaf->tree_sum += pnode->leaf->value;
	}
	else
	{
		eval_sum_func(pnode->leaf);
	}

	return 0;
}

long double eval_geom_mean_func(node* leaf)
{
	unsigned short power = 1;
	if (leaf->left != nullptr)
	{
		leaf->tree_geometric_mean *= eval_geom_mean_func(leaf->left);
		power++;
	}
	if (leaf->right != nullptr)
	{
		leaf->tree_geometric_mean *= eval_geom_mean_func(leaf->right);
		power++;
	}
	leaf->tree_geometric_mean = pow(leaf->tree_geometric_mean * leaf->value, 1.0 / power);
	return leaf->tree_geometric_mean;
}

void *eval_geom_mean_pthread_func(void *args)
{
	struct pthread_node *pnode = (struct pthread_node *)args;
	if (pnode->level > 0)
	{
		pthread_t thread1, thread2;
		int iret1, iret2;
		unsigned short power = 1;

		if (pnode->leaf->left != nullptr)
		{
			struct pthread_node left_pnode;
			left_pnode.leaf = pnode->leaf->left;
			left_pnode.level = pnode->level - 1;
			iret1 = pthread_create(&thread1, NULL, eval_geom_mean_pthread_func, (void*) &left_pnode);
		}
		if (pnode->leaf->right != nullptr)
		{
			struct pthread_node right_pnode;
			right_pnode.leaf = pnode->leaf->right;
			right_pnode.level = pnode->level - 1;
			iret2 = pthread_create(&thread2, NULL, eval_geom_mean_pthread_func, (void*) &right_pnode);
		}

		if (pnode->leaf->left != nullptr)
		{
			pthread_join(thread1, NULL);
			if (iret1 == 0)
			{
				pnode->leaf->tree_geometric_mean *= pnode->leaf->left->tree_geometric_mean;
			} 
			else 
			{
				cerr << "Could not join thread " << iret1 << endl;
				exit(-2);
			}
		}

		if (pnode->leaf->right != nullptr)
		{
			pthread_join(thread2, NULL);
			if (iret2 == 0)
			{
				pnode->leaf->tree_geometric_mean *= pnode->leaf->right->tree_geometric_mean;
			}
			else 
			{
				cerr << "Could not join thread. Status: " << iret2 << endl;
				exit(-2);
			}
		}

		pnode->leaf->tree_geometric_mean = pow(pnode->leaf->value * pnode->leaf->tree_geometric_mean, 1.0 / power);
	}
	else
	{
		eval_geom_mean_func(pnode->leaf);
	}

	return 0;
}