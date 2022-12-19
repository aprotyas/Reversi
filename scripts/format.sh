git_root=$(git rev-parse --show-toplevel)
pushd ${git_root}
find . -iname *.cpp -o -iname *.hpp -o -path ./build -prune | xargs clang-format -i --style=Google --verbose
popd
