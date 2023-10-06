#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zoo.h>

int main() {
  char* names[5] = {"Cute", "Sea", "Panda", "Penguin", "Starfish"};
  Area zoo[2];
  Animal animals[4];
  zoo[0].name = (char*)malloc(strlen(names[0]) + 1);
  strcpy(zoo[0].name, names[0]);
  zoo[0].list = NULL;
  zoo[1].name = (char*)malloc(strlen(names[1]) + 1);
  strcpy(zoo[1].name, names[1]);
  zoo[1].list = NULL;

  animals[0].area = (char*)malloc(strlen(names[0]) + 1);
  animals[0].animal_kind = (char*)malloc(strlen(names[2]) + 1);
  strcpy(animals[0].area, names[0]);
  strcpy(animals[0].animal_kind, names[2]);
  animals[1].area = (char*)malloc(strlen(names[0]) + 1);
  animals[1].animal_kind = (char*)malloc(strlen(names[3]) + 1);
  strcpy(animals[1].area, names[0]);
  strcpy(animals[1].animal_kind, names[3]);
  animals[2].area = (char*)malloc(strlen(names[1]) + 1);
  animals[2].animal_kind = (char*)malloc(strlen(names[3]) + 1);
  strcpy(animals[2].area, names[1]);
  strcpy(animals[2].animal_kind, names[3]);
  animals[3].area = (char*)malloc(strlen(names[1]) + 1);
  animals[3].animal_kind = (char*)malloc(strlen(names[4]) + 1);
  strcpy(animals[3].area, names[1]);
  strcpy(animals[3].animal_kind, names[4]);

  int result;
  printf("\n=======  Test area_add  ========\n\n");
  result = area_add(zoo, 2, animals, 4, "Panda");
  printf("The result should be 1 and your result is %i\n", result);
  printf("The first animal in Area 'Cute' is Panda. In your zoo, it is %s.\n",
         (zoo[0].list)->animal->animal_kind);

  result = area_add(zoo, 2, animals, 4, "Apple");
  printf("The result should be 0 and your result is %i\n", result);

  result = area_add(zoo, 2, animals, 4, "Penguin");
  printf("The result should be 1 and your result is %i\n", result);
  printf(
      "The Second animal in Area 'Cute' is Penguin. In your zoo, it is %s.\n",
      (zoo[0].list)->next->animal->animal_kind);
  printf("The first animal in Area 'Sea' is Penguin. In your zoo, it is %s.\n",
         (zoo[1].list)->animal->animal_kind);

  printf("\n=======  Test area_remove  ========\n\n");

  Animal* result_animal = area_remove(zoo, 2, "Cute", "Penguin");
  int comparison_result = (result_animal == &animals[1]);
  printf(
      "The comparison result should be 1 because the returned animal should be "
      "the second animal in the animal list. Your result is %i.\n",
      comparison_result);

  result_animal = area_remove(zoo, 2, "Cute", "Penguin");
  printf(
      "The returned result of removing Cute Penguin again should be NULL(nil). "
      "Your "
      "result is %p.\n",
      result_animal);

  result_animal = area_remove(zoo, 2, "Sea", "Penguin");
  comparison_result = (result_animal == &animals[2]);
  printf(
      "The comparison result should be 1 because the returned animal should be "
      "the third animal in the animal list. Your result is %i.\n",
      comparison_result);
  printf(
      "The zoo[1].list should be NULL(nil) because the area Sea has "
      "no animal. Your result is %p.\n",
      zoo[1].list);

  printf("\n=======  Test area_change  ========\n\n");

  result_animal = area_change(zoo, 2, animals, 4, "Starfish", "Sea", "Cute");
  comparison_result = (result_animal == &animals[3]);
  printf(
      "The comparison result should be 1 because the returned animal should be "
      "the forth animal in the animal list. Your result is %i.\n",
      comparison_result);
  printf(
      "The area of Starfish should be changed to 'Cute', and your Starfish's "
      "area is '%s'.\n",
      animals[3].area);

  result_animal = area_change(zoo, 2, animals, 4, "Panda", "Sea", "Cute");
  printf(
      "The returned result of changing Panda from 'Sea' to 'Cute' should be "
      "NULL(nil). Your result is %p.\n",
      result_animal);

  result_animal = area_change(zoo, 2, animals, 4, "Starfish", "Cute", "Wild");
  printf(
      "The returned result of changing Starfish from 'Sea' to 'Wild' should be "
      "NULL(nil) because there is no area Wild in the zoo. Your result "
      "is %p.\n",
      result_animal);

  result_animal = area_change(zoo, 2, animals, 4, "Penguin", "Sea", "Cute");
  printf(
      "The returned result of changing Penguin from 'Sea' to 'Cute' should be "
      "NULL(nil). Your result is %p.\n",
      result_animal);
  printf(
      "The area of Penguin should not be changed from 'Sea' since the list "
      "aleady has Penguin in area 'Cute'. Your result area is '%s'.\n",
      animals[2].area);

  result_animal = area_change(zoo, 2, animals, 4, "Panda", "Cute", "Sea");
  comparison_result = (result_animal == &animals[0]);
  printf(
      "The comparison result should be 1 because the returned animal should be "
      "the first animal in the animal list. Your result is %i.\n",
      comparison_result);
  printf(
      "The area of Panda should be changed to 'Sea', and your Panda's "
      "area is '%s'.\n",
      animals[0].area);
  printf("Panda should be the first animal of zoo[1]. Your result is %s.\n",
         (zoo[1].list)->animal->animal_kind);
  printf(
      "The zoo[0].list should be NULL(nil) because the area Cute has "
      "no animal. Your result is %p.\n",
      zoo[0].list);

  return 0;
}