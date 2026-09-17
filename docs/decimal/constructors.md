# decimal 建構函式 (Constructors & Factory)

初始化 `numeric::decimal` 類別的新執行個體。

- **命名空間**：`numeric`
- **標頭檔**：`<numeric/Decimal.hpp>`

---

## 多載清單 (Overload List)

| 多載 | 說明 |
| :--- | :--- |
| [`decimal() noexcept`](#1-預設建構函式) | 將 `decimal` 初始化為數值 `0`。 |
| [`decimal(const decimal&)` / `decimal(decimal&&) noexcept`](#2-複製與移動建構函式) | 複製或移動既有的 `decimal` 執行個體。 |
| [`decimal(bigint unscaled, int64_t scale)`](#3-未縮放整數與縮放位數建構) | 自未縮放整數與縮放位數建構，並自動規格化。 |
| [`decimal(const bigint&)` / `decimal(bigint&&) noexcept`](#4-自-bigint-建構) | 自 `numeric::bigint` 隱式建構（小數位數為 0）。 |
| [`decimal(bool b) noexcept`](#5-自布林值建構) | 自布林值建構（`true` 為 1，`false` 為 0）。 |
| [`decimal(int8_t ~ int64_t) noexcept`<br>`decimal(uint8_t ~ uint64_t) noexcept`](#6-自原生整數建構) | 自具體寬度之原生有號/無號整數隱式建構。 |
| [`template <typename T> decimal(T v) noexcept`](#7-泛型原生整數建構函式-template) | 自其他任意標準原生整數型別建構之泛型樣板。 |
| [`decimal(float)` / `decimal(double)` / `decimal(long double)`](#8-自浮點數建構) | 自原生單精度、倍精度或延伸倍精度浮點數建構。 |
| [`explicit decimal(numeric::string_view)`<br>`explicit decimal(const char*)`<br>`explicit decimal(const std::string&)`](#9-自字串明確建構) | 明確解析十進位格式字串。 |
| [`static decimal from_string(numeric::string_view)`](#10-from_string-靜態工廠方法) | 靜態輔助工廠方法，自字串解析 `decimal`。 |

---

## 1. 預設建構函式

```cpp
NUMERIC_CONSTEXPR_20 decimal() noexcept = default;
```

### 摘要
初始化為數值 `0`（未縮放值為 0，scale 為 0，非 NaN 且非 Infinity）。

---

## 2. 複製與移動建構函式

```cpp
NUMERIC_CONSTEXPR_20 decimal(const decimal& other) = default;
NUMERIC_CONSTEXPR_20 decimal(decimal&& other) noexcept = default;
```

### 參數
- `other`: 來源 `decimal` 執行個體。

---

## 3. 未縮放整數與縮放位數建構

```cpp
NUMERIC_CONSTEXPR_20 decimal(bigint unscaled, int64_t scale);
```

### 摘要
直接指定未縮放整數（Unscaled Significand）與 10 的次冪負指數（Scale）。數值模型為：
$$\text{Value} = \text{unscaled} \times 10^{-\text{scale}}$$

### 參數
- `unscaled`: `bigint` 未縮放整數。
- `scale`: `int64_t` 十進位小數點後位數。

### 備註
建構過程中會自動執行 `DecimalCore::normalize`，化簡多餘的尾隨零以保持最簡緊湊內部表示。

---

## 4. 自 bigint 建構

```cpp
NUMERIC_CONSTEXPR_20 decimal(const bigint& val);
NUMERIC_CONSTEXPR_20 decimal(bigint&& val) noexcept;
```

### 參數
- `val`: 來源 `numeric::bigint` 整數。小數位數 `scale` 初始化為 0。

---

## 5. 自布林值建構

```cpp
NUMERIC_CONSTEXPR_20 decimal(bool b) noexcept;
```

### 參數
- `b`: 若為 `true` 則數值為 1；若為 `false` 則數值為 0。

---

## 6. 自原生整數建構

```cpp
NUMERIC_CONSTEXPR_20 decimal(int8_t v) noexcept;
NUMERIC_CONSTEXPR_20 decimal(int16_t v) noexcept;
NUMERIC_CONSTEXPR_20 decimal(int32_t v) noexcept;
NUMERIC_CONSTEXPR_20 decimal(int64_t v) noexcept;
NUMERIC_CONSTEXPR_20 decimal(uint8_t v) noexcept;
NUMERIC_CONSTEXPR_20 decimal(uint16_t v) noexcept;
NUMERIC_CONSTEXPR_20 decimal(uint32_t v) noexcept;
NUMERIC_CONSTEXPR_20 decimal(uint64_t v) noexcept;
```

### 摘要
提供對標準定寬整數的非明確（隱式）建構，便於混合算術表達式求值。

---

## 7. 泛型原生整數建構函式 (Template)

```cpp
template <typename T, typename std::enable_if<
    std::is_integral<T>::value &&
    !std::is_same<T, bool>::value &&
    !std::is_same<T, int8_t>::value &&
    !std::is_same<T, int16_t>::value &&
    !std::is_same<T, int32_t>::value &&
    !std::is_same<T, int64_t>::value &&
    !std::is_same<T, uint8_t>::value &&
    !std::is_same<T, uint16_t>::value &&
    !std::is_same<T, uint32_t>::value &&
    !std::is_same<T, uint64_t>::value, int>::type = 0>
NUMERIC_CONSTEXPR_20 decimal(T v) noexcept;
```

### 型別參數 (Type Parameters)
- `T`: 任意合法的 C++ 原生整數型別（例如 `long`, `unsigned long`, `char`, `wchar_t` 等平台相依型別）。

### 備註 (Remarks)
使用 SFINAE (`std::enable_if`) 嚴格限制僅有非特化的整數型別得以匹配此樣板，防止非預期的指標或使用者自訂型別錯誤介入。

---

## 8. 自浮點數建構

```cpp
decimal(float v);
decimal(double v);
decimal(long double v);
```

### 參數
- `v`: 來源浮點數。

### 備註與注意事項 (Remarks & Caveats)

> [!WARNING]
> **二進位浮點誤差警告**：
> 原生 `float`、`double` 與 `long double` 是以二進位（base-2）儲存，常見的小數如 `0.1` 無法在二進位中精確表示（其實際值為 `0.10000000000000000555...`）。若傳入浮點數，`decimal` 會如實捕獲該浮點數的二進位近似值。
>
> **強烈建議**：若要獲得絕對精確的十進位表示，請務必改用字串建構子，例如 `numeric::decimal("0.1")`。

---

## 9. 自字串明確建構

```cpp
explicit NUMERIC_CONSTEXPR_20 decimal(numeric::string_view sv);
explicit NUMERIC_CONSTEXPR_20 decimal(const char* s);
explicit decimal(const std::string& s);
```

### 參數
- `sv` / `s`: 包含十進位數值之字串。

### 例外狀況 (Exceptions)
- `std::invalid_argument`:
  - 當字串指標為 `nullptr`。
  - 當字串為空或包含無效字元。
  - 當小數點或正負號語法不合法。

### 備註
- 支援標準整數表示法：`"123"`, `"-456"`。
- 支援標準小數表示法：`"0.001"`, `"+3.14159"`。
- 支援科學記號：`"1.23e-4"`, `"-5E+6"`。
- 支援特殊值（不分大小寫）：`"inf"`, `"-inf"`, `"+infinity"`, `"nan"`。

---

## 10. from_string 靜態工廠方法

```cpp
NUMERIC_CONSTEXPR_20 static decimal from_string(numeric::string_view sv);
```

### 傳回值
解析完成之 `decimal` 執行個體。功能等同於明確字串建構子，適用於鏈式表達式或函數式程式設計風格。

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <iostream>

int main() {
    // 1. 預設與整數建構
    numeric::decimal d1;           // 0
    numeric::decimal d2 = 42;      // 泛型/整數隱式建構
    numeric::decimal d3 = -100LL;  // 64 位元整數

    // 2. 字串精準建構 (推薦)
    numeric::decimal price("19.99");
    numeric::decimal sci("1.25e-3"); // 0.00125

    // 3. 特殊值解析
    numeric::decimal inf = numeric::decimal::from_string("inf");
    numeric::decimal nan = numeric::decimal::from_string("nan");

    std::cout << "d2: " << d2 << "\n";
    std::cout << "price: " << price << "\n";
    std::cout << "sci: " << sci << "\n";
    std::cout << "inf: " << inf << "\n";

    // 4. 浮點數精度差異對比
    numeric::decimal from_flt(0.1);       // 可能帶有微小二進位浮點尾差
    numeric::decimal from_str("0.1");     // 絕對精準 0.1
    std::cout << "from_str: " << from_str << "\n";

    return 0;
}
```

### 預期輸出 (Output)

```text
d2: 42
price: 19.99
sci: 0.00125
inf: inf
from_str: 0.1
```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上（字串與整數建構子於 C++20+ 支援 `constexpr`）
- **標頭檔**：`<numeric/Decimal.hpp>`
