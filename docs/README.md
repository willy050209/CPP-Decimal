# CPP-Decimal API 參考文件

歡迎查閱 **CPP-Decimal** 官方技術文件。CPP-Decimal 是一套專為現代 C++ 開發的高效、高精度、零外部依賴十進位浮點數與數學函式庫，遵循 **IEEE 754-2008 decimal128** 標準，提供 **34 位十進位有效數字** 的高精度數值計算能力。

---

## 快速導覽 (Quick Navigation)

| 模組 | 命名空間 / 標頭檔 | 說明 |
| :--- | :--- | :--- |
| [**numeric::decimal 類別**](decimal/index.md) | `numeric`<br>`<numeric/Decimal.hpp>` | 核心十進位浮點數門面類別，包含建構子、運算子、轉型、狀態查詢及精度控制。 |
| [**DecimalMath 數學函式庫**](decimal-math/index.md) | `numeric`<br>`<numeric/DecimalMath.hpp>` | 高精度標準數學函式庫（三角、指數、對數、方根、捨入與模運算等）。 |
| [**常數與常數代理**](constants.md) | `numeric`<br>`<numeric/Decimal.hpp>` | `decimal::zero`, `decimal::infinity`, `decimal::nan` 與高精度數學常數 ($\pi, \ln 2, \ln 10$)。 |
| [**標準程式庫特化與整合**](standard-library/formatting-and-hashing.md) | `std`<br>`<numeric/Decimal.hpp>` | C++20 `std::formatter` 支援、`std::hash` 特化與 `std` 命名空間重載。 |
| [**numeric::bigint 簡介**](bigint/index.md) | `numeric`<br>`<numeric/BigInt.hpp>` | 底層 128-bit SBO 任意精度整數支援。 |

---

## 核心設計理念 (Core Architecture)

### 1. IEEE 754-2008 decimal128 標準相容
二進位浮點數（如 `double`、`float`）以 $2^{-n}$ 分數逼近數值，因而無法精準表示如 $0.1$、$0.2$ 等常見十進位小數。CPP-Decimal 將數值儲存為未縮放整數（Significand / Mantissa）與 10 的次冪縮放因子（Scale），使數值模型表示為：
$$\text{Value} = \text{unscaled} \times 10^{-\text{scale}}$$
在此模型下，$0.1 + 0.2 == 0.3$ 精確成立，完全消除財務、計量與科學計算中的捨入誤差。

### 2. 128-bit Small Buffer Optimization (SBO)
`numeric::decimal` 及其底層未縮放整數 `numeric::bigint` 採用 128 位元本機緩衝區最佳化機制：
- 當整數值於 128 位元（約 38 位十進位數字）範圍內時，**配置於 Stack 本機記憶體**，達成 **0 次 Heap 動態記憶體配置**。
- 超出 128 位元時自動平滑切換至動態記憶體配置，兼顧極致效能與任意精度擴充能力。

### 3. 銀行家捨入法 (Round Half-to-Even)
除法與自訂精度運算預設採用 IEEE 推薦之銀行家捨入法：
- 當捨去部分剛好等於半數（$0.5$）時，捨入至最接近的偶數。
- 有效消除統計計算中長期的向上或向下偏誤。

---

## 編譯需求與組態巨集 (Compilation & Configuration)

CPP-Decimal 向下相容至 **C++11**，並在 **C++20** 及更高版本提供編譯期求值 (`constexpr`) 與格式化 (`std::format`) 支援。

| 巨集名稱 | 預設值 | 說明 |
| :--- | :--- | :--- |
| `NUMERIC_NO_GLOBAL_TYPE_ALIAS` | 未定義 | 預設會在全域命名空間引入 `using numeric::decimal;` 與 `using numeric::bigint;`。若定義此巨集，則僅限於 `numeric::` 命名空間下使用。 |
| `NUMERIC_HAS_STD_FORMAT` | 自動偵測 | 若編譯器支援 `<format>` 且啟用 C++20，會自動啟用 `std::formatter<numeric::decimal>` 特化。 |
| `NUMERIC_CONSTEXPR_20` | 自動偵測 | 在支援 C++20 的編譯器下展開為 `constexpr`，在舊版本編譯器下展開為空。 |

---

## 快速範例 (Quick Example)

```cpp
#include <numeric/Decimal.hpp>
#include <numeric/DecimalMath.hpp>
#include <iostream>

int main() {
    // 1. 精準計算，無浮點誤差
    numeric::decimal price("19.99");
    numeric::decimal tax_rate("0.05");
    numeric::decimal total = price * (1 + tax_rate);
    std::cout << "Total: " << total << "\n"; // 20.9895

    // 2. 銀行家捨入至小數點後兩位
    numeric::decimal rounded = total.round(2);
    std::cout << "Rounded: " << rounded << "\n"; // 20.99

    // 3. 高精度數學函數
    numeric::decimal pi = numeric::detail::cmath_pi();
    numeric::decimal sin_val = numeric::sin(pi / 6);
    std::cout << "sin(pi/6) = " << sin_val << "\n"; // 0.5

    return 0;
}
```
