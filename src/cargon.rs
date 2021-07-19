use libc::{c_int, c_uint, size_t, c_void, c_char};

extern "C" {
   pub fn argon2id_hash_encoded(t_cost:c_uint, m_cost:c_uint, parallelism:c_uint, pwd: *const c_void, pwdlen:size_t, salt: *const c_void, saltlen:size_t, hashlen:size_t, encoded: *mut c_char, encodedlen: size_t) -> c_int;
}

pub struct ArgonOutput {
   pub code: i32,
   pub encoded: Vec<u8>,
}

pub async fn run_hash(t: u32, m: u32, p: u32, password: &str, salt: &str) -> ArgonOutput {
   let mut encoded_buffer = vec![0_u8; 108];
   let (encoded, encodedlen) = (encoded_buffer.as_mut_ptr().cast::<i8>(), encoded_buffer.len());
   let (pwd, pwdlen) = (password.as_bytes().as_ptr().cast::<libc::c_void>(), password.len());
   let (salt, saltlen) = (salt.as_bytes().as_ptr().cast::<libc::c_void>(), salt.len());

   let code = unsafe {
      argon2id_hash_encoded(
         t,
         m,
         p,
         pwd,
         pwdlen,
         salt,
         saltlen,
         32_usize,
         encoded,
         encodedlen
      )
   };
   ArgonOutput {
      code,
      encoded: encoded_buffer,
   }
}

pub async fn run_decode(encoded: Vec<u8>) -> String {
   return String::from_utf8(encoded).unwrap().trim_matches(char::from(0)).to_owned();
}
