#include <gtest/gtest.h>
#include "autodiff_engine.hpp"
#include <chrono>
#include "mesh_importer.hpp"

namespace test_autodiff_engine {


void test_function(vectorADCG& output, const Eigen::Matrix<ADCG, Eigen::Dynamic, 1>& input){

    output = (input.transpose() * input);
};


TEST(Test, VarInitialization) {

    /*

        CppAD::ADFun<CGD> fun;

        Eigen::Matrix<ADCG, Eigen::Dynamic, 1> input_vars(2);
        input_vars.setOnes();

        AutodiffEngine::GenerateFunction(fun, test_function, input_vars);
        AutodiffEngine::ComputeJacobianFunction(fun, fun, input_vars);
        AutodiffEngine::SaveFunctionAsModel(fun, "test_function");


        std::vector<double> xv {2.5, 3.5};

        std::vector<double> second_order = AutodiffEngine::gen_models()["test_function"]->Jacobian(xv);










        ASSERT_NEAR(second_order[0], 2, 1e-12);
        ASSERT_NEAR(second_order[1], 0, 1e-12);
        ASSERT_NEAR(second_order[2], 0, 1e-12);
        ASSERT_NEAR(second_order[3], 2, 1e-12);
         */


    Eigen::VectorXd ret_val, input;
    input = Eigen::VectorXd::Ones(400);







    for (int i = 0; i < 100; ++i){
        auto start = std::chrono::steady_clock::now();

        AutodiffEngine::Eval(ret_val,
                             test_function,
                             input,
                             1,
                             "test1",
                             false);

        auto end = std::chrono::steady_clock::now();
        std::cout << "time per eval1 is " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;
        std::cout << ret_val << std::endl;
    }


    for (int i = 0; i < 100; ++i){
        auto start = std::chrono::steady_clock::now();

        AutodiffEngine::Eval(ret_val,
                             test_function,
                             input,
                             2,
                             "test2",
                             false);



        auto end = std::chrono::steady_clock::now();
        std::cout << "time per eval2 is " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;
        std::cout << ret_val << std::endl;

    }


    /*
        for (int i = 0; i < 100; ++i){
            auto start = std::chrono::steady_clock::now();

            AutodiffEngine::Eval(ret_val,
                                 test_function,
                                 input,
                                 3,
                                 "test3");

            auto end = std::chrono::steady_clock::now();
            std::cout << "time per eval3 is " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;
        }

        std::cout << "Done" << std::endl;
        */





}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(filter) = "Test.*";
    srand((unsigned int)time(0));
    return RUN_ALL_TESTS();
}

}
