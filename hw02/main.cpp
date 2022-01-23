/* 基于智能指针实现双向链表 */
#include <iostream>
#include <cstdio>
#include <memory>

template <typename T>
struct Node {
    // 这两个指针会造成什么问题？请修复
    std::shared_ptr<Node> next;
    //std::shared_ptr<Node> prev;
    std::weak_ptr<Node> prev; 
    // 如果能改成 unique_ptr 就更好了!
    

    T value;

    // 这个构造函数有什么可以改进的？
    //Node(int val) {
    //    value = val;
    //}
    
   // 防止隐式类型转化 
    explicit Node(const T val) : value(val) {}
    
    void insert(T val) {
        auto node = std::make_shared<Node>(val);
        node->next = next;
        node->prev = prev;
        if (!prev.expired())
            prev.lock()->next = node;
        if (next)
            next->prev = node;
    }

    void erase() {
        if (!prev.expired())
            prev.lock()->next = next;
        if (next)
            next->prev = prev;
    }

    ~Node() {
        printf("~Node()\n");   // 应输出多少次？为什么少了？
    }
};

template <typename T>
struct List {
    std::shared_ptr<Node<T>> head;

    List() = default;

    List(List const &other) {
        printf("List 被拷贝！\n");
        //head = other.head;  // 这是浅拷贝！
        // 请实现拷贝构造函数为 **深拷贝**
     
        auto other_cur = other.head;
        head = std::make_shared<Node<T>>(other_cur->value);
        other_cur = other_cur->next;
        std::shared_ptr<Node<T>> cur = head;

        while (other_cur) {
            auto tmp = std::make_shared<Node<T>>(other_cur->value);
            cur->next = tmp;
            tmp->prev = cur;
            cur = tmp;
            other_cur = other_cur->next;
        }
    }

    List &operator=(List const &) = delete;  // 为什么删除拷贝赋值函数也不出错
    // 编译器会自动调用移动赋值函数

    List(List &&) = default;
    List &operator=(List &&) = default;

    Node<T> *front() const {
        return head.get();
    }

    T pop_front() {
        int ret = head->value;
        head = head->next;
        return ret;
    }

    void push_front(int value) {
        auto node = std::make_shared<Node<T>>(value);
        node->next = head;
        if (head)
            head->prev = node;
        head = node;
    }

    Node<T> *at(size_t index) const {
        auto curr = front();
        for (size_t i = 0; i < index; i++) {
            curr = curr->next.get();
        }
        return curr;
    }

};

template <typename T>
void print(const List<T>& lst) {  // 有什么值得改进的？
    printf("[");
    for (auto curr = lst.front(); curr; curr = curr->next.get()) {
      //  printf(" %d", curr->value);
        std::cout << " " << curr->value;
    }
    printf(" ]\n");
}

int main() {
    List<int> a;

    a.push_front(7);
    a.push_front(5);
    a.push_front(8);
    a.push_front(2);
    a.push_front(9);
    a.push_front(4);
    a.push_front(1);

    print(a);   // [ 1 4 9 2 8 5 7 ]

    a.at(2)->erase();

    print(a);   // [ 1 4 2 8 5 7 ]

    List<int> b = a;

    a.at(3)->erase();

    print(a);   // [ 1 4 2 5 7 ]
    print(b);   // [ 1 4 2 8 5 7 ]

    b = {};
    a = {};

    return 0;
}
