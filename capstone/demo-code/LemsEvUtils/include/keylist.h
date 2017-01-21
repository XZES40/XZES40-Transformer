/*
 * 'C' Source Library - Common Utilities - Supported by GNU and Windows VC7.1
 *
 * Copyright (c) 2008-2009 - This is open source software in the pulbic domain.
 *
 * Developed by: Steven J. Hathaway
 *
 * For:  State of Oregon - Oregon State Police / CJIS-LEDS
 *
 * Updated: 4/4/2009
 * Changed format of the general k_function item to afford greater utility.
 *
 * Updated: 7/17/2009
 * Added KeyList sorting utilities using helper callback comparison.
 */

#ifndef OR_KEY_LIST_LIB_H
#define OR_KEY_LIST_LIB_H

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
 * next = pointer to next node of KeyListEntry type.
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
 *			If k_data remains not NULL, then free() is attempted when the
 *			KeyListEntry node is deallocated.
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
 *
 *- - - - - - - - - - -
 * Creating a List - and initialize an empty node:
 *
 *   KeyListEntry * theList;
 *   theList = NewKeyListEntry();
 *
 * Dropping a Node:
 *
 *   KeyListEntry * theNode;
 *   KeyListEntry * theParent;
 *   . . .
 *
 *   theNode = theParent->next;
 *
 *   theParent->next = DropKeyListEntry(theNode);
 *
 * Deallocate Entire List:
 *
 *   FreeKeyList(&theList);
 *- - - - - - - - - - -
 */


#if defined(__cplusplus)
extern "C"
{
#endif


typedef struct key_list_entry
{
  struct key_list_entry * next;
  char * k_name;			// malloc() string content
  int    k_id;				// arbitrary integer value
  size_t k_size;			// size of k_data
  void * k_data;			// malloc() arbitrary data
  void * (* k_function)(struct key_list_entry * theNode,
		int f_argc, void * f_args[]);
  int    k_argc;			// Copy of f_argc.
  void ** k_argbuf;			// Pointer to f_args[];
  int (* k_destroy)(struct key_list_entry * theNode);
  struct key_list_entry * k_list;	// supplemental Key List
}
KeyListEntry;

typedef void * KeyEntFunctionType (KeyListEntry * theNode, int f_argc, void * f_args[]);
typedef int    KeyEntDestroyType  (KeyListEntry * theNode);

/* ================================================================
 * SetKeyEntryFunction
 *
 * Add or register a helper function to the key list entry.
 * This function can be used to supplement the processing of arbitrary data.
 */

void SetKeyEntryFunction(KeyListEntry * node, KeyEntFunctionType * fcn);

/* ================================================================
 * CallKeyEntryFunction
 *
 * This function may do its own allocation to node->k_data
 * and may install any user_defined side affects.  The companion
 * CallKeyEntryDestroy is required to cleanup any side affects
 * before the node is deallocated.
 */

void * CallKeyEntryFunction(KeyListEntry * node, int f_argc, void * f_args[]);

/* ================================================================
 * SetKeyEntryDestroy
 *
 * Add or register a helper function to deallocate references assumed in the
 * aritrary data before the key list entry is itself deallocated.
 */

void SetKeyEntryDestroy(KeyListEntry * node, KeyEntDestroyType * fcn);

/* ================================================================
 * CallKeyEntryDestroy
 *
 * This function is called before the key list entry node is deallocated.
 * If side affects (suchas pointers to other allocated memory) reside in
 * the k_data element, then the k_destroy function is called to clean up
 * the mess.
 */

int CallKeyEntryDestroy(KeyListEntry * node);

/* ================================================================
 * SetKeyEntryName
 *
 * Add a character string name to the key list entry.
 * Returning a pointer to the new name allocation.
 */

char * SetKeyEntryName(KeyListEntry * node, const char * name);

/* ================================================================
 * GetKeyEntryName
 *
 * Return a pointer to the name string associated with the list node element.
 */

char * GetKeyEntryName(KeyListEntry * node);

/* ================================================================
 * SetKeyEntryIdValue
 *
 * Add an identity value to the Key Entry Node.  The value
 * returned is the value added.
 */

int SetKeyEntryIdValue(KeyListEntry * node, int idval);

/* ================================================================
 * GetKeyEntryIdValue
 *
 * Return the ID Value integer stored in the list node element.
 */

int GetKeyEntryIdValue(KeyListEntry * node);

/* ================================================================
 * GetKeyEntryNext
 *
 * Return a pointer to the next KeyListEntry node.
 */

KeyListEntry *
GetKeyEntryNext(KeyListEntry * node);

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
GetKeyEntrySubList(KeyListEntry * node);

/* ================================================================
 * NewKeyListEntry
 *
 * Create a new empty KeyListEntry element.
 * Return a pointer to the created element.
 */

KeyListEntry * 
NewKeyListEntry();

/* ================================================================
 * NewKeySubList
 *
 * Create a new empty sublist element.
 * Return a pointer to the created element.
 */

KeyListEntry * 
NewKeySubList(KeyListEntry * node);

/* ================================================================
 * SetKeyEntryDataAndDestroy
 *
 * Set the k_data pointer in the node.  This can be any applicaiton-dependent data.
 * And set the k_destroy destructor function that is to be called before the node
 * is deallocated.  If the k_destroy function sets k_data to NULL, then the node
 * deallocation will not attempt to deallocate the k_data content.
 */

void * SetKeyEntryDataAndDestroy(KeyListEntry * theNode, void * theData, KeyEntDestroyType * theDestroy);

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

void * NewKeyEntryData(KeyListEntry *node, const size_t nmemb, const size_t bufsz);

/* ================================================================
 * GetKeyEntryData
 *
 * Return a pointer to the key list node data element.
 */

void * GetKeyEntryData(KeyListEntry * node);

/* ================================================================
 * DropKeyEntryData
 *
 * Deallocate the k_data element in the KeyEntryList node.
 *
 * If the k_destroy function is registered, it is called before 
 * free() is attempted on the k_data element.
 */

void DropKeyEntryData(KeyListEntry * node);

/* ================================================================
 * DropKeyListEntry
 *
 * Deallocates the memory for the current KeyListEntry node and
 * returns the next node in the KeyListEntry chain.
 *
 * A call to node->k_destroy is performed before any node content
 * deallocated or freed.
 */

KeyListEntry * 
DropKeyListEntry(KeyListEntry * node);

/* ===============================================================
 * FreeKeyList
 *
 * drops all KeyListEntries in the list.
 */

void FreeKeyList(KeyListEntry ** theListAdr);

/* ===============================================================
 * DestoyKeyArgBuf
 *
 * A useful destructor for an array of argument buffer pointers.
 */

void DestroyKeyArgBuf(KeyListEntry * theNode);

/* ===============================================================
 * FindLastKeyListEntry
 *
 * Return a pointer to the last KeyListEntry node.
 */

KeyListEntry *
FindLastKeyListEntry(KeyListEntry * theList);

/* ===============================================================
 * KeyListSort
 *
 * theList = pointer to an unsorted list.
 * KCompare = pointer to the helper comparison function.
 *
 * Result = pointer to the head of the sorted list returned in ascending order.
 *
 * The helper comparison function returns an integer value.
 *		Positive (A > B)
 *		Zero     (A = B)
 *		Negative (A < B)
 * where A and B are KeyListEntry nodes.
 *
 * The link order of nodes with identical key-value content is preserved.
 */

typedef int KeyNodeCompareFunction (const KeyListEntry * A, const KeyListEntry * B);

KeyListEntry *
KeyListSort(KeyListEntry * theList, KeyNodeCompareFunction * KCompare);

/* ===============================================================
 * KeyListNameSort
 *
 * Returns a pointer to a sorted list of KeyListEntry nodes sorted by name
 * by calling KeyListSort with the KeyNodeNameCompare helper function.
 *
 * The KeyListEntry nodes are linked in ascending Name order.
 */

KeyListEntry *
KeyListNameSort(KeyListEntry * theList);

/* ===============================================================
 * KeyListIDValueSort
 *
 * Returns a pointer to a sorted list of KeyListEntry nodes sorted by name
 * by calling KeyListSort with the KeyNodeIDValueCompare helper function.
 *
 * The KeyListEntry nodes are linked in ascending IDValue order.
 */

KeyListEntry *
KeyListIDValueSort(KeyListEntry * theList);

/* ===============================================================
 * KeyListReverseOrder
 *
 * Returns a pointer to the list of KeyListEntry nodes in reversed order.
 */

KeyListEntry *
KeyListReverseOrder(KeyListEntry * theList);

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

int KeyNodeNameCompare(const KeyListEntry * nodeA, const KeyListEntry * nodeB);

/* ===============================================================
 * KeyNodeIDValueCompare
 *
 * A helper function for KeyListSort() that performs integer comparison
 * on the k_id elements of two KeyListEntry nodes.
 *
 * Returns positive if (nodeA > nodeB)
 * Returns zero if (nodeA == nodeB)
 * Returns negative if (nodeA < nodeB)
 */

int KeyNodeIDValueCompare (const KeyListEntry * nodeA, const KeyListEntry * nodeB);



#if defined(__cplusplus)
}
#endif



#endif /* OR_KEY_LIST_LIB_H */

