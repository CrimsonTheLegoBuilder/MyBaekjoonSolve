use std::io;

fn main() {
    let mut input = String::new();//empty string
    
    io::stdin().read_line(&mut input).expect("Fxxk");
    let val = input.trim().split_whitespace()
        .map(|x| x.parse::<i32>().unwrap())
        .collect::<Vec<_>>();
    
    println!("{}", val[0] - val[1]);
}