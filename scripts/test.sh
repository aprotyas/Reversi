git_root=$(git rev-parse --show-toplevel)
pushd ${git_root}
ctest --test-dir build/test
popd
