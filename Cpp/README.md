# Data Structures From Scratch - C++

مجموعة تطبيقات تعليمية لهياكل البيانات والخوارزميات بلغة C++. كل ملف `.cpp`
برنامج مستقل يحتوي على `main()` وقائمة تفاعلية خاصة به؛ شغّل ملفاً واحداً في كل
مرة، ولا تجمع كل الملفات في أمر ترجمة واحد.

> النطاق هنا هو مجلد C++ فقط. ملفات Python خارج نطاق هذا الدليل والتغييرات.

## المحتويات

| المجلد | ما ستتعلمه | الملفات |
|---|---|---|
| [Array](./Array/README.md) | المصفوفات الثابتة والديناميكية وعملياتها | `Static_Array.cpp`, `Dynamic_Array.cpp` |
| [BalancedTree](./BalancedTree/README.md) | شجرتا Red-Black وTreap | `Red_Black_Tree.cpp`, `Treap.cpp` |
| [Bitset](./Bitset/README.md) | تخزين مجموعات البتات وعملياتها | `Bitset.cpp` |
| [BTree](./BTree/README.md) | B-Tree وB+ Tree | `B_Tree.cpp`, `B_Plus_Tree.cpp` |
| [Cache](./Cache/README.md) | ذاكرة LRU المؤقتة | `LRU_Cache.cpp` |
| [CircularBuffer](./CircularBuffer/README.md) | مخزن دائري محدود السعة | `Circular_Buffer.cpp` |
| [DisjointSet](./DisjointSet/README.md) | Union-Find والمجموعات المتصلة | `Disjoint_Set.cpp` |
| [Graph](./Graph/README.md) | الرسوم البيانية وBFS وDFS وDijkstra | `Graph.cpp` |
| [HashTable](./HashTable/README.md) | التجزئة بالسلاسل والعنونة المفتوحة وCuckoo | ثلاثة ملفات |
| [Heap](./Heap/README.md) | أكوام الأولوية والـ indexed heap | أربعة ملفات |
| [IntervalTree](./IntervalTree/README.md) | البحث عن الفترات المتداخلة | `Interval_Tree.cpp` |
| [LinkedList](./LinkedList/README.md) | القوائم المرتبطة الأحادية والمزدوجة والدائرية | ثلاثة ملفات |
| [Probabilistic](./Probabilistic/README.md) | Bloom Filter وCounting Bloom Filter | ملفان |
| [Queue](./Queue/README.md) | الطابور الثابت والديناميكي وطابور القوائم وDeque | أربعة ملفات |
| [RangeQuery](./RangeQuery/README.md) | Fenwick وSegment وSparse Table | ثلاثة ملفات |
| [SkipList](./SkipList/README.md) | قائمة مرتبة بطبقات بحث | `Skip_List.cpp` |
| [Stack](./Stack/README.md) | المكدس الثابت والديناميكي ومكدس القائمة | ثلاثة ملفات |
| [Text](./Text/README.md) | Rope وSuffix Array وSuffix Automaton | ثلاثة ملفات |
| [Tree](./Tree/README.md) | Binary Tree وBST وAVL | ثلاثة ملفات |
| [Trie](./Trie/README.md) | تخزين الكلمات والبحث بالبادئة | `Trie.cpp` |

## المتطلبات والتشغيل

- مترجم C++ يدعم C++17، مثل GCC أو Clang أو MSVC.
- لا توجد مكتبات خارجية مطلوبة لهذه الأمثلة.
- تُترجم الملفات منفردة؛ مثال باستخدام GCC من مجلد هذا الملف:

```powershell
g++ -std=c++17 -Wall -Wextra .\Graph\Graph.cpp -o .\Graph\Graph.exe
.\Graph\Graph.exe
```

استبدل `Graph\Graph.cpp` بمسار الملف الذي تريد تجربته. على Windows يمكن أيضاً
استخدام `g++ -fsyntax-only` لفحص الترجمة دون إنشاء ملف تنفيذي.

معظم البرامج تفاعلية: ابدأ بإنشاء البنية أو إدخال قيم، ثم اختر عملية من القوائم
التي تظهر في الطرفية. اتبع ترتيب الرسائل؛ الفهارس في الشروح تبدأ من الصفر ما لم
تذكر العملية غير ذلك.

## كيف تقرأ الشروح المتحركة؟

كل رسم متتابع هو لقطة لحالة البنية بعد خطوة من الخوارزمية. تابع المؤشرات أو
العقد التي تغيّر موضعها، ثم راقب كيف تصبح الحالة النهائية:

```text
قبل:       [10] -> [20] -> [30] -> nullptr
إضافة 25:  [10] -> [20] -> [30] -> [25] -> nullptr
                         ^ ربط العقدة الجديدة بنهاية القائمة
```

الرسوم توضيحية لقيم صغيرة وليست نوافذ رسومية متحركة داخل البرنامج. ملفات C++
تعرض الحالة والنتائج من خلال قوائم الطرفية.

## مصطلحات وتعقيد

- `n`: عدد العناصر، `h`: ارتفاع الشجرة، `V`: عدد الرؤوس، `E`: عدد الحواف.
- `O(1)` ثابت، `O(log n)` لوغاريتمي، `O(n)` خطي، و`O(n log n)` شبه خطي.
- التعقيد المذكور في ملفات المجلدات هو المعتاد للبنية؛ قد تختلف بعض العمليات
  عند امتلاء السعة، أو إعادة بناء الجدول، أو في أسوأ حالة.
- هياكل البيانات المكتوبة يدوياً هنا تعليمية؛ ليست بديلاً تلقائياً عن حاويات
  المكتبة القياسية مثل `std::vector` و`std::map`.

## الملكية والنسخ

بعض التطبيقات تدير الذاكرة يدوياً باستخدام مؤشرات، وتعرّف destructor وعمليات
نسخ عميق لتفادي مشاركة الذاكرة المملوكة بين نسختين. لا تفترض أن كل ملف يشترك في
تنفيذ أو واجهة موحدة؛ كل برنامج مثال مستقل بواجهته الخاصة.

## التحقق

تم فحص ملفات C++ الـ42 في هذا المستودع بنجاح باستخدام:

```powershell
g++ -std=c++17 -fsyntax-only <file.cpp>
```

هذا يؤكد سلامة الترجمة، لكنه لا يغني عن تجربة القوائم والعمليات بمدخلات مختلفة.
