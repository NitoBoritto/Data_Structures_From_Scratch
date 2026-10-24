# Trees

**English:** Compare a binary tree, a binary search tree, and a self-
balancing AVL tree. **بالعربي:** قارن بين الشجرة الثنائية وBST وشجرة AVL
ذاتية التوازن.

## Animated walkthrough | شرح متحرك

![AVL rotation animation | دوران AVL](../animations/tree.gif)

Inserting 10, 20, 30 creates a right-right imbalance; a left rotation repairs
it. إضافة القيم بتعمل عدم توازن ناحية اليمين؛ الدوران لليسار بيصلحه.

## Read the source step by step | اقرأ الكود بالترتيب

1. Binary Tree defines parent/child relationships but not key ordering.
   الشجرة الثنائية بتحدد علاقة الأب والأبناء، مش ترتيب القيم.
2. BST sends smaller keys left and larger keys right. BST بتحط الأصغر شمال
   والأكبر يمين.
3. Inorder traversal of a BST visits keys in ascending order. `Inorder`
   بتعرض قيم BST تصاعدياً.
4. AVL tracks subtree heights and rotates after an imbalance. AVL بتتابع
   ارتفاع الفروع وبتدور بعد اختلال التوازن.
5. A full traversal still costs `O(n)` because it visits every node. المرور
   الكامل تكلفته `O(n)` لأنه بيزور كل العقد.

**Try / جرّب:** Insert 10, 20, 30 in BST and AVL; compare tree shapes, then
search for 30. أضف القيم في BST وAVL وقارن الشكل، وبعدها ابحث عن 30.

تطبيقات أشجار ثنائية عامة، وشجرة بحث ثنائية، وشجرة AVL ذاتية التوازن.

## الملفات

- `Binary_Tree.cpp` — إدراج/حذف وعروض traversal ومعلومات الشجرة؛ لا يشترط
  ترتيب BST بين القيم.
- `Binary_Search_Tree.cpp` — البحث عن قيمة أصغر يساراً وأكبر يميناً، مع
  عمليات الإضافة والحذف والاستعلام.
- `AVL_Tree.cpp` — BST توازن ارتفاع الفرعين بعد التعديلات باستخدام الدورانات.

### 🎬 Traversal على شجرة

```text
       4
      / \
     2   6
    / \
   1   3

Inorder (يسار، جذر، يمين): 1, 2, 3, 4, 6
Preorder (جذر، يسار، يمين): 4, 2, 1, 3, 6
Postorder (يسار، يمين، جذر): 1, 3, 2, 6, 4
```

في BST يعطي inorder القيم بترتيب تصاعدي. إن لم تكن الشجرة متوازنة فقد يصل
ارتفاعها إلى `n`.

## التعقيد

BST: العمليات `O(h)`؛ `O(log n)` متوازنة و`O(n)` أسوأ حالة. AVL تحافظ على
`h = O(log n)`، لذلك البحث والإضافة والحذف `O(log n)`. المرور على كل العقد
`O(n)`، والذاكرة `O(n)`.

## التشغيل

```powershell
g++ -std=c++17 -Wall -Wextra .\AVL_Tree.cpp -o .\avl_tree.exe
.\avl_tree.exe
```
