# Data Structures From Scratch - C++

**English:** This repository is a set of standalone, interactive C++17 programs
that implement data structures and algorithms from scratch. Each `.cpp` file
has its own `main()` function: compile and run one source file at a time.

**العربية:** المشروع مجموعة برامج تفاعلية مستقلة بلغة C++17 لشرح هياكل البيانات
والخوارزميات وتنفيذها من الصفر. كل ملف `.cpp` فيه `main()` خاص به؛ ترجم وشغّل
ملفاً واحداً في كل مرة، ولا تجمع الملفات كلها في برنامج واحد.

> **Scope / النطاق:** This guide covers the `Cpp` directory only. It does not
> document or modify the Python examples. / الدليل ده خاص بمجلد `Cpp` فقط،
> ومش بيشرح أو يغيّر أمثلة Python.

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

**Project tools / أدوات المشروع:** [Animated GIF index](./animations/README.md)
| [VS Code build and debug guide](./.vscode/README.md)

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

**These are real animated GIF files**, not text diagrams. Each animation
repeats a small example one algorithm step at a time. Watch the highlighted
step and data state; the folder README explains what changed and why.
**دي ملفات GIF متحركة بالفعل**، مش رسومات نصية. كل Animation بتكرر مثال صغير
خطوة بخطوة. تابع رقم الخطوة وحالة البيانات، وبعدها اقرأ شرح المجلد لمعرفة
إيه اللي اتغير وليه.

![Animated walkthrough: graph BFS](./animations/graph.gif)

| Animation | Animated topic |
|---|---|
| [Array](./animations/array.gif) | Insert by shifting elements |
| [Balanced tree](./animations/balanced-tree.gif) | Treap rotation |
| [Bitset](./animations/bitset.gif) | Set and flip bits |
| [B-Tree](./animations/btree.gif) | Split a full node |
| [Cache](./animations/cache.gif) | LRU access and eviction |
| [Circular buffer](./animations/circular-buffer.gif) | Wrap the write index |
| [Disjoint set](./animations/disjoint-set.gif) | Find, path compression, and union |
| [Graph](./animations/graph.gif) | BFS queue and visit order |
| [Hash table](./animations/hashtable.gif) | Resolve a collision by chaining |
| [Heap](./animations/heap.gif) | Min-heap sift-up |
| [Interval tree](./animations/interval-tree.gif) | Prune an impossible subtree |
| [Linked list](./animations/linked-list.gif) | Insert a node by reconnecting links |
| [Probabilistic](./animations/probabilistic.gif) | Bloom Filter bit updates |
| [Queue](./animations/queue.gif) | FIFO enqueue and dequeue |
| [Range query](./animations/range-query.gif) | Fenwick point update |
| [Skip list](./animations/skip-list.gif) | Move across search levels |
| [Stack](./animations/stack.gif) | Push and pop (LIFO) |
| [Text](./animations/text.gif) | Sort the suffixes of “banana” |
| [Tree](./animations/tree.gif) | AVL rotation |
| [Trie](./animations/trie.gif) | Shared prefix and word endings |

**Note / ملاحظة:** The GIFs are illustrative walkthroughs of small examples;
they do not execute or capture the C++ programs. The programs themselves show
their output in the terminal. / ملفات GIF بتشرح أمثلة صغيرة للتوضيح، ومش
بتشغّل أو بتسجّل برامج C++ نفسها. ناتج البرامج بيظهر في الـ Terminal.

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

**How to learn / إزاي تبدأ التعلّم؟**

1. Read the folder README first, then open the listed `.cpp` file beside it.
   / اقرأ README المجلد الأول، وبعدها افتح ملف `.cpp` المذكور جنبه.
2. Find the class and its member data; identify what each node, index, pointer,
   or counter represents. / دور على الـ class والبيانات اللي جواه، وافهم كل
   Node أو index أو pointer أو counter بيمثل إيه.
3. Trace one operation manually using the GIF. Match every animation step to
   the corresponding method in the source. / تابع عملية واحدة على الورق
   باستخدام الـ GIF، واربط كل خطوة بالـ method المنفذة لها في الكود.
4. Run the program, choose the same operation from its menu, then test an
   empty, typical, and boundary case. / شغّل البرنامج واختار نفس العملية من
   القائمة، وجرّب حالة فاضية وعادية وحالة على الحدود.

**Glossary / مصطلحات:** A *node* (`عقدة`) stores a value and links or metadata.
*Capacity* (`السعة`) is available storage; *length/size* (`عدد العناصر`) is
the number currently stored. A *traversal* (`اجتياز`) visits elements in an
order. An *invariant* (`خاصية ثابتة`) is a rule the structure must preserve
after each operation, such as sorted keys or heap order.

## التحقق

تم فحص ملفات C++ الـ42 في هذا المستودع بنجاح باستخدام:

```powershell
g++ -std=c++17 -fsyntax-only <file.cpp>
```

هذا يؤكد سلامة الترجمة، لكنه لا يغني عن تجربة القوائم والعمليات بمدخلات مختلفة.
