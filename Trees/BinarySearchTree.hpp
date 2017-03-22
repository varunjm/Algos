#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

struct Node {
    int value;
    Node* left_child;
    Node* right_child;
};

Node* get_node();

class BinarySearchTree {
private:
    Node* root;

    void traversal_inorder_helper(Node * tree);
    void traversal_preorder_helper(Node * tree);
    void traversal_postorder_helper(Node * tree);

public:

    BinarySearchTree();
    bool insert_node(int);
    bool remove_node(int);
    void traversal_inorder_recursive();
    void traversal_inorder_iterative();
    void traversal_preorder_recursive();
    void traversal_preorder_iterative();
    void traversal_postorder_recursive();
    void traversal_postorder_iterative();
};

#endif
