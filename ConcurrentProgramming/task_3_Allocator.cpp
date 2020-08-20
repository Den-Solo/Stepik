#include <iostream>

#include <string>
#include <memory>
#include <stdlib.h>


class SmallAllocator {
private:
    char* Memory;
    int offset_free{ 0 };

public:
    SmallAllocator()
        : Memory(new char[1048576])
    {
        //memset(Memory, 0, 1048576);
    }
    ~SmallAllocator() {
        delete[] Memory;
    }
    int& size(void* p) {
        return *(int*)((char*)p - 4);
    }
    void* Alloc(unsigned int Size) {
        *(int*)(Memory + offset_free) = Size;
        int offset = offset_free;
        offset_free += Size + 4;
        return Memory + offset + 4;
    };
    void* ReAlloc(void* Pointer, unsigned int Size) {
        int& sz = size(Pointer);

        if (Memory + offset_free == (char*)Pointer + sz) {
            offset_free += Size - sz;
            sz = Size;
            return Pointer;
        }
        
        void* new_data = Alloc(Size);
        memcpy(new_data, Pointer, sz);
        return new_data;
    };
    void Free(void* Pointer) {
        int& sz = size(Pointer);
        if (Memory + offset_free == (char*)Pointer + sz) {
            offset_free -= (sz + 4);
        }
    };
};

int main() {
    SmallAllocator A1;
    int* A1_P1 = (int*)A1.Alloc(sizeof(int));
    A1_P1 = (int*)A1.ReAlloc(A1_P1, 2 * sizeof(int));
    A1.Free(A1_P1);
    SmallAllocator A2;
    int* A2_P1 = (int*)A2.Alloc(10 * sizeof(int));
    for (unsigned int i = 0; i < 10; i++) A2_P1[i] = i;
    for (unsigned int i = 0; i < 10; i++) if (A2_P1[i] != i) std::cout << "ERROR 1" << std::endl;
    int* A2_P2 = (int*)A2.Alloc(10 * sizeof(int));
    for (unsigned int i = 0; i < 10; i++) A2_P2[i] = -1;
    for (unsigned int i = 0; i < 10; i++) if (A2_P1[i] != i) std::cout << "ERROR 2" << std::endl;
    for (unsigned int i = 0; i < 10; i++) if (A2_P2[i] != -1) std::cout << "ERROR 3" << std::endl;
    A2_P1 = (int*)A2.ReAlloc(A2_P1, 20 * sizeof(int));
    for (unsigned int i = 10; i < 20; i++) A2_P1[i] = i;
    for (unsigned int i = 0; i < 20; i++) if (A2_P1[i] != i) std::cout << "ERROR 4" << std::endl;
    for (unsigned int i = 0; i < 10; i++) if (A2_P2[i] != -1) std::cout << "ERROR 5" << std::endl;
    A2_P1 = (int*)A2.ReAlloc(A2_P1, 5 * sizeof(int));
    for (unsigned int i = 0; i < 5; i++) if (A2_P1[i] != i) std::cout << "ERROR 6" << std::endl;
    for (unsigned int i = 0; i < 10; i++) if (A2_P2[i] != -1) std::cout << "ERROR 7" << std::endl;
    A2.Free(A2_P1);
    A2.Free(A2_P2);
}