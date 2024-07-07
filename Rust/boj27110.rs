use std::io;

fn read_line_as_number() -> i32 {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    input.trim().parse().unwrap()
}

fn main() {
    let n: i32 = read_line_as_number();

    let mut s = String::new();

    io::stdin().read_line(&mut s).unwrap();

    let values:Vec<i32> = s
        .as_mut_str()
        .split_whitespace()
        .map(|s| s.parse().unwrap())
        .collect();

    let a: i32 = values[0];
    let b: i32 = values[1];
    let c: i32 = values[2];

    let mut ans: i32 = 0;
    if a >= n { ans += n; } else { ans += a; }
    if b >= n { ans += n; } else { ans += b; }
    if c >= n { ans += n; } else { ans += c; }
    //println!("{}", ans);
}