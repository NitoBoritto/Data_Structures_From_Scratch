# 📚 Array Data Structure في C++
## Static Array  vs  Dynamic Array

مشروع بيشرح ويطبّق **الـ Array كـ Data Structure** في C++ بنسختين:

| الملف | الـ Class | الفكرة |
|---|---|---|
| `Static_Array.cpp` | `Static_Array_Number` | سعة **ثابتة** — لو الـ array اتملت، العملية بتفشل |
| `Dynamic_Array.cpp` | `Dynamic_Array_Number` | سعة **بتكبر لوحدها** — لو اتملت بتضاعف نفسها |

الاتنين فيهم نفس الـ Menu ونفس العمليات (إضافة، حذف، بحث، عمليات حسابية، عمليات مجموعات…) وبرنامج تفاعلي كامل في `main()`.

---

## 📑 الفهرس

1. شكل المشروع
2. التشغيل
3. الـ Array في الميموري: `size` و `length`
4. الفرق الجوهري بين النسختين
5. الـ Class من جوه
6. العمليات الأساسية (Append / Insert / Delete …)
7. البحث
8. العمليات الحسابية
9. عمليات على الـ Array (Reverse / Is Sorted / Rearrange)
10. عمليات المجموعات (Merge / Union / Intersection / Difference)
11. شرح `main()` والـ Menu
12. جدول التعقيد Big-O
13. مثال تشغيل حقيقي
14. ملاحظات من مراجعة الكود

---

## 🎬 دليل رموز الأنيميشن

كل "أنيميشن" في الملف ده عبارة عن **Frames** (لقطات) بتتابعها من فوق لتحت، كأنك بتشوف الكود وهو شغال خطوة خطوة.

| الرمز | المعنى |
|:-:|---|
| `_` أو `?` | خانة فاضية / قيمة مش مهمة |
| `i` `j` | مؤشرات (indexes) في الكود |
| `L` `M` `H` | `low` / `mid` / `high` في الـ Binary Search |
| `S` `E` | `start` / `end` في الـ Reverse |
| `(30)` | قيمة لسه موجودة في الميموري بس **برة** الـ `length` (يعني مش محسوبة) |

---

## 1. شكل المشروع

```
Array-Project/
├── README.md              ← الملف ده
├── Static_Array.cpp       ← النسخة ذات السعة الثابتة
└── Dynamic_Array.cpp      ← النسخة اللي بتكبر تلقائياً
```

كل ملف فيه **3 أجزاء**:

1. **الـ Class** (`Static_Array_Number` / `Dynamic_Array_Number`) — فيه البيانات والعمليات.
2. **`read_input()`** — دالة بتقرا رقم صحيح من المستخدم وبتقفل البرنامج لو دخل حاجة مش رقم.
3. **`main()`** — الـ Menu التفاعلي اللي بيستخدم الـ class.

---

## 2. التشغيل

محتاج compiler بيدعم C++11 أو أحدث (مثلاً g++ أو clang++ أو MSVC).

```bash
# النسخة الثابتة
g++ -std=c++17 -Wall -Wextra Static_Array.cpp -o static_array
./static_array

# النسخة الديناميكية
g++ -std=c++17 -Wall -Wextra Dynamic_Array.cpp -o dynamic_array
./dynamic_array
```

على Windows استخدم `static_array.exe` بدل `./static_array`.

أول حاجة البرنامج هيسألك عنها هي **السعة (Capacity)**، وبعدها بيظهر الـ Main Menu.

---

## 3. الـ Array في الميموري: `size` و `length`

الـ class عنده **3 متغيرات** بس (كلهم `private`):

| المتغير | معناه |
|---|---|
| `int* Array` | مؤشر على الـ array الفعلي اللي متخزن في الـ Heap |
| `int size` | **السعة** (Capacity): أقصى عدد عناصر الـ array تقدر تشيلهم دلوقتي |
| `int length` | **الطول**: عدد العناصر اللي **فعلاً** متخزنة |

دايماً: `0 ≤ length ≤ size`

```
 الـ object (arr)               الـ Heap
┌─────────────────┐
│ Array  ─────────┼───►  ┌────┬────┬────┬────┬────┐
│ size   = 5      │      │ 10 │ 20 │ 30 │  ? │  ? │
│ length = 3      │      └────┴────┴────┴────┴────┘
└─────────────────┘        ◄─ length = 3 ─►
                           ◄────────── size = 5 ─────►
```

> **مهم:** أي عملية بتشتغل على `length` بس (من index `0` لحد `length-1`)، والخانات بعد كده تعتبر فاضية حتى لو فيها قيم قديمة.

---

## 4. الفرق الجوهري بين النسختين

### 🎬 أنيميشن: Append لما الـ array تتملي

**Static** (`size = 3`):

```
Frame 0   append(10)   │ 10 │  _ │  _ │    length=1   ✅
Frame 1   append(20)   │ 10 │ 20 │  _ │    length=2   ✅
Frame 2   append(30)   │ 10 │ 20 │ 30 │    length=3   ✅  (ممتلية)
Frame 3   append(40)   length < size ؟  3 < 3 ؟  لأ
                       return 0  ❌   →  "Array is full!"
```

**Dynamic** (`size = 2`):

```
Frame 0   append(10)   │ 10 │  _ │              size=2 length=1   ✅
Frame 1   append(20)   │ 10 │ 20 │              size=2 length=2   ✅  (ممتلية)
Frame 2   append(30)   ممتلية؟ آه → ensure_capacity(3) → size: 2 → 4
                       │ 10 │ 20 │ 30 │  _ │    size=4 length=3   ✅
Frame 3   append(40)   │ 10 │ 20 │ 30 │ 40 │    size=4 length=4   ✅  (ممتلية)
Frame 4   append(50)   ممتلية؟ آه → ensure_capacity(5) → size: 4 → 8
                       │ 10 │ 20 │ 30 │ 40 │ 50 │  _ │  _ │  _ │   size=8 length=5  ✅
```

### جدول الاختلافات بين الملفين (سطر بسطر)

| الجزء | `Static_Array.cpp` | `Dynamic_Array.cpp` |
|---|---|---|
| **اسم الـ class** | `Static_Array_Number` | `Dynamic_Array_Number` |
| **Constructor** (`size`) | `size >= 0` (ممكن تبقى `0`) | `size >= 1` (لازم خانة واحدة على الأقل عشان تقدر تكبر) |
| **دوال خاصة** | مفيش | `resize()` و `ensure_capacity()` |
| **`append`** | بترجع `0` لو الـ array ممتلية | بتكبّر الـ array تلقائياً وبترجع `1` دايماً |
| **`insert`** | بتفشل لو ممتلية | بتكبّر تلقائياً |
| **`insert_sorted`** | بتفشل لو ممتلية | بتكبّر تلقائياً |
| **`set_array`** | بترجع `0` لو `newLength > size` | بتكبّر تلقائياً بـ `ensure_capacity` |
| **`merge`** / **`union_array`** | السعة الجديدة = `size + arr2.size` | السعة الجديدة = `length + arr2.length` (على الأقل `1`) |
| **`intersection`** / **`difference`** | نفس المنطق | نفس المنطق |
| **رسايل الفشل في `main`** | `"Array is full!"` | `"Failed to add number!"` (عملياً مبتظهرش لأن `append` بترجع `1` دايماً) |
| **عنوان الترحيب** | `Welcome To Array Application` | `Welcome To Dynamic Array Application` |
| **سؤال السعة** | `Enter Array Capacity` | `Enter Starting Capacity` |

باقي الدوال (`delete_index`, `linear_search`, `binary_search`, `get`, `set`, `max`, `min`, `sum`, `average`, `reverse`, `is_sorted`, `rearrange`, `display`, …) **متطابقة** في الملفين.

### 🎬 أنيميشن: الـ Dynamic Array بيكبر إزاي (`resize`)

الحالة: `size=2` و `length=2`، وبنعمل `append(30)`.

```
Frame 1   الـ array ممتلية
          Array ──►  │ 10 │ 20 │                 size=2  length=2

Frame 2   ensure_capacity(3):  newSize = 2 ... 2 < 3 → newSize = 4  (بنضاعف)
          new int[4]
          newArray ─► │  ? │  ? │  ? │  ? │

Frame 3   نسخ العناصر القديمة (for i < length)
          newArray ─► │ 10 │ 20 │  ? │  ? │

Frame 4   delete[] Array      ← الميموري القديمة اتمسحت
          Array = newArray    ← الـ pointer بقى يشاور على الجديدة
          size  = 4

Frame 5   Array[length] = 30 ;  length++
          Array ──►  │ 10 │ 20 │ 30 │  _ │      size=4  length=3
```

**ليه بنضاعف (×2) بدل ما نزود خانة خانة؟**
عشان الـ resize غالي (بينسخ كل العناصر). لو كبّرنا خانة واحدة كل مرة، كل `append` هيبقى `O(n)`. بالمضاعفة، الـ resize بيحصل **نادراً** (عند 2 ثم 4 ثم 8 ثم 16 …)، فمتوسط تكلفة الـ `append` بيبقى `O(1)` — ده اسمه **Amortized O(1)**.

| رقم الـ append | `size` قبل | بيحصل إيه | `size` بعد |
|:-:|:-:|---|:-:|
| 1 | 2 | فيه مكان | 2 |
| 2 | 2 | فيه مكان | 2 |
| 3 | 2 | **resize** → ×2 | 4 |
| 4 | 4 | فيه مكان | 4 |
| 5 | 4 | **resize** → ×2 | 8 |

---

## 5. الـ Class من جوه

### 5.1 الـ Constructor

```cpp
Static_Array_Number(int Size, int Length, int* arr)
```

| الـ Parameter | معناه |
|---|---|
| `Size` | السعة المطلوبة |
| `Length` | عدد العناصر اللي هتتنسخ من `arr` في البداية |
| `arr` | الـ array المصدر (ممكن `nullptr` ← يبقى الـ array فاضية) |

الـ constructor بيحمي نفسه من القيم الغلط:
- لو `Length` سالب ← بيبقى `0`.
- لو `Length > size` ← بيتقص لـ `size`.
- لو `arr == nullptr` ← `length = 0`.

في `main()` بيتنادى كده: `arr(size, 0, nullptr)` يعني **array فاضية بالسعة اللي المستخدم اختارها**.

### 5.2 الـ Destructor

```cpp
~Static_Array_Number() { delete[] Array; }
```
بيرجّع الميموري اللي اتحجزت بـ `new[]` أول ما الـ object يموت، عشان ميحصلش **Memory Leak**.

### 5.3 الـ Copy Constructor و الـ Assignment Operator (Deep Copy)

من غيرهم، C++ بتعمل **Shallow Copy** (بتنسخ الـ pointer بس مش الداتا)، وده كارثة:

```
❌ Shallow Copy                          ✅ Deep Copy (اللي الكود بيعمله)

 a.Array ──►  [1, 2, 3]                   a.Array ──►  [1, 2, 3]
 b.Array ──┘  (نفس الميموري!)             b.Array ──►  [1, 2, 3]   (نسخة مستقلة)

 لما a تموت وتعمل delete[]               كل object له ميموري خاصة بيه
 b بيشاور على ميموري ممسوحة 💥            تعديل واحد مش بيأثر على التاني
```

تجربة حقيقية على الكود:

```
c1 = [1]
c2 = c1        // Copy Constructor
c2[0] = 42
→  c1[0] = 1 ,  c2[0] = 42      ✅ مستقلين
```

الـ **Assignment Operator** (`b = a`) خطواته:

```
1. if (this == &other) return *this;   ← حماية من b = b
2. delete[] Array;                     ← امسح الميموري القديمة بتاعتي
3. size = other.size; length = other.length;
4. Array = new int[size];              ← احجز ميموري جديدة
5. انسخ العناصر واحد واحد
6. return *this;                       ← عشان تقدر تكتب a = b = c
```

الـ 3 مع بعض (Destructor + Copy Constructor + Assignment) اسمهم **Rule of Three**.

### 5.4 الدوال الخاصة (Dynamic فقط)

| الدالة | وظيفتها |
|---|---|
| `resize(newSize)` | بتحجز array أكبر، تنسخ فيها القديم، تمسح القديمة. **مبتصغّرش أبداً** (`if newSize <= size return`). |
| `ensure_capacity(required)` | بتضاعف `size` لحد ما يكفي `required`، وبعدين تنادي `resize`. |

### 5.5 الـ Getters

| الدالة | بترجّع |
|---|---|
| `get_size()` | السعة |
| `get_length()` | عدد العناصر |
| `get_array()` | `const int*` — وصول للقراءة فقط |
| `set_array(arr, newLength)` | بتستبدل المحتوى بنسخة من `arr` |

---

## 6. العمليات الأساسية

### 6.1 `append(value)` — إضافة في الآخر

بتحط القيمة في `Array[length]` وتزوّد `length`. تكلفتها `O(1)`.

### 6.2 `insert(index, value)` — إضافة في مكان معين

شرط الـ index: `0 ≤ index ≤ length` (يعني تقدر تضيف في الآخر كمان).
بتزحزح كل العناصر من `index` لآخر الـ array **يمين** خانة، وبعدين تحط القيمة.

### 🎬 أنيميشن: `insert(1, 99)` على `[10, 20, 30]` بسعة 5

```
index        0    1    2    3    4
Frame 0   │ 10 │ 20 │ 30 │  _ │  _ │    البداية  (length=3)
Frame 1   │ 10 │ 20 │ 30 │ 30 │  _ │    i=3 : Array[3] = Array[2]
Frame 2   │ 10 │ 20 │ 20 │ 30 │  _ │    i=2 : Array[2] = Array[1]
Frame 3   │ 10 │ 99 │ 20 │ 30 │  _ │    Array[1] = 99   ،   length = 4   ✅
```

> الزحزحة بتتعمل **من الآخر للأول** عشان منكتبش فوق قيمة لسه محتاجينها.

### 6.3 `insert_sorted(value)` — إضافة مع الحفاظ على الترتيب

**شرط:** الـ array لازم تكون مرتبة تصاعدياً من قبل كده.
بتبدأ من آخر عنصر وتزحزح كل عنصر **أكبر** من القيمة يمين، وبعدين تحط القيمة في الفجوة.

### 🎬 أنيميشن: `insert_sorted(20)` على `[10, 30, 50]`

```
Frame 0   │ 10 │ 30 │ 50 │  _ │      i = 2 (آخر عنصر)
Frame 1   │ 10 │ 30 │ 50 │ 50 │      Array[2]=50 > 20  → زحزح (i=1)
Frame 2   │ 10 │ 30 │ 30 │ 50 │      Array[1]=30 > 20  → زحزح (i=0)
Frame 3   │ 10 │ 30 │ 30 │ 50 │      Array[0]=10 > 20 ؟ لأ → وقّف
Frame 4   │ 10 │ 20 │ 30 │ 50 │      Array[i+1] = Array[1] = 20   ✅
```

### 6.4 `delete_index(index)` — حذف

بترجّع **القيمة المحذوفة**، أو `-1` لو الـ index غلط. بتزحزح العناصر **شمال** عشان تقفل الفجوة.

### 🎬 أنيميشن: `delete_index(1)` على `[10, 99, 20, 30]`

```
index        0    1    2    3    4
Frame 0   │ 10 │ 99 │ 20 │ 30 │  _ │    x = Array[1] = 99   (بنحفظ القيمة)
Frame 1   │ 10 │ 20 │ 20 │ 30 │  _ │    i=1 : Array[1] = Array[2]
Frame 2   │ 10 │ 20 │ 30 │ 30 │  _ │    i=2 : Array[2] = Array[3]
Frame 3   │ 10 │ 20 │ 30 │(30)│  _ │    length-- → 3     return 99   ✅
                          └── برة الـ length، يعتبر مش موجود
```

### 6.5 `get(index, value)` و `set(index, value)`

| الدالة | وظيفتها | بترجّع |
|---|---|---|
| `get(index, value&)` | بتقرا العنصر وتحطه في `value` (by reference) | `true` / `false` |
| `set(index, value)` | بتغيّر قيمة عنصر موجود | `1` / `0` |

الاتنين `O(1)`، وبيتأكدوا إن `0 ≤ index < length`.

---

## 7. البحث

### 7.1 `linear_search(key)` — بحث خطي

بيعدّي على العناصر واحد واحد من الأول. مبيشترطش ترتيب. بيرجّع الـ index أو `-1`.

### 🎬 أنيميشن: دوّر على `7` في `[4, 9, 7, 2]`

```
index     0    1    2    3
value     4    9    7    2

Step 1    ^                   Array[0] = 4  ==  7 ?  no
Step 2         ^              Array[1] = 9  ==  7 ?  no
Step 3              ^         Array[2] = 7  ==  7 ?  YES -> return 2
```

### 7.2 `binary_search(key)` — بحث ثنائي

**شرط:** الـ array لازم تكون **مرتبة تصاعدياً**. كل خطوة بتقسم المساحة للنص.
الكود بيحسب `mid = low + (high - low) / 2` بدل `(low + high) / 2` عشان يتفادى الـ **Integer Overflow**.

### 🎬 أنيميشن: دوّر على `11` في `[1, 3, 5, 7, 9, 11, 13]`

```
index     0    1    2    3    4    5    6
value     1    3    5    7    9   11   13

Step 1    L              M              H    low=0 high=6 mid=3
          1    3    5    7    9   11   13
          Array[3] = 7 < 11  ->  low = mid + 1 = 4

Step 2                        L    M    H    low=4 high=6 mid=5
          1    3    5    7    9   11   13
          Array[5] = 11 == 11  ->  FOUND at index 5
```

---

## 8. العمليات الحسابية

| الدالة | بترجّع | ملاحظات |
|---|---|---|
| `max()` | أكبر قيمة | بترجّع `-1` لو الـ array فاضية |
| `min()` | أصغر قيمة | بترجّع `-1` لو الـ array فاضية |
| `sum()` | المجموع كـ `long long` | `long long` عشان المجموع ميعملش overflow بسهولة |
| `average()` | المتوسط كـ `double` | بترجّع `0.0` لو فاضية |

`average()` بتحوّل المجموع لـ `double` قبل القسمة (`static_cast<double>`) عشان تتفادى **Integer Division** (يعني `7/2` تطلع `3.5` مش `3`).

مثال على `[5, 2, 8, -1, -3, -7]`:

```
max = 8     min = -7     sum = 4     average = 4 / 6 = 0.666667
```

في `main()` بيتم فحص `get_length() == 0` الأول ويطبع `"Array is empty!"` قبل ما ينادي `max/min/average`.

---

## 9. عمليات على الـ Array

### 9.1 `reverse()` — عكس الـ array

مؤشرين (`start` من الأول، `end` من الآخر)، بيبدلوا القيم ويتقابلوا في النص. `O(n)` ومن غير array إضافية.

### 🎬 أنيميشن: `reverse()` على `[1, 2, 3, 4, 5]`

```
index     0    1    2    3    4
start     1    2    3    4    5    start=0 , end=4
          S                   E

swap 1    5    2    3    4    1    swap(Array[0], Array[4])
               S         E         start=1 , end=3

swap 2    5    4    3    2    1    swap(Array[1], Array[3])
                   SE              start=2 , end=2

done      5    4    3    2    1    start >= end  ->  stop
```

### 9.2 `is_sorted()` — هل الـ array مرتبة؟

بيقارن كل عنصر باللي بعده؛ أول ما يلاقي `Array[i] > Array[i+1]` بيرجّع `0`. لو عدّى على الكل بيرجّع `1`.

```
[1, 2, 2, 5]  →  1 (مرتبة، التكرار مسموح)
[1, 5, 3, 9]  →  0 (5 > 3)
```

### 9.3 `rearrange()` — السالب الأول والموجب بعدين

بتحرّك كل الأرقام **السالبة** ناحية البداية، والموجبة (والصفر) ناحية الآخر، بمؤشرين `i` و `j` من الطرفين. **مبتحافظش على الترتيب الأصلي** داخل كل مجموعة.

### 🎬 أنيميشن: `rearrange()` على `[5, -3, 8, -1, 2, -7]`

```
index     0    1    2    3    4    5
start     5   -3    8   -1    2   -7

find 1    i                        j    i=0 (first non-negative) , j=5 (last negative)
swap 1   -7   -3    8   -1    2    5    swap(Array[0], Array[5])

find 2              i    j              i=2 (first non-negative) , j=3 (last negative)
swap 2   -7   -3   -1    8    2    5    swap(Array[2], Array[3])

find 3              j    i              i=3 (first non-negative) , j=2 (last negative)
         -7   -3   -1    8    2    5    i >= j  ->  stop

done     -7   -3   -1    8    2    5
```

---

## 10. عمليات المجموعات

> ⚠️ **شروط مهمة:**
> 1. العمليات دي كلها بتفترض إن **الـ array الأولى والتانية مرتبتين تصاعدياً**. الكود **مبيتحققش** من ده.
> 2. العملية **بتعدّل الـ array الأصلية (`arr`) نفسها** وبتحط النتيجة فيها. الـ array التانية (`arr2`) بتتعمل مؤقتاً جوه `main()` وبتتمسح بعد العملية.

كلهم بيستخدموا تقنية **Two Pointers** (مؤشرين `i` و `j`) وكل واحد بيمشي على array ببطء، فالتكلفة `O(n + m)` بدل `O(n × m)`.

الأمثلة تحت كلها على:

```
A = [1, 3, 5, 7, 9]        B = [3, 4, 5, 6, 7]
```

### 10.1 `merge` — دمج الاتنين (مع الاحتفاظ بالتكرار)

قاعدة بسيطة: قارن `A[i]` و `B[j]` وخد الأصغر. لو متساويين الكود بياخد من `B`.

| # | الخطوة | النتيجة لحد دلوقتي |
|:-:|---|---|
| 1 | 1 < 3  -> خد 1 من A | `1` |
| 2 | 3 >= 3 -> خد 3 من B | `1 3` |
| 3 | 3 < 4  -> خد 3 من A | `1 3 3` |
| 4 | 5 >= 4 -> خد 4 من B | `1 3 3 4` |
| 5 | 5 >= 5 -> خد 5 من B | `1 3 3 4 5` |
| 6 | 5 < 6  -> خد 5 من A | `1 3 3 4 5 5` |
| 7 | 7 >= 6 -> خد 6 من B | `1 3 3 4 5 5 6` |
| 8 | 7 >= 7 -> خد 7 من B | `1 3 3 4 5 5 6 7` |
| 9 | B خلصت -> انسخ 7 من A | `1 3 3 4 5 5 6 7 7` |
| 10 | B خلصت -> انسخ 9 من A | `1 3 3 4 5 5 6 7 7 9` |

**النتيجة: `1 3 3 4 5 5 6 7 7 9`**

### 10.2 `union_array` — الاتحاد (من غير تكرار)

زي الـ merge، بس قبل ما يضيف قيمة بيشيّك إنها **مش نفس آخر قيمة اتضافت**.

| # | الخطوة | النتيجة لحد دلوقتي |
|:-:|---|---|
| 1 | 1 < 3 -> القيمة 1 (اتضاف) | `1` |
| 2 | 3 == 3 -> القيمة 3 (اتضاف) | `1 3` |
| 3 | 5 > 4 -> القيمة 4 (اتضاف) | `1 3 4` |
| 4 | 5 == 5 -> القيمة 5 (اتضاف) | `1 3 4 5` |
| 5 | 7 > 6 -> القيمة 6 (اتضاف) | `1 3 4 5 6` |
| 6 | 7 == 7 -> القيمة 7 (اتضاف) | `1 3 4 5 6 7` |
| 7 | B خلصت -> القيمة 9 (اتضاف) | `1 3 4 5 6 7 9` |

**النتيجة: `1 3 4 5 6 7 9`**

### 10.3 `intersection` — التقاطع (المشترك بس)

بيضيف القيمة **بس لو موجودة في الاتنين**. لو أصغر في أي ناحية بيتخطاها.

| # | الخطوة | النتيجة لحد دلوقتي |
|:-:|---|---|
| 1 | 1 < 3 -> تجاهل 1 (i++) | (فاضية) |
| 2 | 3 == 3 -> مشترك! اتضاف | `3` |
| 3 | 5 > 4 -> تجاهل 4 (j++) | `3` |
| 4 | 5 == 5 -> مشترك! اتضاف | `3 5` |
| 5 | 7 > 6 -> تجاهل 6 (j++) | `3 5` |
| 6 | 7 == 7 -> مشترك! اتضاف | `3 5 7` |

**النتيجة: `3 5 7`**

> السعة بعد التقاطع بتبقى `min(size, arr2.size)` — وده الحد الأقصى المنطقي لعدد العناصر المشتركة.

### 10.4 `difference` — الفرق (`A − B`)

بيرجّع العناصر **اللي في `A` ومش في `B`**.

| # | الخطوة | النتيجة لحد دلوقتي |
|:-:|---|---|
| 1 | 1 < 3 -> 1 مش في B -> اتضاف | `1` |
| 2 | 3 == 3 -> موجود في B -> اتحذف | `1` |
| 3 | 5 > 4 -> تجاهل 4 (j++) | `1` |
| 4 | 5 == 5 -> موجود في B -> اتحذف | `1` |
| 5 | 7 > 6 -> تجاهل 6 (j++) | `1` |
| 6 | 7 == 7 -> موجود في B -> اتحذف | `1` |
| 7 | B خلصت -> 9 اتضاف | `1 9` |

**النتيجة: `1 9`**

### 📊 ملخص العمليات الأربعة على نفس الـ arrays

| العملية | النتيجة | بتحتفظ بالتكرار؟ |
|---|---|:-:|
| `merge` | `1 3 3 4 5 5 6 7 7 9` | ✅ آه |
| `union_array` | `1 3 4 5 6 7 9` | ❌ لأ |
| `intersection` | `3 5 7` | ❌ |
| `difference` (A − B) | `1 9` | ❌ |

---

## 11. شرح `main()` والـ Menu

### 11.1 خريطة الـ Menu

```
Welcome  →  "Enter Capacity:"  →  arr(size, 0, nullptr)
                │
                ▼
            MAIN MENU
            ├── 1  Basic Operations
            │      ├─ 1 Append Number
            │      ├─ 2 Insert Number
            │      ├─ 3 Insert Sorted
            │      ├─ 4 Delete Number
            │      ├─ 5 Get Number
            │      ├─ 6 Set Number
            │      └─ 7 Back
            ├── 2  Search Operations
            │      ├─ 1 Linear Search
            │      ├─ 2 Binary Search
            │      └─ 3 Back
            ├── 3  Mathematical Operations
            │      ├─ 1 Maximum     2 Minimum
            │      ├─ 3 Sum         4 Average
            │      └─ 5 Back
            ├── 4  Array Operations
            │      ├─ 1 Reverse
            │      ├─ 2 Check If Sorted
            │      ├─ 3 Rearrange Negative / Positive
            │      └─ 4 Back
            ├── 5  Set Operations
            │      ├─ 1 Merge       2 Union
            │      ├─ 3 Intersection   4 Difference
            │      └─ 5 Back
            ├── 6  Array Information
            │      ├─ 1 Display
            │      ├─ 2 Get Capacity
            │      ├─ 3 Get Length
            │      └─ 4 Back
            └── 7  Exit
```

### 11.2 إزاي الـ Menu شغال؟

كل قائمة عبارة عن **`do { ... } while (choice != Back)`** جوه **`do { ... } while (mainChoice != 7)`**:

```
do {                                    ← الـ Main Menu
    اطبع القائمة  →  اقرا mainChoice
    if (mainChoice == 1) {
        do {                            ← الـ Sub Menu
            اطبع القائمة الفرعية → اقرا choice
            نفّذ العملية
        } while (choice != 7);          ← لحد ما يختار "Back"
    }
    else if (mainChoice == 2) { ... }
    ...
} while (mainChoice != 7);              ← لحد ما يختار "Exit"
```

### 11.3 `read_input(int& value)`

```cpp
void read_input(int& value)
{
    if (!(cin >> value))
    {
        cerr << "Input error: expected an integer." << endl;
        std::exit(EXIT_FAILURE);
    }
}
```
بتقرا رقم صحيح. لو المستخدم كتب حروف أو أي حاجة مش رقم البرنامج **بيقفل برسالة خطأ** بدل ما يدخل في loop لا نهائي.

### 11.4 إزاي الـ Set Operations بتتنفذ في `main()`؟

```
1. اسأل: "second array capacity"   →  Dynamic_Array_Number arr2(size2, 0, nullptr)
2. اسأل: "How many numbers?"       →  n
3. كرر n مرة:  اقرا رقم  →  arr2.append(value)
4. arr.merge(arr2)   (أو union / intersection / difference)
5. اطبع النتيجة  arr.display()
6. arr2 بتتمسح أوتوماتيك (الـ destructor) أول ما الـ block يخلص
```

---

## 12. جدول التعقيد Big-O

`n` = عدد عناصر الـ array، `m` = عدد عناصر الـ array التانية.

| العملية | Static | Dynamic |
|---|:-:|:-:|
| `get` / `set` | O(1) | O(1) |
| `append` | O(1) | O(1) متوسط (Amortized)، O(n) في لحظة الـ resize |
| `insert` | O(n) | O(n) |
| `insert_sorted` | O(n) | O(n) |
| `delete_index` | O(n) | O(n) |
| `linear_search` | O(n) | O(n) |
| `binary_search` | O(log n) | O(log n) |
| `max` / `min` / `sum` / `average` | O(n) | O(n) |
| `reverse` / `is_sorted` / `rearrange` | O(n) | O(n) |
| `merge` / `union` / `intersection` / `difference` | O(n + m) | O(n + m) |
| `display` | O(n) | O(n) |

---

## 13. مثال تشغيل حقيقي

نفس الخطوات على النسختين: عمل array صغيرة ثم إضافة أرقام بـ `Append`.

**Dynamic** (السعة الابتدائية `2`، أضفنا `10, 20, 30, 40, 50`):

```
Enter Starting Capacity: 2
Number added successfully.      ← 10
Number added successfully.      ← 20
Number added successfully.      ← 30   (resize: 2 → 4)
Number added successfully.      ← 40
Number added successfully.      ← 50   (resize: 4 → 8)

Array Capacity = 8
Array Length   = 5
Array: 10 20 30 40 50
```

**Static** (السعة `3`، حاولنا نضيف `10, 20, 30, 40`):

```
Enter Array Capacity: 3
Number added successfully.      ← 10
Number added successfully.      ← 20
Number added successfully.      ← 30
Array is full!                  ← 40  ❌

Array Capacity = 3
Array Length   = 3
Array: 10 20 30
```

---

## 14. ملاحظات من مراجعة الكود

راجعت الملفين بالكامل وجربتهم. **الكودين بيتعملهم compile من غير أي warnings** مع `-Wall -Wextra`، وشغّلت العمليات (الأساسية والبحث والحسابية والـ Set Operations والـ copy/assignment والحالات الحدّية زي الـ array الفاضية) مع `AddressSanitizer` و `UndefinedBehaviorSanitizer` ومفيش memory leaks أو أخطاء في الذاكرة. المنطق صح، والنتايج في الأنيميشن اللي فوق هي مخرجات حقيقية من الكود.

الملاحظات دي **تحسينات**، مش أخطاء بتكسر البرنامج:

| # | الملاحظة | التفاصيل واقتراح الحل |
|:-:|---|---|
| 1 | **`delete_index` بترجّع `-1` للخطأ** | لو القيمة المحذوفة نفسها `-1`، البرنامج هيطبع `Invalid index!` غلط (جربتها وبتحصل). الحل: نفس أسلوب `get`، بـ `bool delete_index(int index, int& removed)`. |
| 2 | **`#include` ناقصين** | الكود بيستخدم `std::swap` و `std::min` من غير `#include <algorithm>` / `<utility>`. اشتغل على g++، بس ممكن يفشل على compilers تانية. الحل: ضيف `#include <algorithm>`. |
| 3 | **الشرط "مرتبة" مش متحقق منه** | `binary_search` و `insert_sorted` و `merge` و `union_array` و `intersection` و `difference` بيدّوا نتيجة غلط **من غير أي تحذير** لو الـ array مش مرتبة. الحل: نادي `is_sorted()` الأول أو اطبع تحذير. |
| 4 | **اسم `Static_Array`** | الـ class بيحجز ذاكرته بـ `new` (Heap) وكمان `merge` و `union_array` بيغيروا السعة. فهي أقرب لـ "Fixed-Capacity Array" منها لـ Static بالمعنى الحرفي. مجرد ملاحظة في التسمية. |
| 5 | **قيم بتضيع بصمت في Set Operations (Static)** | لو `arr2` سعتها أقل من `n`، `arr2.append(value)` بتفشل وقيمتها مبتتخزنش، والنتيجة بترجع من غير ما حد يعرف. الحل: اطبع رسالة لو `append` رجّعت `0`. |
| 6 | **`max()` و `min()` بيرجعوا `-1` للـ array الفاضية** | ممكن تتلخبط مع قيمة حقيقية. في `main()` ده متأمّن بفحص `get_length() == 0`، بس لو استخدمت الـ class في مكان تاني خلّي بالك. |
| 7 | **الـ Assignment Operator مش Exception-Safe** | بيعمل `delete[]` **قبل** `new[]`، فلو `new` فشلت الـ object بيبقى في حالة بايظة. الحل المعروف: **Copy-and-Swap idiom**. |
| 8 | **رسالة الخروج** | في `Dynamic_Array.cpp` الرسالة بتقول `Array App` مش `Dynamic Array App` — ممكن توحّدها مع الترحيب. |

### 💡 أفكار للتطوير

- `std::vector` هو النسخة الجاهزة من الـ Dynamic Array (بيستخدم نفس الفكرة بالظبط) — مفيد تقارن بيه كودك.
- تحويل الـ class لـ **template** (`template <typename T>`) عشان يشتغل مع `double` و `string` مش `int` بس.
- إضافة `operator[]` عشان تكتب `arr[2]` بدل `arr.get(2, x)`.
- إضافة `shrink_to_fit()` للـ Dynamic لما الـ array تبقى كبيرة وفاضية أغلبها.
- إضافة Unit Tests (مثلاً بـ Catch2 أو GoogleTest).
