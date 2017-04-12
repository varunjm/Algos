#ifndef AVL_TREE
#define AVL_TREE

struct Node{
    int value;
    int depth;
    Node* left;
    Node* right;
};

Node* get_node();

class AvlTree {
private:
    Node* root;

    void left_rotate(Node* rnode, Node* parent);
    void right_rotate(Node* rnode, Node* parent);
    void balance(Node* node, Node* parent);

public:

    AvlTree();
    bool insert_node(int value);
    bool remove_node(int value);
};

#endif
