#pragma once

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>
#include <unordered_map>

namespace Inode {

#include <string>

struct node_t {
    virtual int calc() = 0;
    virtual ~node_t() {}
};

//binary operators
enum class bin_op {
    /* arithm */
    plus_,
    minus_,
    div_,
    mod_,
    mult_,

    /* logic */
    and_,
    or_,
    equal_,
    not_equal_,
    greater_,
    less_,
    greater_or_equal_,
    less_or_equal_,

    /* assign */
    assign_,
};

// unary operators
enum class unary_op {
    /* logical */
    not_,

    /* IO */
    output_,
    input_,
};

node_t* make_value(int val);
node_t* make_bin_op(node_t* lhs, bin_op op, node_t* rhs);
node_t* make_unary_op(unary_op op, node_t* node);
node_t* make_unary_op(node_t* node, unary_op op);

node_t* make_if(node_t* condition, node_t* scope);
node_t* make_while(node_t* condition, node_t* scope);


class integer_t final : public node_t {
public:
    integer_t(int val) : val_(val) {}

    int calc() override {return val_;}

private:    
    int val_;
};

class decl_t final : public node_t {
public:
    decl_t(int val) : val_(val) {}
    decl_t() = default;

    int calc() override {return val_;};
    void set(int val) {val_ = val;}
    int get_value() const {return val_;}
private:    
    int val_;    
};

decl_t* make_decl();

class scope_t final : public node_t {
public:
    scope_t(scope_t* prev_scope) : prev_scope_(prev_scope) {}
    ~scope_t() override;

    int calc() override;

    scope_t* duplicate() {return new scope_t(this);}
    scope_t* get_prev() const {return prev_scope_;}
    void add_branch(node_t* branch) {branches_.push_back(branch);}

    node_t* add(const std::string& name);
    node_t* visible(const std::string& name) const; 

private:    
    std::vector<node_t*> branches_;
    scope_t* prev_scope_;    
    std::unordered_map<std::string, decl_t*> symtab_;
};

scope_t* make_scope();

class bin_op_t final : public node_t {
public:
    bin_op_t(node_t* lhs, bin_op op, node_t* rhs) : lhs_(lhs), op_(op), rhs_(rhs) {}
    ~bin_op_t() override;

    int calc() override;
private:    
    node_t* lhs_;
    node_t* rhs_;
    bin_op op_;
};

class unary_op_t final : public node_t {
public:
    unary_op_t(node_t* node, unary_op op) : node_(node), op_(op) {}
    ~unary_op_t() override; 

    int calc() override;
private:    
    node_t* node_;
    unary_op op_;   
};

class if_t final : public node_t {
public:
    if_t(node_t* condition, node_t* scope) : condition_(condition), scope_(scope) {}  

    int calc() override;   
private:
    node_t* condition_;
    node_t* scope_;
};

class while_t final : public node_t {
private:
public:
    while_t(node_t* condition, node_t* scope) : condition_(condition), scope_(scope) {}

    int calc() override;

    node_t* condition_;
    node_t* scope_;
};

}