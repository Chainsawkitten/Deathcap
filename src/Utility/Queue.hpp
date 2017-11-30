#pragma once

namespace Utility {
    /// Queue container class.
    template<typename T> class Queue {
        public:
            /// Constructor.
            Queue() = default;

            /// Push value at front of queue.
            /**
             * @param val Value to push.
             */
            void Push(const T& val);

            /// Get value at front of queue.
            /**
             * @return Value at front of queue.
             */
            T* Front();

            /// Get value at back of queue.
            /**
             * @return Value at back of queue.
             */
            T* Back();

            /// Remoce value at front of queue.
            void Pop();

            /// Check if queue is empty.
            /**
            * @return Wether queue is empty or not.
            */
            bool Empty();

            /// Iterate over all values in queue.
            /**
             * @return Value from queue (nullptr if end).
             */
            T* Iterate();

        private:
            struct Node {
                Node() = default;
                T val;
                Node* forward = nullptr;
                Node* backward = nullptr;
            };

            Node* front = nullptr;
            Node* back = nullptr;
            Node* iterator = nullptr;
            unsigned int count = 0;
    };

    template<typename T> void Queue<T>::Push(const T& val) {
        Node* node = new Node();
        node->val = val;
        if (Empty()) {
            front = node;
            back = node;
        } else {
            back->backward = node;
            node->forward = back;
            back = node;
        }
        ++count;
    }

    template<typename T> T* Queue<T>::Front() {
        return front ? &front->val : nullptr;
    }

    template<typename T> T* Queue<T>::Back() {
        return back ? &back->val : nullptr;
    }

    template<typename T> void Queue<T>::Pop() {
        if (!Empty()) {
            Node* node = front;
            front = node->backward;
            if (front)
                front->forward = nullptr;
            else
                back = nullptr;
            delete node;
            --count;
        }
    }

    template<typename T> bool Queue<T>::Empty() {
        return front == nullptr;
    }

    template<typename T> T* Queue<T>::Iterate() {
        if (!iterator)
            iterator = front;
        else
            iterator = iterator->backward;
        return iterator ? &iterator->val : nullptr;
    }
}
