# decimal 狀態查詢與屬性方法 (Inspection Methods)

提供查詢 `numeric::decimal` 執行個體之數值屬性（符號、小數位數、未縮放整數）以及特殊狀態（NaN、無窮大、零、SBO 記憶體狀態）之成員方法。

- **命名空間**：`numeric`
- **標頭檔**：`<numeric/Decimal.hpp>`

---

## 方法清單 (Method List)

| 方法 | 傳回型別 | 屬性標記 | 說明 |
| :--- | :--- | :--- | :--- |
| [`is_sbo()`](#1-is_sbo--is_small) / [`is_small()`](#1-is_sbo--is_small) | `bool` | `[[nodiscard]] noexcept` | 查詢是否正使用 128-bit SBO 本機緩衝區（無 Heap 配置）。 |
| [`is_nan()`](#2-is_nan) | `bool` | `[[nodiscard]] noexcept` | 查詢數值是否為非數值 (NaN)。 |
| [`is_infinite()`](#3-is_infinite--is_infinity) / [`is_infinity()`](#3-is_infinite--is_infinity) | `bool` | `[[nodiscard]] noexcept` | 查詢數值是否為無窮大（包含正負無窮大）。 |
| [`is_finite()`](#4-is_finite) | `bool` | `[[nodiscard]] noexcept` | 查詢數值是否為有限值（既非 NaN 亦非無窮大）。 |
| [`is_zero()`](#5-is_zero) | `bool` | `[[nodiscard]] noexcept` | 查詢數值是否精確為 0。 |
| [`sign()`](#6-sign) | `int8_t` | `[[nodiscard]] noexcept` | 取得數值符號（正數傳回 1，負數傳回 -1，零或 NaN 傳回 0）。 |
| [`scale()`](#7-scale) | `int64_t` | `[[nodiscard]] noexcept` | 取得十進位小數位數（即負指數階）。 |
| [`unscaled()`](#8-unscaled) | `const bigint&` | `[[nodiscard]] noexcept` | 取得底層未縮放整數常數參考。 |

---

## 詳細說明 (Detailed Descriptions)

### 1. is_sbo() / is_small()

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 bool is_sbo() const noexcept;
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 bool is_small() const noexcept;
```
- **說明**：查詢底層未縮放整數是否儲存於內部 128 位元緩衝區（Small Buffer Optimization）。
- **傳回值**：若未觸發動態堆積配置，傳回 `true`；若數值位數過大已配置堆積記憶體，傳回 `false`。

---

### 2. is_nan()

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 bool is_nan() const noexcept;
```
- **說明**：判斷目前數值是否為 IEEE 754 非數值 (NaN)。

---

### 3. is_infinite() / is_infinity()

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 bool is_infinite() const noexcept;
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 bool is_infinity() const noexcept;
```
- **說明**：判斷目前數值是否為正無窮大 (`+Infinity`) 或負無窮大 (`-Infinity`)。兩函式互為別名。

---

### 4. is_finite()

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 bool is_finite() const noexcept;
```
- **說明**：判斷目前數值是否為一般的有效實數。等價於 `!is_nan() && !is_infinite()`。

---

### 5. is_zero()

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 bool is_zero() const noexcept;
```
- **說明**：判斷目前數值是否為 0。當且僅當數值非 NaN、非無窮大且未縮放整數為 0 時回傳 `true`。

---

### 6. sign()

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 int8_t sign() const noexcept;
```
- **說明**：取得數值之符號方向。
- **傳回值**：
  - 正數或正無窮大：傳回 `1`。
  - 負數或負無窮大：傳回 `-1`。
  - 數值為 0 或數值為 NaN：傳回 `0`。

---

### 7. scale()

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 int64_t scale() const noexcept;
```
- **說明**：傳回十進位小數位數。數值幾何意義為 $\text{unscaled} \times 10^{-\text{scale}}$。例如數值 `3.1415` 之 `scale()` 傳回 `4`。

---

### 8. unscaled()

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 const bigint& unscaled() const noexcept;
```
- **說明**：傳回底層未縮放整數之常數參考。可用於直接檢查整數內部結構、位元長度或進行底層最佳化。

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <iostream>

int main() {
    numeric::decimal val("-123.450");
    numeric::decimal inf = numeric::decimal::infinity;
    numeric::decimal nan = numeric::decimal::nan;

    std::cout << std::boolalpha;

    // 1. 數值屬性查詢
    std::cout << "Value:    " << val << "\n";
    std::cout << "Sign:     " << static_cast<int>(val.sign()) << "\n";     // -1
    std::cout << "Scale:    " << val.scale() << "\n";                     // 2 (經 normalize 去除尾隨 0)
    std::cout << "Unscaled: " << val.unscaled() << "\n";                  // -12345
    std::cout << "Is SBO:   " << val.is_sbo() << "\n";                    // true (未超出 128 位元)

    // 2. 特殊值查詢
    std::cout << "inf.is_infinite(): " << inf.is_infinite() << "\n";     // true
    std::cout << "inf.is_finite():   " << inf.is_finite() << "\n";       // false
    std::cout << "nan.is_nan():      " << nan.is_nan() << "\n";          // true
    std::cout << "nan.sign():        " << static_cast<int>(nan.sign()) << "\n"; // 0

    return 0;
}
```

### 預期輸出 (Output)

```text
Value:    -123.45
Sign:     -1
Scale:    2
Unscaled: -12345
Is SBO:   true
inf.is_infinite(): true
inf.is_finite():   false
nan.is_nan():      true
nan.sign():        0
```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上（所有查詢函式在 C++20+ 全面支援 `constexpr`）
- **標頭檔**：`<numeric/Decimal.hpp>`
