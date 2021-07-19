<h1 class="rich-diff-level-zero">NathanArgon - Rust language implementation</h1>

Simple API written in Rust language

Processes JSON plain text and returns hashed response

[![C](https://img.shields.io/badge/lang-rust-orange.svg)]()

Example request in apitest.sh file

## Performance

Fast response times, mostly affected by argon2 hashing

C bindings to phc-winner-argon2 library are used to achieve best performance
![Hyperfine measurement](img/Hyperfine.png?raw=true)

## Building

Following dependencies are required to build the application:

* Rust
* gcc / clang

To download, run:

```bash
git clone https://github.com/EETagent/NathanArgon.git -b rust-language --recursive
```

To compile the application, run:  

```bash
cargo build --release
```
