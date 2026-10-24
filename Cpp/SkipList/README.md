# Skip List

**English:** A sorted linked list with express lanes for faster searches.
**بالعربي:** قائمة مترتبة بطبقات روابط بتسمح بتجاوز عناصر وتسريع البحث.

## Animated walkthrough | شرح متحرك

![Skip-list search animation | البحث في Skip List](../animations/skip-list.gif)

Start high, move right while the next key is small enough, and drop a level
before passing the target. ابدأ من فوق، اتحرك يمين لو القيمة مناسبة، وانزل
مستوى قبل ما تعدي الهدف.

## Read the source step by step | اقرأ الكود بالترتيب

1. Level 0 contains every value in sorted order. المستوى صفر فيه كل القيم
   المرتبة.
2. Higher levels add shortcut links over portions of level 0. المستويات
   الأعلى بتضيف وصلات اختصار.
3. Search moves right, then descends when the next value is too large.
   البحث بيتحرك يمين، وبينزل مستوى لما القيمة التالية تبقى أكبر.
4. Insert/remove update the links at every level containing that node.
   الإضافة والحذف بيحدثوا روابط كل المستويات اللي فيها العقدة.
5. Duplicate values are not stored by this implementation. التنفيذ لا يخزن
   قيماً مكررة.

**Expected vs worst case / المتوسط والأسوأ:** Search is `O(log n)` on
average, but may be `O(n)` in the worst case. البحث `O(log n)` في المتوسط
وممكن يبقى `O(n)` في أسوأ حالة.

`Skip_List.cpp` قائمة مرتبة ذات طبقات من الروابط. المستوى الأدنى يحتوي كل
العناصر، والمستويات الأعلى تقفز فوق مجموعات منها لتسريع البحث.

## العمليات

إضافة وحذف وبحث، `lower_bound`، المرور المرتب، المسح، واستعلام عدد العناصر
والمستوى الأعلى. الإدخالات المكررة لا تُخزن.

### 🎬 البحث عبر المستويات

```text
Level 2:  1 ----------------> 19
Level 1:  1 ------> 9 ------> 19
Level 0:  1 -> 4 -> 7 -> 9 -> 12 -> 19
                         ابحث عن 12

من 1 في المستوى الأعلى: 19 أكبر من 12، انزل مستوى
من 9: انتقل إلى 12، تم العثور
```

## التعقيد

البحث والإضافة والحذف `O(log n)` متوقعاً، و`O(n)` في أسوأ حالة؛ المرور المرتب
`O(n)` والذاكرة `O(n)` متوقعاً. عدد المستويات الأقصى مضبوط عند إنشاء القائمة.

## التشغيل

```powershell
g++ -std=c++17 -Wall -Wextra .\Skip_List.cpp -o .\skip_list.exe
.\skip_list.exe
```
