# CPP-Decimal

高效、高精度、零外部依賴的現代 C++ 十進位浮點數與數學函式庫 (Decimal Floating-Point Library)。

向下相容至 **C++11**，並於 **C++20+** 全面支援編譯期常數求值 (`constexpr`)。

---

## 核心特性

- **IEEE 754-2008 decimal128 標準相容**：
  - 預設提供 **34 位十進位有效數字** 高精度數值表示。
  - 完全消除二進位浮點數（如 `double` / `float`）固有的進位誤差與捨入陷阱（例如 `0.1 + 0.2 == 0.3` 精準成立）。
- **128-bit Small Buffer Optimization (SBO)**：
  - 底層未縮放整數（Unscaled Significand）在 128 位元以內時達成 **0 次 Heap 動態記憶體配置**。
- **標準捨入法支援**：
  - 預設實作 IEEE 754 推薦之 **銀行家捨入法 (Banker's Rounding / Round Half-to-Even)**。
  - 支援以指定精度（小數位數）進行四則運算、捨入與截斷。
- **完備的特殊值語意**：
  - 正負無窮大 (`+Infinity`, `-Infinity`) 與非數值 (`NaN`)。
  - 符合 IEEE 規格的比較邏輯（`NaN != NaN` 恆為 true）。
- **豐富的數學函式庫 (`numeric/DecimalMath.hpp`)**：
  - **數值分類與符號**：`isnan`, `isinf`, `isfinite`, `signbit`, `copysign`, `abs`。
  - **捨入取模**：`floor`, `ceil`, `trunc`, `round`, `fmod`, `remainder`。
  - **開方與幾何**：`sqrt`（二次收斂至 34 位精準度）, `cbrt`, `hypot`。
  - **指數與對數**：`exp`, `exp2`, `expm1`, `log`, `log10`, `log2`, `log1p`, `pow`。
  - **三角與雙曲**：`sin`, `cos`, `tan`, `asin`, `acos`, `atan`, `atan2`, `sinh`, `cosh`, `tanh`。
  - **高精度常數**：$\pi$、$\ln 2$、$\ln 10$。
- **現代格式化與容器**：
  - 支援 C++20 `std::format`（支援格式化語法如 `{:.2f}`, `{:.4f}`, `{}`）。
  - 支援 `std::hash<numeric::decimal>`（已規格化處理，`1.0` 與 `1.00` 雜湊值一致）。

---

## 快速開始

### CMake 引入

本函式庫為 Header-only，直接將 `include` 目錄加入至專案即可：

```cmake
add_subdirectory(CPP-Decimal)
target_link_libraries(your_target PRIVATE decimal)
```

### 範例程式碼

```cpp
#include <numeric/Decimal.hpp>
#include <numeric/DecimalMath.hpp>
#include <iostream>

int main() {
    // 1. 高精度數值建構
    numeric::decimal a("0.1");
    numeric::decimal b("0.2");
    numeric::decimal c = a + b;
    std::cout << "0.1 + 0.2 = " << c << std::endl; // 0.3 (無二進位浮點誤差)

    // 2. 常數代理與 IEEE 特殊值
    numeric::decimal zero = numeric::decimal::zero;
    numeric::decimal inf  = numeric::decimal::infinity;
    std::cout << "isinf: " << std::boolalpha << numeric::isinf(inf) << std::endl;

    // 3. 高精度開方與三角函數
    numeric::decimal sqrt2 = numeric::sqrt(numeric::decimal(2));
    std::cout << "sqrt(2) = " << sqrt2 << std::endl;

    numeric::decimal pi = numeric::detail::cmath_pi();
    numeric::decimal sin_half_pi = numeric::sin(pi / 2);
    std::cout << "sin(pi/2) = " << sin_half_pi << std::endl; // 1.0

    return 0;
}
```

---

## 授權條款

本專案採用 MIT 授權條款。詳見 [LICENSE](LICENSE)。
