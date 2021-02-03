#pragma once

namespace Inode {

#include <string>

struct Inode_t {
    virtual int calc() = 0;
    virtual ~Inode_t() {}
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


struct Iscope_t : Inode_t {
    virtual void add_branch(Inode_t* branch) = 0;
    virtual Iscope_t* duplicate() = 0;
    virtual Iscope_t* reset() const = 0;
    virtual Inode_t* add(const std::string& name) = 0;
    virtual Inode_t* visible(const std::string& name) = 0; 
};

Inode_t* make_value(int val);
Inode_t* make_decl();
Inode_t* make_bin_op(Inode_t* lhs, bin_op op, Inode_t* rhs);
Inode_t* make_unary_op(unary_op op, Inode_t* node);
Inode_t* make_unary_op(Inode_t* node, unary_op op);

Inode_t* make_if(Inode_t* condition, Inode_t* scope);
Inode_t* make_while(Inode_t* condition, Inode_t* scope);

Iscope_t* make_scope();
}