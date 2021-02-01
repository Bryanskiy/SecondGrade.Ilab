#pragma once

#include <stdexcept>
#include <stack>

#ifdef DEBUG_
#include <iostream>
#endif

namespace avl {

template<typename T>
class tree_t final {

private:

    class node_t final {
    
        public:
            node_t(const T& key) : key_(key), height_(1), left_(nullptr), right_(nullptr) {}

            T        get_key()    const                           { return key_; }
            unsigned get_height() const                           { return height_; }
            node_t*  get_left()   const                           { return left_; };
            node_t*  get_right()  const                           { return right_; }

            void     set_height(unsigned char height)             { height_ = height; }
            void     set_left(node_t* left)                       { left_ = left; }
            void     set_right(node_t* right)                     { right_ = right; }

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


public:

    class iterator final {
        public:

            iterator() = default;
            iterator(const iterator &) = default;
		    iterator &operator=(const iterator &) = default;

            const T    operator*() const                            { return node_->get_key(); }
		    const T*   operator->() const                           { return node_; }

            bool       operator==(const iterator& rhs) const        { return this->node_ == rhs.node_; }
            bool       operator!=(const iterator& rhs) const        { return !(*this == rhs); }
            iterator   operator++();

        private:

            iterator(const tree_t<T>& outer_tree, node_t* node) : outer_tree_{outer_tree}, node_{node} {}
            friend tree_t<T>;

            const tree_t<T>&   outer_tree_ = nullptr;
            node_t*            node_       = nullptr;
    };

    tree_t() : head_(nullptr) {}
    ~tree_t();

    void     insert(const T& key);
    iterator lower_bound(const T& key) const;
    iterator upper_bound(const T& key) const;
    iterator begin() const;
    iterator end() const;

#ifdef DEBUG_
    void check_height_invariant() const {check_height_invariant(head_);}
    void dump() const {dump(head_);}
#endif    

    friend iterator;
};


/*---------------------------------------------------------
*
*   Implementation of tree methods
*
---------------------------------------------------------*/
template <typename T>
tree_t<T>::~tree_t() {
	if (head_)
	{
		std::vector<node_t*> arr;
		for (auto iter = begin(); iter != end(); ++iter)
			arr.push_back(iter.node_);
		for (node_t* node : arr)
			delete node;
	}
}

template<typename T>
int tree_t<T>::balance_factor(node_t* node) const {
    int left = (!node) ? node->get_left()->get_key() : 0;
    int right = (!node) ? node->get_right()->get_key() : 0;

    return right - left;
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
        if(!balance_factor(current)) {
            break;
        }

        node_t* tmp = balance(current);
        if(!path.size()) {
            head_ = tmp;
            break;
        }
        node_t* prev = path.top(); path.pop();
        if(prev->get_left() == current) {
            prev->set_left(tmp);
        } else {
            prev->set_right(tmp); 
        }

        current = prev;
    }    
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

    unsigned char left = (!node) ? node->get_left()->get_height() : 0;
    unsigned char right = (!node) ? node->get_right()->get_height() : 0;

    node->set_height(std::max(left, right));
}

template<typename T> 
typename tree_t<T>::iterator tree_t<T>::lower_bound(const T& key) const {
    node_t* prev = nullptr;
    node_t* current = head_;

    while(current != nullptr) {
        if(current->get_key() < key) {
            prev = current;
            current = current->get_right();
        } else if(current->get_key() > key) {
            prev = current;
            current = current->get_left();
        } else {
            return iterator{*this, current};
        }
    }

    iterator ret{*this, prev};

    if(prev->get_key() > key) {
        return ret;
    }

    return ++ret;
}

template<typename T> 
typename tree_t<T>::iterator tree_t<T>::upper_bound(const T& key) const {
    tree_t<T>::iterator tmp = lower_bound(key);
    if(tmp == this->end()) {
        return this->end();
    } else if(*tmp == key) {
        return ++tmp;
    }

    return tmp;
}

template<typename T> 
typename tree_t<T>::iterator tree_t<T>::begin() const {
    if(head_ == nullptr) {
        return iterator{*this, nullptr};
    }

    node_t* current = head_;
    while(current->get_left() != nullptr) {
        current = current->get_left();
    }

    return iterator{*this, current};
}

template<typename T> 
typename tree_t<T>::iterator tree_t<T>::end() const {
    return iterator{*this, nullptr};
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
/*---------------------------------------------------------
*
*   Implementation of iterator methods
*
---------------------------------------------------------*/

template<typename T>
typename tree_t<T>::iterator tree_t<T>::iterator::operator++() {
    node_t* current = outer_tree_.head_;
    node_t* successor = nullptr;

    T key = node_->get_key();

    while(current != nullptr) {
        if(current->get_key() > key) {
            successor = current;
            current = current->get_left();
        } else {
            current = current->get_right();
        }
    }

    node_ = successor;
    return iterator(this->outer_tree_, node_);
}

}
