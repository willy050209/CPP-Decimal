# numeric::decimal 類別 (Class Overview)

表示高精度十進位浮點數門面類別，相容於 IEEE 754-2008 decimal128 標準，提供 34 位有效十進位數字、128-bit SBO（小緩衝區最佳化）以及完整運算子與數學支援。

- **命名空間**：`numeric`
- **標頭檔**：`<numeric/Decimal.hpp>`
- **繼承關係**：`numeric::decimal` 繼承自 `detail::DecimalConstants<>`

---

## 語法 (Syntax)

```cpp
namespace numeric {

class decimal : public detail::DecimalConstants<> {
    // 內部成員：
    // bigint  m_unscaled;     // 未縮放整數 (Significand)，支援 128-bit SBO
    // int64_t m_scale;        // 縮放位數 (Scale)，數值為 unscaled * 10^-scale
    // bool    m_is_infinity; // 是否為無窮大 (+/- Infinity)
    // bool    m_is_nan;      // 是否為非數值 (NaN)
};

}
```

---

## 備註與架構特點 (Remarks)

1. **數值模型**：
   數值以未縮放整數與 10 的次冪縮放因子表示：
   $$\text{Value} = \text{m\_unscaled} \times 10^{-\text{m\_scale}}$$
   例如 `123.45` 儲存為 `m_unscaled = 12345`, `m_scale = 2`。
2. **自動規格化 (Normalization)**：
   在運算或建構過程中，底層自動調用 `DecimalCore::normalize`，化簡末尾不必要的零，除非運算明確要求保留指定精度。
3. **無例外狀態保證**：
   除明確轉型可能拋出 `std::domain_error` 或字串解析錯誤拋出 `std::invalid_argument` 之外，所有算術運算遵循 IEEE 754 語意，以 `NaN` 或 `Infinity` 反映邊界條件，不會拋出硬體除零例外。
4. **Constexpr 支援**：
   在 C++20 及更新版本中，多數成員函式、運算子與常數建構均支援編譯期求值 (`constexpr`)。

---

## 成員清單速查表 (Member Summary)

### 建構函式與工廠方法 (Constructors & Factory)
詳見 [建構函式文件 (constructors.md)](constructors.md)。

| 方法 | 說明 |
| :--- | :--- |
| `decimal()` | 預設建構子，初始化為 `0`。 |
| `decimal(const decimal&)` / `decimal(decimal&&)` | 複製與移動建構子。 |
| `decimal(bigint unscaled, int64_t scale)` | 自未縮放整數與縮放位數建構。 |
| `decimal(const bigint&)` / `decimal(bigint&&)` | 自 `bigint` 整數建構（scale 為 0）。 |
| `decimal(bool)` | 自布林值建構（`true` 為 1，`false` 為 0）。 |
| `decimal(int8_t ~ int64_t, uint8_t ~ uint64_t)` | 自原生具體寬度整數隱式建構。 |
| `template <typename T> decimal(T)` | 自其他原生整數型別建構之泛型模板（含 SFINAE 約束）。 |
| `decimal(float)`, `decimal(double)`, `decimal(long double)` | 自原生浮點數隱式建構。 |
| `explicit decimal(numeric::string_view)` | 自字串視圖建構。 |
| `explicit decimal(const char*)` | 自 C 字串建構。 |
| `explicit decimal(const std::string&)` | 自 `std::string` 建構。 |
| `static decimal from_string(numeric::string_view)` | 靜態工廠方法：自字串解析 `decimal`。 |

### 型別轉換運算子 (Conversion Operators)
詳見 [型別轉換文件 (conversion-operators.md)](conversion-operators.md)。

| 運算子 / 方法 | 說明 |
| :--- | :--- |
| `explicit operator bool() const` | 明確轉型為布林值。 |
| `operator!() const` | 邏輯非運算子。 |
| `explicit operator bigint() const` | 明確轉型為 `bigint`（截斷小數）。 |
| `explicit operator int8_t ~ int64_t() const`<br>`explicit operator uint8_t ~ uint64_t() const` | 明確轉型為原生整數型別（截斷小數）。 |
| `explicit operator float() const`<br>`explicit operator double() const`<br>`explicit operator long double() const` | 明確轉型為原生浮點數。 |
| `to_string() const` | 轉換為標準十進位字串。 |

### 算術與取模運算子 (Arithmetic Operators)
詳見 [算術運算子文件 (arithmetic-operators.md)](arithmetic-operators.md)。

| 運算子 | 說明 |
| :--- | :--- |
| `+`, `-` (一元) | 一元正號與一元負號運算子。 |
| `++`, `--` (前置/後置) | 遞增與遞減運算子。 |
| `+`, `-`, `*`, `/`, `%` | 雙目加、減、乘、除、取模運算子。 |
| `+=`, `-=`, `*=`, `/=`, `%=` | 複合賦值運算子。 |

### 比較運算子 (Comparison Operators)
詳見 [比較運算子文件 (comparison-operators.md)](comparison-operators.md)。

| 運算子 | 說明 |
| :--- | :--- |
| `==`, `!=` | 相等與不相等比較運算子。 |
| `<`, `<=`, `>`, `>=` | 小於、小於等於、大於、大於等於關係運算子。 |
| `<=>` (C++20) | 三向比較運算子（回傳 `std::partial_ordering`）。 |

### 邏輯運算子 (Logical Operators)
詳見 [邏輯運算子文件 (logical-operators.md)](logical-operators.md)。

| 運算子 | 說明 |
| :--- | :--- |
| `&&`, `||` | 同型別邏輯及、邏輯或運算子。 |
| `template <typename T> operator&&`, `operator||` | 與原生整數/布林型別之混合泛型邏輯運算子。 |

### 狀態檢查與屬性查詢方法 (Inspection Methods)
詳見 [狀態查詢方法文件 (inspection-methods.md)](inspection-methods.md)。

| 方法 | 說明 |
| :--- | :--- |
| `is_sbo() const` / `is_small() const` | 查詢是否處於 128-bit SBO 本機緩衝區（零 Heap 配置）。 |
| `is_nan() const` | 查詢是否為非數值 (NaN)。 |
| `is_infinite() const` / `is_infinity() const` | 查詢是否為無窮大。 |
| `is_finite() const` | 查詢是否為有限數值。 |
| `is_zero() const` | 查詢數值是否為 0。 |
| `sign() const` | 取得符號：正數傳回 1，負數傳回 -1，零或 NaN 傳回 0。 |
| `scale() const` | 取得十進位小數縮放位數。 |
| `unscaled() const` | 取得未縮放整數之常數參考 (`const bigint&`)。 |

### 類別內建數學方法 (Math Methods)
詳見 [數學方法文件 (math-methods.md)](math-methods.md)。

| 方法 | 說明 |
| :--- | :--- |
| `divide(const decimal& other, int32_t precision = 34) const` | 依指定有效位數精度執行除法。 |
| `round(int64_t decimal_places = 0) const` | 依指定小數位數執行銀行家捨入 (Round Half-to-Even)。 |

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上（全面相容 C++14, C++17, C++20, C++23）
- **標頭檔**：`<numeric/Decimal.hpp>`
