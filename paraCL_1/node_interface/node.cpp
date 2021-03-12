#include "node.hpp"

namespace node {

/* -------------------------------------------------- */


int scope_t::execute() {
    for(const auto& branch : branches_) {
        branch->execute();
    }

    return 0;
}

node_t* scope_t::search_global(const std::string& variable) const {
    node_t* ret = nullptr;
    if(table_.find(variable) != table_.end()) {
        return table_.at(variable);
    } 

    if(parent_scope_) {
        ret = parent_scope_->search_global(variable);
    }

    return ret;
}

node_t* scope_t::search_local(const std::string& variable) const {
    node_t* ret = nullptr;
    if(table_.find(variable) != table_.end()) {
        ret = table_.at(variable);
    }

    return ret;
}

scope_t::~scope_t() {
    for (auto& node : branches_) {
        delete node;
    }    
}

/* -------------------------------------------------- */

int operator_t::execute() {
    int ret = 0;
    switch(op_) {
        case operator_type::plus_:
            return lhs_->execute() + rhs_->execute();

        case operator_type::minus_:
            return lhs_->execute() - rhs_->execute();

        case operator_type::greater_:
            return lhs_->execute() > rhs_->execute();

        case operator_type::less_:
            return lhs_->execute() < rhs_->execute();

        case operator_type::greater_or_equal_:
            return rhs_->execute() >= rhs_->execute();

        case operator_type::less_or_equal_:
            return lhs_->execute() <= rhs_->execute();

        case operator_type::assign_:
            ret = rhs_->execute();
            dynamic_cast<variable_t*>(lhs_)->set(ret);
            return ret;

        case operator_type::equal_:
            return lhs_->execute() == rhs_->execute();

        case operator_type::not_equal_: 
            return lhs_->execute() != rhs_->execute();
        

        case operator_type::and_:
            return lhs_->execute() && rhs_->execute();

        case operator_type::or_:
            return lhs_->execute() || rhs_->execute();

        case operator_type::div_:
            return lhs_->execute() / rhs_->execute();
            break;

        case operator_type::mult_:
            return lhs_->execute() * rhs_->execute();
            break;

        case operator_type::mod_:
            return lhs_->execute() % rhs_->execute();
            break;

        case operator_type::output_:
            std::cout << rhs_->execute() << ::std::endl;
            return 0;

        case operator_type::input_:
            std::cin >> ret;
            return ret;

        case operator_type::not_:
            return !static_cast<bool>(lhs_->execute());    
    }

    return 0;
}

/* -------------------------------------------------- */

int while_t::execute() {
    while(condition_->execute()) {
        scope_->execute();
    }

    return 0;
}

/* -------------------------------------------------- */

int if_t::execute() {
    if(condition_->execute()) {
        scope_->execute();
    }
    
    return 0;
}

}