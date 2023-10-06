#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zoo.h>

/**
 * Accepts a zoo and its length, an array of animals and
 * its length, and an input animal kind
 *
 * Search through the animal list
 * Add any that match the given animal kind into
 * its corresponding area,
 * unless they are already there in the area
 */
int area_add(Area zoo[], int number_of_areas, Animal animals[],
             int number_of_animals, const char *animal_kind)
{
  // TODO
  int added = 0;                              // Tracker to see if an animal was added
  for (int i = 0; i < number_of_animals; i++) // Iterating through animals
  {
    if (strcmp(animal_kind, animals[i].animal_kind) == 0) // given animal_kind matches the current animal's animal kind
    {
      for (int j = 0; j < number_of_areas; j++) // Iterating through zoos
      {
        if (strcmp(animals[i].area, zoo[j].name) == 0) // animal's area matches zoo's area
        {
          // Allocating memory for node to be inserted, and initializing data
          struct AnimalNode *goal = (struct AnimalNode *)malloc(sizeof(AnimalNode));
          goal->animal = &animals[i];
          goal->next = NULL;
          // Checks to see if the list is empty, and assigning list to goal if so
          if (zoo[j].list == NULL)
          {
            zoo[j].list = goal;
            added = 1;
            continue;
          }
          // Two tracker pointers, created to iterate through the list and insert goal between or after
          struct AnimalNode *cur = zoo[j].list;
          struct AnimalNode *prev = NULL;
          /**
           * We run the loop until we reach the end of the list
           * cur will function as the element we insert before, and prev
           * will function as the element we insert after
           *
           * If a matching AnimalNode is found we break the loop.
           * Next, if goal occurs before cur. If it does, check
           * to see if the list has been iterated through yet
           * (by checking prev == NULL),
           * prepending goal to cur(and possibly appending to prev),
           * changing added to 1 and breaking.
           *
           * Otherwise, we iterate through the list
           * Finally, we check if we have iterated all the way
           * through, in which case we append goal to the end of
           * the list and breaking.
           */
          while (cur != NULL)
          {
            if (strcmp(goal->animal->animal_kind, cur->animal->animal_kind) == 0)
              break;
            else if (strcmp(goal->animal->animal_kind, cur->animal->animal_kind) < 0)
            {
              if (prev == NULL)
                zoo[j].list = goal;
              else
                prev->next = goal;
              goal->next = cur;
              added = 1;
              break;
            }
            else
            {
              prev = cur;
              cur = cur->next;
              if (cur == NULL)
              {
                prev->next = goal;
                added = 1;
                break;
              }
            }
          }
        }
      }
    }
  }

  return added; // returning the boolean
}

// Remove the target animal from the target area
Animal *area_remove(Area zoo[], int number_of_areas, const char *area,
                    const char *animal_kind)
{
  // TODO
  struct Animal *goal = NULL;
  for (int i = 0; i < number_of_areas; i++)
  {
    if (strcmp(area, zoo[i].name) == 0)
    {
      /**
       * There are 3 scenarios:
       * 1) The animal to be removed is in the head spot
       * 2) The animal to be removed is in the tail spot, or in between two elements
       * 3) The animal is not in the list
       */
      struct AnimalNode *cur = zoo[i].list;
      struct AnimalNode *prev = NULL;
      while (cur != NULL)
      {
        // If the animal is found>>>
        if (strcmp(cur->animal->animal_kind, animal_kind) == 0)
        {
          goal = cur->animal;
          if (prev == NULL) // Scenario 1: animal is the head, meaning prev is NULL
            zoo[i].list = cur->next;
          else // Scenario 3: Neither prev nor cur is NULL, meaning that animal is between prev and cur->next
            prev->next = cur->next;
          break;
        }
        else
        {
          // Animal is not found, meaning we iterate through the list
          prev = cur;
          cur = cur->next;
        }
      }
    }
  }
  return goal;
}

/**
 * Accepts zoo, number of areas in zoo, animals array, number of animals,
 * target animal_kind, old and new areas respectively.
 *
 * Search through the animal array
 * Change the area of matching animal kinds
 * UNLESS
 * They are already in the new area
 * the new area is not in the zoo
 *
 * MAINTAIN ORDERING(ALPHABETICAL ASCENDING)
 */
Animal *area_change(Area zoo[], int number_of_areas, Animal animals[],
                    int number_of_animals, const char *animal_kind,
                    const char *old_area, const char *new_area)
{
  // TODO
  int oldExists, newExists = 0;
  struct Animal *goal;
  /**
   * Checks if both new and old area are in the Area array provided,
   * and returns NULL if either one is not present
   */
  for (int i = 0; i < number_of_areas; i++)
  {
    if (oldExists && newExists)
      break;
    else if (strcmp(zoo[i].name, old_area) == 0)
      oldExists = 1;
    else if (strcmp(zoo[i].name, new_area) == 0)
      newExists = 1;
  }
  if (!newExists || !oldExists)
    return NULL;
  oldExists = -1; // oldExists reassigned to -1 to check if old animal is in the array
  /**
   * This loop checks for an animla object matching with animal kind
   * If there is match, it checks the area.
   *
   * If there is a match with the new_area, we return NULL, as
   * there is no need to change
   * If there is a match with old_area, we assign its index to
   * oldExists for further checking
   */
  for (int i = 0; i < number_of_animals; i++)
  {

    if (strcmp(animals[i].animal_kind, animal_kind) == 0)
      if (strcmp(animals[i].area, new_area) == 0)
        return NULL;
      else if (strcmp(animals[i].area, old_area) == 0)
        oldExists = i;
  }
  /**
   * If we are able to successfully remove an Animal from the lists,
   * we change its area value, add it back, and return it.
   *
   * If we are not able to remove an Animal, we check to see if
   * it exists in the animals array
   *
   * If it does we can change its area and add it into the lists
   * If not, we return NULL as the animal does not exist.
   */
  goal = area_remove(zoo, number_of_areas, old_area, animal_kind);
  if (goal == NULL)
  {
    if (oldExists == -1)
      return NULL;
    else
      goal = &animals[oldExists];
  }
  goal->area = new_area;
  area_add(zoo, number_of_areas, animals, number_of_animals, animal_kind);
  return goal;
}
