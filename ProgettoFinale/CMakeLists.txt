cmake_minimum_required(VERSION 3.20)
project(circuit_solver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 CONFIG REQUIRED)
list(APPEND libraries_list PUBLIC Eigen3::Eigen)

# ----------------------------- Eseguibile principale -----------------------------
add_executable(circuit_solver main/main.cpp)
target_include_directories(circuit_solver PRIVATE include)
target_link_libraries(circuit_solver PRIVATE Eigen3::Eigen)
target_compile_options(circuit_solver PRIVATE -Wall -Wextra -O2)

# ----------------------------------- Test ----------------------------------------

enable_testing()

# Ogni file in tests/ e' una suite autonoma con il proprio main(): viene compilato
# in un eseguibile separato e registrato come un test indipendente, cosi' ctest
# riporta il superamento di ciascuna suite singolarmente.
# NOTA: i test della classe arco (edge) e del grafo non sono inclusi: si assumono funzionanti.
set(TEST_SUITES
	test_bool
	test_depina
	test_input
	test_circuit
)

foreach(suite ${TEST_SUITES})
	add_executable(${suite} tests/${suite}.cpp)
	target_include_directories(${suite} PRIVATE include)
	target_link_libraries(${suite} PRIVATE Eigen3::Eigen)
	target_compile_options(${suite} PRIVATE -Wall -Wextra -O2)
	add_test(NAME ${suite} COMMAND ${suite})
endforeach()
