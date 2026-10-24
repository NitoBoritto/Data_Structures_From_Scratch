# Circular Buffer

**English:** Reuse a fixed array by wrapping read/write indices instead of
shifting every element. **بالعربي:** أعد استخدام خانات مصفوفة ثابتة عن طريق
تدوير مؤشري القراءة والكتابة من غير نقل باقي العناصر.

## Animated walkthrough | شرح متحرك

![Circular buffer animation | التفاف المؤشر](../animations/circular-buffer.gif)

Follow the write index reaching the last slot, then wrapping to index zero
after a removal frees space. تابع مؤشر الكتابة لحد آخر خانة، وبعد إزالة عنصر
لفّه ورجوعه لأول خانة. ترتيب العناصر المنطقي مش شرط يطابق ترتيب الخانات.

## Read the source step by step | اقرأ الكود بالترتيب

1. `front` points to the oldest stored value. `front` بيشاور على أقدم قيمة.
2. `length` tracks occupied slots; capacity is not the number of live items.
   `length` بيحسب العناصر الموجودة، والسعة هي عدد الخانات الكلي.
3. Advance indices modulo capacity to wrap from the last slot to zero.
   المؤشر بيتحدّث بباقي القسمة عشان يرجع للصفر بعد آخر خانة.
4. This implementation rejects insertion when full; it does not overwrite
   old data. التنفيذ بيرفض الإضافة وقت الامتلاء بدل ما يكتب فوق بيانات قديمة.

**Practice / تدريب:** Use capacity 4, enqueue four values, dequeue one, and
enqueue a fifth. Trace both the logical order and physical slot indices.
جرّب سعة 4، أضف أربع قيم، احذف واحدة، وأضف خامسة. تابع ترتيب القراءة وأرقام
الخانات الفعلية.

`Circular_Buffer.cpp` يطبق مخزناً محدود السعة يلتف مؤشره إلى البداية بعد آخر
خانة. يحافظ على ترتيب الإدخال من الأقدم إلى الأحدث؛ الإضافة إلى مخزن ممتلئ
تفشل بدلاً من الكتابة فوق البيانات الموجودة.

## العمليات

الإضافة إلى الخلف، إزالة/قراءة العنصر الأقدم، قراءة المقدمة دون إزالة، التحقق
من الامتلاء أو الفراغ، العرض، والمسح. السعة محددة عند إنشاء المخزن.

### 🎬 التفاف المؤشر

```text
الخانات:       [ A ][ B ][ C ][ _ ]
مؤشر الكتابة:                  ^
إضافة D:       [ A ][ B ][ C ][ D ]   (أصبح ممتلئاً)
إزالة A ثم إضافة E:
               [ E ][ B ][ C ][ D ]
مؤشر الكتابة:      ^
الترتيب المنطقي:   B -> C -> D -> E
```

الإضافة والإزالة والقراءة `O(1)`، والعرض `O(n)`، والمساحة `O(capacity)`.

## التشغيل

```powershell
g++ -std=c++17 -Wall -Wextra .\Circular_Buffer.cpp -o .\circular_buffer.exe
.\circular_buffer.exe
```
