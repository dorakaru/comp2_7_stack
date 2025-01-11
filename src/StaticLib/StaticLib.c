#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"


// mem_size の容量でスタック用のメモリを確保する
void initialize(STACK* s, size_t mem_size)
{
	if (s == NULL) return;

	s->stack_memory = malloc(mem_size);
	if (s->stack_memory == NULL)
	{
		s->end = NULL;
		s->stack_pointer = NULL;
		return;
	}

	s->end = (int*)&s->stack_memory[mem_size / sizeof(int)];
	s->stack_pointer = s->end;
}


// 確保したメモリを解放する
void finalize(STACK* s)
{
	if (s == NULL)
		return;

	free(s->stack_memory);
	s->stack_pointer = s->end = s->stack_memory = NULL;
}


// valの値をスタックに積む。実行の成否を返す
bool push(STACK* s, int val)
{
	if (s == NULL || s->stack_memory == NULL)
		return false;

	if (s->stack_pointer < s->stack_memory + 1)
	{
		return false; //格納するメモリがない
	}

	*(--s->stack_pointer) = val;

	return true;
}


// addrから始まるnum個の整数をスタックに積む。実行の成否を返す
bool push_array(STACK* s, int* addr, int num)
{
	if (s == NULL || s->stack_memory == NULL || num <= 0)
		return false;

	if (s->stack_pointer < s->stack_memory + num)
	{
		return false; //格納するメモリが足りない
	}

	s->stack_pointer -= num;
	memcpy(s->stack_pointer, addr, num * sizeof(int));

	return true;
}

// スタックから一つの要素を取り出す
int pop(STACK* s)
{
	if (s == NULL || s->stack_memory == NULL)
		return 0; // 不具合時は0を返す

	if (s->end < s->stack_pointer + 1)
		return 0; //データが積まれていない

	int val = *s->stack_pointer++;

	return val;
}

// addrにスタックからnumの要素を取り出す。取り出せた個数を返す
int pop_array(STACK* s, int* addr, int num)
{
	if (s == NULL || s->stack_memory == NULL || num <= 0)
		return 0;

	int n = (s->end < s->stack_pointer + num) ? (int)(s->end - s->stack_pointer) : num;

	memcpy(addr, s->stack_pointer, n * sizeof(int));
	s->stack_pointer += n;

	return n;
}