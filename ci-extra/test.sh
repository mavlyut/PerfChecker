#!/bin/bash
set -euo pipefail
IFS=$' \t\n'

cmake-build-$1/perf/bench_json   --benchmark_format=json > json_fix.json
cmake-build-$1/perf/bench_jpeg   --benchmark_format=json > jpeg_fix.json
cmake-build-$1/perf/bench_logger --benchmark_format=json > logger_fix.json
cmake-build-$1/perf/bench_pi     --benchmark_format=json > pi_fix.json
cmake-build-$1/perf/bench_radix_sort   --benchmark_format=json > sort_fix.json

cmake-build-$1/ci-extra/perf/base_bench_json   --benchmark_format=json > json_base.json
cmake-build-$1/ci-extra/perf/base_bench_jpeg   --benchmark_format=json > jpeg_base.json
cmake-build-$1/ci-extra/perf/base_bench_logger --benchmark_format=json > logger_base.json
cmake-build-$1/ci-extra/perf/base_bench_pi     --benchmark_format=json > pi_base.json
cmake-build-$1/ci-extra/perf/base_bench_radix_sort   --benchmark_format=json > sort_base.json

echo "Testing pi..."
python3 ci-extra/check.py pi_fix.json pi_base.json 10
echo "Testing json..."
python3 ci-extra/check.py json_fix.json json_base.json 1.4
echo "Testing sort..."
python3 ci-extra/check.py sort_fix.json sort_base.json 1.2
echo "Testing logger..."
python3 ci-extra/check.py logger_fix.json logger_base.json 6
echo "Testing jpeg..."
python3 ci-extra/check.py jpeg_fix.json jpeg_base.json 4
