/*
 * 'C' Source Library - Common Utilities - Supported by GNU and Windows VC7.1
 *
 * Copyright (c) 2008-2009 - This is open source software in the pulbic domain.
 *
 * Developed by: Steven J. Hathaway
 *
 * For:  State of Oregon - Oregon State Police / CJIS-LEDS
 *
 * Release: 4/4/2009
 * Changed format of the general k_function item to afford greater utility.
 *
 * Updated: 7/17/2009
 * Added KeyList sorting utilities using helper callback comparison.
 *
 * Updated: 10/3/2009
 * Validated KeyList sorting KMovA optimization - sorting lists of descending order.
 */

/* The Files:
 *
 *		keylist.h		Is the header file for this (keylist) library.
 *		keylist.c		Is the source file for this (keylist) library.
 */

#include "keylist.hpp"

#if defined(__cplusplus)
extern "C"
{
#endif

/* This is a general purpose library for list management.  The lists are 
 * single-link lists of type KeyListEntry.  Sublists are also supported and
 * are treated as the head of a supplemental linked list.  The routines
 * in this library work with structures of KeyListEntry type.
 *
 * The KeyListEntry nodes are allocated with the 'C' malloc subsystem.
 * The nodes are initialized with zero and NULL values.
 *
 * The KeyListEntry structure:
 *
 * node = pointer to next node of KeyListEntry type.
 *
 * k_name = character string that can be used for anything suchas a name.
 *			It is initiallized via strdup() using malloc() memory management
 *			when a string value is assigned.
 *
 * k_id = an arbitrary integer that can be used for anything suchas node ID.
 *
 * k_size = integer that is commonly used to indicate the amount of data 
 *			referenced by the k_data pointer.
 *
 * k_data = pointer to an arbitrary data buffer, usually initialized to zeros
 *			and allocated by calloc() memory management.  An application may
 *			use this pointer for anything, suchas a pointer to a C++ class.
 *
 *			If k_data points to data that should not be deallocated using the
 *			free() function, the application should register a k_destroy
 *			function that is called before free() deallocation is attempted.
 *
 * k_destroy = a 'C' namespace function to cleanup a KeyListEntry node of
 *			unwanted references before k_data is deallocated.  The k_destroy
 *			function must conform to the KeyEntDestroyType definition.
 *			The k_destroy function acts on the content of a KeyListEntry node
 *			before any node data is deallocated or freed.
 *
 *			The k_destroy function can be called with CallKeyEntryDestroy(node).
 *
 * k_function = a 'C' namespace function is any arbitrary function that can
 *			be registered to the KeyListEntry node.  This function must
 *			conform to the KeyEntFunction type definition. It returns a pointer
 *			of type void that can be cast as required.  It can be programmed
 *			to return application-specific data found in or linked to k_data.
 *
 *			The k_function function can be called by the application via
 *			CallKeyEntryFunction(KeyListEntry * node, int argc, void * args[]).
 *
 * k_argc = an integer helper for k_function to cache the argument count.
 *			The CallKeyEntryFunction stores the argc here for use upon return.
 *
 * k_argbuf = a 'C' pointer to an array of arguments (void * args[]).
 *			Allocation is not released. It is a helper for k_function in which
 *			to return multiple values.  The CallKeyEntryFunction stores the
 *			pointer to the argument array for use upon return.  The k_argbuf
 *			array IS NOT AUTOMATICALLY DEALLOCATED.
 *
 *			Memory allocation requirements should be handled by the k_destroy
 *			function.  Otherwise such deallocation is ignored when the node
 *			is dropped.
 *
 * k_list = pointer to a sublist of KeyListEntry type.  The k_list is treated
 *			as the head of a new KeyListEntry list.  When the node containing
 *			this k_list element is dropped, the entire sublist tree is freed.
*/

#if 0
/* These typedef's are found in #include "keylist.h" */

typedef struct key_list_entry
{
  struct key_list_entry * next;
  char * k_name;			// malloc() string content
  int    k_id;				// arbitrary integer value
  size_t k_size;			// size of k_data
  void * k_data;			// malloc() arbitrary data
  void * (* k_function)(struct key_list_entry * theNode,
		int f_argc, void * f_args[]);
  int (* k_destroy)(struct key_list_entry * theNode);
  struct key_list_entry * k_list;	// supplemental Key List
}
KeyListEntry;

typedef void * KeyEntFunctionType (KeyListEntry * theNode, int f_argc, void * f_args[]);
typedef int KeyEntDestroyType  (KeyListEntry * theNode);

#endif /* 0 */

/* ================================================================
 * SetKeyEntryFunction
 *
 * Add or register a helper function to the key list entry.
 * This function can be used to supplement the processing of arbitrary data.
 */

void SetKeyEntryFunction(KeyListEntry * node, KeyEntFunctionType * fcn)
{
	if (node != NULL)
	node->k_function = fcn;
}


/* ================================================================
 * CallKeyEntryFunction
 *
 * This function may do its own allocation to node->k_data
 * and may install any user_defined side affects.  The companion
 * CallKeyEntryDestroy is required to cleanup any side affects
 * before the node is deallocated.  The function can also be used
 * to return application-specific data stored in or linked to
 * the k_data element of the node.
 */

void * CallKeyEntryFunction(KeyListEntry * node, int argc, void * args[])
{
	if (node == NULL)
		return NULL;
	if (node->k_function == NULL)
		return NULL;
	node->k_argc = argc;
	node->k_argbuf = args;
	return (* node->k_function)(node, argc, args);
}


/* ================================================================
 * SetKeyEntryDestroy
 *
 * Add or register a helper function to deallocate references assumed in the
 * aritrary data before the key list entry is itself deallocated.
 */

void SetKeyEntryDestroy(KeyListEntry * node, KeyEntDestroyType * fcn)
{
	if (node != NULL)
	node->k_destroy = fcn;
}


/* ================================================================
 * CallKeyEntryDestroy
 *
 * This function is called before the key list entry node is deallocated.
 * If side affects (suchas pointers to other allocated memory) reside in
 * the k_data element, then the k_destroy function is called to clean up
 * the mess.
 */

int CallKeyEntryDestroy(KeyListEntry * node)
{
	if (node == NULL)
		return 0;
	if (node->k_destroy == NULL)
		return 0;
	return (* node->k_destroy)(node);
}


/* ================================================================
 * SetKeyEntryName
 *
 * Add a character string name to the key list entry.
 * Returning a pointer to the new name allocation.
 */

char * SetKeyEntryName(KeyListEntry * node, const char * name)
{
	if (node == NULL)
		return NULL;
	
	if (node->k_name != NULL)
		free(node->k_name);

	if (name == NULL)
		node->k_name = NULL;
	else
	{
		node->k_name = strdup(name);
	}
	return node->k_name;
}


/* ================================================================
 * GetKeyEntryName
 *
 * Return a pointer to the name string associated with the list node element.
 */

char * GetKeyEntryName(KeyListEntry * node)
{
	if (node == NULL)
		return NULL;
	return node->k_name;
}


/* ================================================================
 * SetKeyEntryIdValue
 *
 * Add an identity value to the Key Entry Node.  The value
 * returned is the value added.
 */

int SetKeyEntryIdValue(KeyListEntry * node, int idval)
{
	if (node == NULL)
		return 0;
	node->k_id = idval;
	return node->k_id;
}


/* ================================================================
 * GetKeyEntryIdValue
 *
 * Return the ID Value integer stored in the list node element.
 */

int GetKeyEntryIdValue(KeyListEntry * node)
{
	if (node == NULL)
		return 0;
	return node->k_id;
}


/* ================================================================
 * GetKeyEntryNext
 *
 * Return a pointer to the next KeyListEntry node.
 */

KeyListEntry *
GetKeyEntryNext(KeyListEntry * node)
{
	if (node == NULL)
		return NULL;
	return node->next;
}


/* ================================================================
 * GetKeyEntrySubList
 *
 * Return a pointer to the first KeyListEntry node in the sublist.
 * Sublists are initialized via the NewKeySubList() function.
 * A sublist is actually the head of a supplemental KeyListEntry list.
 *
 * If you want to transfer memory allocation ownership, you should
 * set node->k_list to NULL after extracting the sublist address.
 */

KeyListEntry *
GetKeyEntrySubList(KeyListEntry * node)
{
	if (node == NULL)
		return NULL;
	return node->k_list;
}


/* ================================================================
 * NewKeyListEntry
 *
 * Create a new empty KeyListEntry element.
 * Return a pointer to the created element.
 */

KeyListEntry * 
NewKeyListEntry()
{
  return (KeyListEntry *) calloc(1, sizeof(KeyListEntry));
}


/* ================================================================
 * NewKeySubList
 *
 * Create a new empty sublist element.
 * Return a pointer to the created element.
 */

KeyListEntry * 
NewKeySubList(KeyListEntry * node)
{
	if (node == NULL)
		return NULL;

	if (node->k_list)
		return NULL;	/* Sub List Already Started */

	node->k_list = (struct key_list_entry *) calloc(1, sizeof(KeyListEntry));
	return node->k_list;
}

/* ================================================================
 * SetKeyEntryDataAndDestroy
 *
 * Set the k_data pointer in the node.  This can be any applicaiton-dependent data.
 * And set the k_destroy destructor function that is to be called before the node
 * is deallocated.  If the k_destroy function sets k_data to NULL, then the node
 * deallocation will not attempt to deallocate the k_data content.

 */

void * SetKeyEntryDataAndDestroy(KeyListEntry * theNode, void * theData, KeyEntDestroyType * theDestroy)
{
	if (theNode == NULL)
		return NULL;

	if (theNode->k_data)
	{
		if (theNode->k_destroy)
			CallKeyEntryDestroy(theNode);
		if (theNode->k_data)
		{
			free(theNode->k_data);
			theNode->k_size = 0;
		}
	}

	theNode->k_data = theData;
	theNode->k_destroy = theDestroy;
	return theData;
}

/* ================================================================
 * NewKeyEntryData
 *
 * Allocate an empty array of nmemb members of bufsz length to the key list node.
 * Return a pointer to the allocated memory.  This allocation does not require
 * a user application destructor.
 *
 * BEWARE: If k_data is already allocated -- CAUTION!
 * If a k_destroy deallocator exists, it will be called.
 * The k_destroy deallocator will become unregistered.
 * Then if k_data is allocated, it will be freed.  After
 * perfoming these functions, new data will be allocated.
 */

void * NewKeyEntryData(KeyListEntry *node, const size_t nmemb, const size_t bufsz)
{
	if (node == NULL)
		return NULL;

	if (node->k_data)
	{
		if (node->k_destroy)
		{
			CallKeyEntryDestroy(node);
			node->k_destroy = NULL;
		}
		if (node->k_data)
		{
			free(node->k_data);
			node->k_size = 0;
			node->k_data = NULL;
		}
	}

	if (bufsz && nmemb)
	{
		node->k_size = (bufsz * nmemb);
		node->k_data = calloc(nmemb, bufsz);
		if (node->k_data == NULL)
			node->k_size = 0;
	}

	return node->k_data;
}


/* ================================================================
 * GetKeyEntryData
 *
 * Return a pointer to the key list node data element.
 */

void * GetKeyEntryData(KeyListEntry * node)
{
	if (node == NULL)
		return NULL;
	return node->k_data;
}


/* ================================================================
 * DropKeyEntryData
 *
 * If the k_destroy function is registered, it is called.
 *
 * If the k_data element appears allocated, then free() is called.
 */

void DropKeyEntryData(KeyListEntry * node)
{
	if (node == NULL)
		return;

	if (node->k_destroy != NULL)
		(* node->k_destroy) (node);

	if (node->k_data)
		free(node->k_data);

	node->k_size = 0;
	node->k_data = NULL;
	return;
}

/* ================================================================
 * DropKeyListEntry
 *
 * returns the next node in the KeyListEntry chain
 * and frees the memory for the current KeyListEntry
 */

KeyListEntry * 
DropKeyListEntry(KeyListEntry * node)
{
	KeyListEntry * rtn;

	if (!node)
		return NULL;

	rtn = node->next;

    CallKeyEntryDestroy(node);

	if (node->k_name)
		free(node->k_name);

	if (node->k_data)
		free(node->k_data);

	if (node->k_list)
		FreeKeyList(&node->k_list);

	free(node);
	return rtn;
}

/* ===============================================================
 * FreeKeyList
 *
 * drops all KeyListEntries in the list.
 */

void FreeKeyList(KeyListEntry ** theListAdr)
{
  KeyListEntry * tmpList;

  if (!theListAdr || !(*theListAdr))
    return;

  do
  {
    tmpList = *theListAdr;
    *theListAdr = (*theListAdr)->next;

    CallKeyEntryDestroy(tmpList);

    if (tmpList->k_name)
		free(tmpList->k_name);

	if (tmpList->k_data)
		free(tmpList->k_data);

	if (tmpList->k_list)
		FreeKeyList(&tmpList->k_list);

	free(tmpList);

  }while (*theListAdr);

  return;
}

/* ===============================================================
 * DestoyKeyArgBuf
 *
 * A useful destructor for an array of argument buffer pointers.
 */

void DestroyKeyArgBuf(KeyListEntry * theNode)
{
	if (theNode == NULL)
		return;
	theNode->k_argc = 0;
	if (theNode->k_argbuf)
		free(theNode->k_argbuf);
	theNode->k_argbuf = NULL;
	return;
}


/* ===============================================================
 * FindLastKeyListEntry
 *
 * Return a pointer to the last KeyListEntry node.
 */

KeyListEntry *
FindLastKeyListEntry(KeyListEntry * theList)
{
	KeyListEntry *	LP;
	for (LP = theList; LP && LP->next; LP = LP->next);
	return LP;
}


/**
 * Sorting Functions for KeyListEntry library return the address of the re-linked list.
 *
 * KeyListSort creates an assending order of the single-linked list.
 *
 * KeyListReverseOrder reverses the linked order of the single-linked list.
 *
 *
 * The KeyListSort requires a comparison helper function that takes two KeylistEntry
 * arguments and returns an integer value.
 *
 *     NewList = KeyListSort(theList, &KeyNodeCompareFunction)
 *
 * This library provides two helper functions.  Users can create their own.
 *
 * KeyNodeNameCompare - tests the k_name items in two KeyListEntry nodes.
 * KeyNodeIDValueCompare - tests the k_id items in two KeyListEntry nodes.
 *
 * The comparison functions return an integer
 *   Positive = (First > Second)
 *   Zero     = (First = Second) or nothing to compare
 *   Negative = (First < Second)
 */

/* ===============================================================
 * KeyNodeNameCompare
 *
 * A helper function for KeyListSort() that performs a strcmp() comparison
 * on the k_name elements of two KeyListEntry nodes.
 *
 * Returns positive if (nodeA > nodeB)
 * Returns zero if (nodeA == nodeB)
 * Returns negative if (nodeA < nodeB)
 */

int KeyNodeNameCompare (const KeyListEntry * aNode, const KeyListEntry *bNode)
{
	if (!aNode || !bNode)
		return 0;
	return (strcmp((char *) aNode->k_name, (char *) bNode->k_name));
}

/*================================================================
 * KeyNodeIDValueCompare
 *
 * A helper function for KeyListSort() that performs integer comparison
 * on the k_id elements of two KeyListEntry nodes.
 *
 * Returns positive if (nodeA > nodeB)
 * Returns zero if (nodeA == nodeB)
 * Returns negative if (nodeA < nodeB)
 */

int KeyNodeIDValueCompare (const KeyListEntry *aNode, const KeyListEntry *bNode)
{
	if (!aNode || !bNode)
		return 0;
	if (aNode->k_id > bNode->k_id)
		return 1;
	if (aNode->k_id < bNode->k_id)
		return -1;
	return 0;
}


/* ===============================================================
 * KeyListSort
 *
 * theList = pointer to an unsorted list.
 * KCompare = pointer to the helper comparison function.
 *
 * Result = pointer to the head of the sorted list returned in ascending order.
 */

/* The following typedef for helper functions is found in header file keylist.h
 *
 * typedef int KeyNodeCompareFunction (const KeyListEntry * A, const KeyListEntry * B);
 *
 * The helper comparison function returns an integer value.
 *		Positive (A > B)
 *		Zero     (A = B)
 *		Negative (A < B)
 * where A and B are KeyListEntry nodes.
 *
 * The KeyListSort has been adjusted by calling KeyListReverseOrder before entering
 * the actual sort algorithm loop.
 *
 * The sort algorithm reverses the link order of nodes with identical key-value content.
 * This is fixed by calling KeyListReverseOrder before getting into the sort loops.
 * The sort algorithm also works hardest on lists that have ascending key-value order.
 * Calling KeyListReverseOrder turns ascending key-list sequences into descending sequences
 * and preserves original sequence order of nodes with equal comparison value.
 */


KeyListEntry * KeyListSort (KeyListEntry * theList, KeyNodeCompareFunction * KCompare)
{

	KeyListEntry	* OutList;	// head of sorted list
	KeyListEntry	* RevList;	// head of reversed list

	KeyListEntry	* KPtrA;	// the base test node pointer
	KeyListEntry	* KRefA;	// parent adjacency pointer

	KeyListEntry	* KPtrB;	// forward test node pointer
	KeyListEntry	* KRefB;	// forward separation pointer

	KeyListEntry	* HoldB;	// forward next node pointer
	KeyListEntry	* KTemp;	// address swapping pointer

	KeyListEntry	* KMovA;	// forward base move test - optimization
					// for initial list in descending order


	if (!theList)
		return NULL;			// no list to sort

	if (!KCompare)				// returning NULL here could cause memory leaks.
		return theList;			// no comparison function, return unsorted.

	/* The following sort algorithm works hardest on ascending key-value lists
	 * and reverses the node-link order when key-values appear identical.
	 * Reversing the order before sorting resolves these issues.
	 */
	
	RevList = KeyListReverseOrder(theList);

	/* The OutList points to the last KeyListEntry node with the lowest 
	 * common key-value comparison.  If (<) comparison instead of (<=) is used,
	 * then identical key-value comparisons cause problems.
	 */

	for (OutList = KTemp = RevList; KTemp; KTemp = KTemp->next)
	{
		if ((*KCompare)(KTemp, OutList) <= 0)
			OutList = KTemp;
	}

	KRefA = NULL;		// no adjacent parent
	KRefB = NULL;		// no forward separation (A & B are adjacent)

	for (KPtrA = RevList; KPtrA; KPtrA = KRefA->next)
	{
		KRefB = NULL;				// No Forward Separation
		KMovA = NULL;

		for (KPtrB = KPtrA->next; KPtrB; KPtrB = KRefB->next)
		{

			/* The comparison (<=) here must be the same which determines OutList earlier */
			if ((*KCompare)(KPtrB, KPtrA) <= 0)
			{
				HoldB = KPtrB->next;	// Forward Looking Reference
	
				// Swap A B Pointers
				KTemp = KPtrB;
				KPtrB = KPtrA;
				KPtrA = KTemp;

				// Relink the nodes
				KPtrA->next = KPtrB;
				if (!KRefB)		// A & B are adjacent
				{
					KPtrB->next = HoldB;
				}
				else			// B has Forward Separation
				{
					KRefB->next = HoldB;
				}
				if (KRefA)		// A has a parent
				{
					KRefA->next = KPtrA;
				}
				KMovA = KPtrB;		// Show that KPtrA has moved
			}
			else
			{
				if (KRefB && ((*KCompare)(KPtrB, KRefB) <= 0))
					KMovA = KPtrB;	// move KPtrA on future cycle
				KRefB = KPtrB;
			}
			if (!KRefB)
			{
				KRefB = KPtrB;
			}
		}
		if (!KMovA && KRefB)
			KRefA = KRefB;			// points to end-of-list
		else
			KRefA = KPtrA;			// advance one node
	}

	return OutList;
}


/* ===============================================================
 * KeyListNameSort
 *
 * Returns a pointer to a sorted list of KeyListEntry nodes sorted by name
 * by calling KeyListSort with the KeyNodeNameCompare helper function.
 *
 * The KeyListEntry nodes are linked in ascending Name order.
 *
 */

KeyListEntry * KeyListNameSort(KeyListEntry * theList)
{
	return KeyListSort(theList, &KeyNodeNameCompare);
}


/* ===============================================================
 * KeyListIDValueSort
 *
 * Returns a pointer to a sorted list of KeyListEntry nodes sorted by name
 * by calling KeyListSort with the KeyNodeIDValueCompare helper function.
 *
 * The KeyListEntry nodes are linked in ascending IDValue order.
 */

KeyListEntry * KeyListIDValueSort(KeyListEntry * theList)
{
	return KeyListSort(theList, &KeyNodeIDValueCompare);
}


/* ===============================================================
 * KeyListReverseOrder
 *
 * Returns a pointer to the list of KeyListEntry nodes in reversed order.
 */

KeyListEntry * KeyListReverseOrder(KeyListEntry * theList)
{
	KeyListEntry * PNode;
	KeyListEntry * P1 = NULL;
	KeyListEntry * P2 = NULL;

	for (PNode = theList; PNode; PNode = PNode->next)
	{
		if (P2)
		{
			P2->next = P1;
			P1 = P2;
		}
		P2 = PNode;
	}
	if (P2)
		P2->next = P1;

	return P2;
}

/* ============================================================================
 * Test The List Library
 */

#ifdef MAKE_LIST_TEST_EXE

#include <stdio.h>

void * fcn_test(KeyListEntry * node, int ii, void * args[])
{
	int i;
	printf("Function: Node %s\n", node->k_name);
	for (i = 0; i < ii; i++)
		printf("   argAdrs=%x   arg[%d] = %s\n", &args, i, (char *) args[i]);
	return NULL;
}

int dstr_test(KeyListEntry * node)
{
	printf("Destroy: Node %s, DataSize %d\n",
		node->k_name, node->k_size);
	return 0;
}

int main(int argc, char *argv[])
{
	int i;
	char *KeyName[] = {"First", "Second", "Third", "Fourth", "Fifth", "Sixth", "Seventh", NULL};
    KeyListEntry * MyList = NULL;
	KeyListEntry * listPtr = NULL;

	listPtr = NewKeyListEntry();
	MyList = listPtr;
	listPtr->k_name = strdup(KeyName[0]);

	for (i = 1; i<7 ;i++)
	{
		listPtr->next = NewKeyListEntry();
		listPtr = listPtr->next;
		listPtr->k_name = strdup(KeyName[i]);
		if (i > 2)
			SetKeyEntryFunction(listPtr, & fcn_test);
		NewKeyEntryData(listPtr, i, 10);
		SetKeyEntryDestroy(listPtr, & dstr_test);
	}

	listPtr = MyList->next;
	for (i = 0; i<3; i++)
	{
		listPtr->k_list = NewKeyListEntry();
		listPtr = listPtr->k_list;
		listPtr->k_name = calloc(1,30);
		strcpy(listPtr->k_name, "  Supplemental: ");
		strcat(listPtr->k_name, KeyName[i]);
	}

	listPtr = MyList->next->next->next;
	for (i = 0; i<3; i++)
	{
		listPtr->k_list = NewKeyListEntry();
		listPtr = listPtr->k_list;
		listPtr->k_name = calloc(1,30);
		strcpy(listPtr->k_name, "  Supplemental: ");
		strcat(listPtr->k_name, KeyName[i]);
	}

	for (listPtr = MyList; listPtr; listPtr = listPtr->next)
	{
		char * args[2] = {"arg1-text","arg2-text"};
		KeyListEntry * supplKey;

		printf("ArgumentArray = %x\n", &args);
		printf("List Entry: %s\n", listPtr->k_name);
		CallKeyEntryFunction(listPtr, 2, args);
		printf("  k_argc = %d, k_argbuf = %x\n", listPtr->k_argc, listPtr->k_argbuf);
		for (supplKey = listPtr->k_list; supplKey; supplKey = supplKey->k_list)
			printf("%s\n", supplKey->k_name);
	}

	printf("\n -- Now to drop second entry \n");

	MyList->next = DropKeyListEntry(MyList->next);

	for (listPtr = MyList; listPtr; listPtr = listPtr->next)
	{
		KeyListEntry * supplKey;
		printf("List Entry: %s\n", listPtr->k_name);
		for (supplKey = listPtr->k_list; supplKey; supplKey = supplKey->k_list)
			printf("%s\n", supplKey->k_name);
	}


	printf("\n -- Now to drop first entry \n");

	MyList = DropKeyListEntry(MyList);

	for (listPtr = MyList; listPtr; listPtr = listPtr->next)
	{
		KeyListEntry * supplKey;
		printf("List Entry: %s\n", listPtr->k_name);
		for (supplKey = listPtr->k_list; supplKey; supplKey = supplKey->k_list)
			printf("%s\n", supplKey->k_name);
	}

	printf("\n -- Now to drop entire list \n");

	FreeKeyList(&MyList);

	if (!MyList)
	{
		printf("SUCCESS\n");
	}
	else
	{
		printf("FAILURE\n");
	}
}
#endif /* MAKE_LIST_TEST_EXE */



#if defined(__cplusplus)
}
#endif


