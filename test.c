#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct Student
{
	char* first_name; /* This will be malloc'd!*/
	char* last_name; /* This will also be malloc'd */
	int grade;
	unsigned int gtid;
} Student;

Student* create_student(const char* first_name, const char* last_name, int grade, unsigned int gtid)
{
    Student *s = (Student*)malloc(sizeof(Student));
    s->first_name = (char*)malloc(sizeof(char)*(strlen(first_name)+1));
    strcpy(s->first_name, first_name);
    s->last_name = (char*)malloc(sizeof(char)*(strlen(last_name)+1));
    strcpy(s->last_name, last_name);
    s->grade = grade;
    s->gtid = gtid;
	return s;
}

void print_student(void* data)
{
	/* Your output should exactly look like:
	Student: FIRSTNAME LASTNAME
	Grade: GRADE
	GtID: GTID
	*/
	Student *s = (Student*)data;
	printf("\nStudent: %s %s\nGrade: %d\nGtID: %d\n", s->first_name, s->last_name, s->grade, s->gtid);
}

void free_student(void* data)
{
    Student *s = (Student*)data;
	/* Free any fields that were malloc'd. */
    free(s->first_name);
    free(s->last_name);
	/* Then free the struct in this order */
	free(s);
}

int student_eq(const void* a, const void* b)
{
	Student *sa = (Student*)a;
	Student *sb = (Student*)b;
	if(sa->grade != sb->grade)
        return 0;
    if(sa->gtid != sb->gtid)
        return 0;
	if(strcmp(sa->first_name, sb->first_name))
        return 0;
    if(strcmp(sa->last_name, sb->last_name))
        return 0;
	return 1;
}

int last_name_student(const void* a)
{
    Student *s = (Student*)a;
    return !strcmp(s->last_name, "Student");
}

/* This main function does testing */
int main(void)
{
	list* llist = create_list();

  	/* What does an empty list contain? */
  	printf("TEST CASE 1\nAn Empty list should print nothing here:");
  	traverse(llist, print_student);
	printf("\n");

 	/* Add a student and then print */
 	push_front(llist, create_student("Nick", "Nack", 56, 239402128));
 	printf("TEST CASE 2\nA List with one student should print that student:");
 	traverse(llist, print_student);
 	printf("\n");

 	/* Remove that student and then print */
 	remove_front(llist, free_student);
 	printf("TEST CASE 3\nAnother Empty list should print nothing here:");
 	traverse(llist, print_student);
 	printf("\n");

 	/* Add two elements and then print */
 	push_front(llist, create_student("Sherlock", "Holmes", 92, 123456789));
 	push_front(llist, create_student("John", "Watson", 65, 902313373));
 	printf("TEST CASE 4\nA List with two students should print those two students:");
 	traverse(llist, print_student);
 	printf("\n");

 	/* Test find occurence when the node is not found */
	// find_occurence(llist, create_student("Nurse", "Joy", 3, 131313137), student_eq));
	Student* p = create_student("Nurse", "Joy", 3, 131313137);
	printf("TEST CASE 5\nFinding occurence when a node is not in the list should fail: ");
	printf("%d\n", find_occurrence(llist, p, student_eq));
	free_student(p);

  	/* Kill the list */
  	empty_list(llist, free_student);
 	printf("TEST CASE 6\nAfter freeing all nodes the list should be empty:");
 	traverse(llist, print_student);
 	printf("\n");

    //Testing push_front
    push_front(llist, create_student("Able", "Mable", 14, 902111222));
    push_front(llist, create_student("None", "Special", 55, 987654321));
    push_front(llist, create_student("George", "Curious", 42, 123456789));
    printf("TEST CASE 6\nA list with three students with George in front Able in back:");
    traverse(llist, print_student);
 	printf("\n");

    //Testing push_back
    push_back(llist, create_student("JK", "Rowling", 76, 902321456));
    printf("TEST CASE 7\nA list with four student with JK at the end: ");
    traverse(llist, print_student);
 	printf("\n");

    //Testing remove_front
    remove_front(llist, free_student);
    printf("TEST CASE 8\nGeorge should no longer be in the list:");
    traverse(llist, print_student);
 	printf("\n");

    //Testing remove_index and get index
    printf("TEST CASE 9\nGetting student at index 2:");
    print_student(get_index(llist, 2));
    printf("Removing student at index 2; list should not have above student:");
    if(remove_index(llist, 2, free_student))
        printf("remove_index returned an error.\n");
    traverse(llist, print_student);
 	printf("\n");

    //Testing remove_back
    printf("TEST CASE 10\nThe last student in the list should be removed:");
    if(remove_back(llist, free_student))
        printf("remove_back returned an error.\n");
    traverse(llist, print_student);
 	printf("\n");

    //Testing empty_list and is_empty
    printf("TEST CASE 11\nAn empty list: ");
    empty_list(llist, free_student);
    traverse(llist, print_student);
 	printf("\n");
    if(is_empty(llist))
        printf("is_empty correctly affirmed an empty list.\n");
    else
        printf("is_empty incorrectly returned false.\n");

    //Testing remove_data
    push_front(llist, create_student("First", "Student", 15, 902888999));
    push_front(llist, create_student("Remove", "Me", 10, 101010101));
    push_front(llist, create_student("Second", "Student", 15, 902999888));
    push_front(llist, create_student("Remove", "Me", 10, 101010101));
    push_front(llist, create_student("Third", "Student", 15, 902888777));
    push_front(llist, create_student("Real", "Person", 14, 902468294));
    printf("TEST CASE 12\nA list with six students: ");
    traverse(llist, print_student);
    printf("The student Remove Me should now be removed from above list:");
    Student *rm = create_student("Remove", "Me", 10, 101010101);
    if(remove_data(llist, rm, student_eq, free_student))
        printf("remove_data returned an error.\n");
    free_student(rm);
    traverse(llist, print_student);
 	printf("\n");

    //Testing remove_if
    printf("TEST CASE 13\nRemoving all students with last name Student.\nThis count should be three: %d\nA list with one student:", remove_if(llist, last_name_student, free_student));
    traverse(llist, print_student);
 	printf("\n");

    //Testing front, back, and size
    printf("TEST CASE 14\nThe front of the list:");
    print_student(front(llist));
    printf("should be the same as the back of the list: ");
    print_student(back(llist));
    printf("and one should be the size of the list: %d\nThe list:", size(llist));
    traverse(llist, print_student);
    remove_front(llist, free_student);
    printf("An empty list: ");
    traverse(llist, print_student);
    printf("\n");

 	/* Testing over clean up*/
 	free(llist);

  	return 0;
}
