import java.util.*;

public class JoshuaBMI {
    Scanner s = new Scanner(System.in);
    int n;
    List<double[]> circles;
    List<double[]> points;

    double[][] dist;
    boolean[][] adj;

    public static void main(String[] args) {
        (new JoshuaBMI()).solve();
    }

    double[] parsePt() {
        double x = s.nextDouble();
        double y = s.nextDouble();
        return new double[]{x, y};
    }

    double[] parseC() {
        double x = s.nextDouble();
        double y = s.nextDouble();
        double r = s.nextDouble();
        return new double[]{x, y, r};
    }

    double[][] intersect(double[] c1, double[] c2) {
        double diffx = c2[0] - c1[0];
        double diffy = c2[1] - c1[1];
        double Lsqr = diffx * diffx + diffy * diffy;
        double L = Math.sqrt(Lsqr);

        // no overlap
        if (L > c1[2] + c2[2]) return new double[0][];

        // containment or equality
        if (L <= Math.abs(c1[2] - c2[2])) return new double[0][];

        double a = (c1[2] * c1[2] - c2[2] * c2[2] + Lsqr) / (2 * L);
        double mx = c1[0] + (a / L) * diffx;
        double my = c1[1] + (a / L) * diffy;
        double h = Math.sqrt(c1[2] * c1[2] - a * a);

        // kissing circles
        if (h <= 0) return new double[][]{new double[]{mx, my}};

        // proper intersection w/ two points
        double offsetx = (h / L) * diffy;
        double offsety = -(h / L) * diffx;
        return new double[][]{new double[]{mx + offsetx, my + offsety}, new double[]{mx - offsetx, my - offsety}};
    }

    double dot(double[] p1, double[] p2) {
        return p1[0] * p2[0] + p1[1] * p2[1];
    }

    double[] intersect(double[] p1, double[] p2, double[] circ) {
        double[] diff = new double[]{p2[0] - p1[0], p2[1] - p1[1]};
        double a = dot(diff, diff);
        double b = 2 * dot(diff, new double[]{p1[0] - circ[0], p1[1] - circ[1]});
        double c = dot(circ, circ) + dot(p1, p1) - 2 * dot(circ, p1) - circ[2] * circ[2];

        double det = b * b - 4 * a * c;

        if (det < 0) return new double[0];

        // ignore kissing circles, because we added an epsilon in line_covered
        if (det == 0) {
            return new double[0];
        }

        double mu1 = (-b + Math.sqrt(det)) / (2 * a);
        double mu2 = (-b - Math.sqrt(det)) / (2 * a);
        if (mu1 < mu2) {
            return new double[]{mu1, mu2};
        } else {
            return new double[]{mu2, mu1};
        }
    }

    boolean line_covered(double[] p1, double[] p2) {
        if (Math.hypot(p2[0] - p1[0], p2[1] - p1[1]) < 1e-10) return true;

        class SC implements Comparable<SC> {
            double mu;
            int open_close;

            public SC(double mu, int open_close) {
                this.mu = mu;
                this.open_close = open_close;
            }

            @Override
            public int compareTo(SC o) {
                int r = Double.compare(mu, o.mu);
                if (r == 0) {
                    return Double.compare(open_close, o.open_close);
                }
                return r;
            }
        }


        List<SC> scope = new ArrayList<>(2 + 2*circles.size());
        scope.add(new SC(0, 0));
        scope.add(new SC(1, 0));

        for (double[] c : circles) {
            double[] ret = intersect(p1, p2, c);
            if (ret.length == 2) {
                scope.add(new SC(ret[0], -1));
                scope.add(new SC(ret[1], 1));
            }
        }

        Collections.sort(scope);

        int depth = 0;
        boolean on_line = false;
        double last_open = -1e50;
        double measure = 0;
        for (SC p : scope) {
            if (on_line && depth == 0 && p.open_close == -1)
                last_open = p.mu;

            if (on_line && depth == 1 && p.open_close == 1)
                measure += p.mu - last_open;

            depth -= p.open_close;

            if (p.open_close == 0) {
                if (!on_line && depth > 0)
                    last_open = p.mu;

                if (on_line && depth > 0)
                    measure += p.mu - last_open;

                on_line = !on_line;
            }
        }

        return 1 - measure < 1e-10;
    }

    void solve() {
        double[] barley = parsePt();
        double[] mom = parsePt();
        n = s.nextInt();

        circles = new ArrayList<>(n);
        for (int i = 0; i < n; i++) {
            circles.add(parseC());
        }

        points = new ArrayList<>(2 + n * (n - 1));
        points.add(barley);
        points.add(mom);
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                double[][] ret = intersect(circles.get(i), circles.get(j));
                points.addAll(Arrays.asList(ret));
            }
        }

        int m = points.size();

        dist = new double[m][m];
        adj = new boolean[m][m];

        for (int i = 0; i < m; ++i) {
            for (int j = i + 1; j < m; ++j) {
                if (line_covered(points.get(i), points.get(j))) {
                    double diffx = points.get(j)[0] - points.get(i)[0];
                    double diffy = points.get(j)[1] - points.get(i)[1];
                    dist[i][j] = dist[j][i] = Math.sqrt(diffx * diffx + diffy * diffy);
                    adj[i][j] = adj[j][i] = true;
                }
            }
        }

        // Dijkstra

        double[] sdist = new double[m];
        Arrays.fill(sdist, 1e12);

        boolean[] seen = new boolean[m];

        class QE implements Comparable<QE> {
            double dist;
            int node;

            public QE(double dist, int node) {
                this.dist = dist;
                this.node = node;
            }

            @Override
            public int compareTo(QE o) {
                return Double.compare(dist, o.dist);
            }
        }

        Queue<QE> work = new PriorityQueue<>();

        sdist[0] = 0;
        work.add(new QE(0, 0));

        while (!work.isEmpty()) {
            QE c = work.poll();

            if (seen[c.node]) continue;

            // We found mom!
            if (c.node == 1) {
                System.out.println(sdist[1]);
                return;
            }

            seen[c.node] = true;

            for (int i = 0; i < m; i++) {
                if (!adj[c.node][i]) continue;

                if (seen[i] || sdist[i] < sdist[c.node] + dist[c.node][i]) continue;

                sdist[i] = sdist[c.node] + dist[c.node][i];
                work.add(new QE(sdist[i], i));
            }
        }

        System.out.println("impossible");
    }
}
