// not my code. is copied. refer to jeoffrey0522.

mod io {
    use std::fmt::Debug;
    use std::str::*;

    pub trait InputStream {
        fn token(&mut self) -> &[u8];
        fn line(&mut self) -> &[u8];

        fn skip_line(&mut self) {
            self.line();
        }

        #[inline]
        fn value<T>(&mut self) -> T
        where
            T: FromStr,
            T::Err: Debug,
        {
            let token = self.token();
            let token = unsafe { from_utf8_unchecked(token) };
            token.parse().unwrap()
        }
    }

    #[inline]
    fn is_whitespace(c: u8) -> bool {
        c <= b' '
    }

    fn trim_newline(s: &[u8]) -> &[u8] {
        let mut s = s;
        while matches!(s.last(), Some(b'\n' | b'\r' | 0)) {
            s = &s[..s.len() - 1];
        }
        s
    }

    impl InputStream for &[u8] {
        fn token(&mut self) -> &[u8] {
            let idx = self.iter().position(|&c| !is_whitespace(c)).unwrap();
            //.expect("no available tokens left");
            *self = &self[idx..];
            let idx = self
                .iter()
                .position(|&c| is_whitespace(c))
                .unwrap_or_else(|| self.len());
            let (token, buf_new) = self.split_at(idx);
            *self = buf_new;
            token
        }

        fn line(&mut self) -> &[u8] {
            let idx = self
                .iter()
                .position(|&c| c == b'\n')
                .map_or_else(|| self.len(), |idx| idx + 1);
            let (line, buf_new) = self.split_at(idx);
            *self = buf_new;
            trim_newline(line)
        }
    }
}

use std::io::{BufReader, Read, Write};

fn stdin() -> Vec<u8> {
    let stdin = std::io::stdin();
    let mut reader = BufReader::new(stdin.lock());

    let mut input_buf: Vec<u8> = vec![];
    reader.read_to_end(&mut input_buf).unwrap();
    input_buf
}

#[allow(dead_code)]
mod cheap_rand {
    use std::ops::Range;
    // SplitMix64 random number generator
    pub struct Random {
        state: u64,
    }

    impl Random {
        pub fn new(seed: u64) -> Self {
            assert_ne!(seed, 0);
            Self { state: seed }
        }

        pub fn next_u64(&mut self) -> u64 {
            self.state = self.state.wrapping_add(0x9e3779b97f4a7c15);
            let mut x = self.state;
            x = (x ^ (x >> 30)).wrapping_mul(0xbf58476d1ce4e5b9);
            x = (x ^ (x >> 27)).wrapping_mul(0x94d049bb133111eb);
            x ^ (x >> 31)
        }

        pub fn range_u64(&mut self, range: Range<u64>) -> u64 {
            let Range { start, end } = range;
            assert!(start < end);

            let width = end - start;
            let test = (u64::MAX - width) % width;
            loop {
                let value = self.next_u64();
                if value >= test {
                    return start + value % width;
                }
            }
        }

        pub fn shuffle<T>(&mut self, xs: &mut [T]) {
            let n = xs.len();
            for i in 0..n - 1 {
                let j = self.range_u64(i as u64..n as u64) as usize;
                xs.swap(i, j);
            }
        }
    }
}

use std::ops::{Add, Mul, Sub};

fn signed_area<T: Copy + Sub<Output = T> + Mul<Output = T>>(p: [T; 2], q: [T; 2], r: [T; 2]) -> T {
    (q[0] - p[0]) * (r[1] - p[1]) - (q[1] - p[1]) * (r[0] - p[0])
}

// monotone chain algorithm
fn convex_hull(points: &mut [[f64; 2]]) -> Vec<[f64; 2]> {
    let n = points.len();
    if n <= 1 {
        return points.to_vec();
    }
    assert!(n >= 2);

    points.sort_unstable_by(|&p, &q| p.partial_cmp(&q).unwrap());

    let ccw = |p, q, r| signed_area(p, q, r) > 0.0;

    let mut lower = Vec::new();
    let mut upper = Vec::new();
    for &p in points.iter() {
        while matches!(lower.as_slice(), [.., l1, l2] if !ccw(*l1, *l2, p)) {
            lower.pop();
        }
        lower.push(p);
    }
    for &p in points.iter().rev() {
        while matches!(upper.as_slice(), [.., l1, l2] if !ccw(*l1, *l2, p)) {
            upper.pop();
        }
        upper.push(p);
    }
    lower.pop();
    upper.pop();

    lower.extend(upper);
    lower
}

fn convex_hull_area<I>(points: I) -> f64
where
    I: IntoIterator<Item = [f64; 2]>,
    I::IntoIter: Clone,
{
    let mut area: f64 = 0.0;
    let points = points.into_iter();
    let points_shifted = points.clone().skip(1).chain(points.clone().next());
    for ([x1, y1], [x2, y2]) in points.zip(points_shifted) {
        area += x1 * y2 - x2 * y1;
    }
    area = (area / 2.0).abs();
    area
}

fn sub_point3d<T: Copy + Sub<Output = T>>(a: [T; 3], b: [T; 3]) -> [T; 3] {
    [a[0] - b[0], a[1] - b[1], a[2] - b[2]]
}

fn dot_3d<T: Copy + Add<Output = T> + Mul<Output = T>>(a: [T; 3], b: [T; 3]) -> T {
    a[0] * b[0] + a[1] * b[1] + a[2] * b[2]
}

fn cross_3d<T: Copy + Sub<Output = T> + Mul<Output = T>>(a: [T; 3], b: [T; 3]) -> [T; 3] {
    [
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    ]
}

fn signed_volume<T: Copy + Add<Output = T> + Sub<Output = T> + Mul<Output = T>>(
    a: [T; 3],
    b: [T; 3],
    c: [T; 3],
    d: [T; 3],
) -> T {
    let [b, c, d] = [sub_point3d(b, a), sub_point3d(c, a), sub_point3d(d, a)];
    let result = dot_3d(b, cross_3d(c, d));
    result
}

// points should be sorted randomly
// incremental convex hull
fn convex_hull_3d(points: &[[i64; 3]]) -> Vec<[usize; 3]> {
    macro_rules! unwrap_or_return {
        ($e:expr, $r: expr) => {
            match $e {
                Some(x) => x,
                None => return $r,
            }
        };
    }
    use std::collections::HashSet;
    use std::iter::once;

    if points.len() <= 2 {
        return vec![];
    }

    let [i, j] = [0, 1];
    let mut k = 2 + unwrap_or_return!(
        points[2..].iter().position(|&pk| {
            cross_3d(
                sub_point3d(points[j], points[i]),
                sub_point3d(pk, points[i]),
            ) != [0, 0, 0]
        }),
        vec![]
    );

    let mut l = k + unwrap_or_return!(
        (k + 1 < points.len())
            .then(|| points[k..]
                .iter()
                .position(|&pl| signed_volume(points[i], points[j], points[k], pl) != 0))
            .flatten(),
        (2..points.len()).map(|k| [0, 1, k]).collect()
    );

    if signed_volume(points[i], points[j], points[k], points[l]) > 0 {
        std::mem::swap(&mut k, &mut l)
    }
    let mut faces = vec![[i, j, k], [i, k, l], [i, l, j], [j, l, k]];

    for (p_idx, &p) in points.iter().enumerate() {
        // drain_filter?
        let (visible_faces, invisible_faces) = faces
            .into_iter()
            .partition(|&[i, j, k]| signed_volume(p, points[i], points[j], points[k]) > 0);
        faces = visible_faces;

        // point is inside of convex hull
        if invisible_faces.is_empty() {
            continue;
        }

        let iter_boundary =
            |[i, j, k]: [usize; 3]| once([i, j]).chain(once([j, k])).chain(once([k, i]));
        let invisible_half_edges: HashSet<[usize; 2]> = invisible_faces
            .iter()
            .flat_map(|&face| iter_boundary(face))
            .collect();
        let boundary = invisible_half_edges
            .iter()
            .copied()
            .filter(|&[i, j]| !invisible_half_edges.contains(&[j, i]));
        faces.extend(boundary.map(|[i, j]| [i, j, p_idx]));
    }
    faces
}

#[test]
fn gen_test_cases() {
    use std::time::{SystemTime, UNIX_EPOCH};
    let mut rng = cheap_rand::Random::new(loop {
        let seed = (SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .unwrap()
            .as_millis()
            % u64::MAX as u128) as u64;
        if seed != 0 {
            break seed;
        }
    });

    let mut output_buf = Vec::<u8>::new();

    let n = 10000;
    let n_queries = 10;
    let mut pick = || rng.range_u64(0..4001) as i64 - 2000;
    writeln!(output_buf, "{} {}", n, n_queries).unwrap();
    for _ in 0..n {
        let [x, y, z] = loop {
            let [x, y, z] = [pick(), pick(), pick()];
            let [dx, dy, dz] = [x, y, z];
            if dx * dx + dy * dy + dz * dz <= 1000 * 1000 {
                break [x, y, z];
            }
        };
        writeln!(output_buf, "{} {} {}", x, y, z * 0 + 300).unwrap();
    }
    for _ in 0..n_queries {
        // writeln!(output_buf, "{} {} {} {}", pick(), pick(), pick(), pick()).unwrap();
        let [a, b, c] = [pick(), pick(), pick()];
        let d = 0;
        writeln!(output_buf, "{} {} {} {}", a, b, c, d).unwrap();
    }
    std::fs::write("input.txt", &output_buf).unwrap();
}

fn main() {
    use io::*;
    let input_buf = stdin();
    let mut input: &[u8] = &input_buf[..];

    let mut output_buf = Vec::<u8>::new();

    use std::cmp::Ordering;
    use std::collections::HashMap;
    use std::iter::once;

    let n_points: usize = input.value();
    let n_queries: usize = input.value();
    let mut points: Vec<[i64; 3]> = (0..n_points)
        .map(|_| [input.value(), input.value(), input.value()])
        .collect();

    let mut rng = cheap_rand::Random::new(10905525723936348110);
    rng.shuffle(&mut points);

    let mut faces = convex_hull_3d(&points);

    // remove unused points
    let mut transition_map = HashMap::new();
    faces
        .iter_mut()
        .flat_map(|[ref mut i, ref mut j, ref mut k]| once(i).chain(once(j)).chain(once(k)))
        .for_each(|i: &mut usize| {
            let new_idx = transition_map.len();
            *i = *transition_map.entry(*i).or_insert(new_idx);
        });
    let points: Vec<[i32; 3]> = {
        let mut v = vec![[0, 0, 0]; transition_map.len()];
        for (old_idx, new_idx) in transition_map {
            let [x, y, z] = points[old_idx];
            v[new_idx] = [x as i32, y as i32, z as i32];
        }
        v
    };

    let edges = faces
        .iter()
        .flat_map(|&[i, j, k]| once([i, j]).chain(once([j, k])).chain(once([k, i])))
        .filter(|&[i, j]| i < j)
        .collect::<Vec<_>>();

    /*
    // graphical debugging
    let mut mesh_str = Vec::<u8>::new();
    for &[x, y, z] in &points {
        writeln!(mesh_str, "v {} {} {}", x, y, z).unwrap();
    }
    for &[i, j, k] in &faces {
        writeln!(mesh_str, "f {} {} {}", i + 1, j + 1, k + 1).unwrap();
    }
    std::fs::write("cvhull.obj", mesh_str).unwrap();
    */
    for _ in 0..n_queries {
        use Ordering::*;
        let [a, b, c, d]: [i32; 4] = [input.value(), input.value(), input.value(), input.value()];
        let normal = [a, b, c];

        let signed_dist: Vec<i32> = points.iter().map(|&p| dot_3d(p, normal) + d).collect();

        let section = (points.iter().zip(&signed_dist))
            .filter_map(|(&p, &signed_dist)| {
                (signed_dist == 0).then(|| [p[0] as f64, p[1] as f64, p[2] as f64])
            })
            .chain(edges.iter().filter_map(|&[i, j]| {
                matches!(
                    (signed_dist[i].cmp(&0), signed_dist[j].cmp(&0)),
                    (Greater, Less) | (Less, Greater)
                )
                .then(|| {
                    let [p, q] = [points[i], points[j]];
                    let dr = sub_point3d(q, p);
                    let denom = signed_dist[j] - signed_dist[i];
                    let num = signed_dist[i];

                    let t = -num as f64 / denom as f64;
                    // debug_assert!(0.0 - 1e9 <= t && t <= 1.0 + 1e9);

                    [
                        p[0] as f64 + t * dr[0] as f64,
                        p[1] as f64 + t * dr[1] as f64,
                        p[2] as f64 + t * dr[2] as f64,
                    ]
                })
            }));

        fn cvhull_area(section: impl IntoIterator<Item = [f64; 2]>) -> f64 {
            let mut points: Vec<[f64; 2]> = section.into_iter().collect();
            let cvhull = convex_hull(&mut points);
            convex_hull_area(cvhull)
        }

        let normalizing_factor = ((a * a + b * b + c * c) as f64).sqrt();
        let result = if a != 0 {
            cvhull_area(section.map(|[_, y, z]| [y, z])) as f64 / a.abs() as f64
        } else if b != 0 {
            cvhull_area(section.map(|[x, _, z]| [x, z])) as f64 / b.abs() as f64
        } else if c != 0 {
            cvhull_area(section.map(|[x, y, _]| [x, y])) as f64 / c.abs() as f64
        } else {
            0.0
            // unimplemented!()
        } * normalizing_factor;

        writeln!(output_buf, "{:.16}", result).unwrap();
    }

    std::io::stdout().write_all(&output_buf[..]).unwrap();
}

// not my code. is copied. refer to jeoffrey0522.