///////////////////////////////////////////////////////////////////////////////
// Copyright 2017, the Oregon State University Capstone Project
// 'XZES40-Transformer' with the Apache Software Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Filename: cache.cpp
// Description: Definition and documentation for custom functions,
// and classes used in XZES-Transformer cacheing pipeline.
///////////////////////////////////////////////////////////////////////////////

#include <cache.hpp>

// --------------------------------------------------------------------
// xzes::Cache::Cache()
//
// Default constructor
// --------------------------------------------------------------------
xzes::Cache::Cache()
{
	theList = NewKeyListEntry();
	theList->next = NULL;
}
// --------------------------------------------------------------------
// xzes::Cache::search(uid)
//
// search node in the keyList
// return 0 means found, 1 means NOT found.
// --------------------------------------------------------------------


bool xzes::Cache::search( xzes::id_t uid ){
	int objid = uid.id;
	KeyListEntry *head = theList;
	while (head != NULL){
		int keyid = GetKeyEntryIdValue(head);
		if (keyid == objid){
			return SUCCESS;
		}
		head = head->next;
	}
	return FAILURE;
}


// --------------------------------------------------------------------
// Document* Cache::get( Document* )
//
// Attempts to get a Document from the cache.
// Returns the object if it is in the cache.
// Returns NULL if it does not exist in the cache.
// --------------------------------------------------------------------
doc_t* xzes::Cache::get( xzes::id_t uid)
{	
	int objid = uid.id;
	KeyListEntry *head = theList;
	while (head != NULL){
		int keyid = GetKeyEntryIdValue(head);
		if (keyid == objid){
			return (doc_t*)GetKeyEntryData(head);
		}
		head = head->next;
	}
	printf("Get FAILURE!!");
    return NULL;
}

// --------------------------------------------------------------------
// Document* Cache::set( Document* )
//
// Attempts to put a Document object in the in-memory cache.
// Returns the object if successful.
// Returns NULL if not successful.
// --------------------------------------------------------------------
int xzes::Cache::set( xzes::id_t uid, xzes::doc_t *doc, xzes::uri_t uri)
{
	int status = SUCCESS;
	//Find the last node;
	KeyListEntry *last = FindLastKeyListEntry(theList);
	//Create empty node for next last node
	last->next = NewKeyListEntry();
	//Point to next last node
	last = last->next;
	//Set the name of the object
	SetKeyEntryName(last,uri.uri.c_str());
	//Set the id of the object
	SetKeyEntryIdValue(last,uid.id);
	//Set the content of the object
	SetKeyEntryDataAndDestroy(last, doc, NULL);
	//Set the next last node is null
	last->next = NULL;

    return status;
}

// --------------------------------------------------------------------
// int Cache::print_name()
//
// print all name of Keylist.
// Debug using
// --------------------------------------------------------------------
int xzes::Cache::print_name()
{
	KeyListEntry *itr = theList;
	int i = 1;
	while (itr != NULL){
		printf("Name%d: %s \n", i ,GetKeyEntryName(itr));
		itr = itr->next;
		i++;
	}
	return 0;
}


// --------------------------------------------------------------------
// int Cache::print_id()
//
// print all id of Keylist.
// Debug using
// --------------------------------------------------------------------
int xzes::Cache::print_id()
{
	KeyListEntry *itr = theList;
	int i = 1;
	while (itr != NULL){
		printf("ID%d: %d \n", i ,GetKeyEntryIdValue(itr));
		itr = itr->next;
		i++;
	}
	return 0;
}
