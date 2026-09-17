# 三角函數 (Trigonometric Functions)

提供高精度正弦、餘弦與正切三角函數計算。

- **命名空間**：`numeric` 與 `std`
- **標頭檔**：`<numeric/DecimalMath.hpp>`

---

## 函式清單 (Function List)

| 函式 | 傳回型別 | 屬性標記 | 說明 |
| :--- | :--- | :--- | :--- |
| [`sin(x)`](#1-sin) | `decimal` | `[[nodiscard]]` | 正弦函數 $\sin x$（弧度制）。 |
| [`cos(x)`](#2-cos) | `decimal` | `[[nodiscard]]` | 餘弦函數 $\cos x$（弧度制）。 |
| [`tan(x)`](#3-tan) | `decimal` | `[[nodiscard]]` | 正切函數 $\tan x = \sin x / \cos x$。 |

---

## 詳細語法與說明 (Detailed Syntax)

### 1. sin 與 cos

```cpp
NUMERIC_NODISCARD inline decimal sin(const decimal& x);
NUMERIC_NODISCARD inline decimal cos(const decimal& x);
```
- **參數**：`x` - 弧度角（Radian）。
- **演算法核心**：
  1. **週期性範圍縮減**：先模 $2\pi$ 縮減至小角度區間。
  2. **象限縮減 (Quadrant Reduction)**：再依 $\pi / 2$ 劃分象限，將計算核心壓縮至 $[-\pi/4, \pi/4]$ 極窄區間。
  3. **高階泰勒展開 (High-order Taylor Series)**：在核心區間內展開多達 25 階級數項，中間過程維持 42 位計算精度。
- **特殊值處理**：若 $x$ 為 `NaN` 或 `Infinity`，傳回 `NaN`（無窮大角度之正餘弦無定義）。

---

### 2. tan

```cpp
NUMERIC_NODISCARD inline decimal tan(const decimal& x);
```
- **摘要**：計算正切值 $\tan x = \sin x / \cos x$。
- **奇異點處理**：當 $\cos x = 0$（例如 $x = \pi/2 + k\pi$）時，傳回同號之 `+Infinity` 或 `-Infinity`。

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <numeric/DecimalMath.hpp>
#include <iostream>

int main() {
    numeric::decimal pi = numeric::detail::cmath_pi();
    numeric::decimal pi_over_2 = numeric::detail::cmath_pi_over_2();
    numeric::decimal pi_over_6 = pi / 6;

    // 1. 特殊角計算
    std::cout << "sin(0):      " << numeric::sin(numeric::decimal(0)) << "\n"; // 0
    std::cout << "cos(0):      " << numeric::cos(numeric::decimal(0)) << "\n"; // 1
    std::cout << "sin(pi/6):   " << numeric::sin(pi_over_6) << "\n";           // 0.5
    std::cout << "sin(pi/2):   " << numeric::sin(pi_over_2) << "\n";           // 1.0
    std::cout << "cos(pi/2):   " << numeric::cos(pi_over_2) << "\n";           // 0 (在 34 位精準為 0)

    // 2. 正切 tan
    std::cout << "tan(pi/4):   " << numeric::tan(pi / 4) << "\n";              // 1.0

    return 0;
}
```

### 預期輸出 (Output)

```text
sin(0):      0
cos(0):      1
sin(pi/6):   0.5
sin(pi/2):   1
cos(pi/2):   0
tan(pi/4):   1
```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上
- **標頭檔**：`<numeric/DecimalMath.hpp>`
