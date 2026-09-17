# DecimalMath 數學函式庫 (Overview)

提供專為 `numeric::decimal` 打造的高精度標準數學函式庫，所有函式皆定義於 `<numeric/DecimalMath.hpp>` 中，命名與行為嚴格對齊 C++ 標準庫 `<cmath>`。

- **命名空間**：`numeric` (並於 `std` 命名空間提供對應的引數相關查閱 ADL 重載)
- **標頭檔**：`<numeric/DecimalMath.hpp>`

---

## 函式分類索引 (Category Index)

| 分類 | 說明文件 | 包含函式 |
| :--- | :--- | :--- |
| **數值分類與符號** | [數值分類與符號 (classification-and-sign.md)](classification-and-sign.md) | `isnan`, `isinf`, `isfinite`, `signbit`, `copysign`, `abs` |
| **捨入取模與餘數** | [捨入取模與餘數 (rounding-and-modulo.md)](rounding-and-modulo.md) | `floor`, `ceil`, `trunc`, `round`, `fmod`, `remainder` |
| **方根與乘冪** | [方根與乘冪 (powers-and-roots.md)](powers-and-roots.md) | `sqrt`, `cbrt`, `hypot`, `pow` (整數與實數冪) |
| **指數與對數** | [指數與對數 (exponential-and-logarithmic.md)](exponential-and-logarithmic.md) | `exp`, `log`, `log10`, `log2` |
| **三角函數** | [三角函數 (trigonometric.md)](trigonometric.md) | `sin`, `cos`, `tan` |
| **高精度常數** | [常數與代理 (../constants.md)](../constants.md) | `cmath_pi`, `cmath_two_pi`, `cmath_pi_over_2`, `cmath_ln2`, `cmath_ln10` |

---

## 實作特色與精度保證 (Key Features)

1. **零內部依賴**：完全以現代純 C++ 演算法自研實作，無須引入外部龐大套件（如 MPFR 或 GMP）。
2. **高精度收斂**：
   - 乘方根、對數與三角函數採用高階泰勒級數（Taylor Series）、牛頓-拉弗森切線法（Newton-Raphson）與快速冪技術。
   - 運算過程採用多重範圍縮減（Range Reduction，例如模 $2\pi$、模 $\ln 2$、模 $\ln 10$），計算過程均保留高達 40~42 位中間計算餘裕，最後規格化除法截斷至 34 位有效數字，精準消除累積誤差。
3. **無縫取代標準庫**：
   同時在 `numeric::` 與 `std::` 命名空間中注入重載版本，支援泛型演算法中的未限定名稱呼叫（Argument-Dependent Lookup, ADL）：
   ```cpp
   using std::sqrt;
   auto r = sqrt(d); // 自動路由至 numeric::sqrt
   ```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上（分類與符號函式在 C++20+ 支援 `constexpr`）
- **標頭檔**：`<numeric/DecimalMath.hpp>`
