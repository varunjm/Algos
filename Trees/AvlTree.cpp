#include "AvlTree.hpp"
#include <iostream>
#include <cmath>
#include <climits>
#include <stack>
#define BLOCKSIZE 256
#define INF INT_MAX

Node* get_node() {
    Node* newNode = new Node();
    newNode->left = newNode->right = nullptr;
    newNode->depth = 0;

    return newNode;
}

int max(int a, int b) {
    return a > b? a : b;
}

AvlTree::AvlTree() {
    root = get_node();
    root->value = INF;
}

void AvlTree::left_rotate(Node* rnode, Node* parent) {
    if(parent->left == rnode) {
        parent->left = rnode->right;
        rnode->right = parent->left->left;
        parent->left->left = rnode;

        rnode->depth = 1 + max(rnode->left->depth, rnode->right->depth);
        parent->left->depth = 1 + max(rnode->depth, parent->left->right->depth);
        parent->depth =  1 + max(parent->left->depth, (parent->right) ? parent->right->depth : 0);
    } else {
        parent->right = rnode->right;
        rnode->right = parent->right->left;
        parent->right->left = rnode;

        rnode->depth = 1 + max(rnode->left->depth, rnode->right->depth);
        parent->right->depth = 1 + max(rnode->depth, parent->right->right->depth);
        parent->depth =  1 + max(parent->left->depth, (parent->right) ? parent->right->depth : 0);
    }
}

void AvlTree::right_rotate(Node* rnode, Node* parent) {
    if(parent->left == rnode) {
        parent->left = rnode->left;
        rnode->left = parent->left->right;
        parent->left->right = rnode;

        rnode->depth = 1 + max(rnode->left->depth, rnode->right->depth);
        parent->left->depth = 1 + max(rnode->depth, parent->left->left->depth);
        parent->depth =  1 + max(parent->left->depth, (parent->right) ? parent->right->depth : 0);
    } else {
        parent->right = rnode->left;
        rnode->left = parent->right->right;
        parent->right->right = rnode;

        rnode->depth = 1 + max(rnode->left->depth, rnode->right->depth);
        parent->right->depth = 1 + max(rnode->depth, parent->right->left->depth);
        parent->depth =  1 + max(parent->left->depth, (parent->right) ? parent->right->depth : 0);
    }
}

void AvlTree::balance(Node* node, Node* parent) {
    int root_flag = 0;   // True if node == root
    Node *temp = get_node();
    int flag = 0, left_depth = -1, right_depth = -1;

    if(parent == nullptr) {
        root_flag = 1;
        parent = get_node();
        parent->left = node;
    }
    if(node->left) {
        left_depth = node->left->depth;
    }
    if(node->right) {
        right_depth = node->right->depth;
    }
    if(left_depth > right_depth) {        // right rotate
        flag = 1;
        int left_left_depth = -1;

        if(node->left->left) {
	        left_left_depth = node->left->left->depth;
	    }
        if(node->left->left->depth - node->right->depth == 1) { // no double rotation (Case 2.1)
            right_rotate(node, parent);
        } else {                                                // left right rotation (Case 2.2)
            left_rotate(node->left, node);
            right_rotate(node, parent);
        }
    } else {                                            // left rotate
        flag = 2;
        int right_right_depth = -1;

        if(node->right->right) {
	        right_right_depth = node->right->right->depth;
	    }
	    if(right_right_depth - left_depth == 1) { 				// no double rotation (Case 2.1)
            left_rotate(node, parent);
        } else {                                                // right left rotation (Case 2.2)
            right_rotate(node->right, node);
            left_rotate(node, parent);
        }
    }

    if(root_flag) {     // swap node and parent->left (to maintain root node)
        temp->left = parent->left->left;
        temp->right = parent->left->right;
        temp->depth = parent->left->depth;

        parent->left->left = node->left;
        parent->left->right = node->right;
        parent->left->depth = node->depth;

        switch(flag) {
            case 1:
                node->left = temp->left;
                node->right = parent->left;     // address of the original root (ignoring dummy root)
                break;
            case 2:
                node->left = parent->left;
                node->right = temp->right;     // address of the original root (ignoring dummy root)
                break;
        }
        node->depth = temp->depth;

        delete parent;
        delete temp;
    }
}

bool AvlTree::insert_node(int value) {
    // If first insertion, replace the value of the already created node
    if(root->value == INF) {
        root->value = value;
        return true;
    }

    Node* current = root;
    Node* newNode = get_node();
    std::stack<Node *> ancestors;

    // Failed to allocate memory to a new node
    if (newNode == nullptr) {
        return false;
    }

    while (true) {
        ancestors.push(current);
    	if (value <= current->value) {
            if (current->left == nullptr) {
                current->left = newNode;
                current->left->value = value;
                break;
            } else {
                current = current->left;
            }
        } else {
            if (current->right == nullptr) {
                current->right = newNode;
                current->right->value = value;
                break;
            } else {
                current = current->right;
            }
        }
    }

    while(!ancestors.empty()) {
        Node* ancestor = ancestors.top();
        ancestors.pop();
        int left_depth = -1, right_depth = -1;
        // if there is an imbalance at this point
        if(ancestor->left) {
            left_depth = ancestor->left->depth;
        }
        if(ancestor->right) {
            right_depth = ancestor->right->depth;
        }
        if(abs(left_depth - right_depth) > 1) {
            // if we are at the root node, special case for root balancing
            if(ancestors.empty())
                balance(ancestor, nullptr);
            // normal balancing, pass node(stack[i]) and its parent(stack[i-1])
            else
                balance(ancestor, ancestors.top());
        } else {    // No imbalance, update depth accoring to children
            ancestor->depth = max(left_depth, right_depth) + 1;
        }
    }

    return true;
}

bool AvlTree::remove_node(int value) {
    // If tree is empty remove is unsucessful
    if (root->value == INF) {
        return false;
    }

    Node* current = root;
    Node* parent = nullptr;
    Node* toBeDeleted;
    Node* successor;
    Node* successorParent = nullptr;
    std::stack<Node *> ancestors;

    while (true) {
        ancestors.push(current);
    	if (value == current->value) {
            if (current->right == nullptr and current->left == nullptr) {
                if(parent == nullptr) {
                    current->value = INF;
                } else {
                    parent->left = nullptr;
                    delete current;
                }
            } else if (current->right == nullptr) {
                toBeDeleted = current->left;
                current->value = current->left->value;
                current->right = current->left->right;
                current->left = current->left->left;
                delete toBeDeleted;
            } else {
                successor = current->right;
                successorParent = nullptr;

                while(successor->left != nullptr) {
                    successorParent = successor;
                    successor = successor->left;
                }

                if (successorParent == nullptr) {
                    current->right = successor->right;
                } else {
                    successorParent->left = successor->right;
                }
                current->value = successor->value;
                delete successor;
            }

            break;
        } else {
            parent = current;

            if (current->value < value) {  // Search in the right subtree
                current = current->right;
            } else {                    // Search in the left subtree
                current = current->left;
            }

            if (current == nullptr) {
            	return false;
            }
        }
    }

    while(!ancestors.empty()) {
        Node* ancestor = ancestors.top();
        ancestors.pop();
        int left_depth = -1, right_depth = -1;
        // if there is an imbalance at this point
        if(ancestor->depth == 0) {          // after deletion ancestor changes, this handles the corner cases stemming from that
            continue;
        }
        if(ancestor->left) {
            left_depth = ancestor->left->depth;
        }
        if(ancestor->right) {
            right_depth = ancestor->right->depth;
        }
        if(abs(left_depth - right_depth) > 1) {
            // if we are at the root node, special case for root balancing
            if(ancestors.empty())
                balance(ancestor, nullptr);
            // normal balancing, pass node(stack[i]) and its parent(stack[i-1])
            else
                balance(ancestor, ancestors.top());
        } else {    // No imbalance, update depth accoring to children
            ancestor->depth = max(left_depth, right_depth) + 1;
        }
    }
    return true;
}

int main() {
    AvlTree tree;
    
    tree.insert_node(10);
    tree.insert_node(20);
    tree.insert_node(30);
    tree.insert_node(40);

    return 0;
}
