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
v0.0.3-2-g22dd7fd

> ip_filter --info
author: JN <70240447+nj-eka@users.noreply.github.com>
url: https://github.com/nj-eka/ipfilter.git
branch: heads/v0.0.3
rev: 22dd7fd
rev date: 2023.07.25-10:30
rev hist: Sun Jul 23 01:04:56 2023 +0300         JN      refs/heads/main
Sat Jul 22 19:11:16 2023 +0300   JN      refs/heads/v0.0.2
Tue Jul 25 10:30:58 2023 +0300   JN      refs/heads/v0.0.3
Tue Jul 25 10:30:58 2023 +0300   JN      refs/remotes/origin/HEAD
Tue Jul 25 10:30:58 2023 +0300   JN      refs/remotes/origin/main
Sun Jul 16 22:10:16 2023 +0300   Jack New        refs/remotes/origin/v0.0.1
Sat Jul 22 09:59:42 2023 +0300   Jack New        refs/remotes/origin/v0.0.2
Tue Jul 25 10:26:32 2023 +0300   Jack New        refs/remotes/origin/v0.0.3
                 refs/tags/v0.0.1
                 refs/tags/v0.0.3

> cat tests/ip_filter.tsv | ip_filter | md5sum
24e7a7b2270daee89c64d3ca5fb3da1a -

> ip_filter --input=tests/ip_filter.tsv  | md5sum
24e7a7b2270daee89c64d3ca5fb3da1a -

> ip_filter --input=tests/ip_filter.tsv --counts=`wc -l tests/ip_filter.tsv`  | md5sum
24e7a7b2270daee89c64d3ca5fb3da1a -

```
