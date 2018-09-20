cmake_minimum_required(VERSION 3.9)
project(derivativesCalc)

set(CMAKE_CXX_STANDARD 11)

add_executable(derivativesCalc main.cpp Monomial.h Monomial.cpp Polynomial.h Polynomial.cpp Parser.h Parser.cpp Term.h Term.cpp)