#include <iostream>
#include <random>
#include <vector>
#include <chrono>
using namespace std;

struct Node {
    int key;
    int priority;
    Node* left;
    Node* right;

    Node(int k) : key(k), priority(rand()), left(nullptr), right(nullptr) {}
};

class SplayTree {
private:
    Node* root;

    // Левый поворот
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    // Правый поворот
    Node* rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        y->right = x;
        return y;
    }

    Node* splay(Node* root, int k) {
        if (!root)
            return nullptr;

        if (k < root->key) {
            if (!root->left)
                return root;

            if (k < root->left->key) {
                root->left->left = splay(root->left->left, k);
                root = rightRotate(root);
            }
            else if (k > root->left->key) {
                root->left->right = splay(root->left->right, k);
                if (root->left->right)
                    root->left = leftRotate(root->left);
            }

            if (!root->left)
                return root;
            else
                return rightRotate(root);
        }
        else if (k > root->key) {
            if (!root->right)
                return root;

            if (k < root->right->key) {
                root->right->left = splay(root->right->left, k);
                if (root->right->left)
                    root->right = rightRotate(root->right);
            }
            else if (k > root->right->key) {
                root->right->right = splay(root->right->right, k);
                root = leftRotate(root);
            }

            if (!root->right)
                return root;
            else
                return leftRotate(root);
        }
        else
            return root;
    }

public:
    SplayTree() : root(nullptr) {}

    Node* search(int key) {
        root = splay(root, key);
        return root->key == key ? root : nullptr;
    }

    // Вставка 
    void insert(int key) {
        if (!root) {
            root = new Node(key);
            return;
        }

        root = splay(root, key);
        if (root->key == key)
            return;

        Node* newRoot = new Node(key);
        if (key < root->key) {
            newRoot->right = root;
            newRoot->left = root->left;
            root->left = nullptr;
        }
        else {
            newRoot->left = root;
            newRoot->right = root->right;
            root->right = nullptr;
        }
        root = newRoot;
    }

    // Удаление 
    void remove(int key) {
        if (!root)
            return;

        root = splay(root, key);
        if (root->key == key) {
            if (!root->left) {
                Node* newRoot = root->right;
                delete root;
                root = newRoot;
            }
            else if (!root->right) {
                Node* newRoot = root->left;
                delete root;
                root = newRoot;
            }
            else {
                Node* newRoot = splay(root->left, key);
                newRoot->right = root->right;
                delete root;
                root = newRoot;
            }
        }
    }
};

// Рандомизированное дерево поиска
class RBST {
private:

    Node* root;
    random_device rd;
    mt19937 gen;

    void split(Node* tree, int key, Node*& left, Node*& right) {
        if (!tree) {
            left = nullptr;
            right = nullptr;
        } else if (key < tree->key) {
            split(tree->left, key, left, tree->left);
            right = tree;
        } else {
            split(tree->right, key, tree->right, right);
            left = tree;
        }
    }

    Node* merge(Node* left, Node* right) {
        if (!left || !right) {
            return left ? left : right;
        }
        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            return left;
        } else {
            right->left = merge(left, right->left);
            return right;
        }
    }

    void insert(Node*& tree, Node* newNode) {
        if (!tree) {
            tree = newNode;
        } else if (newNode->priority > tree->priority) {
            split(tree, newNode->key, newNode->left, newNode->right);
            tree = newNode;
        } else if (newNode->key < tree->key) {
            insert(tree->left, newNode);
        } else {
            insert(tree->right, newNode);
        }
    }

    bool search(Node* tree, int key) const {
        if (!tree) {
            return false;
        }
        if (key == tree->key) {
            return true;
        } else if (key < tree->key) {
            return search(tree->left, key);
        } else {
            return search(tree->right, key);
        }
    }

    Node* remove(Node* tree, int key) {
        if (!tree) {
            return nullptr;
        }
        if (key == tree->key) {
            Node* newRoot = merge(tree->left, tree->right);
            delete tree;
            return newRoot;
        } else if (key < tree->key) {
            tree->left = remove(tree->left, key);
        } else {
            tree->right = remove(tree->right, key);
        }
        return tree;
    }    

public:
    RBST() : root(nullptr), gen(rd()) {}

    void insert(int key) {
        Node* newNode = new Node(key);
        insert(root, newNode);
    }

    bool search(int key) const {
        return search(root, key);
    }

    void remove(int key) {
        root = remove(root, key);
    }
};

class Treap {
private:
    Node* root;

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    Node* insert(Node* root, int key, int priority) {
        if (!root)
            return new Node(key);

        if (key < root->key) {
            root->left = insert(root->left, key, priority);
            if (root->left->priority > root->priority)
                root = rotateRight(root);
        } else {
            root->right = insert(root->right, key, priority);
            if (root->right->priority > root->priority)
                root = rotateLeft(root);
        }

        return root;
    }

    Node* remove(Node* root, int key) {
        if (!root)
            return root;

        if (key < root->key)
            root->left = remove(root->left, key);
        else if (key > root->key)
            root->right = remove(root->right, key);
        else {
            if (!root->left) {
                Node* temp = root->right;
                delete root;
                root = temp;
            } else if (!root->right) {
                Node* temp = root->left;
                delete root;
                root = temp;
            } else {
                if (root->left->priority > root->right->priority)
                    root = rotateRight(root);
                else
                    root = rotateLeft(root);

                root = remove(root, key);
            }
        }

        return root;
    }

    bool search(Node* root, int key) const {
        if (!root)
            return false;
        if (key == root->key)
            return true;
        if (key < root->key)
            return search(root->left, key);
        return search(root->right, key);
    }

public:
    Treap() : root(nullptr) {}

    void insert(int key, int priority) {
        root = insert(root, key, priority);
    }

    void remove(int key) {
        root = remove(root, key);
    }

    bool search(int key) const {
        return search(root, key);
    }
};

int main() {

    SplayTree splayRandom, splayOrdered;
    RBST rbstRandom, rbstOrdered;
    Treap treapRandom, treapOrdered;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(1, 10000);

    // Заполнение деревьев
    vector<int> randomElements, orderedElements;
    for (int i = 0; i < 1000; ++i) {
        int randomNum = distribution(gen);

        splayRandom.insert(randomNum);
        rbstRandom.insert(randomNum);
        treapRandom.insert(randomNum, randomNum);
        randomElements.push_back(randomNum);

        orderedElements.push_back(i + 1);
        splayOrdered.insert(i + 1);
        rbstOrdered.insert(i + 1);
        treapOrdered.insert(i + 1, i + 1);
    }

    // Функция для тестирования
    auto Test = [](SplayTree& splay, RBST& rbst, Treap& treap, const vector<int>& numbers, const string& taskName) {
        auto start = chrono::high_resolution_clock::now();
        for (int num : numbers) {
            splay.search(num);
        }
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "SplayTree " + taskName + ": " << duration.count() << " микросекунд" << endl;

        start = chrono::high_resolution_clock::now();
        for (int num : numbers) {
            rbst.search(num);
        }
        end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "RBST " + taskName + ": " << duration.count() << " микросекунд" << endl;

        start = chrono::high_resolution_clock::now();
        for (int num : numbers) {
            treap.search(num);
        }
        end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Treap " + taskName + ": " << duration.count() << " микросекунд" << endl;
    };

    // Поиск 100 случайных чисел
    vector<int> searchNumbers;
    for (int i = 0; i < 100; ++i) {
        int randomIndex = distribution(gen) % 1000;
        searchNumbers.push_back(randomElements[randomIndex]);
    }

    Test(splayRandom, rbstRandom, treapRandom, searchNumbers, "поиск 100 случайных чисел");

    // Поиск чисел от 1 до 10
    vector<int> sequentialNumbers;
    for (int i = 1; i <= 10; ++i) {
        sequentialNumbers.push_back(i);
    }

    Test(splayOrdered, rbstOrdered, treapOrdered, sequentialNumbers, "поиск чисел от 1 до 10");

    // Удаление 100 случайных чисел
    vector<int> deleteNumbers;
    for (int i = 0; i < 100; ++i) {
        int randomIndex = distribution(gen) % 1000;
        deleteNumbers.push_back(randomElements[randomIndex]);
    }

    Test(splayRandom, rbstRandom, treapRandom, deleteNumbers, "удаление 100 случайных чисел");

    return 0;
}
