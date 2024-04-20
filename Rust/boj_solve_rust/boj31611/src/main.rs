use std::io;

fn read_line_as_number() -> i32 {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    input.trim().parse().unwrap()
}

fn main() {
    let x = read_line_as_number();
    let rem = x % 7;
    let ans = if rem == 2 {1} else {0};
    println!("{}", ans);
}