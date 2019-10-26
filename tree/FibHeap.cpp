#include <stdio.h>
#include <cmath>
#include <algorithm>
#define MIN -2147483648
struct Node {
    Node* child;
    Node* p;
    Node* left;
    Node* right;
    int degree;
    int key;
    bool mark;
    Node(int key): key(key) {
        degree = mark = 0;
        child = p = NULL;
        left = right = this;
    }
};

class FibHeap {
public:
    FibHeap(int initialSize) {
        maxSize = initialSize;
        cacheSize = ceil(log2(initialSize));
        cache = (Node**) malloc((cacheSize + 1) * sizeof(Node*));
        tmp = (Node**) malloc(maxSize * sizeof(Node*));
    }
    Node* min = NULL;
    int size = 0;
    void push(int key) {
        Node* newNode = new Node(key);
        insert(newNode);
    }
    int peek() {
        if (NULL == min) return MIN;
        return min->key;
    }
    int poll() {
        if (min == NULL) return MIN;
        Node* z = min;
        Node* itr = z->child;
        if (itr != NULL) {
            itr->p = NULL;
            itr = itr->right;
            while (itr != z->child) {
                itr->p = NULL;
                itr = itr->right;
            }
            merge_list(min, z->child);
        }
        remove_list(z);
        if (z == z->right) min = NULL;
        else {
            min = z->right;
            consolidate();
        }
        size--;
        int key = z->key;
        free(z);
        return key;
    }
private:
    int maxSize = 100;
    int cacheSize = 20;
    Node** cache = NULL;
    Node** tmp = NULL;
    void merge_list(Node* x, Node* y) {
        Node* xr = x->right;
        Node* yl = y->left;
        x->right = y;
        y->left = x;
        xr->left = yl;
        yl->right = xr;
    }
    void consolidate() {
        cacheSize = floor(log2(size));
        for (int i = 0; i <= cacheSize; i++) {
            cache[i] = NULL;
        }
        Node* itr = min;
        int count = 0;

        tmp[count++] = itr;
        itr = itr->right;
        while (itr != min) {
            tmp[count++] = itr;
            itr = itr->right;
        }
        for (int i = 0; i < count; i++) {
            Node* x = tmp[i];
            int d = x->degree;
            while (cache[d] != NULL) {
                Node* y = cache[d];
                if (x->key > y->key) std::swap(x, y);
                heap_link(y, x);
                cache[d] = NULL;
                d++;
            }
            cache[d] = x;
        }
        min = NULL;
        for (int i = 0; i <= cacheSize; i++) {
            if (cache[i] == NULL) continue;
            if (min == NULL) {
                min = cache[i];
            } else {
                // 下面这句话不能加上去，加上就错了! 
                // min和cache[i]本来就在同一条链表上。
                // insert_list(min, cache[i]);
                if (cache[i]->key < min->key) {
                    min = cache[i];
                }
            }
        }
    }
    void heap_link(Node* y, Node* x) {
        remove_list(y);
        y->p = x;
        x->degree++;
        y->mark = false;
        y->left = y->right = y;
        if (x->child == NULL) {
            x->child = y;
        } else {
            insert_list(x->child, y);
        }
    }
    void insert(Node* x) {
        if (min == NULL) {
            min = x;
        } else {
            insert_list(min, x);
            if (x->key < min->key) min = x;
        }
        size++;
    }
    void insert_list(Node* y, Node* x) {
        Node* z = y->right;
        y->right = x;
        x->left = y;
        x->right = z;
        z->left = x;
    }
    void remove_list(Node* x) {
        Node* xl = x->left;
        Node* xr = x->right;
        xl->right = xr;
        xr->left = xl;
    }
};
FibHeap* heap_union(FibHeap* x, FibHeap* y) {
    if (x == NULL || y == NULL) return x == NULL ? y : x;
    Node* r = x->min;
    Node* h = y->min;
    if (h == NULL) {
        delete y;
        return x;
    }
    if (r == NULL) {
        delete x;
        return y;
    }
    Node* r1 = r->right;
    Node* h1 = h->left;
    r->right = h;
    h->left = r;
    r1->left = h1;
    h1->right = r1;
    x->size += y->size;
    if (r->key > h->key)
        x->min = h;
    delete y;
    return x;
}

int main()
{
    FibHeap* heap = new FibHeap(100);
    int size = 0;
    int n = 20;
    for (int i = 0; i < n; i++) {
        int type = rand()&1;
        if (type || size == 0) {
            size++;
            int key = rand() % 0xFFFF;
            heap->push(key);
            printf("%d: push %d into tree\n", i, key);
        } else {
            size--;
            printf("%d: poll %d from tree\n", i, heap->poll());
        }
    }
    return 0;
}