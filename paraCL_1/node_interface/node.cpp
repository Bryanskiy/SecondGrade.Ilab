#include "node.hpp"

namespace Inode {

/*-------------------------SCOPE------------------------------*/

int scope_t::calc() {
    for(const auto& branch : branches_) {
        branch->calc();
    }

    return 0;
}

node_t* scope_t::add(const std::string& name) {
    node_t* node = visible(name);
    if(node) {
        return node;
    }

    decl_t* decl = new decl_t;
    symtab_[name] = decl;
    return decl;
}

node_t* scope_t::visible(const std::string& name) const {
    node_t* ret = nullptr;
    auto it = symtab_.find(name);

    if(it != symtab_.end()) {
        return it->second;
    }

    if(prev_scope_) {
        ret = prev_scope_->visible(name);
    }

    return ret;
}

scope_t::~scope_t() {
    for(const auto& x : branches_) {
        delete x;
    }
}

/*-------------------------BIN OP------------------------------*/
int bin_op_t::calc() {
    int ret = 0;
    switch(op_) {
        case bin_op::plus_:
            return lhs_->calc() + rhs_->calc();

        case bin_op::minus_:
            return lhs_->calc() - rhs_->calc();

        case bin_op::greater_:
            return lhs_->calc() > rhs_->calc();

        case bin_op::less_:
            return lhs_->calc() < rhs_->calc();

        case bin_op::greater_or_equal_:
            return rhs_->calc() >= rhs_->calc();

        case bin_op::less_or_equal_:
            return lhs_->calc() <= rhs_->calc();

        case bin_op::assign_:
            ret = rhs_->calc();
            dynamic_cast<decl_t*>(lhs_)->set(ret);
            return ret;

        case bin_op::equal_:
            return lhs_->calc() == rhs_->calc();

        case bin_op::not_equal_: 
            return lhs_->calc() != rhs_->calc();
        

        case bin_op::and_:
            return lhs_->calc() && rhs_->calc();

        case bin_op::or_:
            return lhs_->calc() || rhs_->calc();

        case bin_op::div_:
            return lhs_->calc() / rhs_->calc();
            break;

        case bin_op::mult_:
            return lhs_->calc() * rhs_->calc();
            break;

        case bin_op::mod_:
            return lhs_->calc() % rhs_->calc();
            break;
    }

    return 0;
}

bin_op_t::~bin_op_t() {
    delete lhs_;
    delete rhs_;
}

/*-------------------------UNARY OP-------------------------*/
int unary_op_t::calc() {
    int ret;
    switch(op_) {
        case unary_op::output_:
            std::cout << node_->calc() << ::std::endl;
            return 0;

        case unary_op::input_:
            std::cin >> ret;
            return ret;

        case unary_op::not_:
            return !static_cast<bool>(node_->calc());
    }

    return 0;
}


unary_op_t::~unary_op_t() {
    delete node_;
}

/*------------------------------IF------------------------*/

int if_t::calc() {
    if(condition_->calc()) {
        scope_->calc();
    }
    
    return 0;
}

/*--------------------------WHILE--------------------------*/
int while_t::calc() {
    while(condition_->calc()) {
        scope_->calc();
    }

    return 0;
}
}