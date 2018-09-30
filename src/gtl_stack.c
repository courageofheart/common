
#include "gtl_stack.h"

STStack * stack_init()
{
	STStack * pstStack = NULL;

	pstStack = DLink_Head_Init();

	return pstStack;
}

STStackNode * stack_pop(STStack *pstStack)
{
	STStackNode *pstNode = NULL;

	pstNode = DLink_PopEnd(pstStack);

	return pstNode;
}

void stack_push(STStack *pstStack, STStackNode *pstNode)
{
	DLink_PushBack(pstStack, pstNode);
}

STStackNode * stack_top(STStack *pstStack)
{
	STStackNode *pstNode = NULL;

	pstNode = DLink_Back(pstStack);

	return pstNode;
}

int stack_empty(STStack *pstStack)
{
	return DLink__Empty(pstStack);
}

size_t stack_size(STStack *pstStack)
{
	return DLink__Size(pstStack);
}

