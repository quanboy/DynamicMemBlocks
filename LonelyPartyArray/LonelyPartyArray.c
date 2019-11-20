//Victor Quan
// NID Vi705465
#include <stdio.h>
#include <stdlib.h>
#include "LonelyPartyArray.h"

LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
  {

    if (num_fragments <= 0 || fragment_length <= 0) // check if fragment or fragment length is invalid
    {
      return NULL;
    }

    LonelyPartyArray *newLpa = malloc(sizeof(LonelyPartyArray)); // malloc space for a new LPA struct

    newLpa->num_fragments = num_fragments;            // initialize values in the struct
    newLpa->fragment_length = fragment_length;
    newLpa->size = 0;
    newLpa->num_active_fragments = 0;
    newLpa->fragments = malloc(sizeof(int *) * num_fragments); // malloc space for fragments array filled with pointers
    for (int i = 0; i < num_fragments; i++) // initalize all pointers in fragments to NULL
      newLpa->fragments[i] = NULL;
    newLpa->fragment_sizes = malloc(sizeof(int *) * num_fragments); // malloc space for fragment_sizes array
    for (int i = 0; i < num_fragments; i++)
      newLpa->fragment_sizes[i] = 0; // initialize all values in fragment_sizes to zero

      printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n", num_fragments*fragment_length,  num_fragments);

    return newLpa;
  }

LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
  {
    if (party == NULL)
      return NULL;
    free(party->fragment_sizes);     // free the party fragment sizes array, and then back up the structs members
    free(party->fragments);
    free(party);
    printf("-> The LonelyPartyArray has returned to the void.\n");
    return NULL;
  }

int set(LonelyPartyArray *party, int index, int key)
  {
    int firstIndex = 0;
    int secondIndex = 0;

    if (party == NULL)
      {
        printf("-> Bloop! NULL pointer detected in set().\n");
      }

    if (party != NULL) { // check if party is null, if not, then continue with setting the key
      int fragment = 0;
      int offset = 0;
      fragment = index / party->fragment_length;
      offset = index % party->fragment_length;
      if (index < 0)
            printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n", index, fragment, offset);
      if (index > (party->fragment_length * party->num_fragments) - 1)
            printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n", index, fragment, offset);
    }
    if (index < 0 || index > ((party->fragment_length * party->num_fragments) - 1)) // return LPA FAILURE as soon as we see index is invalid
      {
        return LPA_FAILURE;
      }

    firstIndex = index/party->fragment_length; // first index to determine which fragment we are inserting key into
    if (party->fragments[firstIndex] == NULL) // allocate space for new fragment
      {
        int fragment_index = 0;
        int capacity = 0;
        int r = 0;
        int s = 0;
        fragment_index = index / party->fragment_length; // determine which fragment we will be inserting new key into
        r = fragment_index * party->fragment_length; // get the lower bound of the indices
        s = r + (party->fragment_length - 1); // get the upper bound of the indices
        party->fragments[firstIndex] = malloc(sizeof(int) * party->fragment_length);
        party->num_active_fragments++;
        for (int i = 0; i < party->fragment_length; i++) // initialize new fragment values to UNUSED
          party->fragments[firstIndex][i] = UNUSED;
           printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n", fragment_index, party->fragment_length, r, s); // created a new fragment
      }

    secondIndex = index % party->fragment_length; // second index to determine which index in the fragment we are inserting key into

    if (party->fragments[firstIndex][secondIndex] == UNUSED) // if a value is not already at the index, then insert key
      {
        party->fragments[firstIndex][secondIndex] = key;
        party->size++;
        party->fragment_sizes[firstIndex]++;
      }
      else party->fragments[firstIndex][secondIndex] = key;

    return LPA_SUCCESS;
  }

int get(LonelyPartyArray *party, int index)
  {
    if (party == NULL){
      printf("->Bloop! NULL pointer detected in get()\n");
      return LPA_FAILURE;
}

    if (index < 0 && party != NULL) // invalid index for a non-NULL party
      {
         int fragment = 0;
         int offset = 0;
         fragment = index / party->fragment_length;
         offset = index % party->fragment_length;
          printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n", index, fragment, offset);
      }

    if (index > (party->fragment_length * party->num_fragments) - 1 && party!= NULL ) // invalid index for a non-NULL party
      {
        int fragment = 0;
        int offset = 0;
        fragment = index / party->fragment_length;
        offset = index % party->fragment_length;
          printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n", index, fragment, offset);
      }

    if (index < 0 || index > (party->fragment_length * party->num_fragments) - 1) // if index is invalid, automatically return LPA FAILURE
        return LPA_FAILURE;

    int firstIndex, secondIndex;

    firstIndex = index/party->fragment_length;
    secondIndex = index % party->fragment_length;

    if (party->fragments[firstIndex] == NULL) // if the value at the firstIndex of fragments* array, then that means that fragment is unused currently
      return UNUSED;

    if (party->fragments[firstIndex][secondIndex] == UNUSED) // if the fragment has been allocated, and the value at the index is unused, return unused
      {
        return UNUSED;
      }
    if (party->fragments[firstIndex] != NULL) // if a a fragment has been allocated and the key at that value does not equal UNUSED, then return the key
      {
        return party->fragments[firstIndex][secondIndex];
      }

    return 0;
  }

int delete(LonelyPartyArray *party, int index)
  {
    int firstIndex;
    int secondIndex;
    firstIndex = index / party->fragment_length;
    secondIndex = index % party->fragment_length;

    if (party == NULL)
      {
        printf("-> Bloop! NULL pointer detected in delete().\n");
        return LPA_FAILURE;
      }

    if (index < 0 || index > (party->fragment_length * party->num_fragments) - 1)
        {
          printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n", index, firstIndex, secondIndex);
          return LPA_FAILURE;
        }

    if (party->fragments[firstIndex] == NULL) // no fragment access
        {
          return LPA_FAILURE;
        }

    if (party->fragments[firstIndex][secondIndex] == UNUSED) // no key to delete
        {
          return LPA_FAILURE;
        }

    if (party->fragments[firstIndex][secondIndex] != UNUSED)
      {
        party->fragments[firstIndex][secondIndex] = UNUSED; // remove the key value and reset index
        party->size--; //decrement the size value to reflect how many cells are being used
        party->fragment_sizes[firstIndex]--; //decrement the fragment size value at given fragment index
      }
        if (party->fragment_sizes[firstIndex] == 0) // if no keys are found in a fragment, then free that fragment and release the memory
          {
            int fragment_index = 0;
            int r = 0;
            int s = 0;

            fragment_index = index / party->fragment_length;
            r = fragment_index * party->fragment_length;
            s = r + (party->fragment_length - 1);

            party->fragments[firstIndex] = NULL;
            free(party->fragments[firstIndex]);

            party->num_active_fragments--;
            printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n", fragment_index, party->fragment_length, r, s);

          }

      return LPA_SUCCESS;
  }

int containsKey(LonelyPartyArray *party, int key)
  {
    if (party == NULL)
      {
        return 0;
      }
    for (int i = 0; i < party->num_fragments;  i++)  // for loop through fragment pointer array
        {
          for (int j = 0; j < party->fragment_length; j++) // for loop to search through each index at given fragment
          {
          if (party->fragments[i][j] == key) // key is found within the LPA
          return 1;
          }
        }
        return 0;
  }

int isSet(LonelyPartyArray *party, int index)
  {
    int firstIndex;
    int secondIndex;
    firstIndex = index / party->fragment_length;
    secondIndex = index % party->fragment_length;
    if (party == NULL)
      {
        return 0;
      }
    if (index < 0 || index > (party->fragment_length * party->num_fragments) - 1) // invalid index
      {
        return 0;
      }
    if (party->fragments[firstIndex] == NULL) // fragment is not allocated
      {
        return 0;
      }
    if (party->fragments[firstIndex][secondIndex] == UNUSED) // fragment cell is not being used
      {
        return 0;
      }

    return 1;

  }

int printIfValid(LonelyPartyArray *party, int index)
  {
    int firstIndex = 0;
    int secondIndex = 0;
    firstIndex = index / party->fragment_length;
    secondIndex = index % party->fragment_length;

      if (party == NULL)
        {
        return LPA_FAILURE;
        }
      if (index < 0 || index > (party->fragment_length * party->num_fragments) - 1)
        {
       return LPA_FAILURE;
        }
      if (party->fragments[firstIndex] == NULL)
        {
          return LPA_FAILURE;
        }
        if (party->fragments[firstIndex][secondIndex] == UNUSED)
        {
        return LPA_FAILURE;
        }

    printf("%d\n", party->fragments[firstIndex][secondIndex]); // print out the value at given index

    return LPA_SUCCESS;
  }

LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
  {
    if (party == NULL)
      {
        printf("-> Bloop! NULL pointer detected in resetLonelyPartyArray().\n");
        return 0;
      }
    if (party != NULL)
      {
        for (int i = 0; i < party->num_fragments; i++) // free any fragments that are being used
          free(party->fragments[i]);

        for (int i = 0; i < party->num_fragments; i++) // reset fragment pointer array values to NULL
          party->fragments[i] = NULL;

        for (int i = 0; i < party->num_fragments; i++) // reset fragment sizes to 0
          party->fragment_sizes[i] = 0;

          party->size = 0;
          party->num_active_fragments = 0;
          printf("-> The LonelyPartyArray has returned to its nascent state. (capacity: %d, fragments: %d)\n", (party->num_fragments * party->fragment_length), party->num_fragments );
      }
  }

int getSize(LonelyPartyArray *party)
  {
    if (party == NULL)
      {
        return -1;
      }
    return party->size;
  }

int getCapacity(LonelyPartyArray *party)
  {
    if (party == NULL)
      {
        return -1;
      }
      return party->fragment_length * party->num_fragments;
  }

int getAllocatedCellCount(LonelyPartyArray *party)
  {
    if (party == NULL)
      {
        return -1;
      }
      return party->num_active_fragments * party->fragment_length;
  }

long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
  {
    if (party == NULL)
      {
        return 0;
      }
    return (party->num_fragments * party->fragment_length) * sizeof(int);
  }

long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
  {
    long long unsigned int used_bytes = 0;
    if (party == NULL)
      {
        return 0;
      }

    used_bytes = sizeof(LPA) + sizeof(LPA*); // add the size of lpa pointer and lpa struct

    for (int firstIndex = 0; i < party->num_fragments; i++) // loop through each fragment, adding the bytes used from the values in fragment sizes and fragment pointer arrays
      {
        used_bytes += sizeof(party->fragment_sizes[firstIndex]);
        used_bytes += sizeof(party->fragments[firstIndex]);

        if (party->fragments[firstIndex] != NULL)
          {
            for (int secondIndex = 0; j < party->fragment_length; j++)
            used_bytes += sizeof(party->fragments[firstIndex][secondIndex]);
          }
      }

      return bytes;

  }


double difficultyRating(void)
  {
    return 3;
  }

double hoursSpent(void)
  {
    return 15;
  }
