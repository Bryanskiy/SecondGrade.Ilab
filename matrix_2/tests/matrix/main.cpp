#include "generator.hpp"
#include "unit_tests/determinant/determinant.hpp"
#include "unit_tests/gauss.hpp"
#include "unit_tests/resize.hpp"

/* ------------------------------------------------------------------------------------------------------------------------------- 
                                            GAUSS ALGS
---------------------------------------------------------------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------
This test check only gauss algorithm invariant:
    1) after straight pass matrix transports to step matrix
    2) after reverse pass all elements in the same col before step equal to 0   
-----------------------------------------------------------------------------*/
bool check_gauss_passes(matrix::matrix_t<double>& m) {
    matrix::matrix_t<double> copy(m);

    matrix::gauss_straight(m);
    matrix::gauss_reverse(m);

    std::size_t current_m = 0;
    std::size_t current_n = 0;
    std::size_t max_m = m.get_rows_number();
    std::size_t max_n = m.get_cols_number();

    while((current_m < max_m) && (current_n < max_n)) {
        if(matrix::equal(m[current_m][current_n], 0.0)) {
            ++current_n;
            continue;
        }

        for(std::size_t i = 0; i < max_m; ++i) {
            if(i == current_m) {
                continue;
            }

            if(!matrix::equal(m[i][current_n], 0.0)) {
                return false;
            }
        }

        ++current_n;
        ++current_m;
    }

    return true;
}

void check_gauss_passes_runner(std::size_t count) {
    generator_t generator;
    for(std::size_t i = 1; i <= count; ++i) {
         matrix::matrix_t<double> m = generator.generate_matrix(i, count / 2, -20.0, 20.0);
         bool flag = check_gauss_passes(m);
         if(!flag) {
             std::cerr << "Gauss test failed" << std::endl;
             return;
         }
     }

     std::cout << "Straight and reverse algs: SUCESS for " << count  << " matrices " << std::endl;
}

/* ------------------------------------------------------------------------------------------------------------------------------- 
                                            DETERMINANT
---------------------------------------------------------------------------------------------------------------------------------*/

/* this func checks, that after multiplication on orthogonal matrix det can only swap sign */
void check_determinant_property1(std::size_t matrix_size) {
    generator_t generator;
    for(std::size_t i = 0; i < 100; ++i) {
        matrix::matrix_t<double> m = generator.generate_matrix(matrix_size, matrix_size, -5.0, 5.0);
        matrix::matrix_t<double> orthogonal = generator.generate_orthogonal_matrix(matrix_size);

        if(std::abs(std::abs(orthogonal.det()) - 1) > matrix::tolerance) {
            std::cerr << "Incorrect orthogonal matrix" << std::endl;
            return;
        }

        long double first_det = std::abs(m.det());

        matrix::matrix_t<double> mult = matrix::multiplication(m, orthogonal);
        generator.random_shuffle(mult, 100u);
        long double second_det = std::abs(mult.det());

        if(std::abs(first_det - second_det) > matrix::tolerance) {
            std::cerr << "check_determinant_property1 for size: " << matrix_size << " failed" << std::endl;
            return;
        }

    }

    std::cout << "check_determinant_property1 for size: " << matrix_size << " success" << std::endl;
}

int main(int argc, char **argv) {
    std::cout << "---------------------------------------Determinant tests-----------------------" << std::endl;
    check_determinant_property1(7); /* odd */
    check_determinant_property1(8); /* even */
    std::cout << "---------------------------------------Gauss tests-----------------------------" << std::endl;
    check_gauss_passes_runner(100);
    std::cout << "---------------------------------------Unit tests------------------------------" << std::endl;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}