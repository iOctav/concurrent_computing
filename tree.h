struct node
{
    unsigned int value = 0;
    unsigned long long tree_sum = 0;
    long double tree_geometric_mean = 1;
    node *left = nullptr;
    node *right = nullptr;
};

struct pthread_node
{
    unsigned int level = 0;
    node *leaf = nullptr;
};

class tree
{
    public:
        tree();
		tree(unsigned int size, unsigned int *arr);
        ~tree();

        unsigned int get_root_value();
		unsigned long long eval_sum();
		unsigned long long eval_sum_pthread(unsigned int level);
		unsigned long long eval_sum_openmp(unsigned int level);
		long double eval_geom_mean();
		long double eval_geom_mean_pthread(unsigned int level);
		long double eval_geom_mean_openmp(unsigned int level);
		unsigned int* serialize();
		unsigned int height();
        void destroy_tree();

    private:
		void serialize(node *leaf, unsigned int parent_index, unsigned int* arr);
		node* deserialize(unsigned int parent_index, unsigned int size, unsigned int* arr);
        void destroy_tree(node *leaf);
		unsigned int height(node* leaf);

        node *root_;
};

unsigned long long eval_sum_func(node *leaf);
void *eval_sum_pthread_func(void *args);
unsigned long long eval_sum_openmp_func(node *leaf);
long double eval_geom_mean_func(node *leaf);
void *eval_geom_mean_pthread_func(void *args);
long double eval_geom_mean_openmp_func(node *leaf);
