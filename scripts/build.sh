git_root=$(git rev-parse --show-toplevel)
pushd ${git_root}
# Hard coding the TUI build for now
cmake -G Ninja -S . -B build -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DBUILD_APP_VERSION=TUI
cmake --build build
popd
