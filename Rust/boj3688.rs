use std::io::{self, BufRead};
use std::ops::{Add, Sub, Rem, Mul};

static INF: f64 = 1e17;

#[derive(PartialEq, Eq, PartialOrd, Ord, Clone)]
struct Pos {
    x: i64,
    y: i64,
}

impl<'a, 'b> Add<&'b Pos> for &'a Pos {
    type Output = Pos;

    fn add(self, other: &'b Pos) -> Self::Output {
        Self::Output {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl<'a, 'b> Sub<&'b Pos> for &'a Pos {
    type Output = Pos;

    fn sub(self, other: &'b Pos) -> Self::Output {
        Self::Output {
            x: self.x - other.x,
            y: self.y - other.y,
        }
    }
}

impl<'a, 'b> Rem<&'b Pos> for &'a Pos {
    type Output = i64;

    fn rem(self, other: &'b Pos) -> Self::Output {
        self.x * other.y - self.y * other.x
    }
}

impl<'a, 'b> Mul<&'b Pos> for &'a Pos {
    type Output = i64;

    fn mul(self, other: &'b Pos) -> Self::Output {
        self.x * other.x + self.y * other.y
    }
}

impl Pos {
    fn euc(&self) -> i64 { self * self }
    fn mag(&self) -> f64 { (self.euc() as f64).sqrt() }
}

fn cross(p1: &Pos, p2: &Pos, p3: &Pos) -> i64 {
   (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x)
}

fn dot(p1: &Pos, p2: &Pos, p3: &Pos) -> i64 {
    &(p2 - p1) * &(p3 - p2)
 }

fn dist3(p1: &Pos, p2: &Pos, q1: &Pos) -> f64 {
    if dot(p1, p2, q1) * dot(p2, p1, q1) >= 0 {
        (cross(p1, p2, q1) as f64 / (p1 - p2).mag()).abs()
    }
    else {
        f64::min((p1 - q1).mag(), (p2 - q1).mag())
    }
}

fn dist4(p1: &Pos, p2: &Pos, q1: &Pos, q2: &Pos) -> f64 {
    f64::min(dist3(p1, p2, q1), f64::min(dist3(p1, p2, q2), f64::min(dist3(q1, q2, p1), dist3(q1, q2, p2))))
}

fn main() {
    let stdin = io::stdin();
    let mut iter = stdin.lock().lines();
    let mut tc:i32 = iter.next().unwrap().unwrap().trim().parse().unwrap();
    while tc > 0 {
        let n: i32 = iter.next().unwrap().unwrap().trim().parse().unwrap();
        let mut hout: Vec<Pos> = vec![];
        //println!("fuck");
        for _ in 0..n {
            let v: Vec<i64> = iter.next().unwrap().unwrap()
            .split_whitespace().map(|x| x.parse().unwrap()).collect();
            hout.push(Pos{x: v[0], y: v[1]});
            //println!("Pos{}, {}", v[0], v[1]);
        }
        let m: i32 = iter.next().unwrap().unwrap().trim().parse().unwrap();
        let mut hin: Vec<Pos> = vec![];
        for _ in 0..m {
            let v: Vec<i64> = iter.next().unwrap().unwrap()
            .split_whitespace().map(|x| x.parse().unwrap()).collect();
            hin.push(Pos{x: v[0], y: v[1]});
            //println!("Pos{}, {}", v[0], v[1]);
        }
        let mut ret:f64 = INF;
        for i in 0..n {
            for j in 0..n {
                ret = f64::min(ret,
                    dist4(&hout[i as usize], &hout[((i + 1) % n) as usize], &hin[j as usize], &hin[((j + 1) % m) as usize]));
            }
        }
        println!("{:.6}", ret * 0.5);
        tc -= 1;
    }
}