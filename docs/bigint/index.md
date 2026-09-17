# numeric::bigint 類別與未縮放整數 (BigInt Overview)

說明 `numeric::decimal` 底層所採用的高精度整數引擎 `numeric::bigint`，以及如何透過 `decimal::unscaled()` 存取與操作未縮放整數。

- **命名空間**：`numeric`
- **標頭檔**：`<numeric/BigInt.hpp>`

---

## 摘要 (Summary)

`numeric::decimal` 的數值模型為 $\text{unscaled} \times 10^{-\text{scale}}$。其中 $\text{unscaled}$（未縮放整數）即是以 `numeric::bigint` 型別實例進行維護。

`numeric::bigint` 是專為高效能數值計算打造的任意精度整數類別，具備以下特性：
1. **128-bit Small Buffer Optimization (SBO)**：
   內部整合 128 位元緩衝區（由兩個 64 位元無號整數組成）。當整數絕對值在 $2^{127}-1$ 範圍內（約 38 位十進位整數）時，數值完全儲存於物件本機 Stack 記憶體中，不進行任何動態配置（Zero Heap Allocation）。
2. **自動擴充至動態陣列**：
   當數值超出 128 位元門檻時，平滑切換至 Heap 動態記憶體區塊（Big-endian 64-bit 肢段肢解表示），支援任意位數的理論整數上限。
3. **無外部依賴**：
   純 C++ 實作，全面向下相容至 C++11。

---

## 常見用法與未縮放整數互動 (Interacting with unscaled())

### 取得未縮放整數

```cpp
numeric::decimal d("123.456");
const numeric::bigint& u = d.unscaled(); // 回傳 123456
int64_t s = d.scale();                  // 回傳 3
```

### 自未縮放整數重新建構 decimal

```cpp
numeric::bigint my_int("987654321987654321987654321");
int64_t my_scale = 10;
numeric::decimal new_dec(my_int, my_scale);
// 數值為 98765432198765432.1987654321
```

---

## bigint 常用成員速查表 (Common bigint Methods)

| 方法 / 運算子 | 說明 |
| :--- | :--- |
| `bigint()` | 預設建構子，初始化為 0。 |
| `bigint(int64_t)` / `bigint(uint64_t)` | 自原生整數建構。 |
| `explicit bigint(numeric::string_view)` | 自十進位或十六進位字串建構。 |
| `is_sbo() const noexcept` | 判斷是否正處於 128-bit SBO 模式。 |
| `sign() const noexcept` | 傳回符號（1 正數，-1 負數，0 為零）。 |
| `to_string() const` | 轉換為十進位數字字串。 |
| `+`, `-`, `*`, `/`, `%` | 任意精度整數四則與取模運算子。 |
| `&`, `\|`, `^`, `~`, `<<`, `>>` | 任意精度位元運算子。 |

---

## 範例程式碼 (Examples)

```cpp
#include <numeric/Decimal.hpp>
#include <numeric/BigInt.hpp>
#include <iostream>

int main() {
    // 1. 檢視 decimal 的 unscaled 整數與 SBO 狀態
    numeric::decimal d("12345.6789");
    const numeric::bigint& u = d.unscaled();

    std::cout << "Decimal:  " << d << "\n";
    std::cout << "Unscaled: " << u << "\n";
    std::cout << "Scale:    " << d.scale() << "\n";
    std::cout << "Is SBO:   " << std::boolalpha << u.is_sbo() << "\n";

    // 2. 超大位數動態切換
    numeric::decimal huge("1234567890123456789012345678901234567890.5");
    std::cout << "\nHuge decimal: " << huge << "\n";
    std::cout << "Huge is SBO:  " << huge.is_sbo() << " (Exceeds 128-bit, allocated on heap)\n";

    return 0;
}
```

### 預期輸出 (Output)

```text
Decimal:  12345.6789
Unscaled: 123456789
Scale:    4
Is SBO:   true

Huge decimal: 1234567890123456789012345678901234567890.5
Huge is SBO:  false (Exceeds 128-bit, allocated on heap)
```

---

## 適用於 (Applies to)

- **C++ 標準**：C++11 及以上
- **標頭檔**：`<numeric/BigInt.hpp>`
