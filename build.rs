const SOURCES: &[&str] = &[
    "phc-winner-argon2/src/argon2.c",
    "phc-winner-argon2/src/core.c",
    "phc-winner-argon2/src/blake2/blake2b.c",
    "phc-winner-argon2/src/encoding.c",
    "phc-winner-argon2/src/thread.c",
    "phc-winner-argon2/src/opt.c"
];

const INCLUDE: &str = "phc-winner-argon2/include";

fn main() {
    let mut builder = cc::Build::new();
    builder
        .files(SOURCES)
        .include(INCLUDE)
        .debug(false)
        .flag_if_supported("-pthread")
        .flag_if_supported("-O3")
        .flag_if_supported("-flto")
        .flag_if_supported("-mavx2")
        .flag_if_supported("-march=native")
        .warnings(false)
        .extra_warnings(false);

    builder.compile("libargon.a");
}
