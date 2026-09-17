# 常數與常數代理 (Constants and Constant Proxies)

介紹 CPP-Decimal 中的特殊數值常數、常數代理機制 (`DecimalConstantProxy`) 以及高精度數學常數。

- **命名空間**：`numeric`
- **標頭檔**：`<numeric/Decimal.hpp>`、`<numeric/DecimalMath.hpp>`

---

## 摘要 (Summary)

`numeric::decimal` 繼承自常數基底結構，提供標準 IEEE 754 特殊值與常用數值常數。為了在各 C++ 標準版本（尤其是 C++11 / C++14 Header-only 環境）提供最佳相容性，常數採用了 **常數代理模式 (Constant Proxy Pattern)**，使使用者既能以屬性風格存取（如 `decimal::zero`），亦能以函式呼叫風格存取（如 `decimal::zero()`）。

---

## 常數清單 (Constants List)

| 常數名稱 | 型別 | 說明 |
| :--- | :--- | :--- |
| `decimal::zero` | `detail::DecimalConstantProxy` | 數值 `0`。 |
| `decimal::one` | `detail::DecimalConstantProxy` | 數值 `1`。 |
| `decimal::infinity` | `detail::DecimalConstantProxy` | 正無窮大 (`+Infinity`)。支援一元負號運算子取得 `-decimal::infinity`。 |
| `decimal::nan` | `detail::DecimalConstantProxy` | 非數值 (Not a Number)。 |
| `decimal::NaN` | `detail::DecimalConstantProxy` | `decimal::nan` 的別名，符合 .NET / IEEE 大小寫慣例。 |

---

## DecimalConstantProxy 代理結構 (Proxy Syntax & Features)

### 語法 (Syntax)

```cpp
namespace numeric {
namespace detail {

struct DecimalConstantProxy {
    constexpr explicit DecimalConstantProxy(DecimalConstantKind k) noexcept;
    
    // 隱式轉換至 decimal
    NUMERIC_CONSTEXPR_20 operator decimal() const;

    // 函式呼叫運算子
    NUMERIC_CONSTEXPR_20 decimal operator()() const;

    // 一元負號運算子
    NUMERIC_CONSTEXPR_20 decimal operator-() const;

    // 比較運算子 (泛型重載)
    template <typename T>
    friend NUMERIC_CONSTEXPR_20 bool operator==(DecimalConstantProxy p, const T& other);

    template <typename T>
    friend NUMERIC_CONSTEXPR_20 bool operator==(const T& other, DecimalConstantProxy p);

    template <typename T>
    friend NUMERIC_CONSTEXPR_20 bool operator!=(DecimalConstantProxy p, const T& other);

    template <typename T>
    friend NUMERIC_CONSTEXPR_20 bool operator!=(const T& other, DecimalConstantProxy p);
};

} // namespace detail
} // namespace numeric
```

### 備註與注意事項 (Remarks)

> [!NOTE]
> **雙重語法支援**：
> 某些程式庫常數採用靜態函式（如 `std::numeric_limits<T>::infinity()`），另一些則採用靜態成員變數。`DecimalConstantProxy` 同時支援兩種寫法：
> ```cpp
> numeric::decimal a = numeric::decimal::infinity;   // 屬性存取語法
> numeric::decimal b = numeric::decimal::infinity(); // 函式呼叫語法
> ```

> [!IMPORTANT]
> **負無窮大表示法**：
> 透過一元負號運算子 `-` 作用於代理物件，可直接獲得負無窮大：
> ```cpp
> numeric::decimal neg_inf = -numeric::decimal::infinity;
> ```

---

## 高精度數學常數 (Mathematical Constants)

定義於 `<numeric/DecimalMath.hpp>` 之 `numeric::detail` 命名空間中，提供高達 34~50 位有效位數的十進位數學常數。在 C++20+ 環境下為 `constexpr`。

| 函式 | 傳回型別 | 數值（前 34 位） | 說明 |
| :--- | :--- | :--- | :--- |
| `numeric::detail::cmath_pi()` | `decimal` | `3.1415926535897932384626433832795029` | 圓周率 $\pi$ |
| `numeric::detail::cmath_two_pi()` | `decimal` | `6.2831853071795864769252867665590058` | $2\pi$ |
| `numeric::detail::cmath_pi_over_2()` | `decimal` | `1.5707963267948966192313216916397514` | $\pi / 2$ |
| `numeric::detail::cmath_ln2()` | `decimal` | `0.69314718055994530941723212145817657` | $\ln 2$ |
| `numeric::detail::cmath_ln10()` | `decimal` | `2.3025850929940456840179914546843642` | $\ln 10$ |

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <numeric/DecimalMath.hpp>
#include <iostream>

int main() {
    // 1. 常數代理使用
    numeric::decimal zero = numeric::decimal::zero;
    numeric::decimal inf  = numeric::decimal::infinity;
    numeric::decimal ninf = -numeric::decimal::infinity;
    numeric::decimal nan  = numeric::decimal::nan;

    std::cout << "Zero: " << zero << "\n";
    std::cout << "Inf: " << inf << "\n";
    std::cout << "Negative Inf: " << ninf << "\n";
    std::cout << "NaN: " << nan << "\n";

    // 2. 常數直接比較
    numeric::decimal val("0");
    if (val == numeric::decimal::zero) {
        std::cout << "val equals decimal::zero\n";
    }

    // 3. 數學常數使用
    numeric::decimal pi = numeric::detail::cmath_pi();
    std::cout << "Pi (34 digits): " << pi << "\n";

    return 0;
}
```

### 預期輸出 (Output)

```text
Zero: 0
Inf: inf
Negative Inf: -inf
NaN: nan
val equals decimal::zero
Pi (34 digits): 3.1415926535897932384626433832795029
```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11、C++14、C++17、C++20（支援 `constexpr`）、C++23
- **命名空間**：`numeric`
- **標頭檔**：`<numeric/Decimal.hpp>`, `<numeric/DecimalMath.hpp>`
