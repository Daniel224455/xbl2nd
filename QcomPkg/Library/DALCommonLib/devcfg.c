#include "DALSysInt.h"
#include "stdlib.h"
#ifndef DEVCFG_STANDALONE
    #include "stringl/stringl.h"
#endif

#ifdef DEVCFG_BOOT
#include "DALSys.h"
#endif

#ifndef DLL_API_GLOBAL
#define DLL_API_GLOBAL
#endif
typedef struct blobT{
   DALProps * data;
   int blob_id;
   struct blobT * next;
} blob;

blob * head;

blob * createBlob(DALProps * newPropInfo)
{
   blob * newBlob;
#ifdef DEVCFG_BOOT
   DALSYS_Malloc(sizeof(blob), (void**)&newBlob);
#else
   newBlob = malloc(sizeof(blob));
#endif
   if (newBlob) {
      newBlob->data = newPropInfo;
      newBlob->blob_id = -1;
      newBlob->next = NULL;
   }
   return newBlob;
}

//is this needed?
void devcfg_init_new()
{
   //initialize any structures that might be needed
   //everything should be 0,0,0 here

   head = createBlob(NULL);
}

DLL_API_GLOBAL void devcfg_blob_append(DALProps * newPropInfo, int bid)
{
   blob * curr = head;
   blob * newBlob = createBlob(newPropInfo);
   if (newBlob)
       newBlob->blob_id = bid;

   if(curr->data==NULL)
   {
      head=newBlob;
   }
   else
   {
      while(curr->next != NULL)
      {
         curr=curr->next;
      }
      curr->next=newBlob;
   }
}

void devcfg_blob_prepend(DALProps * newPropInfo, int bid)
{
   blob ** headref = &head;
   blob * newBlob = createBlob(newPropInfo);
   if (newBlob)
   {
      newBlob->blob_id = bid;
      newBlob->next = *headref;
      *headref = newBlob;
   }
}

DALProps * devcfgGetBlobByID(int bid)
{
   blob * curr = head;
   while(curr != NULL) 
   {
      if(curr->blob_id == bid) {
         return curr->data;
      }
      curr = curr->next;
   }
   return NULL;
}
/*
devcfg_data_handle devcfg_blob_insert(void * blob, int index)
{
}

devcfg_prop_lookup(int propid)
{
}

devcfg_prop_lookup_explicit(int propid, void * blob)
{
}
*/
