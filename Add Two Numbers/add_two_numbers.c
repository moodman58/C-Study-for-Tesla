/*
2. 
You are given two non-empty linked lists representing two non-negative integers. The digits are stored in reverse order, and each of their nodes contains a single digit. Add the two numbers and return the sum as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Example 1:
Input: l1 = [2,4,3], l2 = [5,6,4]
Output: [7,0,8]
Explanation: 342 + 465 = 807.

Note:
Solve it both without and with recursion
*/

#include <stdio.h>
#include <stdlib.h>


//Definition for singly-linked list.
struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
  // if l1 and l2 .next is not NULL; then keep going
  // sum, sum variable;
  // Input: l1 = [2,4,3], l2 = [5,6,4]
  int carry = 0;
  int val1 = 0;
  int val2 = 0;
  struct ListNode *head;
  while(l1 != NULL && l2 != NULL){
    if(l1 == NULL){
      val1 = 0;
    }else{
      val1 = l1->val;
    }
    if(l2 == NULL){
      val2 = 0;
    }else{
      val2 = l2->val;
    }
    struct ListNode *n = malloc(sizeof(struct ListNode));
    n->val = (val1+val2+carry)%10;
    n->next = head;
    head = n;

    if(val1+val2+carry >= 10){ 
      carry = 1;
    }else{
      carry = 0;
    }
    if(l1 != NULL){
      l1 = l1->next;
    }
    if(l2 != NULL){
      l2 = l2->next;
    }
  }
  return head;
  
    
}

// To execute C, please define "int main()"

int main() {
  struct ListNode list1;
  struct ListNode list2;
  struct ListNode list3;

  list1.val = 2;
  list1.next = &list2;
  list2.val = 4;
  list2.next = &list3;
  list3.val = 3;
  list3.next = NULL;
  
  struct ListNode list4 = {.val = 5};
  struct ListNode list5 = {.val = 6};
  struct ListNode list6 = {.val = 4};

  list4.next = &list5;
  list5.next = &list6;
  list6.next = NULL;
  
  struct ListNode* head = addTwoNumbers(&list1, &list4);
  while(head){
    printf("%d\n", head->val);
    head = head->next;
  }

  

  return 0;
}
