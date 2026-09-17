# decimal 比較運算子 (Comparison Operators)

定義 `numeric::decimal` 之間的相等、關係以及 C++20 三向比較運算子。

- **命名空間**：`numeric`
- **標頭檔**：`<numeric/Decimal.hpp>`

---

## 運算子清單 (Operators List)

| 運算子 | 語法特徵 | 說明 |
| :--- | :--- | :--- |
| `==` | `noexcept` | 相等比較。 |
| `!=` | `noexcept` | 不相等比較。 |
| `<` | `noexcept` | 小於比較。 |
| `<=` | `noexcept` | 小於等於比較。 |
| `>` | `noexcept` | 大於比較。 |
| `>=` | `noexcept` | 大於等於比較。 |
| `<=>` (C++20) | `noexcept` (Spaceship) | 三向比較，傳回 `std::partial_ordering`。 |

---

## 語法宣告 (Syntax)

```cpp
// 相等與不相等
friend NUMERIC_CONSTEXPR_20 bool operator==(const decimal& lhs, const decimal& rhs) noexcept;
friend NUMERIC_CONSTEXPR_20 bool operator!=(const decimal& lhs, const decimal& rhs) noexcept;

// 關係比較
friend NUMERIC_CONSTEXPR_20 bool operator<(const decimal& lhs, const decimal& rhs) noexcept;
friend NUMERIC_CONSTEXPR_20 bool operator<=(const decimal& lhs, const decimal& rhs) noexcept;
friend NUMERIC_CONSTEXPR_20 bool operator>(const decimal& lhs, const decimal& rhs) noexcept;
friend NUMERIC_CONSTEXPR_20 bool operator>=(const decimal& lhs, const decimal& rhs) noexcept;

// C++20 三向比較運算子
#if defined(__cpp_impl_three_way_comparison) && (__cpp_impl_three_way_comparison >= 201907L)
friend NUMERIC_CONSTEXPR_20 std::partial_ordering operator<=>(const decimal& lhs, const decimal& rhs) noexcept;
#endif
```

---

## 參數與傳回值 (Parameters & Return Value)

### 參數
- `lhs`: 左側 `decimal` 運算元。
- `rhs`: 右側 `decimal` 運算元。

### 傳回值
- 雙目二元比較運算子傳回 `bool`（`true` 或 `false`）。
- 三向比較運算子傳回 `std::partial_ordering`：
  - `std::partial_ordering::less`：若 `lhs < rhs`。
  - `std::partial_ordering::greater`：若 `lhs > rhs`。
  - `std::partial_ordering::equivalent`：若兩者數值相等。
  - `std::partial_ordering::unordered`：若任一運算元為 `NaN`。

---

## 備註與注意事項 (Remarks & Caveats)

> [!IMPORTANT]
> **IEEE 754 NaN 比較語意**：
> 本函式庫嚴格遵循 IEEE 754 標準規範：
> 1. `NaN == NaN` 恆為 **`false`**。
> 2. `NaN != NaN` 恆為 **`true`**。
> 3. 包含 `NaN` 的所有大小關係比較（`<`, `<=`, `>`, `>=`）恆為 **`false`**。
> 4. 若需檢查數值是否為非數值，請使用專用方法 [`d.is_nan()`](inspection-methods.md) 或 [`numeric::isnan(d)`](../decimal-math/classification-and-sign.md)。

> [!NOTE]
> **縮放位數無關性 (Scale Invariance)**：
> 比較運算會在內部依據縮放位數進行對齊比較。例如：
> `decimal("1.0") == decimal("1.000")` 評估結果精準為 **`true`**。

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <iostream>

int main() {
    numeric::decimal a("10.50");
    numeric::decimal b("10.5");
    numeric::decimal c("20");

    // 1. 尺度對齊比較
    std::cout << std::boolalpha;
    std::cout << "a == b: " << (a == b) << "\n"; // true
    std::cout << "a < c:  " << (a < c)  << "\n"; // true
    std::cout << "c >= b: " << (c >= b) << "\n"; // true

    // 2. NaN 比較陷阱
    numeric::decimal nan = numeric::decimal::nan;
    std::cout << "nan == nan: " << (nan == nan) << "\n"; // false
    std::cout << "nan != nan: " << (nan != nan) << "\n"; // true
    std::cout << "nan < 10:    " << (nan < 10)    << "\n"; // false

    // 3. C++20 三向比較運算子 (<=>)
#if defined(__cpp_impl_three_way_comparison) && (__cpp_impl_three_way_comparison >= 201907L)
    auto cmp = a <=> c;
    if (cmp == std::partial_ordering::less) {
        std::cout << "a is less than c (Spaceship)\n";
    }
    if ((nan <=> a) == std::partial_ordering::unordered) {
        std::cout << "Comparison with NaN is unordered\n";
    }
#endif

    return 0;
}
```

### 預期輸出 (Output)

```text
a == b: true
a < c:  true
c >= b: true
nan == nan: false
nan != nan: true
nan < 10:    false
a is less than c (Spaceship)
Comparison with NaN is unordered
```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上（所有比較運算子在 C++20+ 均支援 `constexpr`）
- **標頭檔**：`<numeric/Decimal.hpp>`
