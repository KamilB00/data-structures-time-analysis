//
// Created by Kamil Bonkowski on 02/04/2021.
//

#ifndef DATASTRUCTURES_REDBLACKTREE_H
#define DATASTRUCTURES_REDBLACKTREE_H

#include <iostream>

#define BLACK 0
#define RED 1
#define BOTH_BLACK 2

using namespace std;


class Node {
    int color;
    int data;

public:
    Node *parent;
    Node *left;
    Node *right;

    Node(int data) {
        this->data = data;
        parent = left = right = NULL;
        color = RED;
    }

    Node(int data, int color) {
        this->data = data;
        this->color = color;
        parent = left = right = NULL;
    }

    void setColor(int color) {
        this->color = color;
    }

    int getColor() {
        return this->color;
    }

    int getData() {
        return this->data;
    }

    void setData(int data) {
        this->data = data;
    }

};

Node *AddToTree(Node *root, Node *node) {
    if (root == NULL)
        return node;
    if (root->getData() > node->getData()) {
        root->left = AddToTree(root->left, node);
        root->left->parent = root;
    } else if (root->getData() <= node->getData()) {
        root->right = AddToTree(root->right, node);
        root->right->parent = root;
    }
    return root;

}

class RedBlackTree {
    Timer *timer = timer->getInstance();
    Node *root = nullptr;

public:
    RedBlackTree(const string &file_name) {
        string amount;
        string element;
        fstream file;
        file.open(file_name, ios::in);

        if (!file.good()) {
            cout << "File" << file_name << ".txt does not exist ! [RB TREE]" << endl;
        } else {
            cout << "Values are being loaded to the tree !" << endl;
            getline(file, amount);
            for (int i = 0; i < atoi(amount.c_str()); i++) {
                getline(file, element);
                fillTheTree(atoi(element.c_str()));
            }
            cout << "RB Tree filled with data !" << endl;
        }
    }

    void fillTheTree(int data) {
        Node *newNode = new Node(data);
        this->root = AddToTree(this->root, newNode);
        fixInsertViolation(newNode);
    }

    Node *getRoot() {
        return root;
    }

    //Swapping nodes color
    void swapNodeColor(Node *node1, Node *node2) {
        int tempColor = node1->getColor();
        node1->setColor(node2->getColor());
        node2->setColor(tempColor);

    }


    int getBlackHeight(Node *root) {
        if (root == NULL || root->getData() == -1)
            return 0;
        if (root->getColor() == BLACK) {
            return 1 + max(getBlackHeight(root->left), getBlackHeight(root->right));
        } else
            return max(getBlackHeight(root->left), getBlackHeight(root->right));
    }

    Node *getMaxValueNode(Node *root) {
        if (root == NULL)
            return NULL;
        else if (root->right == NULL)
            return root;
        else
            return getMaxValueNode(root->right);
    }


    Node *getGrandParent(Node *node) {
        if ((node != NULL) && (node->parent != NULL))
            return node->parent->parent;
        return NULL;

    }

    void rotateLeft(Node *&node) {
        Node *rightNode = node->right;
        Node *nodeParent = node->parent;
        node->right = rightNode->left;

        if (node->right != NULL)
            node->right->parent = node;

        rightNode->parent = nodeParent;

        if (node->parent == NULL)
            this->root = rightNode;
        else if (node == node->parent->left)
            node->parent->left = rightNode;
        else
            node->parent->right = rightNode;

        rightNode->left = node;
        node->parent = rightNode;
    }

    void rotateRight(Node *&node) {
        Node *nodeLeft = node->left;
        node->left = nodeLeft->right;

        if (node->left != NULL)
            node->left->parent = node;

        nodeLeft->parent = node->parent;

        if (node->parent == NULL)
            this->root = nodeLeft;
        else if (node == node->parent->left)
            node->parent->left = nodeLeft;
        else
            node->parent->right = nodeLeft;

        nodeLeft->right = node;
        node->parent = nodeLeft;

    }


    void fixInsertViolation(Node *&node) {

        Node *parentNode = NULL;
        Node *grandParentNode = NULL;

        //if node is root then color node to black
        //if node is red then it's parent can not be red
        while ((node != root) && (node->getColor() != BLACK) && (node->parent->getColor() == RED)) {

            parentNode = node->parent;
            grandParentNode = getGrandParent(node);


            //  NODE IS PARENT LEFT CHILD
            if (parentNode == grandParentNode->left) {

                Node *uncleNode = grandParentNode->right;

                /* Case: 1
                   The aunt of node is also red  FLIP COLORS
                   */
                if (uncleNode != NULL && uncleNode->getColor() == RED) {
                    grandParentNode->setColor(RED);
                    parentNode->setColor(BLACK);
                    uncleNode->setColor(BLACK);
                    node = grandParentNode;
                } else {
                    /* Case : 2
                       node is right child of its parent ROTATE LEFT
                       */
                    if (node == parentNode->right) {
                        rotateLeft(parentNode);

                        node = parentNode;
                        parentNode = node->parent;
                    }

                    /* Case : 3
                       node is left child of its parent ROTATE RIGHT
                      */
                    rotateRight(grandParentNode);
                    swapNodeColor(parentNode, grandParentNode);
                    node = parentNode;
                }
            }

                // NODE IS PARENT RIGHT CHILD

            else {
                Node *uncleNode = grandParentNode->left;

                /*  Case : 1
                    The aunt of node is also red FLIP COLOR */
                if ((uncleNode != NULL) && (uncleNode->getColor() == RED)) {
                    grandParentNode->setColor(RED);
                    parentNode->setColor(BLACK);
                    uncleNode->setColor(BLACK);
                    node = grandParentNode;
                } else {
                    /* Case : 2
                       node is left child of its parent ROTATE RIGHT
                        */
                    if (node == parentNode->left) {
                        rotateRight(parentNode);
                        node = parentNode;
                        parentNode = node->parent;
                    }

                    /* Case : 3
                       node is right child of its parent ROTATE LEFT
                        */
                    rotateLeft(grandParentNode);
                    swapNodeColor(parentNode, grandParentNode);
                    node = parentNode;
                }
            }
        }

        root->setColor(BLACK);
    }

    void addNode(int data) {

        int times = 0;
        cout << "How many times do you want to add this value: " << endl;
        cin >> times;
        for (int i = 0; i < times; i++) {
            auto start = std::chrono::steady_clock::now(); //START [LIST FIND BY VALUE]
            Node *newNode = new Node(data);

            this->root = AddToTree(this->root, newNode);

            fixInsertViolation(newNode);

            auto end = std::chrono::steady_clock::now();
            double elapsed_time = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
            timer->calculate_average_elapsed_time(elapsed_time, "ADD_TO_RED_BLACK_TREE");
            timer->showAvgTime("ADD_TO_RED_BLACK_TREE");
        }

    }

    Node *getDeletingNodePosition(Node *root, int value) {

        if (root == NULL)
            return root;
        //if value is in right subtree
        if (value > root->getData())
            return getDeletingNodePosition(root->right, value);

            //if value is in left subtree
        else if (value < root->getData())
            return getDeletingNodePosition(root->left, value);

        //if value is in root
        if (root->left == NULL || root->right == NULL)
            return root;

        Node *temp = getMaxValueNode(root->left);
        if (temp != NULL) {
            root->setData(temp->getData());
            return getDeletingNodePosition(root->left, temp->getData());
        }
    }

    void fixDeleteViolation(Node *node) {

        if (node == NULL)
            return;

        cout << root->getData();

        if (node == root) {
            root = NULL;
            return;
        }

        //if one of node or its child is red
        if (node->getColor() == RED || node->left->getColor() == RED || node->right->getColor() == RED) {

            Node *child = nullptr;
            if ((node->left != NULL) && (node->left->getData() != -1)) {
                child = node->left;
            } else if ((node->right != NULL) && (node->right->getData() != -1)) {
                child = node->right;
            }

            //if node is left child
            if (node == node->parent->left) {
                node->parent->left = child;
                if (child != NULL) {
                    child->parent = node->parent;
                    child->setColor(BLACK);
                }
                delete (node);
            }
                //if node is right child
            else {
                node->parent->right = child;
                if (child != NULL) {
                    child->parent = node->parent;
                    child->setColor(BLACK);
                }
                delete (node);
            }
        }
            //if both node and it's child is black
        else {
            Node *child = NULL;
            Node *parent = NULL;
            Node *ptr = node;
            ptr->setColor(BOTH_BLACK);
            while (ptr != root && ptr->getColor() == BOTH_BLACK) {
                parent = ptr->parent;

                if (ptr == parent->left) {
                    child = parent->right;

                    if (!child->right) {
                        child->right = new Node(-1, BLACK);
                        child->right->parent = child;
                    }
                    if (!child->left) {
                        child->left = new Node(-1, BLACK);
                        child->left->parent = child;
                    }

                    //If child is red
                    //Make child black
                    //Make parent red
                    //Rotate parent left
                    if (child->getColor() == RED) {
                        child->setColor(BLACK);
                        parent->setColor(RED);
                        rotateLeft(parent);
                    } else {
                        //If child is black
                        //Make child red
                        //If parent is black make parent double black
                        //If parent is red make parent black
                        if (child->left->getColor() == BLACK && child->right->getColor() == BLACK) {
                            child->setColor(RED);
                            if (parent->getColor() == RED)
                                parent->setColor(BLACK);
                            else
                                parent->setColor(BOTH_BLACK);
                            ptr = parent;
                        } else {
                            if (child->right->getColor() == BLACK) {
                                child->left->setColor(BLACK);
                                child->setColor(RED);
                                rotateRight(child);
                                child = parent->right;
                            }
                            child->setColor(parent->getColor());
                            parent->setColor(BLACK);
                            child->right->setColor(BLACK);
                            rotateLeft(parent);
                            break;
                        }
                    }
                } else {
                    child = parent->left;
                    if (!child->right) {
                        child->right = new Node(-1, BLACK);
                        child->right->parent = child;
                    }
                    if (!child->left) {
                        child->left = new Node(-1, BLACK);
                        child->left->parent = child;
                    }
                    if (child->getColor() == RED) {
                        child->setColor(BLACK);
                        parent->setColor(RED);
                        rotateRight(parent);
                    } else {
                        if (child->left->getColor() == BLACK && child->right->getColor() == BLACK) {
                            child->setColor(RED);
                            if (parent->getColor() == RED)
                                parent->setColor(BLACK);
                            else
                                parent->setColor(BOTH_BLACK);
                            ptr = parent;
                        } else {
                            if (child->left->getColor() == BLACK) {
                                child->right->setColor(BLACK);
                                child->setColor(RED);
                                rotateLeft(child);
                                child = parent->left;
                            }
                            child->setColor(parent->getColor());
                            parent->setColor(BLACK);
                            child->left->setColor(BLACK);
                            rotateRight(parent);
                            break;
                        }
                    }
                }
            }
            if (node == node->parent->left)
                node->parent->left = NULL;
            else
                node->parent->right = NULL;
            delete (node);
            root->setColor(BLACK);
        }

    }

    void deleteNode(int data) {
        int times = 0;
        cout << "How many times do you want to add this value: " << endl;
        cin >> times;
        for (int i = 0; i < times; i++) {


            Node *nodeToDelete = getDeletingNodePosition(this->root, data);
            if (nodeToDelete == nullptr) {
                cout << "Value " << data << " not found in the tree !" << endl;
            } else {
                auto start = std::chrono::steady_clock::now(); //START [RBT REMOVE BY VALUE]
                if (!nodeToDelete->right) {
                    nodeToDelete->right = new Node(-1, BLACK);
                    nodeToDelete->right->parent = nodeToDelete;
                }
                if (!nodeToDelete->left) {
                    nodeToDelete->left = new Node(-1, BLACK);
                    nodeToDelete->left->parent = nodeToDelete;
                }
                fixDeleteViolation(nodeToDelete);
                auto end = std::chrono::steady_clock::now();
                double elapsed_time = double(
                        std::chrono::duration_cast<std::chrono::nanoseconds>(
                                end - start).count());
                timer->calculate_average_elapsed_time(elapsed_time,
                                                      "REMOVE_FROM_RED_BLACK_TREE");
                timer->showAvgTime("REMOVE_FROM_RED_BLACK_TREE");
            }

        }

    }

    void preorderTraversal(Node *node) {
//        cout<<"ROOT (preorder traversal): "<<root->getData()<<endl;
        if (node != NULL) {
            cout << node->getData() << " " << (node->getColor() == 1 ? "[RED]" : "[BLACK] ") << endl;
            if (node->parent != NULL) {
                cout << "Parent [" << node->parent->getData() << "] "
                     << (node->parent->getColor() == 1 ? "[RED]" : "[BLACK] ") << endl;
            } else if (node->parent == NULL) {
                cout << "Parent [NULL] [BLACK]" << endl;

            }
            if (node->left != NULL) {
                cout << "Left Child [" << node->left->getData() << "] "
                     << (node->left->getColor() == 1 ? "[RED]" : "[BLACK] ") << endl;
            } else if (node->left == NULL) {
                cout << "Left Child [NULL] [BLACK]" << endl;
            }
            if (node->right != NULL) {
                cout << "Right Child [" << node->right->getData() << "]"
                     << (node->right->getColor() == 1 ? "[RED]" : "[BLACK] ") << endl;
            } else if (node->right == NULL) {
                cout << "Right Child [NULL] [BLACK]" << endl;
            }
            cout << endl;
            preorderTraversal(node->left);
            preorderTraversal(node->right);
        }
    }

    bool find(Node *node, int data) {

        if (node == nullptr) {
            return false;
        } else {
            if (node->getData() == data) {
                return true;
            } else if (data < node->getData()) {
                return find(node->left, data);
            } else if (data >= node->getData()) {
                return find(node->right, data);
            }
        }

    }

};


#endif //DATASTRUCTURES_REDBLACKTREE_H
