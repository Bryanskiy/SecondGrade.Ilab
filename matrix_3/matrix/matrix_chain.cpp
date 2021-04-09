#include "matrix_chain.hpp"

namespace matrix {

Imatrix_chain_t::Imatrix_chain_t() {
    dp_table_.resize(dp_table_capacity_, dp_table_capacity_);
}

void Imatrix_chain_t::push(std::size_t rows, std::size_t cols) {
    if(!cols || !rows) {
        throw std::runtime_error("matrix_chain_t::push: invalid matrix sizes");
    }

    if(dp_table_size_ == dp_table_capacity_) {
        dp_table_capacity_ *= 2;
        dp_table_.resize(dp_table_capacity_, dp_table_capacity_);
    }

    if(!dp_table_size_) {
        sizes_.push_back(rows);
        sizes_.push_back(cols);
        ++dp_table_size_;
        return;
    } 
    
    if(sizes_.back() != rows) {
        throw std::runtime_error("matrix_chain_t::push: invalid matrix sizes, can't multiplicate");
    }

    sizes_.push_back(cols);

    std::size_t j = dp_table_size_;
    for(int i = dp_table_size_ - 1; i >= 0; --i) {
        std::vector<std::size_t> variants;
        std::size_t variants_size = j - i;
        variants.reserve(variants_size);

        for(std::size_t k = i; k < j; ++k) {
            std::size_t candidate = dp_table_[i][k] + dp_table_[k+1][j] + sizes_[i] * sizes_[k+1] * sizes_[j+1];
            variants.push_back(candidate);
        }

       auto&& it = std::min_element(variants.begin(), variants.end());
       dp_table_[i][j] = *it;
       dp_table_[j][i] = i + std::distance(variants.begin(), it);
    }

    optimal_order_.clear();
    calculate_optimal_order(0, dp_table_size_);

    ++dp_table_size_;
}

void Imatrix_chain_t::calculate_optimal_order(std::size_t i, std::size_t j) {
    if(i >= j) {
        return;
    }

    if((i + 1) == j) {
        optimal_order_.push_back(i);
        return;
    }

    std::size_t pivot = dp_table_[j][i];
    calculate_optimal_order(pivot + 1, j);    
    calculate_optimal_order(i, pivot);

    optimal_order_.push_back(pivot);
}

void Imatrix_chain_t::dump(std::ostream& stream) const {
    stream << dp_table_;
}

} /* namespace matrix */