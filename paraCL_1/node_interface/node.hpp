#pragma once

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

#include "Inode.hpp"

namespace Inode {

class integer_t final : public Inode_t {
private:    
    int val_;
public:
    int calc() override {return val_;}
    integer_t(int val) : val_(val) {}
};

class decl_t final : public Inode_t {
private:    
    int val_;
public:
    int calc() override {return val_;};
    decl_t(int val) : val_(val) {}
    decl_t() = default;
    void set(int val) {val_ = val;}
};

class scope_t final : public Iscope_t {
private:    
    std::vector<Inode_t*> branches_;
    Iscope_t* prev_scope_;
public:
    int calc() override;

    Iscope_t* duplicate() override {return new scope_t(this);}
    Iscope_t* reset() const override {return prev_scope_;}
    void add_branch(Inode_t* branch) override {branches_.push_back(branch);}
    virtual Inode_t* add(const std::string& name) override;
    virtual Inode_t* visible(const std::string& name) override; 

    scope_t(scope_t* prev_scope) : prev_scope_(prev_scope) {}
    ~scope_t() override;
};

class bin_op_t final : public Inode_t {
private:    
    Inode_t* lhs_;
    Inode_t* rhs_;
    bin_op op_;
public:
    int calc() override;

    bin_op_t(Inode_t* lhs, bin_op op, Inode_t* rhs) : lhs_(lhs), op_(op), rhs_(rhs) {}
    ~bin_op_t() override;
};

class unary_op_t final : public Inode_t {
private:    
    Inode_t* node_;
    unary_op op_;
public:
    int calc() override;

    unary_op_t(Inode_t* node, unary_op op) : node_(node), op_(op) {}
    ~unary_op_t() override;    
};

class if_t final : public Inode_t {
private:
    Inode_t* condition_;
    Inode_t* scope_;

public:
    int calc() override;

    if_t(Inode_t* condition, Inode_t* scope) : condition_(condition), scope_(scope) {} 
};

class while_t final : public Inode_t {
private:
    Inode_t* condition_;
    Inode_t* scope_;

public:
    int calc() override;

    while_t(Inode_t* condition, Inode_t* scope) : condition_(condition), scope_(scope) {}
};

}