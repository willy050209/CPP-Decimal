# decimal 型別轉換運算子 (Conversion Operators)

定義 `numeric::decimal` 與 C++ 原生基本型別（布林、整數、浮點數）、`numeric::bigint` 以及字串之間的轉換運算子。

- **命名空間**：`numeric`
- **標頭檔**：`<numeric/Decimal.hpp>`

---

## 轉換運算子清單 (Conversion Operators List)

| 運算子 / 方法 | 語法特徵 | 說明 |
| :--- | :--- | :--- |
| [`explicit operator bool()`](#1-布林轉型運算子) | `explicit noexcept` | 明確轉換為布林值。 |
| [`bool operator!()`](#2-邏輯非運算子) | `noexcept` | 邏輯非運算子。 |
| [`explicit operator bigint()`](#3-bigint-整數轉型) | `explicit` (可能拋出例外) | 截斷小數部分後轉換為任意精度整數 `bigint`。 |
| [`explicit operator int8_t ~ int64_t()`<br>`explicit operator uint8_t ~ uint64_t()`](#4-原生整數轉型) | `explicit` (可能拋出例外) | 截斷小數部分後轉換為原生整數。 |
| [`explicit operator float()`<br>`explicit operator double()`<br>`explicit operator long double()`](#5-原生浮點數轉型) | `explicit noexcept` | 轉換為原生浮點數近似值。 |
| [`std::string to_string()`](#6-to_string-字串序列化) | `[[nodiscard]]` | 將數值序列化為標準十進位字串。 |

---

## 1. 布林轉型運算子

```cpp
explicit NUMERIC_CONSTEXPR_20 operator bool() const noexcept;
```

### 摘要
判斷 `decimal` 是否代表「非零」的有效真值。

### 傳回值
- 若數值為 `NaN`，回傳 `false`。
- 若數值為正負無窮大 (`+Infinity` 或 `-Infinity`)，回傳 `true`。
- 對於一般有限數值：若數值不等於 0，回傳 `true`；若數值等於 0，回傳 `false`。

---

## 2. 邏輯非運算子

```cpp
NUMERIC_CONSTEXPR_20 bool operator!() const noexcept;
```

### 摘要
等同於 `!static_cast<bool>(*this)`。當數值為 `0` 或 `NaN` 時傳回 `true`，否則傳回 `false`。

---

## 3. bigint 整數轉型

```cpp
explicit NUMERIC_CONSTEXPR_20 operator bigint() const;
```

### 摘要
截斷小數部分（向零捨入），取得整數部分的 `numeric::bigint`。

### 例外狀況 (Exceptions)
- `std::domain_error`: 當數值為 `NaN` 或 `Infinity`（無窮大）時拋出，因為特殊值無法映射為有限整數。

### 備註 (Remarks)
若 `scale > 0`，內部會將未縮放整數除以 $10^{\text{scale}}$；若 `scale < 0`，則乘上 $10^{-\text{scale}}$。此運算保證不會發生二進位截斷溢位。

---

## 4. 原生整數轉型

```cpp
explicit NUMERIC_CONSTEXPR_20 operator int64_t() const;
explicit NUMERIC_CONSTEXPR_20 operator uint64_t() const;
explicit NUMERIC_CONSTEXPR_20 operator int32_t() const;
explicit NUMERIC_CONSTEXPR_20 operator uint32_t() const;
explicit NUMERIC_CONSTEXPR_20 operator int16_t() const;
explicit NUMERIC_CONSTEXPR_20 operator uint16_t() const;
explicit NUMERIC_CONSTEXPR_20 operator int8_t() const;
explicit NUMERIC_CONSTEXPR_20 operator uint8_t() const;
```

### 摘要
先將 `decimal` 截斷為整數，再轉換為對應的原生整數型別。

### 例外狀況 (Exceptions)
- `std::domain_error`: 當數值為 `NaN` 或 `Infinity` 時拋出。

### 注意事項 (Caveats)

> [!CAUTION]
> **整數溢位截斷**：
> 原生整數寬度有限（例如 `int32_t` 上限約 $2 \times 10^9$）。當 `decimal` 數值超出目標整數型別的表示範圍時，轉換將依循 C++ 原生整數轉型的截斷行為。若需安全處理超大整數，請先轉型為 `numeric::bigint` 檢查或使用。

---

## 5. 原生浮點數轉型

```cpp
explicit operator double() const noexcept;
explicit operator float() const noexcept;
explicit operator long double() const noexcept;
```

### 摘要
將高精度十進位浮點數轉換為標準二進位浮點型別近似值。

### 特殊值對應規則
- 若為 `NaN`，傳回 `std::numeric_limits<T>::quiet_NaN()`。
- 若為正無窮大，傳回 `+std::numeric_limits<T>::infinity()`。
- 若為負無窮大，傳回 `-std::numeric_limits<T>::infinity()`。

---

## 6. to_string 字串序列化

```cpp
NUMERIC_NODISCARD std::string to_string() const;
```

### 摘要
依標準十進位格式將數值轉換為 `std::string`。

### 傳回值
標準十進位表示法字串：
- 一般小數：如 `"123.45"`, `"-0.0078"`。
- 整數形式：如 `"100"`, `"-42"`。
- 特殊值：`"nan"`, `"inf"`, `"-inf"`。

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <iostream>

int main() {
    numeric::decimal val("123.456");

    // 1. 布林判斷
    if (val) {
        std::cout << "val is non-zero\n";
    }

    // 2. 整數截斷轉型
    int32_t i_val = static_cast<int32_t>(val);
    std::cout << "Truncated int: " << i_val << "\n"; // 123

    // 3. 浮點數轉型
    double d_val = static_cast<double>(val);
    std::cout << "Double value: " << d_val << "\n";

    // 4. 字串序列化
    std::string s = val.to_string();
    std::cout << "String: " << s << "\n";

    // 5. 特殊值轉型例外處理
    try {
        numeric::decimal inf = numeric::decimal::infinity;
        int32_t bad = static_cast<int32_t>(inf); // 拋出 std::domain_error
        (void)bad;
    } catch (const std::domain_error& e) {
        std::cout << "Caught expected exception: " << e.what() << "\n";
    }

    return 0;
}
```

### 預期輸出 (Output)

```text
val is non-zero
Truncated int: 123
Double value: 123.456
String: 123.456
Caught expected exception: cannot convert special value to bigint
```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上（布林與整數轉型運算子在 C++20+ 支援 `constexpr`）
- **標頭檔**：`<numeric/Decimal.hpp>`
