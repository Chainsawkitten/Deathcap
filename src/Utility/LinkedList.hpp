#pragma once

namespace Utility {
    //TMPTODO
    template<typename T> class LinkedList {
        public:
            /// Constructor that defaults to referencing no data.
            LinkedList() = default;

            //TMPTODO
            void Push(T val);

            //TMPTODO
            T Pop();

            //TMPTODO
            bool Empty();

        private:
            struct Node {
                T val;
                Node* next = nullptr;
            };

            Node* front = nullptr;
            Node* back = nullptr;
    };

    template<typename T> void LinkedList<T>::Push(T val) {
        Node node = new Node();
        node->val = val;
        if (front)
            front->next = node;
        else
            back = node;
        front = node;
    }

    template<typename T> T LinkedList<T>::Pop() {
        Node* node = back;
        if (node)
            back = node->next;
        else
            back = nullptr;
        return node;
    }

    template<typename T> bool LinkedList<T>::Empty() {
        return back == nullptr;
    }
}
