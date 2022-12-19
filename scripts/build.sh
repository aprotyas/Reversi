git_root=$(git rev-parse --show-toplevel)
pushd ${git_root}
cmake -G Ninja -S . -B build -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
popd
