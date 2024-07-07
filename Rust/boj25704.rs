use std::cmp::min;

fn read_line_as_number() -> i32 {
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    input.trim().parse().unwrap()
}

fn main() {
    let n: i32 = read_line_as_number();
    let p: i32 = read_line_as_number();

    let mut v: Vec<i32> = vec![];
    v.push(p);
    if n >= 5 { v.push(p - 500); }
    if n >= 10 { v.push(p * 9 / 10); }
    if n >= 15 { v.push(p - 2000); }
    if n >= 20 { v.push(p * 3 / 4 ); }
    let mut ans: i32 = 1000000000;
    for i in 0..v.len() { ans = min(ans, v[i]); }
    if ans < 0 { ans = 0; }
    //println!("{}", ans);
}