# 乘冪與方根函式 (Powers and Roots Functions)

提供高精度平方根、立方根、直角三角形斜邊長（歐幾里得距離）以及整數與實數冪次計算函式。

- **命名空間**：`numeric` 與 `std`
- **標頭檔**：`<numeric/DecimalMath.hpp>`

---

## 函式清單 (Function List)

| 函式 | 傳回型別 | 屬性標記 | 說明 |
| :--- | :--- | :--- | :--- |
| [`sqrt(x)`](#1-sqrt) | `decimal` | `[[nodiscard]]` | 平方根 $\sqrt{x}$（牛頓迭代二次收斂至 34 位）。 |
| [`cbrt(x)`](#2-cbrt) | `decimal` | `[[nodiscard]]` | 立方根 $\sqrt[3]{x}$（支援負數）。 |
| [`hypot(x, y)`](#3-hypot) | `decimal` | `[[nodiscard]]` | 斜邊長度 $\sqrt{x^2 + y^2}$（具備防溢位縮放機制）。 |
| [`pow(base, exp)`](#4-pow-整數冪次) | `decimal` | `[[nodiscard]]` | 整數冪次 $\text{base}^{\text{exp}}$（二分快速冪演算法）。 |
| [`pow(base, exp)`](#5-pow-實數冪次) | `decimal` | `[[nodiscard]]` | 實數冪次 $\text{base}^{\text{exp}}$（支援 `decimal` 指數）。 |

---

## 詳細語法與說明 (Detailed Syntax)

### 1. sqrt

```cpp
NUMERIC_NODISCARD inline decimal sqrt(const decimal& x);
```
- **摘要**：計算非負十進位數之平方根。
- **演算法核心**：採用 **牛頓-拉弗森切線法 (Newton-Raphson)**：
  $$y_{n+1} = \frac{1}{2} \left(y_n + \frac{x}{y_n}\right)$$
  具備平方二次收斂速度，運算過程保留 40 位內部精度，最終截斷捨入為 34 位精準數字。
- **特殊值處理**：
  - $x < 0$：傳回 `NaN`。
  - $x = 0$：傳回 `0`。
  - $x = +\infty$：傳回 `+Infinity`。

---

### 2. cbrt

```cpp
NUMERIC_NODISCARD inline decimal cbrt(const decimal& x);
```
- **摘要**：計算實數立方根。
- **特性**：奇函數特性，支援負實數輸入，若 $x < 0$ 則滿足 $\text{cbrt}(x) = -\text{cbrt}(-x)$。

---

### 3. hypot

```cpp
NUMERIC_NODISCARD inline decimal hypot(const decimal& x, const decimal& y);
```
- **摘要**：計算直角三角形斜邊長 $\sqrt{x^2 + y^2}$。
- **備註**：內部使用最大值縮放演算法（Moler-Morrison 風格縮減），有效避免中間平方項 $x^2$ 或 $y^2$ 出現非必要的位數膨脹。

---

### 4. pow (整數冪次)

```cpp
NUMERIC_NODISCARD inline decimal pow(const decimal& base, int exp);
```
- **摘要**：以二分快速冪演算法（Binary Exponentiation）計算整數次方。
- **複雜度**：時間複雜度僅 $O(\log |\text{exp}|)$，計算過程保留 38 位精確除法截斷。

---

### 5. pow (實數冪次)

```cpp
NUMERIC_NODISCARD inline decimal pow(const decimal& base, const decimal& exp);
```
- **摘要**：計算底數與指數皆為 `decimal` 之實數冪。
- **分支最佳化 (Fast-path)**：
  - 若 `exp` 在數值上恰為整數，優先走整數快速冪路徑以確保最高精度。
  - 一般實數路徑：透過關係式 $x^y = \exp(y \ln x)$ 求值。
- **定義域防護**：若 $\text{base} < 0$ 且 $\text{exp}$ 為非整數，結果非實數，傳回 `NaN`。

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <numeric/DecimalMath.hpp>
#include <iostream>

int main() {
    // 1. 平方根與立方根
    numeric::decimal two(2);
    numeric::decimal neg_eight("-8");

    numeric::decimal sqrt2 = numeric::sqrt(two);
    numeric::decimal cbrt_val = numeric::cbrt(neg_eight);

    std::cout << "sqrt(2):  " << sqrt2 << "\n";      // 1.414213562373095048801688724209698
    std::cout << "cbrt(-8): " << cbrt_val << "\n";    // -2

    // 2. 斜邊長 hypot
    numeric::decimal a("3.0");
    numeric::decimal b("4.0");
    std::cout << "hypot(3, 4): " << numeric::hypot(a, b) << "\n"; // 5

    // 3. 乘冪 pow
    numeric::decimal base("1.05");
    std::cout << "1.05^10 (int):  " << numeric::pow(base, 10) << "\n";
    std::cout << "1.05^0.5 (dec): " << numeric::pow(base, numeric::decimal("0.5")) << "\n";

    return 0;
}
```

### 預期輸出 (Output)

```text
sqrt(2):  1.414213562373095048801688724209698
cbrt(-8): -2
hypot(3, 4): 5
1.05^10 (int):  1.6288946267774414
1.05^0.5 (dec): 1.02469507659595983832378888796853
```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上
- **標頭檔**：`<numeric/DecimalMath.hpp>`
