public final class Mathx {
    static{
        loadNativeLibrary();
    }
    private Mathx() {
        // Private constructor to prevent instantiation
    }

    public static final double Euler = 2.7182818284590455;
    public static final double PI = 3.14159265358979323846;
    private static final double LN10 = 2.302585092994046;

    public static double sum(double a, double b) {
        return a + b;
    }

    public static int sum(int a, int b) {
        return a + b;
    }

    public static double sub(double a, double b) {
        return a - b;
    }

    public static int sub(int a, int b) {
        return a - b;
    }

    public static double div(double a, double b) {
        return a / b;
    }

    public static double mul(double a, double b) {
        return a * b;
    }

    public static int mul(int a, int b) {
        return a * b;
    }

    public static double mod(double a, double b) {
        return a % b;
    }

    public static int mod(int a, int b) {
        return a % b;
    }

    public static double max(double a, double b) {
        return (a > b) ? a : b;
    }

    public static int max(int a, int b) {
        return (a > b) ? a : b;
    }

    public static double min(double a, double b) {
        return (a > b) ? b : a;
    }

    public static int min(int a, int b) {
        return (a > b) ? b : a;
    }

    public static double abs(double a) {
        return (a < 0) ? -a : a;
    }

    public static int abs(int a) {
        return (a < 0) ? -a : a;
    }

    public static double power(double base, int exponent) {
        double result = 1.0;
        if (exponent == 0) {
            return 1;
        } else if (exponent < 0) {
            for (int i = 0; i > exponent; i--) {
                result /= base;
            }
        } else {
            for (int i = 0; i < exponent; i++) {
                result *= base;
            }
        }
        return result;
    }

    public static native double factorial(int n);

    public static native double euler(int n);

    public static native double pi(int n);

    public static native double sqrt(double s);

    public static native double exp(double x);

    public static native double ln(double x);

    public static double log10(double x) {
        return ln(x) / LN10;
    }

    public static double log(double x, double base) {
        return ln(x) / ln(base);
    }

    public static final class Geo {
        static{
            loadNativeLibrary();
        }
        private Geo() {
            // Private constructor to prevent instantiation
        }
        public static double toRadians(double degrees) {
            return degrees * (PI / 180.0);
        }

        public static double toDegrees(double radians) {
            return radians * (180.0 / PI);
        }

        public static double sin(double value, boolean isDegree) {
            if (isDegree) {
                return sin(toRadians(value));
            } else {
                return sin(value);
            }
        }

        private static native double sin(double radians);

        public static double cos(double value, boolean isDegree) {
            if (isDegree) {
                return cos(toRadians(value));
            } else {
                return cos(value);
            }
        }

        private static native double cos(double radians);

        public static double tan(double value, boolean isDegree) {
            if (isDegree) {
                return tan(toRadians(value));
            } else {
                return tan(value);
            }
        }

        private static double tan(double radians) {
            return sin(radians) / cos(radians);
        }
    }

    public static final class Stats {
        static{
            loadNativeLibrary();
        }
        private Stats() {
            // Private constructor to prevent instantiation
        }
        public static native double mean(double[] values);

        public static native double mean(int[] values);

        public static native double mean(long[] values);

        public static native double mean(float[] values);

        public static native double stdDev(double[] values);

        public static native double stdDev(int[] values);

        public static native double stdDev(long[] values);

        public static native double stdDev(float[] values);
    }

    private static void loadNativeLibrary(){
        try {
            System.loadLibrary("mathx");
        } catch (UnsatisfiedLinkError e) {
            throw new RuntimeException(e + "Native library 'mathx' could not be loaded. Make sure it is in the library path.");
        }
    }
}

class Test {
    public static void main(String[] args) {
        System.out.println("Welcome to Mathx Library!");
    }
}