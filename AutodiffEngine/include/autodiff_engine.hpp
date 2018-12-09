#include <cstring>
#include <iostream>
#include <vector>
#include <cppad/cg.hpp>
#include "eigen_num_traits.hpp"
#include <unordered_map>
#include "hash_utils.hpp"
#include <unordered_set>

using CGD = CppAD::cg::CG<double>;
using ADCG = CppAD::AD<CGD>;
using library = CppAD::cg::DynamicLib<double>;
using library_ptr = std::unique_ptr<library>;
using model = CppAD::cg::GenericModel<double>;
using model_ptr = std::unique_ptr<model>;
using matrixADCG = Eigen::Matrix<ADCG, -1, -1>;
using vectorADCG = Eigen::Matrix<ADCG, Eigen::Dynamic, 1>;



template<typename T>
class Variable : public CppAD::AD<CppAD::cg::CG<T> >{

public:
    //using variable_ptr = std::shared_ptr<Variable<T> >;

protected:
    static std::vector<Variable<T> *> & all_vars(){
        static std::vector< Variable<T> *>  all_vars_; return all_vars_; //TOOD: abstract paradigm this away?
    }
    //TODO: Should be const, make const somehow
    uint index;
public:

    Variable() :
            index(all_vars().size())
    {
        all_vars().push_back(this);
    }



    //TODO: Testing
    int get_index(){
        return this->index;
    }



};

class AutodiffEngine{

protected:
    using SparType = std::vector<std::set<size_t> >;

public:
    static void Eval(Eigen::VectorXd& ret_value,
                     void (*f)(vectorADCG&, const vectorADCG&),
                     Eigen::VectorXd& decision_variables,
                     const int n,
                     const std::string& name,
                     bool optimize=false){

        CppAD::ADFun<CGD> fun;
        vectorADCG independent_variables(decision_variables.size());
        GenerateFunction(fun, f, independent_variables, optimize);
        const int range = fun.Range();
        std::string model_name = name + "_" + std::to_string(n);


        if (gen_models().find(model_name) == gen_models().end()) { //short circuit the whole thing for efficiency.

            for (uint i = 1; i < n + 1; ++i){
                model_name = name + "_" + std::to_string(i);
                if (gen_models().find(model_name) == gen_models().end()) {
                    ComputeJacobianFunction(fun, fun, independent_variables, model_name, optimize);
                    //TODO: this does a little extra work in that it does one extra AD that's not necessary - but doesn't save it
                }
            }
        }


        Eigen::VectorXd output_value;
        //ret_value = gen_models()[model_name]->ForwardZero(decision_variables);
        gen_models()[model_name]->Jacobian(decision_variables, ret_value);


        // ret_value = Eigen::VectorXd(fun.Range());

        /*
        ret_value = Eigen::VectorXd::Zero(pow(decision_variables.size(), n) * range); //not sure I'm using pow when this is only supported for n = 1

        for (std::tuple<int, int, int> transcribe : sparsity_pattern()[model_name]) {
            ret_value(std::get<2>(transcribe)  + std::get<1>(transcribe) * decision_variables.size()) = output_value(std::get<0>(transcribe));
        }
         */

    }


public:
    static void GenerateFunction(
            CppAD::ADFun<CGD>& fun,
            void (*f)(vectorADCG&, const vectorADCG&),
            vectorADCG& decision_variables,
            bool optimize)
    {
        CppAD::Independent(decision_variables);
        vectorADCG y;
        f(y, decision_variables);
        fun = CppAD::ADFun<CGD> (decision_variables, y);
        if (optimize) {
            fun.optimize("no_conditional_skip");
        }
    }


    static void ComputeJacobianFunction(
            CppAD::ADFun<CGD>& jacFun,
            CppAD::ADFun<CGD>& fun,
            vectorADCG& decision_variables,
            const std::string& name,
            bool optimize){

        CppAD::cg::CodeHandler<double> handler;


// independent variable vector
        const uint n = decision_variables.size();
        std::vector<CGD> x0(n);
        handler.makeVariables(x0);


/*
        SparType sparsity = CppAD::cg::jacobianSparsitySet<SparType, CGD>(fun); // this could be improved


        std::vector<size_t> rows, cols;
        for (size_t i = 0; i < sparsity.size(); i++) {
            for (size_t j : sparsity[i]) {
                if (j >= n) break;
                rows.push_back(i);
                cols.push_back(j);
            }
        }


*/
        SaveJacobianAsModel(fun, name);



// compute the expressions for the Jacobian using the original model
        std::vector<CGD> jac = fun.SparseJacobian(x0);




// the independent variables for the new model
        std::vector<ADCG> xNew(n);
        CppAD::Independent(xNew);

// change the type from CGD (jac) to ADCG (jacNew)
        CppAD::cg::Evaluator<double, CGD, ADCG> evaluator(handler);
        std::vector<ADCG> funNew = evaluator.evaluate(xNew, jac);

        jacFun = CppAD::ADFun<CGD> (xNew, funNew); // the new model tape
        if (optimize) {
            jacFun.optimize("no_conditional_skip");
        }

    }


//TODO: optimize this by using sparsity properly, changing ForwardZero to sparseJacobian calls.
    static void SaveFunctionAsModel(
            CppAD::ADFun<CGD>& fun,
            const std::string& name) {



        CppAD::cg::ModelCSourceGen<double> cgen(fun, name);

        /*
        using SparType = std::vector<std::set<size_t> >;
        SparType sparsity = CppAD::cg::jacobianSparsitySet<SparType, CGD>(fun);
        */

        cgen.setCreateJacobian(true);
        cgen.setCreateHessian(true);
        cgen.setCreateForwardZero(true);
        CppAD::cg::ModelLibraryCSourceGen<double> libcgen(cgen);

        // compile source code
        CppAD::cg::DynamicModelLibraryProcessor<double> p(libcgen);
        p.setLibraryName(name);


        CppAD::cg::GccCompiler<double> compiler;
        compiler.setSaveToDiskFirst(true); // << not really required
        CppAD::cg::SaveFilesModelLibraryProcessor<double> p2(libcgen);
        p2.saveSources();


        library_ptr dynamicLib = p.createDynamicLibrary(compiler);




        gen_models()[name] = dynamicLib->model(name);
        //std::cout << dynamicLib->model(name) << std::endl;
        gen_libraries()[name] = std::move(dynamicLib); //Have to store this, otherwise the model can't be evaluated without an error


    }

    static void SaveJacobianAsModel(
            CppAD::ADFun<CGD>& fun,
            const std::string& name) {



        CppAD::cg::ModelCSourceGen<double> cgen(fun, name);

        /*
        using SparType = std::vector<std::set<size_t> >;
        SparType sparsity = CppAD::cg::jacobianSparsitySet<SparType, CGD>(fun);
        */

        cgen.setCreateJacobian(true);

        //cgen.setCreateSparseJacobian(true);
//        cgen.setCustomSparseJacobianElements(rows, cols);

        cgen.setJacobianADMode(CppAD::cg::JacobianADMode::Automatic);

        CppAD::cg::ModelLibraryCSourceGen<double> libcgen(cgen);

        // compile source code
        CppAD::cg::DynamicModelLibraryProcessor<double> p(libcgen);
        p.setLibraryName(name);


        CppAD::cg::GccCompiler<double> compiler;
        compiler.setSaveToDiskFirst(true); // << not really required
        CppAD::cg::SaveFilesModelLibraryProcessor<double> p2(libcgen);
        p2.saveSources();


        library_ptr dynamicLib = p.createDynamicLibrary(compiler);





        gen_models()[name] = dynamicLib->model(name);
        //std::cout << dynamicLib->model(name) << std::endl;
        gen_libraries()[name] = std::move(dynamicLib); //Have to store this, otherwise the model can't be evaluated without an error

                /*
        for (uint i = 0; i < rows.size(); ++i){
            sparsity_pattern()[name].insert(std::tuple<uint, uint, uint>(i, rows[i], cols[i]));
        }
                 */


    }

    static std::unordered_map<std::string, library_ptr>& gen_libraries(){
        static std::unordered_map<std::string, library_ptr> gen_libraries_;
        return gen_libraries_;
    }

    static std::unordered_map<std::string, model_ptr>& gen_models() {
        static std::unordered_map<std::string, model_ptr> gen_models_;
        return gen_models_;
    }


    static std::unordered_map<std::string, std::unordered_set<std::tuple<uint, uint, uint>>> & sparsity_pattern() {
        static std::unordered_map<std::string, std::unordered_set<std::tuple<uint, uint, uint>>> sparsity_pattern_;
        return sparsity_pattern_;
    }


    //TODO: perhaps add dims for size checking?

};




