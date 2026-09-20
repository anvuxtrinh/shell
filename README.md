# develop shell from scratch by c

## prerequisites
- **CMake** (v3.10+)
- **C Compiler** (GCC / Clang) & **Make**

---

## how to build and run

```bash
# cfg and build
cmake -B build -DENABLE_TESTING=ON
make -C build

# run
./build/shell

# run test
cd ./build
ctest --test-dir build --output-on-failure