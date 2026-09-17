# 數值分類與符號函式 (Classification and Sign Functions)

提供針對 `numeric::decimal` 數值分類檢查（NaN、無窮大、有限值）以及符號處理（符號位元檢查、符號複製、絕對值）之數學函式。

- **命名空間**：`numeric` 與 `std`
- **標頭檔**：`<numeric/DecimalMath.hpp>`

---

## 函式清單 (Function List)

| 函式 | 傳回型別 | 屬性標記 | 說明 |
| :--- | :--- | :--- | :--- |
| [`isnan(x)`](#1-isnan) | `bool` | `[[nodiscard]] constexpr noexcept` | 判斷是否為非數值 (NaN)。 |
| [`isinf(x)`](#2-isinf) | `bool` | `[[nodiscard]] constexpr noexcept` | 判斷是否為無窮大 (+/- Infinity)。 |
| [`isfinite(x)`](#3-isfinite) | `bool` | `[[nodiscard]] constexpr noexcept` | 判斷是否為有限值。 |
| [`signbit(x)`](#4-signbit) | `bool` | `[[nodiscard]] constexpr noexcept` | 檢查符號位元是否為負。 |
| [`copysign(mag, sgn)`](#5-copysign) | `decimal` | `[[nodiscard]] constexpr noexcept` | 結合 `mag` 之絕對值與 `sgn` 之符號。 |
| [`abs(x)`](#6-abs) | `decimal` | `[[nodiscard]] constexpr noexcept` | 計算數值之絕對值。 |

---

## 詳細語法與說明 (Detailed Syntax)

### 1. isnan

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 inline bool isnan(const decimal& x) noexcept;
```
- **參數**：`x` - 待檢查之 `decimal` 數值。
- **傳回值**：若 `x` 為 `NaN` 則傳回 `true`，否則傳回 `false`。
- **備註**：由於 IEEE 754 規範下 `NaN == NaN` 恆為 `false`，因此不能使用 `==` 判斷非數值，必須呼叫 `isnan(x)`。

---

### 2. isinf

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 inline bool isinf(const decimal& x) noexcept;
```
- **參數**：`x` - 待檢查之 `decimal` 數值。
- **傳回值**：若 `x` 為 `+Infinity` 或 `-Infinity` 傳回 `true`，否則傳回 `false`。

---

### 3. isfinite

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 inline bool isfinite(const decimal& x) noexcept;
```
- **參數**：`x` - 待檢查之 `decimal` 數值。
- **傳回值**：若 `x` 為正常的有限實數（非 NaN 且非 Infinity）傳回 `true`，否則傳回 `false`。

---

### 4. signbit

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 inline bool signbit(const decimal& x) noexcept;
```
- **參數**：`x` - 待檢查之 `decimal` 數值。
- **傳回值**：若符號為負數（包含負無窮大或未縮放整數為負）傳回 `true`，正數或零傳回 `false`。

---

### 5. copysign

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 inline decimal copysign(
    const decimal& mag, 
    const decimal& sgn
) noexcept;
```
- **摘要**：產生一個大小等於 `mag` 且符號等於 `sgn` 的數值。
- **參數**：
  - `mag`: 提供大小（絕對值）之來源數值。
  - `sgn`: 提供符號之來源數值。
- **傳回值**：符號調整後之 `decimal`。若 `mag` 為 `NaN`，原樣傳回 `mag`。

---

### 6. abs

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 inline decimal abs(const decimal& x) noexcept;
```
- **摘要**：計算 `|x|`。
- **參數**：`x` - 輸入數值。
- **傳回值**：
  - 若 `x` 為負數，傳回 `-x`。
  - 若 `x` 為正數或零，傳回 `x`。
  - 若 `x` 為 `NaN`，傳回 `NaN`。
  - 若 `x` 為 `-Infinity`，傳回 `+Infinity`。

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <numeric/DecimalMath.hpp>
#include <iostream>

int main() {
    numeric::decimal pos("12.34");
    numeric::decimal neg("-56.78");
    numeric::decimal inf = numeric::decimal::infinity;
    numeric::decimal nan = numeric::decimal::nan;

    std::cout << std::boolalpha;

    // 1. 數值分類
    std::cout << "isnan(nan):      " << numeric::isnan(nan) << "\n";     // true
    std::cout << "isinf(inf):      " << numeric::isinf(inf) << "\n";     // true
    std::cout << "isfinite(pos):   " << numeric::isfinite(pos) << "\n";  // true
    std::cout << "isfinite(inf):   " << numeric::isfinite(inf) << "\n";  // false

    // 2. 符號位元檢查
    std::cout << "signbit(neg):    " << numeric::signbit(neg) << "\n";   // true
    std::cout << "signbit(pos):    " << numeric::signbit(pos) << "\n";   // false

    // 3. copysign 與 abs
    std::cout << "abs(neg):        " << numeric::abs(neg) << "\n";       // 56.78
    numeric::decimal signed_val = numeric::copysign(pos, neg);
    std::cout << "copysign(pos, neg): " << signed_val << "\n";           // -12.34

    return 0;
}
```

### 預期輸出 (Output)

```text
isnan(nan):      true
isinf(inf):      true
isfinite(pos):   true
isfinite(inf):   false
signbit(neg):    true
signbit(pos):    false
abs(neg):        56.78
copysign(pos, neg): -12.34
```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上（所有分類函式在 C++20+ 全面支援 `constexpr`）
- **標頭檔**：`<numeric/DecimalMath.hpp>`
