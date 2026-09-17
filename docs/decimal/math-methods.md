# decimal 數學成員方法 (Math Member Methods)

說明 `numeric::decimal` 類別內建之高精度除法運算與銀行家捨入方法。

- **命名空間**：`numeric`
- **標頭檔**：`<numeric/Decimal.hpp>`

---

## 方法清單 (Method List)

| 方法 | 語法特徵 | 說明 |
| :--- | :--- | :--- |
| [`divide(other, precision)`](#1-divide-指定精度除法) | `[[nodiscard]] constexpr` | 依指定十進位有效數字精度執行除法，並採銀行家捨入。 |
| [`round(decimal_places)`](#2-round-銀行家捨入) | `[[nodiscard]] constexpr` | 依指定小數位數執行銀行家捨入 (Round Half-to-Even)。 |

---

## 1. divide 指定精度除法

### 語法 (Syntax)

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 decimal divide(
    const decimal& other, 
    int32_t precision = 34
) const;
```

### 摘要
計算當前實例除以 `other` 的商，並保證結果至少具備 `precision` 位十進位有效數字，尾數依銀行家捨入法捨入。

### 參數 (Parameters)
- `other`: `const decimal&` - 除數。
- `precision`: `int32_t` - 計算之有效十進位位數（預設為 **34 位**，相容於 IEEE 754-2008 decimal128 標準）。

### 傳回值 (Return Value)
`decimal`：計算完成之商。

### 備註與注意事項 (Remarks & Caveats)

> [!NOTE]
> **無限循環小數防護**：
> 十進位中除以非 2 與 5 的質因數（如 $1 / 3 = 0.333333...$）會產生無限循環小數。透過指定 `precision` 參數，可強制演算法在計算至目標有效位數後執行捨入，防止無限配置與記憶體溢位。

> [!IMPORTANT]
> **特殊值除法規則**：
> - 任何非零數值除以 0：傳回 `+Infinity` 或 `-Infinity`（依符號相乘規則）。
> - 0 除以 0：傳回 `NaN`。
> - 任一運算元為 `NaN`：傳回 `NaN`。
> - 有限數值除以 `Infinity`：傳回 `0`。
> - `Infinity` 除以有限數值：傳回同號之 `Infinity`。
> - `Infinity` 除以 `Infinity`：傳回 `NaN`。

---

## 2. round 銀行家捨入

### 語法 (Syntax)

```cpp
NUMERIC_NODISCARD NUMERIC_CONSTEXPR_20 decimal round(
    int64_t decimal_places = 0
) const;
```

### 摘要
將當前數值依據 **銀行家捨入法 (Banker's Rounding / Round Half-to-Even)** 捨入至指定的小數點後位數。

### 參數 (Parameters)
- `decimal_places`: `int64_t` - 目標小數位數。
  - 預設值為 `0`，代表捨入至整數位。
  - 大於 0 代表捨入至小數點後指定位數（例如 `2` 代表捨入至百分位）。
  - 若傳入負數，則向整數位（十位、百位）捨入。

### 傳回值 (Return Value)
`decimal`：捨入後之數值。若原數值為 `NaN` 或 `Infinity`，則原樣傳回。

### 銀行家捨入原理 (Round Half-to-Even Rules)

> [!TIP]
> **銀行家捨入法特性**：
> 當被捨去的位數小於半數（$< 0.5$）時直接捨去；大於半數（$> 0.5$）時進位。
> 當剛好等於半數（$= 0.5$）時，**捨入至最接近的偶數 (Even)**：
> - `2.5` 捨入至整數為 **`2`**（2 為偶數）。
> - `3.5` 捨入至整數為 **`4`**（4 為偶數）。
> - `0.125` 捨入至 2 位小數為 **`0.12`**（2 為偶數）。
> - `0.135` 捨入至 2 位小數為 **`0.14`**（4 為偶數）。
> 
> 與一般數學上的「四捨五入 (Round half away from zero)」相比，銀行家捨入法在大規模統計、會計與金融結算時能完全抵消統計偏向。

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <iostream>

int main() {
    // 1. divide 自訂精度除法
    numeric::decimal one(1);
    numeric::decimal three(3);

    // 預設 34 位精度
    std::cout << "1/3 (34 digits): " << one.divide(three) << "\n";
    // 限制為 6 位精度
    std::cout << "1/3 (6 digits):  " << one.divide(three, 6) << "\n";
    // 限制為 10 位精度
    std::cout << "1/3 (10 digits): " << one.divide(three, 10) << "\n";

    // 2. 銀行家捨入示範
    std::cout << "\n--- Banker's Rounding (Round Half-to-Even) ---\n";
    numeric::decimal d1("2.5");
    numeric::decimal d2("3.5");
    std::cout << "round(2.5): " << d1.round(0) << " (rounds to even 2)\n";
    std::cout << "round(3.5): " << d2.round(0) << " (rounds to even 4)\n";

    numeric::decimal c1("0.125");
    numeric::decimal c2("0.135");
    std::cout << "round(0.125, 2): " << c1.round(2) << " (rounds to even 0.12)\n";
    std::cout << "round(0.135, 2): " << c2.round(2) << " (rounds to even 0.14)\n";

    return 0;
}
```

### 預期輸出 (Output)

```text
1/3 (34 digits): 0.3333333333333333333333333333333333
1/3 (6 digits):  0.333333
1/3 (10 digits): 0.3333333333

--- Banker's Rounding (Round Half-to-Even) ---
round(2.5): 2 (rounds to even 2)
round(3.5): 4 (rounds to even 4)
round(0.125, 2): 0.12 (rounds to even 0.12)
round(0.135, 2): 0.14 (rounds to even 0.14)
```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上（在 C++20+ 全面支援 `constexpr`）
- **標頭檔**：`<numeric/Decimal.hpp>`
