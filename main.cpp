#include <iostream>
#include <vector>
#include <algorithm>

/*!
 * @brief B+ Tree implementation.
 *
 * @tparam KeyType Type of the keys in the tree.
 * @tparam ValueType Type of the values associated with the keys.
 */
template <typename KeyType, typename ValueType>
class BPlusTree {
private:
    static const int ORDER = 3; /*!< Order of the B+ tree. */

    /*!
     * @brief Node structure representing a node in the B+ tree.
     */
    struct Node {
        bool isLeaf; /*!< Indicates whether the node is a leaf node. */
        std::vector<KeyType> keys; /*!< Keys stored in the node. */
        std::vector<ValueType> values; /*!< Values associated with the keys. */
        Node* parent; /*!< Pointer to the parent node. */
        std::vector<Node*> children; /*!< Pointers to child nodes. */
    };

    Node* root; /*!< Pointer to the root of the tree. */

public:
    /*!
     * @brief Constructor for the B+ Tree.
     */
    BPlusTree() : root(nullptr) {}

    /*!
     * @brief Destructor for the B+ Tree.
     */
    ~BPlusTree() {
        clearTree(root);
    }

    /*!
     * @brief Inserts a key-value pair into the B+ Tree.
     *
     * @param key The key to insert.
     * @param value The value associated with the key.
     */
    void insert(const KeyType& key, const ValueType& value) {
        if (!root) {
            root = new Node;
            root->isLeaf = true;
            root->keys.push_back(key);
            root->values.push_back(value);
        } else {
            insertKey(root, key, value);
        }
    }

    /*!
     * @brief Removes a key from the B+ Tree.
     *
     * @param key The key to remove.
     */
    void remove(const KeyType& key) {
        if (root) {
            removeKey(root, key);
            if (root->keys.empty() && root->children.size() == 1) {
                Node* newRoot = root->children[0];
                delete root;
                root = newRoot;
            }
        }
    }

    /*!
     * @brief Prints the B+ Tree.
     */
    void print() const {
        printTree(root, 0);
    }

private:

/**
     * @brief Inserts a key-value pair into the B+ Tree starting from the given node.
     *
     * @param node The node to start the insertion.
     * @param key The key to insert.
     * @param value The value associated with the key.
     */
    void insertKey(Node* node, const KeyType& key, const ValueType& value) {
        if (node->isLeaf) {
            insertLeaf(node, key, value);
        } else {
            int childIndex = findChildIndex(node, key);
            insertKey(node->children[childIndex], key, value);

            if (node->keys.size() < ORDER - 1) {
                insertNonFull(node, key, value);
            } else {
                splitNode(node, key, value);
            }
        }
    }

    /**
     * @brief Inserts a key-value pair into a leaf node.
     *
     * @param node The leaf node to insert into.
     * @param key The key to insert.
     * @param value The value associated with the key.
     */
    void insertLeaf(Node* node, const KeyType& key, const ValueType& value) {
        auto it = std::lower_bound(node->keys.begin(), node->keys.end(), key);

        if (it == node->keys.end() || *it != key) {
            size_t index = std::distance(node->keys.begin(), it);
            node->keys.insert(it, key);
            node->values.insert(node->values.begin() + index, value);
        }
    }
    /**
     * @brief Inserts a key-value pair into a non-full internal node.
     *
     * @param node The internal node to insert into.
     * @param key The key to insert.
     * @param value The value associated with the key.
     */
    void insertNonFull(Node* node, const KeyType& key, const ValueType& value) {
        auto it = std::lower_bound(node->keys.begin(), node->keys.end(), key);
        size_t index = std::distance(node->keys.begin(), it);

        node->keys.insert(it, key);
        node->values.insert(node->values.begin() + index, value);
        node->children.insert(node->children.begin() + index + 1, nullptr);
    }

    /**
     * @brief Splits an internal node when it is full.
     *
     * @param node The internal node to split.
     * @param key The key to insert after the split.
     * @param value The value associated with the key.
     */
    void splitNode(Node* node, const KeyType& key, const ValueType& value) {
        int middleIndex = ORDER / 2;
        KeyType middleKey = node->keys[middleIndex];

        Node* left = new Node;
        Node* right = new Node;

        left->isLeaf = node->isLeaf;
        right->isLeaf = node->isLeaf;

        left->keys.assign(node->keys.begin(), node->keys.begin() + middleIndex);
        left->values.assign(node->values.begin(), node->values.begin() + middleIndex);
        left->children.assign(node->children.begin(), node->children.begin() + middleIndex + 1);

        right->keys.assign(node->keys.begin() + middleIndex + 1, node->keys.end());
        right->values.assign(node->values.begin() + middleIndex + 1, node->values.end());
        right->children.assign(node->children.begin() + middleIndex + 1, node->children.end());

        Node* parent = node->parent;
        if (!parent) {
            parent = new Node;
            parent->isLeaf = false;
            parent->children.push_back(left);
        }

        auto parentInsertPos = std::lower_bound(parent->keys.begin(), parent->keys.end(), middleKey);
        size_t parentIndex = std::distance(parent->keys.begin(), parentInsertPos);

        parent->keys.insert(parentInsertPos, middleKey);
        parent->values.push_back(value);
        parent->children[parentIndex] = left;
        parent->children.insert(parent->children.begin() + parentIndex + 1, right);

        if (parent->keys.size() >= ORDER) {
            splitNode(parent, middleKey, value);
        }

        delete node;
    }

    /**
     * @brief Finds the index of the child node where the key should be inserted.
     *
     * @param node The internal node.
     * @param key The key to find the child index for.
     * @return The index of the child node.
     */
    int findChildIndex(const Node* node, const KeyType& key) const {
        auto it = std::upper_bound(node->keys.begin(), node->keys.end(), key);
        return std::distance(node->keys.begin(), it);
    }

    /**
     * @brief Removes a key from the B+ Tree starting from the given node.
     *
     * @param node The node to start the removal.
     * @param key The key to remove.
     */
    void removeKey(Node* node, const KeyType& key) {
        if (node->isLeaf) {
            removeKeyFromLeaf(node, key);
        } else {
            int childIndex = findChildIndex(node, key);
            Node* child = node->children[childIndex];

            if (child->keys.size() >= (ORDER + 1) / 2) {
                removeKey(child, key);
            } else {
                handleUnderflow(node, childIndex);
                removeKey(node->children[childIndex], key);
            }
        }
    }

    /**
     * @brief Removes a key from a leaf node.
     *
     * @param node The leaf node to remove from.
     * @param key The key to remove.
     */
    void removeKeyFromLeaf(Node* node, const KeyType& key) {
        auto it = std::lower_bound(node->keys.begin(), node->keys.end(), key);
        if (it != node->keys.end() && *it == key) {
            size_t index = std::distance(node->keys.begin(), it);
            node->keys.erase(it);
            node->values.erase(node->values.begin() + index);
        }
    }

    /**
     * @brief Handles underflow of a node during removal.
     *
     * @param parentNode The parent node.
     * @param childIndex The index of the child node.
     */
    void handleUnderflow(Node* parentNode, int childIndex) {
        Node* leftSibling = (childIndex > 0) ? parentNode->children[childIndex - 1] : nullptr;
        Node* rightSibling = (childIndex < parentNode->children.size() - 1) ? parentNode->children[childIndex + 1] : nullptr;

        if (leftSibling && leftSibling->keys.size() > (ORDER + 1) / 2) {
            borrowFromLeftSibling(parentNode, childIndex, leftSibling);
        } else if (rightSibling && rightSibling->keys.size() > (ORDER + 1) / 2) {
            borrowFromRightSibling(parentNode, childIndex, rightSibling);
        } else if (leftSibling) {
            mergeWithLeftSibling(parentNode, childIndex, leftSibling);
        } else if (rightSibling) {
            mergeWithRightSibling(parentNode, childIndex, rightSibling);
        }
    }

    /**
     * @brief Borrows a key from the left sibling during underflow handling.
     *
     * @param parentNode The parent node.
     * @param childIndex The index of the child node.
     * @param leftSibling The left sibling node.
     */
    void borrowFromLeftSibling(Node* parentNode, int childIndex, Node* leftSibling) {
        Node* childNode = parentNode->children[childIndex];
        childNode->keys.insert(childNode->keys.begin(), parentNode->keys[childIndex - 1]);
        childNode->values.insert(childNode->values.begin(), leftSibling->values.back());

        parentNode->keys[childIndex - 1] = leftSibling->keys.back();
        leftSibling->keys.pop_back();
        leftSibling->values.pop_back();

        if (!childNode->isLeaf) {
            Node* borrowedChild = leftSibling->children.back();
            leftSibling->children.pop_back();
            childNode->children.insert(childNode->children.begin(), borrowedChild);
            borrowedChild->parent = childNode;
        }
    }

    /**
     * @brief Borrows a key from the right sibling during underflow handling.
     *
     * @param parentNode The parent node.
     * @param childIndex The index of the child node.
     * @param rightSibling The right sibling node.
     */
    void borrowFromRightSibling(Node* parentNode, int childIndex, Node* rightSibling) {
        Node* childNode = parentNode->children[childIndex];
        childNode->keys.push_back(parentNode->keys[childIndex]);
        childNode->values.push_back(rightSibling->values.front());

        parentNode->keys[childIndex] = rightSibling->keys.front();
        rightSibling->keys.erase(rightSibling->keys.begin());
        rightSibling->values.erase(rightSibling->values.begin());

        if (!childNode->isLeaf) {
            Node* borrowedChild = rightSibling->children.front();
            rightSibling->children.erase(rightSibling->children.begin());
            childNode->children.push_back(borrowedChild);
            borrowedChild->parent = childNode;
        }
    }

    
    void mergeWithLeftSibling(Node* parentNode, int childIndex, Node* leftSibling) {
        Node* childNode = parentNode->children[childIndex];

        leftSibling->keys.push_back(parentNode->keys[childIndex - 1]);
        leftSibling->keys.insert(leftSibling->keys.end(), childNode->keys.begin(), childNode->keys.end());
        leftSibling->values.insert(leftSibling->values.end(), childNode->values.begin(), childNode->values.end());

        if (!childNode->isLeaf) {
            leftSibling->children.insert(leftSibling->children.end(), childNode->children.begin(), childNode->children.end());
            updateParentPointers(leftSibling->children, leftSibling);
        }

        parentNode->keys.erase(parentNode->keys.begin() + childIndex - 1);
        parentNode->children.erase(parentNode->children.begin() + childIndex);
        delete childNode;
    }
    /**
     * @brief Merges the current node with its right sibling.
     *
     * This function merges the specified child node with its right sibling node.
     * It updates keys, values, and children accordingly and deallocates the right sibling node.
     *
     * @param parentNode The parent node of the merging nodes.
     * @param childIndex The index of the child node within the parent's children.
     * @param rightSibling The right sibling node to merge with.
     */ 
    void mergeWithRightSibling(Node* parentNode, int childIndex, Node* rightSibling) {
        Node* childNode = parentNode->children[childIndex];

        childNode->keys.push_back(parentNode->keys[childIndex]);
        childNode->keys.insert(childNode->keys.end(), rightSibling->keys.begin(), rightSibling->keys.end());
        childNode->values.insert(childNode->values.end(), rightSibling->values.begin(), rightSibling->values.end());

        if (!childNode->isLeaf) {
            childNode->children.insert(childNode->children.end(), rightSibling->children.begin(), rightSibling->children.end());
            updateParentPointers(childNode->children, childNode);
        }

        parentNode->keys.erase(parentNode->keys.begin() + childIndex);
        parentNode->children.erase(parentNode->children.begin() + childIndex + 1);
        delete rightSibling;
    }
    
    void updateParentPointers(std::vector<Node*>& children, Node* parent) {
        for (auto child : children) {
            child->parent = parent;
        }
    }
    
    void printTree(const Node* node, int level) const {
        if (node) {
            std::cout << "Level " << level << ": ";
            for (size_t i = 0; i < node->keys.size(); ++i) {
                std::cout << node->keys[i] << " ";
            }
            std::cout << std::endl;

            for (const auto& child : node->children) {
                printTree(child, level + 1);
            }
        }
    }

    void clearTree(Node* node) {
        if (node) {
            for (auto child : node->children) {
                clearTree(child);
            }
            delete node;
        }
    }
};

int main() {
    BPlusTree<int, std::string> bPlusTree;

    bPlusTree.insert(5, "Five");
    bPlusTree.insert(3, "Three");
    bPlusTree.insert(7, "Seven");
    bPlusTree.insert(1, "One");
    bPlusTree.insert(4, "Four");
    bPlusTree.insert(6, "Six");
    bPlusTree.insert(8, "Eight");
    bPlusTree.insert(2, "Two");

    std::cout << "B+ Tree:" << std::endl;
    bPlusTree.print();

    bPlusTree.remove(4);
    bPlusTree.remove(6);

    std::cout << "\nAfter removing keys 4 and 6:" << std::endl;
    bPlusTree.print();

    return 0;
}
