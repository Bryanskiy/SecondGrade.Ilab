#pragma once

#include <stdexcept>
#include <stack>

#ifdef DEBUG_
#include <iostream>
#endif

namespace avl {

template<typename T>
class tree_t {

public:
    tree_t() : head_(nullptr) {}

    void insert(const T& key);
#ifdef DEBUG_
    void dump() const {dump(head_);}
#endif    

private:

    class node_t {
    
        public:
            node_t(const T& key) : key_(key), height_(1), left_(nullptr), right_(nullptr) {}

            T        get_key()                           {return key_;}
            unsigned get_height() const                  {return height_;}
            node_t*  get_left()   const                  {return left_;};
            node_t*  get_right()  const                  {return right_;}

            void     set_height(unsigned char height)    {height_ = height;}
            void     set_left(node_t* left)              {left_ = left;}
            void     set_right(node_t* right)            {right_ = right;}

        private:
            T key_;
            unsigned char height_;
            node_t* left_;
            node_t* right_;
    };

    int balance_factor(node_t* node) const;
    node_t* rotate_right(node_t* node);
    node_t* rotate_left(node_t* node);
    node_t* balance(node_t* node);
    void fix_height(node_t* node);

#ifdef DEBUG_
    void check_height_invariant(node_t* node) const;
    void dump(node_t* node, const std::string& indent = "") const;
#endif

    node_t* head_;
};

template<typename T>
int tree_t<T>::balance_factor(node_t* node) const {
    node_t* left = node->get_left();
    node_t* right = node->get_right();

    if((left == right) && (left == nullptr)) {
        return 0;
    } else if(left == nullptr) {
        return right->get_height();
    } else if (right == nullptr) {
        return -left->get_height();
    }

    return right->get_height() - left->get_height();
}

template<typename T>
void tree_t<T>::insert(const T& key) {
    if(head_ == nullptr) {
        head_ = new node_t(key);
        return;
    }

    node_t* current = head_;
    std::stack<node_t*> path;

    while(current != nullptr) {
        path.push(current);
        if(current->get_key() < key) {
            current = current->get_right();
        } else if (current->get_key() > key) {
            current = current->get_left();
        } else {
            return;
        }
    }

    current = path.top(); path.pop();
    node_t* new_node = new node_t(key);
    if(current->get_key() < key) {
        current->set_right(new_node);
    } else {
        current->set_left(new_node);
    }

    /* balance */
    while(current != nullptr) {
        fix_height(current);
        node_t* tmp = balance(current);
        if(!path.size()) {
            head_ = tmp;
#ifdef DEBUG_
            check_height_invariant(head_);
#endif              
            return;
        }
        node_t* prev = path.top(); path.pop();
        if(prev->get_left() == current) {
            prev->set_left(tmp);
        } else {
            prev->set_right(tmp); 
        }

        current = prev;
    }

#ifdef DEBUG_
    check_height_invariant(head_);
#endif     
}

template<typename T>
typename tree_t<T>::node_t* tree_t<T>::rotate_right(node_t* node) {
    node_t* tmp = node->get_left();
    node->set_left(tmp->get_right());
    tmp->set_right(node);

    fix_height(node);
    fix_height(tmp);

    return tmp;
}

template<typename T>
typename tree_t<T>::node_t* tree_t<T>::rotate_left(node_t* node) {
    node_t* tmp = node->get_right();
    node->set_right(tmp->get_left());
    tmp->set_left(node);

    fix_height(node);
    fix_height(tmp);

    return tmp;
}

template<typename T>
typename tree_t<T>::node_t* tree_t<T>::balance(node_t* node) {

	if(balance_factor(node) == 2) {
		if((node->get_right() != nullptr) && (balance_factor(node->get_right()) < 0)) {
			node->set_right(rotate_right(node->get_right()));
        }    
		return rotate_left(node);
	}
	if(balance_factor(node) == -2) {
		if((node->get_left() != nullptr) && (balance_factor(node->get_left()) > 0)) {
			node->set_left(rotate_left(node->get_left()));
        }    
		return rotate_right(node);
	}
	return node;
}

template<typename T>
void tree_t<T>::fix_height(node_t* node) {
    node_t* left = node->get_left();
    node_t* right = node->get_right();

    if((left == right) && (left == nullptr)) {
        node->set_height(0);
    } else if(left == nullptr) {
        node->set_height(right->get_height() + 1);
    } else if (right == nullptr) {
        node->set_height(left->get_height() + 1);
    } else { 
        node->set_height((left->get_height() > right->get_height() ? left->get_height() : right->get_height()) + 1);
    }    
}
 
#ifdef DEBUG_
template<typename T>
void tree_t<T>::dump(node_t* node, const std::string& indent) const {
    if(node == nullptr) {
        std::cout << indent << "nullptr" << std::endl;
        return;
    }

    std::cout << indent << "key: " << node->get_key() << " height: " << 
    static_cast<unsigned>(node->get_height()) << std::endl;
    dump(node->get_left(), indent + "  ");
    dump(node->get_right(), indent + "  ");
}

template<typename T>
void tree_t<T>::check_height_invariant(node_t* node) const {
    if(node == nullptr) {
        return;
    }

    check_height_invariant(node->get_left());
    check_height_invariant(node->get_right());

    if(std::abs(balance_factor(node) >= 2)) {
        dump(head_);
        throw std::runtime_error("Height invariant is broken");
    }
}
#endif
}