#ifndef _INCLUDE_LIST_
#define _INCLUDE_LIST_

#include <libk/kprintf.h>

struct list_head
{
  struct list_head *prev, *next;
};

static inline void list_head_init(struct list_head *head)
{
  head->next = head;
  head->prev = head;
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
  new->next = head->next;
  new->prev = head;
  head->next->prev = new;
  head->next = new;
}

static inline void list_del(struct list_head *node)
{
  node->next->prev = node->prev;
  node->prev->next = node->next;
}

static inline int list_is_head(struct list_head *list, struct list_head *head)
{
  return list == head;
}

static inline int list_empty(struct list_head *head)
{
  return head->next == head;
}

#define list_for_each(pos, head) \
  for(pos = (head)->next; pos != head; pos = pos->next)

#endif
