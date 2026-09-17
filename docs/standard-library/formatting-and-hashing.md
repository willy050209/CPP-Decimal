# 標準程式庫特化與整合 (Standard Library Integration)

說明 `numeric::decimal` 與現代 C++ 標準程式庫的整合功能，包含 C++20 `std::format` 格式化特化、雜湊容器支援 (`std::hash`) 以及標準 `namespace std` 中的數學函式重載。

- **標頭檔**：`<numeric/Decimal.hpp>`、`<numeric/DecimalMath.hpp>`

---

## 1. C++20 std::formatter 格式化特化

在支援 C++20 `<format>` 的編譯環境中，CPP-Decimal 提供了 `std::formatter<numeric::decimal, CharT>` 的樣板特化。

### 支援語法規範 (Format Specifiers)

| 格式規格符 | 範例表達式 | 格式化效果 |
| :--- | :--- | :--- |
| `{}` | `std::format("{}", d)` | 預設標準輸出（等同於 `to_string()`）。 |
| `{:.N}` | `std::format("{:.2}", d)` | 指定保留 $N$ 位小數，尾數採銀行家捨入，不足位數補 0。 |
| `{:.Nf}` / `{:.NF}` | `std::format("{:.4f}", d)` | 浮點風格指定保留 $N$ 位小數並補零。 |

### 錯誤處理
若格式字串包含不合法的規格符（非 `:.[0-9]+[fF]?}`），將拋出 `std::format_error`。

---

## 2. std::hash 特化與容器支援

為了讓 `numeric::decimal` 能作為 `std::unordered_map` 或 `std::unordered_set` 的 Key，標準庫命名空間特化了 `std::hash<numeric::decimal>`。

### 語法 (Syntax)

```cpp
namespace std {
template <>
struct hash<numeric::decimal> {
    size_t operator()(const numeric::decimal& d) const noexcept;
};
}
```

### 關鍵注意事項 (Remarks)

> [!IMPORTANT]
> **雜湊規格化保證 (Hash Normalization Invariant)**：
> 在十進位數值中，`1.0`、`1.00` 與 `1.0000` 代表完全相等的數值。
> 為了維護 C++ 容器「**兩鍵若相等 (`a == b`) 則其雜湊值必相等 (`hash(a) == hash(b)`)**」的基礎不變性，`std::hash<numeric::decimal>` 在計算雜湊前會強制對未縮放整數與小數位數進行**規格化化簡**。
> 
> 因此，`std::hash<numeric::decimal>{}(decimal("1.0"))` 與 `std::hash<numeric::decimal>{}(decimal("1.00"))` **精確相等**。

---

## 3. std 命名空間數學函式重載 (ADL Overloads)

引入 `<numeric/DecimalMath.hpp>` 時，不僅在 `numeric::` 命名空間提供函式，亦會在 `namespace std` 中注入針對 `numeric::decimal` 的多載版本。

這使得通用樣板程式庫（Generic Template Libraries）能以標準語法呼叫數學函式：
```cpp
template <typename T>
T compute_hypot(T a, T b) {
    using std::sqrt;
    return sqrt(a * a + b * b); // 當 T 為 decimal 時自動調用 numeric::sqrt
}
```

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <iostream>
#include <unordered_map>

#if NUMERIC_HAS_STD_FORMAT
#include <format>
#endif

int main() {
    // 1. std::hash 與 unordered_map
    std::unordered_map<numeric::decimal, std::string> price_map;
    
    // 插入 key 為 10.50
    price_map[numeric::decimal("10.50")] = "Widget A";

    // 以 10.5 查詢 (小數位數不同，但數值相等)
    numeric::decimal query("10.5");
    if (price_map.find(query) != price_map.end()) {
        std::cout << "Found: " << price_map[query] << " (Scale invariance hash works!)\n";
    }

    // 2. C++20 std::format 示範
#if NUMERIC_HAS_STD_FORMAT
    numeric::decimal pi("3.14159265");
    std::cout << std::format("Default: {:.}\n", pi);
    std::cout << std::format("Two decimals: {:.2f}\n", pi);
    std::cout << std::format("Four decimals: {:.4f}\n", pi);

    numeric::decimal small("0.5");
    std::cout << std::format("Padded zeros: {:.3f}\n", small); // 0.500
#else
    std::cout << "std::format is not supported by current compiler.\n";
#endif

    return 0;
}
```

### 預期輸出 (Output)

```text
Found: Widget A (Scale invariance hash works!)
Default: 3.14159265
Two decimals: 3.14
Four decimals: 3.1416
Padded zeros: 0.500
```

---

## 適用於 (Applies to)

- **`std::hash`**：C++11 及以上
- **`std::formatter`**：C++20 及以上（需編譯器支援 `<format>`）
- **標頭檔**：`<numeric/Decimal.hpp>`
