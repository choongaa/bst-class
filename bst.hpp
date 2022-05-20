#ifndef BST_ASSIGNMENT_HPP
#define BST_ASSIGNMENT_HPP

#include <iostream>
#include <algorithm>

template <typename T>
class BST
{
public:
    class Node 
    {
    public:
        T key;
        int height = 0;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;

        Node(){}

        Node(T k, Node* input_node = nullptr)
        {
            key = k;
            parent = input_node;
        }
    };

private:
    Node* root_ = nullptr;
    unsigned int size_ = 0;

public:
    BST(); 

    ~BST();

    void insert(T k);

    Node* successor(T k);

    void delete_min();

    void erase(T k);

    void rotate_right(Node* node);

    unsigned size();

    void print();

    Node* find(T k);

    std::vector<T> make_vec();

    std::vector<int> your_postorder_heights();

    std::vector<int> real_postorder_heights();

    T get_root_value();

    Node* min();

private:
    int get_height(Node* node);

    void fix_height(Node* node);

    void delete_subtree(Node* node);

    Node* min(Node* node);

    void print(Node* node);

    void make_vec(Node* node, std::vector<T>& vec);

    void your_postorder_heights(Node* node, std::vector<int>& vec);

    int real_postorder_heights(Node* node, std::vector<int>& vec);
};

template <typename T>
BST<T>::BST()
{
}

template <typename T>
BST<T>::~BST()
{
    delete_subtree(root_);
}

template <typename T>
void BST<T>::delete_subtree(Node* node)
{
    if(node==nullptr)
    {
        return;
    }
    delete_subtree(node->left);
    delete_subtree(node->right);
    delete node;
}

template <typename T>
void BST<T>::insert(T k)
{
    Node* node = root_;
    Node* prev_node = node;
    bool went_right;

    if(node == nullptr)
    {
        root_ = new Node(k);
        ++size_;
        return;
    }
    while(node != nullptr)
    {
        prev_node = node;
        if(k < node->key)
        {
            node = node->left;
            went_right = false;
        }
        else if (k > node->key)
        {
            node = node->right;
            went_right = true;
        }
        else
        {
            return;
        }
    }
    if(went_right)
    {
        prev_node->right= new Node(k, prev_node);
    }
    else
    {
        prev_node->left= new Node(k, prev_node);
    }
    ++size_;
    fix_height(find(k));
}

template <typename T>
typename BST<T>::Node* BST<T>::successor(T k)
{
    Node* successor = find(k);
    if(successor->right != nullptr)
    {
        return min(successor->right);
    }
    Node* successor_p = successor->parent;
    while(successor_p != nullptr && successor == successor_p->right)
    {
        successor = successor_p;
        successor_p = successor_p->parent;
    }
    return successor_p;
}

template <typename T>
void BST<T>::delete_min()
{
    if(root_ == nullptr)
    {
        return;
    }
    else
    {
        Node* min_node = min();
        if(min_node->parent == nullptr)
        {
            if(min_node->right != nullptr)
            {
                Node* temp = min_node->right;
                temp->parent = nullptr;
                root_ = temp;;
            }
            else
            {
                root_ = nullptr;
            }
        }
        else if(min_node->parent != nullptr)
        {
            Node* prev_node = min_node->parent;
            if(min_node->right == nullptr)
            {
                prev_node->left = nullptr;
            }
            else if(min_node->right != nullptr)
            {
                prev_node->left = min_node->right;
                min_node->right->parent = prev_node;
            }
        }
        fix_height(min_node);
        delete min_node;
        --size_;
    }
}

template <typename T>
void BST<T>::erase(T k)
{
    if(find(k) == nullptr)
    {
        return;
    }
    Node* node_to_remove = find(k);

    if(node_to_remove->right == nullptr && node_to_remove->left == nullptr)
    {
        if(node_to_remove->parent != nullptr)
        {
            Node* prev_node = node_to_remove->parent;
            if(prev_node->right == node_to_remove)
            {
                prev_node->right = nullptr;
            }
            else
            {
                prev_node->left = nullptr;
            }
        }
        else
        {
            root_ = nullptr;
        }
        fix_height(node_to_remove);
        delete node_to_remove;
        --size_;
    }
    else if(node_to_remove->right == nullptr && node_to_remove->left != nullptr)
    {
        Node* replacement = node_to_remove->left;
        if(node_to_remove->parent != nullptr)
        {
            Node* prev_node = node_to_remove->parent;
            if(prev_node->right == node_to_remove)
            {
                replacement->parent = prev_node;
                prev_node->right = replacement;
            }
            else
            {
                replacement->parent = prev_node;
                prev_node->left = replacement;
            }
        }
        else
        {
            replacement->parent = nullptr;
            root_ = replacement;
        }
        fix_height(node_to_remove);
        delete node_to_remove;
        --size_;
    }
    else if(node_to_remove->right != nullptr && node_to_remove->left == nullptr)
    {
        Node* replacement = node_to_remove->right;
        if(node_to_remove->parent != nullptr)
        {
            Node* prev_node = node_to_remove->parent;
            if(prev_node->right == node_to_remove)
            {
                replacement->parent = prev_node;
                prev_node->right = replacement;
            }
            else
            {
                replacement->parent = prev_node;
                prev_node->left = replacement;
            }
        }
        else
        {
            replacement->parent = nullptr;
            root_ = replacement;
        }
        fix_height(node_to_remove);
        delete node_to_remove;
        --size_;
    }
    else if(node_to_remove->left != nullptr && node_to_remove->right != nullptr)
    {
        Node* replacement = successor(node_to_remove->key);
        // node_to_remove has a parent;
        if(node_to_remove != root_)
        {
            Node* prev_node = node_to_remove->parent;
            Node* replacement_p = replacement->parent;

            if(prev_node->right == node_to_remove)
            {
                prev_node->right = replacement;
            }
            else
            {
                prev_node->left = replacement;
            }
            replacement->parent = prev_node;

            if(node_to_remove->right == replacement)
            {
                replacement->left = node_to_remove->left;
                node_to_remove->left->parent = replacement;
            }
            else
            {
                if(replacement->right != nullptr)
                {
                    replacement_p->left = replacement->right;
                    replacement->right->parent = replacement_p;
                }
                replacement->left = node_to_remove->left;
                node_to_remove->left->parent = replacement;
                replacement->right = node_to_remove->right;
                node_to_remove->right->parent = replacement;
            }
            fix_height(replacement);
            fix_height(node_to_remove);
            delete node_to_remove;
            --size_;
        }
        else
        {
            if(node_to_remove->right == replacement)
            {
                replacement->left = root_->left;
                root_->left->parent = replacement;
                replacement->parent = nullptr;
                root_ = replacement;
            }
            else if(node_to_remove->right != replacement)
            {
                Node* replacement_p = replacement->parent;
                replacement->parent = nullptr;
                root_ = replacement;
                if(replacement->right == nullptr && replacement->left == nullptr)
                {
                    replacement_p->left = nullptr;
                }
                else if(replacement->right != nullptr)
                { 
                    replacement_p->left = replacement->right;
                    replacement->right->parent = replacement_p;
                    fix_height(replacement->right);
                }
                replacement->left = node_to_remove->left;
                node_to_remove->left->parent = replacement;
                replacement->right = node_to_remove->right;
                node_to_remove->right->parent = replacement;
                fix_height(replacement_p);
            }
            fix_height(node_to_remove);
            fix_height(root_);
            delete node_to_remove;
            --size_;
        }
    }
}

template <typename T>
void BST<T>::rotate_right(Node* node)
{
    Node* move_up_node = node->left;

    node->left = move_up_node->right;
    if (node->left != nullptr) 
    {
        node->left->parent = node; 
    }
    move_up_node->parent = node->parent;
    if(node->parent == nullptr)
    {
        root_ = move_up_node;
    }
    else if(node == node->parent->right)
    {
        node->parent->right = move_up_node;
    }
    else 
    {
        node->parent->left = move_up_node; 
    }
    node->parent = move_up_node;
    move_up_node->right = node;
    fix_height(node);
}


template <typename T>
typename BST<T>::Node* BST<T>::min()
{
    if(root_ == nullptr)
    {
        return root_;
    }
    else
    {
        return min(root_);
    }
}

template <typename T>
typename BST<T>::Node* BST<T>::min(Node* node)
{
    while(node->left != nullptr)
    {
        node = node->left;
    } 
    return node;
}

template <typename T>
typename BST<T>::Node* BST<T>::find(T k)
{
    Node* node = root_;  
    while(node != nullptr && node->key != k)
    {
        node = k < node->key ?  node->left : node->right;
    }
    return node;  
}

template <typename T>
unsigned BST<T>::size()
{
    return size_;
}

template <typename T>
void BST<T>::print()
{
    print(root_);
}

template <typename T>
void BST<T>::print(Node* node)
{
    if(node == nullptr)
    {
        return;
    }
    print(node->left);
    std::cout << node->key << " height " << node->height << '\n';
    print(node->right);
}

template <typename T>
typename std::vector<T> BST<T>::make_vec()
{
    std::vector<T> vec;
    vec.reserve(size_);
    make_vec(root_, vec);
    return vec;
}

template <typename T>
void BST<T>::make_vec(Node* node, std::vector<T>& vec)
{
    if(node == nullptr)
    {
        return;
    }
    make_vec(node->left, vec);
    vec.push_back(node->key);
    make_vec(node->right, vec);
    
}

template <typename T>
void BST<T>::your_postorder_heights(Node* node, std::vector<int>& vec)
{
    if(node == nullptr)
    {
        return;
    }
    your_postorder_heights(node->left, vec);
    your_postorder_heights(node->right, vec);
    vec.push_back(node->height);
}

template <typename T>
int BST<T>::real_postorder_heights(Node* node, std::vector<int>& vec)
{
    if(node == nullptr)
    {
        return -1;
    }
    int left_height = real_postorder_heights(node->left, vec);
    int right_height = real_postorder_heights(node->right, vec);
    int node_height = 1 + std::max(left_height, right_height);
    vec.push_back(node_height);
    return node_height;
}

template <typename T>
typename std::vector<int> BST<T>::your_postorder_heights()
{
    std::vector<int> vec;
    vec.reserve(size_);
    your_postorder_heights(root_, vec);
    return vec;
}

template <typename T>
typename std::vector<int> BST<T>::real_postorder_heights()
{
    std::vector<int> vec;
    vec.reserve(size_);
    real_postorder_heights(root_, vec);
    return vec;
}

template <typename T>
T BST<T>::get_root_value()
{
    if(root_ == nullptr)
    {
        T dummy {};
        return dummy;
    }
    else
    {
        return root_->key;
    }
}

template <typename T>
int BST<T>::get_height(Node* node)
{
    if(node == nullptr)
    {
        return -1;
    }

    return 1 + std::max(get_height(node->left), get_height(node->right));
}

template <typename T>
void BST<T>::fix_height(Node* node)
{
    if(node->parent == nullptr)
    {
        node->height = get_height(node);
        return;
    }

    node->height = get_height(node);
    node = node->parent;
    fix_height(node);
}

#endif
