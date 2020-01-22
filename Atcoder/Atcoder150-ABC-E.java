import java.util.Arrays;
import java.util.Scanner;

public class Main {

    static final long MOD = (long) (1e9 + 7);
    static int n;
    static long[] a;
    static long[] f;
    static long[] inv;
    static long[] C;
    static long[] P;
    static long[] F;

    private static long modPow(long b, long e) {
        if(e == 0) return 1;
        if(e == 1) return b;
        return modPow(b*b % MOD, e >> 1) * modPow(b, e & 1) % MOD;
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        n = sc.nextInt();
        // Setup
        f = new long[n + 1];
        f[0] = 1;
        for(int i = 1; i <= n; i++) f[i] = f[i-1] * i % MOD;
        inv = new long[n + 1];
        inv[0] = 1;
        for(int i = 1; i <= n; i++) inv[i] = modPow(f[i], MOD - 2);
        C = new long[n + 1];
        for(int i = 1; i <= n; i++) C[i] = f[n] * inv[n - i] % MOD * inv[i] % MOD;
        P = new long[2 * n  + 10];
        P[0] = 1;
        for(int i = 1; i < P.length; i++) {
            P[i] = P[i-1] * 2 % MOD;
        }
        // F denotes the sum of kC(n, k) from 0 to n (used to calculate final answer)
        // F was found through observation (trial/error) that
        // F[i]/F[i-1] = (F[i-1] - F[i-2])/(F[i-2] - F[i-3])
        F = new long[Integer.max(n + 1, 3)];
        F[0] = 0;
        F[1] = 1;
        F[2] = 4;
        for(int i = 3; i <= n; i++) {
            F[i] = F[i-1] * (F[i-1] - F[i-2]) % MOD * modPow(F[i-2] - F[i-3], MOD - 2) % MOD;
        }
        a = new long[n + 1];
        for(int i = 1; i <= n; i++) a[i] = sc.nextInt();
        Arrays.sort(a);
        for(int i = 1; i <= n / 2; i++) {
            long tmp = a[i];
            a[i] = a[n - i + 1];
            a[n - i + 1] = tmp;
        }


        // Compute final answer
        long ans = 0;
        for(int i = 1; i <= n; i++) {
            // F[i-1] + P[i-1] because we had (k+1)C(n, k) from the form above
            // so we can just factor a sum of C(n, k) out to get 2^n
            ans += (F[i-1] + P[i-1]) * a[i] % MOD * P[n - i] % MOD;
            ans %= MOD;
        }

        ans *= P[n];
        ans %= MOD;
        if(ans < 0) ans += MOD;
        System.out.println(ans);
    }
}
