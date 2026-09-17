# decimal 邏輯運算子 (Logical Operators)

定義 `numeric::decimal` 的邏輯運算子，包含同型別邏輯及/或，以及與 C++ 原生整數/布林型別之泛型混合運算子樣板。

- **命名空間**：`numeric`
- **標頭檔**：`<numeric/Decimal.hpp>`

---

## 運算子清單 (Operators List)

| 運算子 | 類別 | 語法範例 | 說明 |
| :--- | :--- | :--- | :--- |
| `!` | 一元 | `!d` | 邏輯非（若為 0 或 NaN 回傳 true）。 |
| `&&` | 雙目 (同型別) | `a && b` | 兩個 `decimal` 數值之邏輯及。 |
| `\|\|` | 雙目 (同型別) | `a \|\| b` | 兩個 `decimal` 數值之邏輯或。 |
| `&&` | 雙目 (泛型混合) | `d && x` 或 `x && d` | `decimal` 與原生整數/布林之邏輯及。 |
| `\|\|` | 雙目 (泛型混合) | `d \|\| x` 或 `x \|\| d` | `decimal` 與原生整數/布林之邏輯或。 |

---

## 語法宣告 (Syntax)

### 1. 同型別邏輯運算子

```cpp
NUMERIC_CONSTEXPR_20 bool operator!() const noexcept;
friend NUMERIC_CONSTEXPR_20 bool operator&&(const decimal& lhs, const decimal& rhs) noexcept;
friend NUMERIC_CONSTEXPR_20 bool operator||(const decimal& lhs, const decimal& rhs) noexcept;
```

---

### 2. 泛型混合邏輯運算子樣板 (Generic Overloads)

```cpp
// 左側為 decimal，右側為原生整數/布林
template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
NUMERIC_CONSTEXPR_20 inline bool operator&&(const decimal& lhs, T rhs) noexcept;

template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
NUMERIC_CONSTEXPR_20 inline bool operator||(const decimal& lhs, T rhs) noexcept;

// 左側為原生整數/布林，右側為 decimal
template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
NUMERIC_CONSTEXPR_20 inline bool operator&&(T lhs, const decimal& rhs) noexcept;

template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
NUMERIC_CONSTEXPR_20 inline bool operator||(T lhs, const decimal& rhs) noexcept;
```

---

## 型別參數說明 (Type Parameters)

- `T`: 原生整數型別（包含 `bool`, `char`, `int`, `uint32_t`, `int64_t` 等）。透過 SFINAE `std::is_integral<T>` 約束確保不會誤捕捉非整數型別。

---

## 備註與注意事項 (Remarks & Caveats)

> [!WARNING]
> **短路求值 (Short-Circuit Evaluation) 限制**：
> 在 C++ 語言規範中，使用者自訂重載的 `operator&&` 與 `operator||` **無法** 保留原生內建運算子的短路求值特性。
> 例如在 `expr1 && expr2` 中，不論 `expr1` 的真偽值為何，兩邊的表達式均會**無條件預先計算**後才傳入運算子函式。
> 若程式邏輯高度依賴短路特性（例如避免空指標解參考），建議明確轉為布林值運算：
> ```cpp
> if (static_cast<bool>(d) && check_something_else()) { ... }
> ```

> [!NOTE]
> **泛型重載的必要性**：
> 提供泛型 `T` 重載可有效避免 C++ 編譯器在處理 `d && 1` 或 `0 || d` 時，於隱式建構 `decimal(1)` 或隱式轉換 `operator bool()` 之間產生重載歧義（ambiguous overload）。

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <iostream>

int main() {
    numeric::decimal zero("0");
    numeric::decimal one("1");
    numeric::decimal val("12.5");
    numeric::decimal nan = numeric::decimal::nan;

    std::cout << std::boolalpha;

    // 1. 同型別邏輯運算
    std::cout << "one && val:  " << (one && val) << "\n";   // true
    std::cout << "zero && val: " << (zero && val) << "\n";  // false
    std::cout << "zero || val: " << (zero || val) << "\n";  // true
    std::cout << "!nan:        " << (!nan) << "\n";         // true

    // 2. 泛型混合型別邏輯運算 (支援 int, bool 等)
    std::cout << "val && 100:  " << (val && 100) << "\n";   // true
    std::cout << "0 || val:    " << (0 || val) << "\n";     // true
    std::cout << "val && false:" << (val && false) << "\n"; // false

    return 0;
}
```

### 預期輸出 (Output)

```text
one && val:  true
zero && val: false
zero || val: true
!nan:        true
val && 100:  true
0 || val:    true
val && false: false
```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上（所有邏輯運算子在 C++20+ 全面支援 `constexpr`）
- **標頭檔**：`<numeric/Decimal.hpp>`
