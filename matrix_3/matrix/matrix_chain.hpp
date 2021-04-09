#pragma once

#include "matrix.hpp"

namespace matrix {

class Imatrix_chain_t {
public:

    Imatrix_chain_t();

    void push(std::size_t rows, std::size_t cols);

    void dump(std::ostream& stream) const;

    std::vector<std::size_t> get_optimal_order() const {return optimal_order_; }
    std::size_t get_optimal_order_oper() const { return dp_table_[dp_table_size_ - 1][dp_table_size_ - 1];}

private:
    void calculate_optimal_order(std::size_t i, std::size_t j);
private:
    /* table dp_table_size_ * dp_table_size_ , which contain information about optimal order */
    matrix_t<std::size_t> dp_table_;
    std::size_t dp_table_size_ = 0; 
    std::size_t dp_table_capacity_ = 8; /* for amortization */

    std::vector<std::size_t> sizes_;
    std::vector<std::size_t> optimal_order_;
};

/*------------------------------------------------------------
                    REALIZATION
-------------------------------------------------------------*/


} /* namespace matrix */