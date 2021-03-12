#pragma once

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "node.hpp"

namespace node {

enum class operator_type {
    /* arithm */
    plus_,
    minus_,
    div_,
    mod_,
    mult_,

    /* logic */
    and_,
    or_,
    not_,
    equal_,
    not_equal_,
    greater_,
    less_,
    greater_or_equal_,
    less_or_equal_,

    /* assign */
    assign_,

    /* io */
    input_,
    output_
};


/* -------------------------------------------------- */
class node_t {
public:
    virtual int execute() = 0;
    virtual ~node_t() {};
};

/* -------------------------------------------------- */
class scope_t final : public node_t {
public:
    scope_t(scope_t* parent) : parent_scope_(parent) {}

    int execute() override;
    scope_t* get_parent() const {return parent_scope_;}
    void add_branch(node_t* node) {branches_.push_back(node);}

    node_t* search_global(const std::string& variable) const;
    node_t* search_local(const std::string& variable) const;
    void make_visible(const std::string& variable, node_t* node) {table_[variable] = node;}

    ~scope_t() override;
private:    
    scope_t* parent_scope_ = nullptr;

    std::unordered_map<std::string, node_t*> table_;
    std::vector<node_t*> branches_;
};

/* -------------------------------------------------- */
class operator_t final : public node_t {
public:
    operator_t(node_t* lhs, operator_type op, node_t* rhs) : lhs_(lhs), op_(op), rhs_(rhs) {};
    int execute() override;
    ~operator_t() override {
        if(lhs_ != nullptr) delete lhs_;
        if(rhs_ != nullptr) delete rhs_;
    };

private:    
    node_t* lhs_ = nullptr;
    node_t* rhs_ = nullptr;
    operator_type op_;    
};

/* -------------------------------------------------- */
class integer_t final : public node_t {
public:
   integer_t(int value) : value_(value) {};
    int execute() override {return value_;}
   ~integer_t() {};

private:
    const int value_;
};

/* -------------------------------------------------- */
class variable_t final : public node_t {
public:

    variable_t() = default;
    ~variable_t() override {};

    void set(int value) {value_ = value;}
    int execute() override {return value_;}
private:
    int value_;    
};


/* -------------------------------------------------- */
class if_t final : public node_t {
public:
    if_t(node_t* condition, node_t* scope) : condition_(condition), scope_(scope) {}
    ~if_t() override {delete condition_; delete scope_;};

    int execute() override;
private:
    node_t* condition_;
    node_t* scope_;     
};


/* -------------------------------------------------- */
class while_t final : public node_t {
public:
    while_t(node_t* condition, node_t* scope) : condition_(condition), scope_(scope) {}
    ~while_t() override {delete condition_; delete scope_;};

    int execute() override;

private:
    node_t* condition_;
    node_t* scope_;    
};

}