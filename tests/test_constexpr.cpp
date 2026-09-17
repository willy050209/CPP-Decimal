#include "test_helpers.hpp"
#include <numeric/Decimal.hpp>
#include <numeric/DecimalMath.hpp>
#include <iostream>

#if (NUMERIC_CPLUSPLUS >= NUMERIC_CXX_20)

// ----------------------------------------------------------------------------
// 編譯期驗證：Decimal
// ----------------------------------------------------------------------------
namespace test_ce_decimal {
    // 1. 常數建構與代理
    constexpr numeric::decimal d0;
    static_assert(d0.is_zero(), "decimal default ctor is zero");
    static_assert(d0.is_sbo(), "decimal d0 must be SBO");

    constexpr numeric::decimal d_proxy_zero = numeric::decimal::zero;
    constexpr numeric::decimal d_proxy_one = numeric::decimal::one();
    static_assert(d_proxy_zero == 0, "decimal::zero check");
    static_assert(d_proxy_one == 1, "decimal::one check");

    constexpr numeric::decimal d_from_int(12345);
    static_assert(d_from_int == 12345, "d_from_int == 12345");

    constexpr numeric::decimal d_from_unscaled(numeric::bigint(12345), 2);
    static_assert(d_from_unscaled.scale() == 2, "scale == 2");
    static_assert(d_from_unscaled.unscaled() == 12345, "unscaled == 12345");

    // 2. 字串解析
    constexpr numeric::decimal d_pi("3.1415");
    static_assert(d_pi.scale() == 4, "pi scale == 4");
    static_assert(d_pi.unscaled() == 31415, "pi unscaled == 31415");

    // 3. 算術運算
    constexpr numeric::decimal a("1.25");
    constexpr numeric::decimal b("2.50");
    constexpr numeric::decimal c_add = a + b;
    static_assert(c_add == numeric::decimal("3.75"), "1.25 + 2.50 == 3.75");

    constexpr numeric::decimal c_sub = b - a;
    static_assert(c_sub == numeric::decimal("1.25"), "2.50 - 1.25 == 1.25");

    constexpr numeric::decimal c_mul = a * b;
    static_assert(c_mul == numeric::decimal("3.125"), "1.25 * 2.50 == 3.125");

    constexpr numeric::decimal c_div = numeric::decimal("10.0").divide(numeric::decimal("4.0"), 2);
    static_assert(c_div == numeric::decimal("2.5"), "10.0 / 4.0 == 2.5");

    constexpr numeric::decimal c_mod = numeric::decimal("5.5") % numeric::decimal("2.0");
    static_assert(c_mod == numeric::decimal("1.5"), "5.5 % 2.0 == 1.5");

    // 4. 捨入 (Banker's Rounding / Half-Even)
    constexpr numeric::decimal d_round1 = numeric::decimal("2.5").round(0);
    static_assert(d_round1 == 2, "2.5 round half-even == 2");

    constexpr numeric::decimal d_round2 = numeric::decimal("3.5").round(0);
    static_assert(d_round2 == 4, "3.5 round half-even == 4");

    // 5. 比較與邏輯
    static_assert(a < b, "1.25 < 2.50");
    static_assert(b > a, "2.50 > 1.25");
    static_assert(a <= a, "1.25 <= 1.25");
    static_assert(a >= a, "1.25 >= 1.25");
    static_assert(a != b, "1.25 != 2.50");
    static_assert(static_cast<bool>(a), "a is truthy");
    static_assert(!d0, "d0 is falsy");
    static_assert((a && true) == true, "a && true");
    static_assert((d0 || false) == false, "d0 || false");

    // 6. 特殊值
    constexpr numeric::decimal d_inf = numeric::decimal::infinity;
    constexpr numeric::decimal d_nan = numeric::decimal::nan;
    static_assert(d_inf.is_infinite(), "d_inf is infinite");
    static_assert(d_nan.is_nan(), "d_nan is nan");
    static_assert(d_inf > a, "infinity > 1.25");
    static_assert((d_nan == d_nan) == false, "nan == nan is false");
    static_assert((d_nan != d_nan) == true, "nan != nan is true");
}

// ----------------------------------------------------------------------------
// 編譯期驗證：CMath
// ----------------------------------------------------------------------------
namespace test_ce_cmath {
    // 1. 常數函式
    constexpr numeric::decimal pi = numeric::detail::cmath_pi();
    static_assert(pi.is_sbo(), "cmath_pi must be in SBO buffer");
    static_assert(pi > 3, "cmath_pi > 3");
    static_assert(pi < 4, "cmath_pi < 4");

    constexpr numeric::decimal two_pi = numeric::detail::cmath_two_pi();
    static_assert(two_pi > 6, "two_pi > 6");

    constexpr numeric::decimal ln2 = numeric::detail::cmath_ln2();
    static_assert(ln2 > 0 && ln2 < 1, "0 < ln2 < 1");

    constexpr numeric::decimal ln10 = numeric::detail::cmath_ln10();
    static_assert(ln10 > 2 && ln10 < 3, "2 < ln10 < 3");

    // 2. 數值分類與符號
    static_assert(numeric::isnan(numeric::decimal::nan), "isnan check");
    static_assert(!numeric::isnan(pi), "!isnan(pi) check");
    static_assert(numeric::isinf(numeric::decimal::infinity), "isinf check");
    static_assert(numeric::isfinite(pi), "isfinite check");
    static_assert(numeric::signbit(numeric::decimal("-1.5")), "signbit check");
    static_assert(!numeric::signbit(pi), "!signbit(pi) check");

    constexpr numeric::decimal cs = numeric::copysign(numeric::decimal("2.5"), numeric::decimal("-1.0"));
    static_assert(cs == numeric::decimal("-2.5"), "copysign check");

    // 3. 捨入函式
    constexpr numeric::decimal fl = numeric::floor(numeric::decimal("3.7"));
    static_assert(fl == 3, "floor(3.7) == 3");

    constexpr numeric::decimal ce = numeric::ceil(numeric::decimal("3.2"));
    static_assert(ce == 4, "ceil(3.2) == 4");

    constexpr numeric::decimal tr = numeric::trunc(numeric::decimal("-3.7"));
    static_assert(tr == -3, "trunc(-3.7) == -3");

    constexpr numeric::decimal rd = numeric::round(numeric::decimal("3.5"));
    static_assert(rd == 4, "round(3.5) == 4");

    // 4. abs
    constexpr numeric::decimal dec_abs = numeric::abs(numeric::decimal("-12.34"));
    static_assert(dec_abs == numeric::decimal("12.34"), "abs(decimal(-12.34)) == 12.34");
}

#endif // NUMERIC_CPLUSPLUS >= NUMERIC_CXX_20

/// <summary>
/// 執行編譯期常數求值之執行期相容性驗證測試。
/// </summary>
void run_test_constexpr() {
    std::cout << "[Testing Constexpr Evaluation (C++20+)]" << std::endl;

#if (NUMERIC_CPLUSPLUS >= NUMERIC_CXX_20)
    TEST_ASSERT(test_ce_decimal::c_add == numeric::decimal("3.75"));
    TEST_ASSERT(test_ce_decimal::c_mul == numeric::decimal("3.125"));
    TEST_ASSERT(test_ce_decimal::d_round1 == 2);
    TEST_ASSERT(test_ce_decimal::d_round2 == 4);
    TEST_ASSERT(test_ce_cmath::pi > 3 && test_ce_cmath::pi < 4);
    TEST_ASSERT(test_ce_cmath::fl == 3);
    TEST_ASSERT(test_ce_cmath::ce == 4);
    TEST_ASSERT(test_ce_cmath::tr == -3);
    TEST_ASSERT(test_ce_cmath::rd == 4);
    TEST_ASSERT(test_ce_cmath::dec_abs == numeric::decimal("12.34"));
    std::cout << "  -> Constexpr static_assert and runtime verification passed." << std::endl;
#else
    std::cout << "  -> Skipped on C++ < 20 (standard does not support non-literal constexpr types)." << std::endl;
#endif
}
