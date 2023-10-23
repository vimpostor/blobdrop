# Testing Suite

This directory contains a testing suite based on QTest.

It is easy to run the tests locally:
```bash
cd .. # from the root of the repo
cmake -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```
