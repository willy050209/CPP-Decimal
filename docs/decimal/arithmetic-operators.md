# decimal 算術與取模運算子 (Arithmetic Operators)

提供 `numeric::decimal` 完整的四則運算、取模、一元正負號、遞增遞減運算子以及串流輸出運算子重載。

- **命名空間**：`numeric`
- **標頭檔**：`<numeric/Decimal.hpp>`

---

## 運算子清單 (Operators List)

| 運算子 | 類別 | 語法範例 | 說明 |
| :--- | :--- | :--- | :--- |
| `+`, `-` | 一元 | `+a`, `-a` | 正號與負號運算子。 |
| `++`, `--` | 前置/後置遞增減 | `++a`, `a++`, `--a`, `a--` | 將數值遞增或遞減 1。 |
| `+` | 雙目算術 | `a + b`, `a += b` | 任意精度十進位加法。 |
| `-` | 雙目算術 | `a - b`, `a -= b` | 任意精度十進位減法。 |
| `*` | 雙目算術 | `a * b`, `a *= b` | 任意精度十進位乘法。 |
| `/` | 雙目算術 | `a / b`, `a /= b` | 十進位除法（預設 34 位精度與銀行家捨入）。 |
| `%` | 雙目取模 | `a % b`, `a %= b` | 十進位浮點取模運算（整數商截斷餘數）。 |
| `<<` | 串流輸出 | `os << a` | 將 `decimal` 輸出至 `std::ostream`。 |

---

## 語法與詳細說明 (Syntax & Details)

### 1. 一元正負號運算子

```cpp
NUMERIC_CONSTEXPR_20 decimal operator+() const;
NUMERIC_CONSTEXPR_20 decimal operator-() const;
```
- `+a`：傳回自身複本。
- `-a`：傳回正負號反轉後之結果。若為 `NaN`，反轉後仍為 `NaN`；正無窮大反轉為負無窮大。

---

### 2. 遞增與遞減運算子

```cpp
NUMERIC_CONSTEXPR_20 decimal& operator++();       // 前置遞增: ++d
NUMERIC_CONSTEXPR_20 decimal  operator++(int);    // 後置遞增: d++
NUMERIC_CONSTEXPR_20 decimal& operator--();       // 前置遞減: --d
NUMERIC_CONSTEXPR_20 decimal  operator--(int);    // 後置遞減: d--
```
- 前置運算子回傳修改後的自身參考 `decimal&`。
- 後置運算子傳回修改前的舊值複本 `decimal`。

---

### 3. 加法與減法運算子

```cpp
friend NUMERIC_CONSTEXPR_20 decimal operator+(decimal lhs, const decimal& rhs);
friend NUMERIC_CONSTEXPR_20 decimal operator-(decimal lhs, const decimal& rhs);
NUMERIC_CONSTEXPR_20 decimal& operator+=(const decimal& rhs);
NUMERIC_CONSTEXPR_20 decimal& operator-=(const decimal& rhs);
```
- **精度保證**：加法與減法為完全精確運算（Exact Calculation），內部自動將兩數對齊至相同的 scale 後執行整數加減，結果不產生任何精度損失。

---

### 4. 乘法運算子

```cpp
friend NUMERIC_CONSTEXPR_20 decimal operator*(decimal lhs, const decimal& rhs);
NUMERIC_CONSTEXPR_20 decimal& operator*=(const decimal& rhs);
```
- **精度保證**：乘法為完全精確運算。新值的未縮放整數為兩數相乘，小數位數為 $\text{scale}_1 + \text{scale}_2$。

---

### 5. 除法運算子

```cpp
friend NUMERIC_CONSTEXPR_20 decimal operator/(const decimal& lhs, const decimal& rhs);
NUMERIC_CONSTEXPR_20 decimal& operator/=(const decimal& rhs);
```
- **預設規格**：除法運算子 `/` 等同於呼叫 `lhs.divide(rhs, 34)`，即以 **34 位有效十進位數字** 進行計算，並使用 **銀行家捨入法 (Round Half-to-Even)** 決定最後一位數值。
- 若需要更高或自訂精度，請使用成員函式 [`decimal::divide()`](math-methods.md)。

---

### 6. 取模運算子

```cpp
friend NUMERIC_CONSTEXPR_20 decimal operator%(decimal lhs, const decimal& rhs);
NUMERIC_CONSTEXPR_20 decimal& operator%=(const decimal& rhs);
```
- 計算除法之浮點餘數：$r = a - \text{trunc}(a / b) \times b$。餘數正負號與被除數 $a$ 一致。

---

### 7. 串流輸出運算子

```cpp
friend std::ostream& operator<<(std::ostream& os, const decimal& val);
```
- 等同於 `os << val.to_string()`。

---

## 備註與邊界行為 (Remarks & Boundary Behaviors)

> [!NOTE]
> **IEEE 754 特殊值傳遞與除零行為**：
> - **非零除以 0**：`1.0 / 0.0` 傳回 `+Infinity`；`-1.0 / 0.0` 傳回 `-Infinity`。
> - **零除以 0**：`0.0 / 0.0` 傳回 `NaN`。
> - **NaN 傳播**：任何包含 `NaN` 的算術運算（如 `NaN + 1`、`NaN * 0`），其結果恆為 `NaN`。
> - **無窮大加減**：`Infinity + Infinity == Infinity`；`Infinity - Infinity == NaN`。

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <iostream>

int main() {
    numeric::decimal a("10.5");
    numeric::decimal b("3.0");

    // 四則運算
    std::cout << "a + b = " << (a + b) << "\n"; // 13.5
    std::cout << "a - b = " << (a - b) << "\n"; // 7.5
    std::cout << "a * b = " << (a * b) << "\n"; // 31.5
    std::cout << "a / b = " << (a / b) << "\n"; // 3.5
    std::cout << "a % b = " << (a % b) << "\n"; // 1.5

    // 遞增與遞減
    numeric::decimal c("5");
    std::cout << "++c: " << ++c << "\n"; // 6
    std::cout << "c++: " << c++ << "\n"; // 6 (印出舊值)
    std::cout << "after c++: " << c << "\n"; // 7

    // 除零與特殊值
    numeric::decimal zero(0);
    std::cout << "1 / 0 = " << (numeric::decimal(1) / zero) << "\n";  // inf
    std::cout << "0 / 0 = " << (zero / zero) << "\n";                // nan

    return 0;
}
```

### 預期輸出 (Output)

```text
a + b = 13.5
a - b = 7.5
a * b = 31.5
a / b = 3.5
a % b = 1.5
++c: 6
c++: 6
after c++: 7
1 / 0 = inf
0 / 0 = nan
```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上（所有運算子在 C++20+ 全面支援 `constexpr` 求值）
- **標頭檔**：`<numeric/Decimal.hpp>`
