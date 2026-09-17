# 指數與對數函式 (Exponential and Logarithmic Functions)

提供高精度自然指數、自然對數、常用對數（以 10 為底）以及二進位對數（以 2 為底）函式。

- **命名空間**：`numeric` 與 `std`
- **標頭檔**：`<numeric/DecimalMath.hpp>`

---

## 函式清單 (Function List)

| 函式 | 傳回型別 | 屬性標記 | 說明 |
| :--- | :--- | :--- | :--- |
| [`exp(x)`](#1-exp) | `decimal` | `[[nodiscard]]` | 自然指數 $e^x$。 |
| [`log(x)`](#2-log) | `decimal` | `[[nodiscard]]` | 自然對數 $\ln x$ ($x > 0$)。 |
| [`log10(x)`](#3-log10) | `decimal` | `[[nodiscard]]` | 常用對數 $\log_{10} x$ ($x > 0$)。 |
| [`log2(x)`](#4-log2) | `decimal` | `[[nodiscard]]` | 二進位對數 $\log_2 x$ ($x > 0$)。 |

---

## 詳細語法與說明 (Detailed Syntax)

### 1. exp

```cpp
NUMERIC_NODISCARD inline decimal exp(const decimal& x);
```
- **摘要**：計算自然底數指數函式 $e^x$。
- **演算法核心**：
  1. **範圍縮減 (Range Reduction)**：$x = k \ln 2 + r$，將計算約束至小區間 $r$。
  2. **縮放平方 (Scaling and Squaring)**：將 $r' = r / 128$，並利用泰勒級數高階展開求得 $\exp(r')$。
  3. 平方 7 次還原並乘上 $2^k$，達成全域 34 位精準收斂。
- **邊界保護**：
  - $x > 10000$ 時直接傳回 `+Infinity`（避免不必要的巨額計算）。
  - $x < -10000$ 時直接傳回 `0`。

---

### 2. log

```cpp
NUMERIC_NODISCARD inline decimal log(const decimal& x);
```
- **摘要**：計算自然對數 $\ln x$。
- **演算法核心**：
  1. 先依 10 的次冪初步範圍縮減，再依 2 的次冪縮減至區間 $[1/\sqrt{2}, \sqrt{2}]$。
  2. 採用超快速收斂級數展開：
     $$\ln m = 2 u \sum_{j=0}^{\infty} \frac{u^{2j}}{2j+1}, \quad u = \frac{m-1}{m+1}$$
  3. 重組計算 $\ln x = p_{10} \ln 10 + p_2 \ln 2 + \ln m$。
- **定義域防護**：
  - $x < 0$：傳回 `NaN`。
  - $x = 0$：傳回 `-Infinity`。
  - $x = 1$：傳回 `0`。

---

### 3. log10

```cpp
NUMERIC_NODISCARD inline decimal log10(const decimal& x);
```
- **摘要**：計算常用對數 $\log_{10} x$。
- **整數倍率優化**：針對 $10^k$（例如 `10`, `100`, `0.01` 等）具備精確整數提取路徑，保證 $\log_{10}(1000)$ 精確等於 `3` 而無任何小數尾差。

---

### 4. log2

```cpp
NUMERIC_NODISCARD inline decimal log2(const decimal& x);
```
- **摘要**：計算以 2 為底之對數 $\log_2 x$。
- **二的次冪優化**：針對 $2^k$（如 `2`, `4`, `8`, `0.5` 等）具備快速整數還原路徑，保證 $\log_2(8) == 3$ 精準成立。

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <numeric/DecimalMath.hpp>
#include <iostream>

int main() {
    // 1. 指數計算
    numeric::decimal one(1);
    numeric::decimal e = numeric::exp(one);
    std::cout << "e: " << e << "\n"; // 2.718281828459045235360287471352662

    // 2. 對數計算
    numeric::decimal ten(10);
    numeric::decimal thousand(1000);
    numeric::decimal eight(8);

    std::cout << "ln(e):        " << numeric::log(e) << "\n";        // 1
    std::cout << "log10(1000):  " << numeric::log10(thousand) << "\n"; // 3 (整數精準路徑)
    std::cout << "log2(8):      " << numeric::log2(eight) << "\n";     // 3 (整數精準路徑)

    // 3. 特殊定義域檢查
    numeric::decimal zero(0);
    numeric::decimal neg("-5");
    std::cout << "log(0):       " << numeric::log(zero) << "\n"; // -inf
    std::cout << "log(-5):      " << numeric::log(neg) << "\n";  // nan

    return 0;
}
```

### 預期輸出 (Output)

```text
e: 2.718281828459045235360287471352662
ln(e):        1
log10(1000):  3
log2(8):      3
log(0):       -inf
log(-5):      nan
```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上
- **標頭檔**：`<numeric/DecimalMath.hpp>`
