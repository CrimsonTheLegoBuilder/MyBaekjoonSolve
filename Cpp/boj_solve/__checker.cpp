#include "testlib.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef double ld;
typedef std::vector<bool> Vbool;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
const ld PI = acos(-1);

//std::string S[8] = { "R:", "G:", "B:", "Y:", "M:", "C:", "W:", "L:"};

int main(int argc, char* argv[]) {
	registerTestlibCmd(argc, argv);
	int n = 0;
	std::string a, o;
	while (!ans.eof()) {
		ld j, p;
		a = ans.readString();
		if (a == "" && ans.eof()) break;
		o = ouf.readString();
		n++;
		if (a != o) quitf(_wa, "%d lines differ - expected: '%s', found: '%s'", n, a.c_str(), o.c_str());
		for (int i = 0; i < 8; i++) {
			n++;
			a = ans.readString();
			o = ouf.readString();
			size_t acol = a.find(':');
			size_t ocol = o.find(':');

			if (acol == std::string::npos || ocol == std::string::npos) {
				quitf(_wa, "line %d format error - missing ':' in expected or output", n);
			}

			std::string al = a.substr(0, acol);
			al = trim(al);
			std::string ol = o.substr(0, ocol);
			ol = trim(ol);
			if (al != ol) quitf(_wa, "line %d differ - expected: '%s', found: '%s'", n, al.c_str(), ol.c_str());
			
			try {
				al = a.substr(acol + 1);
				al = trim(al);
				ol = o.substr(ocol + 1);
				ol = trim(ol);
				j = std::stod(al);
				p = std::stod(ol);
			}
			catch (const std::exception& e) {
				quitf(_wa, "line %d format error - invalid numeric value in output", n);
			}

			ld rel_err, jmax, jmin;
			jmax = std::max(j * (1.0 + 1e-5 + 1e-15), j * (1.0 - 1e-5 - 1e-15));
			jmin = std::min(j * (1.0 + 1e-5 + 1e-15), j * (1.0 - 1e-5 - 1e-15));
			if (!doubleCompare(j, p, 1e-5)) {
				quitf(_wa, "line %d value mismatch - expected: %.8f, found: %.8f, absolute error: %.8e, relative error error: [%.8f, %.8f] (not in range)",
					n, j, p, fabs(j - p), jmin, jmax);
			}
		}
	}
	o = ouf.readString();
	if (o != "" || !ouf.eof()) {
		quitf(_fail, "Output limit exceeded: Unexpected extra output found.");
	}
	quitf(_ok, "all value ok!");
}