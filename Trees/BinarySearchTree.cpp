/*
    Author:
    Varun Jayathirtha (www.github.com/varunjm)

    Binary Search Tree:
    A graph data structure where each node has a unidirectional path to at most
    two node, none of which are itself or its predecessors, i.e the graph is
    acyclic. The first node is called the root node. Another important feature
    of this tree is that each node has values lesser than or equal to itself in
    the left subtree and values greater than or equal to itself in the right
    subtree.
*/

#include "BinarySearchTree.hpp"
#include <iostream>
#include <climits>
#include <stack>
#define INF INT_MAX

using namespace std;

Node* get_node() {
    Node* newNode = new Node();
    newNode->left_child = newNode->right_child = nullptr;

    return newNode;
}

BinarySearchTree::BinarySearchTree() {
    root = get_node();
    root->value = INF;
}

/*
    Insert
    This traverses the tree to find the right spot to insert the newly given
    value. At each node, it checks if the node has a value lesser than or equal
    to itself, if so it goes to its left subtree. Otherwise it goes to the right
    subtree. When it meets an end point i.e. NULL it inserts the new Node along
    with the new value

    Time complexity:
    Best case : O(log n), where n is the number of nodes in the tree
    Worst case : O(n)

    Space complexity:
    O(1)
*/

bool BinarySearchTree::insert_node(int value) {
    // If first insertion, replace the value of the already created node
    if(root->value == INF) {
        root->value = value;
        return true;
    }

    Node* current = root;
    Node* newNode = get_node();

    // Failed to allocate memory to a new node
    if (newNode == nullptr) {
        return false;
    }

    while (true) {
        if (value <= current->value) {
            if (current->left_child == nullptr) {
                current->left_child = newNode;
                current->left_child->value = value;
                return true;
            } else {
                current = current->left_child;
            }
        } else {
            if (current->right_child == nullptr) {
                current->right_child = newNode;
                current->right_child->value = value;
                return true;
            } else {
                current = current->right_child;
            }
        }
    }
}

/*
    Remove
    This traverses the tree to find the given value. At each node, it checks if
    the node has a value equal to the given value, if so it finds the inorder
    successor of the node, and replaces the value to be deleted with that number.
    Otherwise, it searches in the left subtree if the value is smaller than
    node value and it searches the right subtree if the value is larger.

    Time complexity:
    Best case : O(log n), where n is the number of nodes in the tree
    Worst case : O(n)
*/

bool BinarySearchTree::remove_node(int value) {
    // If tree is empty remove is unsucessful
    if (root->value == INF) {
        return false;
    }

    Node* current = root;
    Node* parent = nullptr;
    Node* toBeDeleted;
    Node* successor;
    Node* successorParent = nullptr;

    while (true) {
        if (value == current->value) {
            if (current->right_child == nullptr and current->left_child == nullptr) {
                if(parent == nullptr) {
                    current->value = INF;
                } else {
                    parent->left_child = nullptr;
                    delete current;
                }
            } else if (current->right_child == nullptr) {
                toBeDeleted = current->left_child;
                current->value = current->left_child->value;
                current->right_child = current->left_child->right_child;
                current->left_child = current->left_child->left_child;
                delete toBeDeleted;
            } else {
                successor = current->right_child;
                successorParent = nullptr;

                while(successor->left_child != nullptr) {
                    successorParent = successor;
                    successor = successor->left_child;
                }

                if (successorParent == nullptr) {
                    current->right_child = successor->right_child;
                } else {
                    successorParent->left_child = successor->right_child;
                }
                current->value = successor->value;
                delete successor;
            }

            return true;
        } else {
            parent = current;

            if (current->value < value) {  // Search in the right subtree
                current = current->right_child;
            } else {                    // Search in the left subtree
                current = current->left_child;
            }

            if (current == nullptr) {
                return false;
            }
        }
    }
}

/*
    In order traversal:
    This Algorithm visits and displays each node in the following order starting
    with the root node. The Left subtree, followed by Parent, followed by the
    Right subtree. This traversal gives the values stored in the tree in a
    sorted order.

    Time complexity:
    O(n), where n is the number of nodes in the tree

    Space complexity:
    O(n), where n is the number if nodes in the tree
*/

void BinarySearchTree::traversal_inorder_helper(Node* tree) {
    if (tree == nullptr or tree->value == INF) {
        return;
    }

    traversal_inorder_helper(tree->left_child);
    cout << tree->value << endl;
    traversal_inorder_helper(tree->right_child);
}

void BinarySearchTree::traversal_inorder_recursive()  {
    traversal_inorder_helper(root);
}

void BinarySearchTree::traversal_inorder_iterative() {
    if (root->value == INF) {
        return;
    }

    stack<Node*> traversal;
    Node* current = root;
    bool complete = false;

    while (!complete) {
        if (current != nullptr) {
            traversal.push(current);
            current = current->left_child;
        } else if (!traversal.empty()) {
            current = traversal.top();
            traversal.pop();
            cout << current->value << endl;
            current = current->right_child;
        } else {
            complete = true;
        }
    }
}

/*
    Pre order traversal:
    This Algorithm visits and displays each node in the following order starting
    with the root node. Parent, followed by the Left subtree, followed by the
    Right subtree.

    Time complexity:
    O(n), where n is the number of nodes in the tree

    Space complexity:
    O(n), where n is the number if nodes in the tree
*/

void BinarySearchTree::traversal_preorder_helper(Node* tree) {
    if (tree == nullptr or tree->value == INF) {
        return;
    }

    cout << tree->value << endl;
    traversal_preorder_helper(tree->left_child);
    traversal_preorder_helper(tree->right_child);
}

void BinarySearchTree::traversal_preorder_recursive() {
    traversal_preorder_helper(root);
}

void BinarySearchTree::traversal_preorder_iterative() {
    if (root->value == INF) {
        return;
    }

    stack<Node*> traversal;
    Node* current;

    traversal.push(root);

    while (!traversal.empty()) {
        current = traversal.top();
        traversal.pop();
        cout << current->value << endl;

        if (current->right_child != nullptr) {
            traversal.push(current->right_child);
        }

        if (current->left_child != nullptr) {
            traversal.push(current->left_child);
        }
    }
}

/*
    Pre order traversal:
    This Algorithm visits and displays each node in the following order starting
    with the root node. The Left subtree, followed by the Right subtree,
    followed by the Parent.

    Time complexity:
    O(n), where n is the number of nodes in the tree

    Space complexity:
    O(n), where n is the number if nodes in the tree
*/

void BinarySearchTree::traversal_postorder_helper(Node* tree) {
    if (tree == nullptr or tree->value == INF) {
        return;
    } else {
        traversal_postorder_helper(tree->left_child);
        traversal_postorder_helper(tree->right_child);
        cout << tree->value << endl;
    }
}

void BinarySearchTree::traversal_postorder_recursive() {
    traversal_postorder_helper(root);
}

void BinarySearchTree::traversal_postorder_iterative() {
    if (root->value == INF) {
        return;
    }

    stack<Node*> traversal;
    Node* current = root;

    do
    {
        while (current)
        {
            if (current->right_child)
                traversal.push(current->right_child);
            traversal.push(current);

            current = current->left_child;
        }

        current = traversal.top();
        traversal.pop();

        // If the popped item has a right child and the right child is not
        // processed yet, then make sure right child is processed before root
        if (current->right_child and traversal.empty() == false and traversal.top() == current->right_child)
        {
            traversal.pop();
            traversal.push(current);
            current= current->right_child;
        } else {
            cout << current->value << endl;
            current = nullptr;
        }
    } while (!traversal.empty());
}

int main() {
    BinarySearchTree tree;

    tree.insert_node(10);
    tree.insert_node(14);
    tree.insert_node(12);
    tree.insert_node(5);

    cout << "In Order Traversal: \n";
    tree.traversal_inorder_recursive();

    cout << "Pre Order Traversal: \n";
    tree.traversal_preorder_recursive();

    cout << "Post Order Traversal: \n";
    tree.traversal_postorder_recursive();

    cout << "Removing 10\n";
    tree.remove_node(10);

    cout << "In Order Traversal: \n";
    tree.traversal_inorder_iterative();

    cout << "Pre Order Traversal: \n";
    tree.traversal_preorder_iterative();

    cout << "Post Order Traversal: \n";
    tree.traversal_postorder_iterative();

    return 0;
}
