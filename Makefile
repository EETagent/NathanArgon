all: build

build:
	RUSTFLAGS="-C target-feature=+avx2 -C target-cpu=native" cargo build --release
	cp ./target/release/nathan_argon2 ./nathan_argon2

upx: build
	cp ./nathan_argon2 ./nathan_argon2_upx
	upx ./nathan_argon2_upx

run:
	sudo ./nathan_argon2

runb: build run

clean:
	rm -rf ./Cargo.lock
	rm -rf ./target
	rm -rf ./nathan_argon2 ./nathan_argon2_upx
