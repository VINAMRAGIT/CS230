#ifndef __ZOO
#define __ZOO

typedef struct Area Area;
typedef struct AnimalNode AnimalNode;
typedef struct Animal Animal;

// Represents a zoo area.
struct Area
{
  char *name;
  AnimalNode *list;
};

// Represents a node in the linked list.
struct AnimalNode
{
  Animal *animal;
  AnimalNode *next;
};

// Represents a kind of animal and its area.
struct Animal
{
  char *animal_kind;
  char *area;
};

int area_add(Area zoo[], int number_of_areas, Animal animals[],
             int number_of_animals, const char *animal_kind);
Animal *area_remove(Area zoo[], int number_of_areas, const char *area,
                    const char *animal_kind);
Animal *area_change(Area zoo[], int number_of_areas, Animal animals[],
                    int number_of_animals, const char *animal_kind,
                    const char *old_area, const char *new_area);

#endif
