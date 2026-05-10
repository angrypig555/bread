use std::process::{Command, Stdio};
use std::panic;

mod logger;

/// Finds the current compiler
/// 
/// Returns either
/// - 1 for gcc
/// - 2 for clang
/// 
/// Will panic if no compilers are found.
pub fn find_compiler() -> u8 {
    println!("scanning for compilers...");
    let compiler = 1; // 1 = gcc, 2 = clang
    let mut gcc = Command::new("gcc")
        .args(["-v"])
        .stdout(Stdio::null())
        .stderr(Stdio::null())
        .status()
        .expect("gcc not found");
    let mut clang = Command::new("clang++")
        .args(["-v"])
        .stdout(Stdio::null())
        .stderr(Stdio::null())
        .status()
        .expect("clang not found");

    if gcc.success() && clang.success() {
        println!("found both clang and gcc, preferring clang");
        return 2;
    } else if gcc.success() {
        println!("using gcc");
        return 1;
    } else if clang.success() {
        println!("using clang");
        return 2;
    } else {
        panic!("error: no installed compiler was found");
    }

}

#[cfg(test)]

mod tests {
    use super::*;
    #[test]
    fn compiler_find_test() {
        let compiler = find_compiler();
        if compiler == 1 {
            println!("compiler is gcc");
        } else if compiler == 2 {
            println!("compiler is clang");
        } else {
            println!("compiler not found") // should never happen
        }
    }
}