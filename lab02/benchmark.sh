#! /usr/bin/env bash

set -o errexit
set -o nounset

readonly TESTS_DIR=tests/e2e
readonly BUILD_DIR=build

function log_info() {
    log_ "info" "$@"
}

function log_error() {
    log_ "error" "$@"
}

function log_() {
    local type=$1
    local message=$2
    local date_str=; date_str=$(date +'%Y-%m-%d %H:%M:%S')
    echo "[${type}] [${date_str}] ${message}"
}

function main() {
    local bin=${BUILD_DIR}/lab02_exe
    log_info "Stage #1. Compiling..."
    if ! cmake -S . -B ${BUILD_DIR} ; then
    log_error "Failed to build"
    return 1
    fi
    if ! cmake --build ${BUILD_DIR} ; then
    log_error "Failed to compile"
    return 1
    fi
    log_info "Compiling OK"

    log_info "Stage #2. Test generating..."
    rm -rf ${TESTS_DIR}
    mkdir ${TESTS_DIR}
    local count_of_tests=3
    if ! ./test_generator.py ${TESTS_DIR} ${count_of_tests} ; then
    log_error "Failed to generate tests"
    return 1
    fi
    log_info "Generating OK"

    log_info "Benchmarking..."
    local benchmark_bin=${BUILD_DIR}/lab02_benchmark
    for test_file in $( ls ${TESTS_DIR}/*.t ) ; do
        log_info "Running ${test_file}"
        if ! ${benchmark_bin} < ${test_file} ; then
            log_error "Failed to run ${benchmark_bin} for ${test_file}."
        return 1
        fi
    done
}

# $1 -- первый аргумент
# %2 -- второй аргумент
# $@ -- все аргументы
main $@
