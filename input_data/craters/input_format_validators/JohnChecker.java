import java.util.*;

// checker for Craters

public class JohnChecker
{
	public static final int MINN = 1;
	public static final int MAXN = 200;
	public static final int MAXXY = 10000;
	public static final int MAXR = 5000;

	public static void printError(int line, String msg)
	{
		System.out.println("ERROR Line " + line + ": " + msg);
		System.exit(-1);
	}

	public static void main(String [] args)
	{
		Scanner in = new Scanner(System.in);
		int n, nLines = 1;
		int x, y, r;
		String line;

		n = in.nextInt();
		if (n < MINN || n > MAXN)
			printError(nLines, "n value out of range: " + n);
		line = in.nextLine();
		while (in.hasNext()) {
			line = in.nextLine();
			nLines++;
			StringTokenizer st = new StringTokenizer(line);
			if (st.countTokens() != 3)
				printError(nLines, "number of values on line incorrect");

			x = Integer.parseInt(st.nextToken());
			y = Integer.parseInt(st.nextToken());
			r = Integer.parseInt(st.nextToken());
			if (x < -MAXXY || x > MAXXY)
				printError(nLines, "invalid x value: " + x);
			if (y < -MAXXY || y > MAXXY)
				printError(nLines, "invalid y value: " + y);
			if (r < 1 || r > MAXR)
				printError(nLines, "invalid r value: " + r);
 		}
		if (nLines != n+1)
			printError(nLines, "incorrect number of lines");
		System.exit(42);
 	}
}
