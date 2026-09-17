# 捨入、取模與餘數函式 (Rounding and Modulo Functions)

提供對齊 C++ 標準庫 `<cmath>` 之數值捨入（向下取整、向上取整、截斷、四捨五入）以及浮點除法取模與 IEEE 餘數函式。

- **命名空間**：`numeric` 與 `std`
- **標頭檔**：`<numeric/DecimalMath.hpp>`

---

## 函式清單 (Function List)

| 函式 | 傳回型別 | 屬性標記 | 說明 |
| :--- | :--- | :--- | :--- |
| [`floor(x)`](#1-floor) | `decimal` | `[[nodiscard]] constexpr` | 向下取整：不大於 $x$ 的最大整數。 |
| [`ceil(x)`](#2-ceil) | `decimal` | `[[nodiscard]] constexpr` | 向上取整：不小於 $x$ 的最小整數。 |
| [`trunc(x)`](#3-trunc) | `decimal` | `[[nodiscard]] constexpr` | 朝零捨入：直接截斷小數部分。 |
| [`round(x)`](#4-round) | `decimal` | `[[nodiscard]] constexpr` | 四捨五入（Half away from zero，符合標準 `<cmath>` 規範）。 |
| [`fmod(x, y)`](#5-fmod) | `decimal` | `[[nodiscard]]` | 浮點除法取模餘數：$x - \text{trunc}(x/y) \times y$。 |
| [`remainder(x, y)`](#6-remainder) | `decimal` | `[[nodiscard]]` | IEEE 754 餘數：$x - \text{round}(x/y) \times y$。 |

---

## 詳細語法與說明 (Detailed Syntax)

### 1. floor

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 inline decimal floor(const decimal& x);
```
- **摘要**：計算 $\lfloor x \rfloor$。
- **數值範例**：`floor(2.8) == 2`、`floor(-2.3) == -3`。

---

### 2. ceil

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 inline decimal ceil(const decimal& x);
```
- **摘要**：計算 $\lceil x \rceil$。
- **數值範例**：`ceil(2.3) == 3`、`ceil(-2.8) == -2`。

---

### 3. trunc

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 inline decimal trunc(const decimal& x);
```
- **摘要**：向零捨入（朝向 0 截斷小數）。
- **數值範例**：`trunc(2.8) == 2`、`trunc(-2.8) == -2`。

---

### 4. round

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 inline decimal round(const decimal& x);
```
- **摘要**：四捨五入至最接近的整數。當數值處於剛好一半（如 $.5$）時，遠離零捨入（Half away from zero）。

> [!IMPORTANT]
> **關鍵語意區分：`numeric::round(d)` vs `d.round()`**：
> - **`numeric::round(d)`**（自由函式）：依循 C++ 標準庫 `<cmath>` 的傳統四捨五入定義（Half away from zero），例如 `round(2.5) == 3`, `round(-2.5) == -3`。
> - **`d.round(places)`**（成員函式）：依循 IEEE 754-2008 與金融標準的 **銀行家捨入法 (Round Half-to-Even)**，例如 `decimal("2.5").round(0) == 2`（捨入至偶數）。

---

### 5. fmod

```cpp
NUMERIC_NODISCARD inline decimal fmod(const decimal& x, const decimal& y);
```
- **摘要**：計算浮點除法餘數 $r = x - q \times y$，其中 $q = \text{trunc}(x / y)$。
- **傳回值**：餘數 $r$ 的正負號恆與被除數 $x$ 相同，且 $|r| < |y|$。
- **特殊值**：若 $x$ 為無窮大或 $y$ 為 0，傳回 `NaN`。

---

### 6. remainder

```cpp
NUMERIC_NODISCARD inline decimal remainder(const decimal& x, const decimal& y);
```
- **摘要**：計算符合 IEEE 754 規範的浮點餘數 $r = x - n \times y$，其中 $n$ 為最接近 $x / y$ 的整數（商採銀行家捨入）。
- **傳回值**：餘數滿足 $|r| \le |y| / 2$。

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <numeric/DecimalMath.hpp>
#include <iostream>

int main() {
    numeric::decimal val_pos("2.5");
    numeric::decimal val_neg("-2.5");

    // 1. 各種捨入方法對比
    std::cout << "--- 捨入比較 (2.5 與 -2.5) ---\n";
    std::cout << "floor(2.5):       " << numeric::floor(val_pos) << "\n"; // 2
    std::cout << "floor(-2.5):      " << numeric::floor(val_neg) << "\n"; // -3
    std::cout << "ceil(2.5):        " << numeric::ceil(val_pos) << "\n";  // 3
    std::cout << "ceil(-2.5):       " << numeric::ceil(val_neg) << "\n";  // -2
    std::cout << "trunc(2.5):       " << numeric::trunc(val_pos) << "\n"; // 2
    std::cout << "trunc(-2.5):      " << numeric::trunc(val_neg) << "\n"; // -2

    // 2. 四捨五入 vs 銀行家捨入
    std::cout << "\n--- round(2.5) 語意差異 ---\n";
    std::cout << "numeric::round(2.5): " << numeric::round(val_pos) << " (Half-away-from-zero: 3)\n";
    std::cout << "val_pos.round(0):    " << val_pos.round(0) << " (Banker's Half-to-even: 2)\n";

    // 3. fmod 與 remainder 取模
    numeric::decimal x("5.3");
    numeric::decimal y("2.0");
    std::cout << "\n--- fmod 與 remainder (5.3 / 2.0) ---\n";
    std::cout << "fmod(5.3, 2.0):      " << numeric::fmod(x, y) << "\n";      // 1.3
    std::cout << "remainder(5.3, 2.0): " << numeric::remainder(x, y) << "\n"; // -0.7

    return 0;
}
```

### 預期輸出 (Output)

```text
--- 捨入比較 (2.5 與 -2.5) ---
floor(2.5):       2
floor(-2.5):      -3
ceil(2.5):        3
ceil(-2.5):       -2
trunc(2.5):       2
trunc(-2.5):      -2

--- round(2.5) 語意差異 ---
numeric::round(2.5): 3 (Half-away-from-zero: 3)
val_pos.round(0):    2 (Banker's Half-to-even: 2)

--- fmod 與 remainder (5.3 / 2.0) ---
fmod(5.3, 2.0):      1.3
remainder(5.3, 2.0): -0.7
```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上（`floor`, `ceil`, `trunc`, `round` 在 C++20+ 支援 `constexpr`）
- **標頭檔**：`<numeric/DecimalMath.hpp>`
