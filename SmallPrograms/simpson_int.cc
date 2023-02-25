#include <fmt/core.h>
#include <cmath>

double func(double x) {
    return x*sin(x);
}

double simpson(double a, double b, double (*f)(double)) {
    double c = (a + b)/2;
    return (b - a)/6*(f(a)+4*f(c)+f(b));
}

// composite quadrature rule
double cqr(double a, double b, double (*f)(double)) {
    double  c = (a + b)/2, \
            d = (a + c)/2, \
            e = (c + b)/2;
    return (b - a)/12*(f(a)+4*f(b)+2*f(c)+4*f(e)+f(b));
}

template <long long N>
double integrate(double a, double b, double (*f)(double)) {
    double base = (b-a)/N;
    double sum = 0;
    for(auto i=a,j=a+base;i<b;i=j,j+=base)
        sum += cqr(i, j, f);
    return sum;
}

template <long long N>
double ints(double a, double b, double (*f)(double), size_t depth = 0) {
    static size_t ints_called_count = 0;
    ints_called_count ++;
    double rs = simpson(a, b, f);
    double rc = cqr(a, b, f);
    if(fabs(rs-rc)<1e-10||depth>=N)
        return rc + (rc - rs)/15;
    else {
        double c = (a + b)/2;
        if(ints_called_count==1) {
            double retval = ints<N>(a, c, f, depth+1) + ints<N>(c, b, f, depth+1);
            fmt::print("{}\n", ints_called_count);
            return retval;
        }
        return ints<N>(a, c, f, depth+1) + ints<N>(c, b, f, depth+1);
    }
}

#include <gsl/gsl_integration.h>
//gsl solution
double gslIntegration(double a, double b, double (*g)(double)) {
    gsl_function gf;
    gf.function = (double (*)(double, void *))g;
 
    double r, er;
    size_t n;
    gsl_integration_qng(&gf, a, b, 1e-10, 1e-10, &r, &er, &n);
    return r;
}

int main() {
    fmt::print("{}\n", integrate<524287>(0.3, 1.5, func));
    fmt::print("{}\n", ints<200>(0.3, 1.5, func));
    fmt::print("{}\n", gslIntegration(0.3, 1.5, func));
    return 0;
}