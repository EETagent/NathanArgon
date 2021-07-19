<h1 class="rich-diff-level-zero">NathanArgon - C language implementation</h1>

Simple quick and dirty API written in C language

Processes JSON plain text and returns hashed response

[![C](https://img.shields.io/badge/lang-c-yellow.svg)]()

Example request in apitest.sh file

## Performance

Fast response times, mostly affected by argon2 hashing
![Hyperfine measurement](img/Hyperfine.png?raw=true)

## Building

Following dependencies are required to build the application:

* gcc / clang
* cmake
* (ninja)

To download, run:

```bash
git clone https://github.com/EETagent/NathanArgon.git -b c-language --recursive
```

To compile the application, run:  

```bash
mkdir build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release  ..
ninja
```
