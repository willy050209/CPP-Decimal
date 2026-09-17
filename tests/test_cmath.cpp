#include "test_helpers.hpp"
#include <numeric/DecimalMath.hpp>
#include <numeric/BigInt.hpp>
#include <numeric/Decimal.hpp>
#include <string>
#include <cstdint>
#include <cmath>

/// <summary>
/// 執行 compat 數學函式庫 (CMath) 完整單元測試套件。
/// </summary>
void run_test_cmath() {
    using numeric::bigint;
    using numeric::decimal;

    std::cout << "--- Running CMath Tests ---" << std::endl;

    // ==========================================
    // 2. Decimal 分類與基本數學函式測試
    // ==========================================
    {
        // 數值分類
        TEST_ASSERT(numeric::isnan(decimal::nan) == true);
        TEST_ASSERT(numeric::isnan(decimal(123)) == false);
        TEST_ASSERT(numeric::isinf(decimal::infinity) == true);
        TEST_ASSERT(numeric::isinf(-decimal::infinity) == true);
        TEST_ASSERT(numeric::isinf(decimal(123)) == false);
        TEST_ASSERT(numeric::isfinite(decimal(42)) == true);
        TEST_ASSERT(numeric::isfinite(decimal::infinity) == false);
        TEST_ASSERT(numeric::isfinite(decimal::nan) == false);
        TEST_ASSERT(numeric::signbit(decimal("-5.5")) == true);
        TEST_ASSERT(numeric::signbit(decimal("5.5")) == false);

        // copysign
        TEST_ASSERT(numeric::copysign(decimal("3.5"), decimal("-1.0")) == decimal("-3.5"));
        TEST_ASSERT(numeric::copysign(decimal("-3.5"), decimal("1.0")) == decimal("3.5"));
        TEST_ASSERT(numeric::copysign(decimal("-3.5"), decimal("-2.0")) == decimal("-3.5"));
        TEST_ASSERT(numeric::copysign(decimal("3.5"), decimal("2.0")) == decimal("3.5"));

        // abs
        TEST_ASSERT(numeric::abs(decimal(0)) == 0);
        TEST_ASSERT(numeric::abs(decimal("123.456")) == decimal("123.456"));
        TEST_ASSERT(numeric::abs(decimal("-123.456")) == decimal("123.456"));
        TEST_ASSERT(numeric::abs(decimal::infinity) == decimal::infinity);
        TEST_ASSERT(numeric::abs(-decimal::infinity) == decimal::infinity);
        TEST_ASSERT(numeric::isnan(numeric::abs(decimal::nan)));
    }

    // ==========================================
    // 3. Decimal 捨入、取模與餘數函式測試
    // ==========================================
    {
        // floor
        TEST_ASSERT(numeric::floor(decimal("2.7")) == 2);
        TEST_ASSERT(numeric::floor(decimal("2.0")) == 2);
        TEST_ASSERT(numeric::floor(decimal("-2.3")) == -3);
        TEST_ASSERT(numeric::floor(decimal("-2.0")) == -2);
        TEST_ASSERT(numeric::floor(decimal(0)) == 0);

        // ceil
        TEST_ASSERT(numeric::ceil(decimal("2.3")) == 3);
        TEST_ASSERT(numeric::ceil(decimal("2.0")) == 2);
        TEST_ASSERT(numeric::ceil(decimal("-2.7")) == -2);
        TEST_ASSERT(numeric::ceil(decimal("-2.0")) == -2);
        TEST_ASSERT(numeric::ceil(decimal(0)) == 0);

        // trunc
        TEST_ASSERT(numeric::trunc(decimal("2.7")) == 2);
        TEST_ASSERT(numeric::trunc(decimal("2.0")) == 2);
        TEST_ASSERT(numeric::trunc(decimal("-2.7")) == -2);
        TEST_ASSERT(numeric::trunc(decimal("-2.0")) == -2);
        TEST_ASSERT(numeric::trunc(decimal(0)) == 0);

        // round (Half away from zero)
        TEST_ASSERT(numeric::round(decimal("2.3")) == 2);
        TEST_ASSERT(numeric::round(decimal("2.5")) == 3);
        TEST_ASSERT(numeric::round(decimal("2.7")) == 3);
        TEST_ASSERT(numeric::round(decimal("-2.3")) == -2);
        TEST_ASSERT(numeric::round(decimal("-2.5")) == -3);
        TEST_ASSERT(numeric::round(decimal("-2.7")) == -3);
        TEST_ASSERT(numeric::round(decimal("3.5")) == 4);
        TEST_ASSERT(numeric::round(decimal("-3.5")) == -4);

        // fmod: x - trunc(x/y) * y
        TEST_ASSERT(numeric::fmod(decimal("5.3"), decimal("2.0")) == decimal("1.3"));
        TEST_ASSERT(numeric::fmod(decimal("-5.3"), decimal("2.0")) == decimal("-1.3"));
        TEST_ASSERT(numeric::fmod(decimal("5.3"), decimal("-2.0")) == decimal("1.3"));
        TEST_ASSERT(numeric::fmod(decimal("5.0"), decimal("2.5")) == 0);

        // remainder: x - round(x/y) * y
        TEST_ASSERT(numeric::remainder(decimal("5.0"), decimal("2.0")) == decimal("1.0"));
        TEST_ASSERT(numeric::remainder(decimal("7.0"), decimal("2.0")) == decimal("-1.0"));
        TEST_ASSERT(numeric::remainder(decimal("7.0"), decimal("3.0")) == decimal("1.0"));
    }

    // ==========================================
    // 4. Decimal 平方根、立方根與 hypot 測試
    // ==========================================
    {
        // sqrt
        TEST_ASSERT(numeric::sqrt(decimal(0)) == 0);
        TEST_ASSERT(numeric::sqrt(decimal(1)) == 1);
        TEST_ASSERT(numeric::sqrt(decimal(4)) == 2);
        TEST_ASSERT(numeric::sqrt(decimal(9)) == 3);
        TEST_ASSERT(numeric::sqrt(decimal(100)) == 10);
        TEST_ASSERT(numeric::sqrt(decimal("0.25")) == decimal("0.5"));
        TEST_ASSERT(numeric::sqrt(decimal::infinity) == decimal::infinity);
        TEST_ASSERT(numeric::isnan(numeric::sqrt(decimal(-1))));

        // 34 位有效位數精度驗證
        decimal sqrt2 = numeric::sqrt(decimal(2));
        decimal diff2 = numeric::abs(sqrt2 * sqrt2 - decimal(2));
        TEST_ASSERT(diff2 < decimal("1e-30"));

        decimal big_val("123456789012345678901234567890.1234");
        decimal r_big = numeric::sqrt(big_val);
        decimal diff_big = numeric::abs(r_big * r_big - big_val);
        TEST_ASSERT(diff_big / big_val < decimal("1e-30"));

        // cbrt
        TEST_ASSERT(numeric::cbrt(decimal(0)) == 0);
        TEST_ASSERT(numeric::cbrt(decimal(1)) == 1);
        TEST_ASSERT(numeric::cbrt(decimal(8)) == 2);
        TEST_ASSERT(numeric::cbrt(decimal(-8)) == -2);
        TEST_ASSERT(numeric::cbrt(decimal(27)) == 3);
        TEST_ASSERT(numeric::cbrt(decimal(-27)) == -3);
        TEST_ASSERT(numeric::cbrt(decimal::infinity) == decimal::infinity);
        TEST_ASSERT(numeric::cbrt(-decimal::infinity) == -decimal::infinity);

        decimal cbrt2 = numeric::cbrt(decimal(2));
        decimal diff_cb = numeric::abs(cbrt2 * cbrt2 * cbrt2 - decimal(2));
        TEST_ASSERT(diff_cb < decimal("1e-30"));

        // hypot
        TEST_ASSERT(numeric::hypot(decimal(3), decimal(4)) == 5);
        TEST_ASSERT(numeric::hypot(decimal(5), decimal(12)) == 13);
        TEST_ASSERT(numeric::hypot(decimal(0), decimal(7)) == 7);
        TEST_ASSERT(numeric::hypot(decimal(-3), decimal(4)) == 5);
        TEST_ASSERT(numeric::hypot(decimal::infinity, decimal(100)) == decimal::infinity);
    }

    // ==========================================
    // 5. Decimal 指數與對數函式測試
    // ==========================================
    {
        // exp
        TEST_ASSERT(numeric::exp(decimal(0)) == 1);
        TEST_ASSERT(numeric::exp(decimal::infinity) == decimal::infinity);
        TEST_ASSERT(numeric::exp(-decimal::infinity) == 0);
        TEST_ASSERT(numeric::isnan(numeric::exp(decimal::nan)));

        decimal e1 = numeric::exp(decimal(1));
        decimal exp_expected("2.718281828459045235360287471352662");
        TEST_ASSERT(numeric::abs(e1 - exp_expected) < decimal("1e-30"));

        decimal e_neg1 = numeric::exp(decimal(-1));
        TEST_ASSERT(numeric::abs(e1 * e_neg1 - decimal(1)) < decimal("1e-30"));

        // log
        TEST_ASSERT(numeric::log(decimal(1)) == 0);
        TEST_ASSERT(numeric::isnan(numeric::log(decimal(-1))));
        TEST_ASSERT(numeric::log(decimal(0)) == -decimal::infinity);
        TEST_ASSERT(numeric::log(decimal::infinity) == decimal::infinity);

        decimal ln_e = numeric::log(e1);
        TEST_ASSERT(numeric::abs(ln_e - decimal(1)) < decimal("1e-30"));

        decimal ln2 = numeric::log(decimal(2));
        decimal ln2_expected("0.6931471805599453094172321214581765");
        TEST_ASSERT(numeric::abs(ln2 - ln2_expected) < decimal("1e-30"));

        // 互逆性驗證 log(exp(x)) == x
        decimal x_val("3.14159");
        TEST_ASSERT(numeric::abs(numeric::log(numeric::exp(x_val)) - x_val) < decimal("1e-28"));

        // log10
        TEST_ASSERT(numeric::log10(decimal(1)) == 0);
        TEST_ASSERT(numeric::log10(decimal(10)) == 1);
        TEST_ASSERT(numeric::log10(decimal(100)) == 2);
        TEST_ASSERT(numeric::log10(decimal(1000)) == 3);
        TEST_ASSERT(numeric::log10(decimal("0.1")) == -1);
        TEST_ASSERT(numeric::log10(decimal("0.01")) == -2);

        // log2
        TEST_ASSERT(numeric::log2(decimal(1)) == 0);
        TEST_ASSERT(numeric::log2(decimal(2)) == 1);
        TEST_ASSERT(numeric::log2(decimal(8)) == 3);
        TEST_ASSERT(numeric::log2(decimal(1024)) == 10);
        TEST_ASSERT(numeric::log2(decimal("0.5")) == -1);
        TEST_ASSERT(numeric::log2(decimal("0.25")) == -2);
    }

    // ==========================================
    // 6. Decimal 冪次 pow 函式測試
    // ==========================================
    {
        // pow(decimal, int)
        TEST_ASSERT(numeric::pow(decimal(2), 0) == 1);
        TEST_ASSERT(numeric::pow(decimal(2), 3) == 8);
        TEST_ASSERT(numeric::pow(decimal(2), -2) == decimal("0.25"));
        TEST_ASSERT(numeric::pow(decimal("2.5"), 2) == decimal("6.25"));
        TEST_ASSERT(numeric::pow(decimal("-2"), 3) == -8);
        TEST_ASSERT(numeric::pow(decimal("-2"), 4) == 16);

        // pow(decimal, decimal)
        TEST_ASSERT(numeric::pow(decimal(5), decimal(0)) == 1);
        TEST_ASSERT(numeric::pow(decimal(0), decimal(0)) == 1);
        TEST_ASSERT(numeric::pow(decimal(1), decimal("3.14")) == 1);
        TEST_ASSERT(numeric::pow(decimal(4), decimal("0.5")) == 2);
        TEST_ASSERT(numeric::abs(numeric::pow(decimal(27), decimal(1) / decimal(3)) - decimal(3)) < decimal("1e-28"));
        TEST_ASSERT(numeric::isnan(numeric::pow(decimal(-2), decimal("0.5"))));
    }

    // ==========================================
    // 7. Decimal 三角函式測試
    // ==========================================
    {
        // sin, cos, tan
        TEST_ASSERT(numeric::sin(decimal(0)) == 0);
        TEST_ASSERT(numeric::cos(decimal(0)) == 1);
        TEST_ASSERT(numeric::tan(decimal(0)) == 0);
        TEST_ASSERT(numeric::isnan(numeric::sin(decimal::infinity)));
        TEST_ASSERT(numeric::isnan(numeric::cos(decimal::infinity)));

        decimal pi("3.141592653589793238462643383279502884");
        decimal pi_over_2 = pi / decimal(2);
        decimal pi_over_4 = pi / decimal(4);
        decimal pi_over_6 = pi / decimal(6);
        decimal pi_over_3 = pi / decimal(3);

        // sin(pi) 接近 0, cos(pi) 接近 -1
        TEST_ASSERT(numeric::abs(numeric::sin(pi)) < decimal("1e-30"));
        TEST_ASSERT(numeric::abs(numeric::cos(pi) - decimal(-1)) < decimal("1e-30"));

        // sin(pi/2) 接近 1, cos(pi/2) 接近 0
        TEST_ASSERT(numeric::abs(numeric::sin(pi_over_2) - decimal(1)) < decimal("1e-30"));
        TEST_ASSERT(numeric::abs(numeric::cos(pi_over_2)) < decimal("1e-30"));

        // sin(pi/6) 接近 0.5, cos(pi/3) 接近 0.5
        TEST_ASSERT(numeric::abs(numeric::sin(pi_over_6) - decimal("0.5")) < decimal("1e-30"));
        TEST_ASSERT(numeric::abs(numeric::cos(pi_over_3) - decimal("0.5")) < decimal("1e-30"));

        // tan(pi/4) 接近 1
        TEST_ASSERT(numeric::abs(numeric::tan(pi_over_4) - decimal(1)) < decimal("1e-25"));

        // 恆等式 sin^2(x) + cos^2(x) == 1
        decimal test_ang("1.23456");
        decimal s = numeric::sin(test_ang);
        decimal c = numeric::cos(test_ang);
        TEST_ASSERT(numeric::abs((s * s + c * c) - decimal(1)) < decimal("1e-30"));
    }

    // ==========================================
    // 8. 命名空間與 ADL 機制驗證
    // ==========================================
    {
        // ADL 測試（無前綴自動查找 numeric::sqrt）
        bigint b(100);
        decimal d(100);
        TEST_ASSERT(sqrt(b) == 10);
        TEST_ASSERT(sqrt(d) == 10);

        // std:: 命名空間直接呼叫驗證
        TEST_ASSERT(std::abs(bigint(-50)) == 50);
        TEST_ASSERT(std::abs(decimal("-50.5")) == decimal("50.5"));
        TEST_ASSERT(std::sqrt(bigint(64)) == 8);
        TEST_ASSERT(std::sqrt(decimal(64)) == 8);
        TEST_ASSERT(std::cbrt(decimal(64)) == 4);
        TEST_ASSERT(std::floor(decimal("3.7")) == 3);
        TEST_ASSERT(std::ceil(decimal("3.2")) == 4);
        TEST_ASSERT(std::round(decimal("3.5")) == 4);
        TEST_ASSERT(std::pow(decimal(2), 5) == 32);
    }

    std::cout << "All CMath tests passed successfully!" << std::endl;
}
