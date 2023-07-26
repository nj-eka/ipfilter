# ipfilter
course c++ developer hw2

## task
- [pdf](02_homework.pdf)

## Install
```bash
> git clone https://github.com/nj-eka/ipfilter.git
> cd ipfilter

# config example
> cmake -B ./build -S . \
    -DCMAKE_BUILD_TYPE=Debug \
    -DBUILD_TESTING=ON \
    -DENABLE_COVERAGE=ON \
    -DCMAKE_CXX_FLAGS="-g -O0 -fsanitize=undefined -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer"

# build
> cmake --build ./build --target install

# run tests
> ctest --test-dir build -T Test

# see tests coverage
> ctest --test-dir build -T Coverage
    Performing coverage
    Processing coverage (each . represents one file):
        ....
    Accumulating results (each . represents one file):
        .......
            Covered LOC:         308
            Not covered LOC:     1
            Total LOC:           309
            Percentage Coverage: 99.68%

```

## Usage
```sh
> ip_filter --help
Usage:
  -h [ --help ]               Print this help message
  -v [ --version ]            Print version
  --info                      Print project info
  -i [ --input ] arg          Input file (tsv) path [default: stdin]
  -o [ --output ] arg         Output file path [default: stdout]
  -c [ --counts ] arg (=1000) Initial size to reserve

> ip_filter --version
v0.0.1-4-g2b6be48

> ip_filter --info
author: JN <70240447+nj-eka@users.noreply.github.com>
url: https://github.com/nj-eka/ipfilter.git
branch: main
rev: 2b6be48
rev date: 2023.07.26-17:04
rev hist: Wed Jul 26 17:04:40 2023 +0300         JN      refs/heads/main
Wed Jul 26 17:04:40 2023 +0300   JN      refs/remotes/origin/main
                 refs/tags/v0.0.1

> cat tests/ip_filter.tsv | ip_filter | md5sum
24e7a7b2270daee89c64d3ca5fb3da1a -

> ip_filter --input=tests/ip_filter.tsv  | md5sum
24e7a7b2270daee89c64d3ca5fb3da1a -

> ip_filter --input=tests/ip_filter.tsv --counts=`wc -l tests/ip_filter.tsv`  | md5sum
24e7a7b2270daee89c64d3ca5fb3da1a -
```
