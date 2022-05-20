# BST
The BST class will use a Node class with more feature. 
In addition to pointers to the left and right children, now the Node class will also have a pointer to the parent of a node.
The BST class has two private member variables, root_, a pointer to the root of the tree, and size_, which maintains the number of keys in the tree.

Implemented Functions:
  - void insert(T k);
      // Inserts the key k into the tree;
      // Update size and height of nodes;
  - Node* successor(T k);
      // Return a pointer to the inorder successor of the key k;
  - void delete_min();
      // Remove the minimum key from the tree;
      // Update size and height of nodes;
  - void erase(T k);
      // Remove node containing key k;
      // Update size and height of nodes;
  - void rotate_right(Node* node);
      // Implement a right rotation about the node pointed to by node;
      // Update size and height of nodes;
  - Node* find(T k);
      // Return a pointer to the node that corresponds the key;
  - Node* min();
      // Return a pointer to the node holding the minimum key;
