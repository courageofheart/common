
#ifndef __GTL_STACK_H_
#define __GTL_STACK_H_

#include "common/gtl_doublelist.h"

#include <stdlib.h>

typedef STDLinkNode STStackNode;

typedef STDLinkHead STStack;

#ifdef __cplusplus
extern "C"
{
#endif
	STStack * stack_init();

	STStackNode * stack_pop(STStack *pstStack);

	void stack_push(STStack *pstStack, STStackNode *pstNode);

	STStackNode * stack_top(STStack *pstStack);

	int stack_empty(STStack *pstStack);

	size_t stack_size(STStack *pstStack);

#ifdef __cplusplus
}
#endif

#endif







