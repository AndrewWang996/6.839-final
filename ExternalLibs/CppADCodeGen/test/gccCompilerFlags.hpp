#ifndef CPPAD_CG_TEST_GCCCOMPILERFLAGS_INCLUDED
#define CPPAD_CG_TEST_GCCCOMPILERFLAGS_INCLUDED
/* --------------------------------------------------------------------------
 *  CppADCodeGen: C++ Algorithmic Differentiation with Source Code Generation:
 *    Copyright (C) 2015 Ciengis
 *
 *  CppADCodeGen is distributed under multiple licenses:
 *
 *   - Eclipse Public License Version 1.0 (EPL1), and
 *   - GNU General Public License Version 3 (GPL3).
 *
 *  EPL1 terms and conditions can be found in the file "epl-v10.txt", while
 *  terms and conditions for the GPL3 can be found in the file "gpl3.txt".
 * ----------------------------------------------------------------------------
 * Author: Joao Leal
 */

namespace CppAD {
namespace cg {

template<class Base>
void prepareTestCompilerFlags(GccCompiler<Base>& compiler) {
    std::vector<std::string> cFlags = compiler.getCompileFlags(); // copy
    cFlags.push_back("-O0");
    cFlags.push_back("-g");
    cFlags.push_back("-ggdb");
    cFlags.push_back("-D_FORTIFY_SOURCE=2");
#ifdef CPPADCG_TEST_SANITIZE
    cFlags.push_back("-fsanitize=address");
    cFlags.push_back("-fno-omit-frame-pointer");
#endif
    compiler.setCompileFlags(cFlags);

#ifdef CPPADCG_TEST_SANITIZE
    cFlags = compiler.getCompileLibFlags(); // copy
    cFlags.push_back("-fsanitize=address");
    compiler.setCompileLibFlags(cFlags);
#endif
}

}
}

#endif