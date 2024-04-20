use std::io;

fn read_line_as_number() -> i32 {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    input.trim().parse().unwrap()
}

fn main() {
    let (x, y) = (read_line_as_number(), read_line_as_number());
    let ans = x + y + 3;
    println!("{}", ans);
}