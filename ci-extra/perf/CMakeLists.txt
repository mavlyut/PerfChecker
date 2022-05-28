
add_executable(base_bench_jpeg bench_jpeg.cpp)
target_link_libraries(base_bench_jpeg benchmark::benchmark)

add_executable(base_bench_pi bench_pi.cpp)
target_link_libraries(base_bench_pi benchmark::benchmark)

add_executable(base_bench_radix_sort bench_radix_sort.cpp)
target_link_libraries(base_bench_radix_sort benchmark::benchmark)

add_executable(base_bench_json bench_json.cpp)
target_link_libraries(base_bench_json benchmark::benchmark)

add_executable(base_bench_logger bench_logger.cpp)
target_link_libraries(base_bench_logger benchmark::benchmark)
