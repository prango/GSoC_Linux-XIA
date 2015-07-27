/*   Hopscotch  hash table provides a simple hash table implementation that resolves hash collisions with
       linear probing from the initial hash entry up to log(n) entries ahead, where n is the  size  of  the
       hash table.

       Usage of this structure needs evaluating the hash values of the elements before insertion. The func‐
       tions always treats the hash values modulo T->n.

       These hash tables are useful when the values to be stored fit in uint.  With larger entries you  are
       better  off using normal hash tables with linked list buckets, which is then fast enough and simpler
       to implement.  With integer entries these tables have the following advantages:

       1)     Memory locality and compactness and thus cache efficiency.

       2)     Serialization. The table is contained in the arrays A and V.

       3)     Concurrency by dividing the array into segments each with locks, though  this  has  not  been
              implemented here.

       HHash  is  a  stucture for the hash table, where H is the array of hops and V is an array containing
       the values stored in the table. Zero values are considered empty  slots  and  thus  zero  cannot  be
       inserted into the table. Each entry in H is a bitmap such that if for a hash value h the bit at i is
       set the element V[h+i] also has the same hash value. Also m is the number of elements stored in  the
       table, n is the size of the arrays H and V.  k is the integer logarithm in base 2 of n.  Hhashnew(n)
       allocates memory for a hash table with capacity n and hhashfree frees it.

       The elements of the hash table are referenced by their hash value and offset. For a hash value h the
       elements with this hash value have offsets 0 <= i < T->k.  Hhashsucc(T,h,i) returns the successor of
       the element at V[h+i].  If the element V[h+i] has the hash value h hhashsucc returns i, and  if  not
       it  returns  the  next  largest i < T->k such that V[h+i] has the hash value h.  If no such i exists
       hhashsucc returns a negative value. Thus V[h+i] for any i does not necessarily have the  hash  value
       h.  Hhashsucc can be used for both checking if an element is contained in the table as well as iter‐
       ating over the elements with the same hash value.

       Hhashput(T,h,x) tries to insert the element x with hash value h into the  table  T.   On  successful
       addition  it  returns  a  non-zero value and zero otherwise.  If an attempt to add an element is not
       successful the table should be resized, x is not added to the table, but  some  other  modifications
       may have taken place. No operation has been provided here for resizing but one can simple allocate a
       new table with a larger size and copy all values from the current table into the new one. Note, try‐
       ing  to insert zero will not succeed. This is considered semantically correct behaviour as after the
       insertion zero will not be found from the table by hhsucc.

       Hhashdel(T,h,i) removes the element at V[h+i] provided that V[h+i] has the hash value h and 0 <= i <
       T->k, if not hhashdel does nothing.

*/


#include <stdlib.h>
#include "hhash.h"
#include "mach.c"

#define mod(x, n) ((x) < (n) ? (x) : (x) - (n))

HHash *hhashnew(uint n) {
  HHash *T = malloc(sizeof(HHash));
  T->m = 0;
  T->n = n;
  T->k = fls(n - 1);
  T->V = malloc(n * sizeof(uint));
  T->H = malloc(n * sizeof(uint));
  for (int i = 0; i < n; ++i) {
    T->H[i] = 0;
    T->V[i] = 0;
  }
  return T;
}

void hhashfree(HHash *T) {
  free(T->V);
  free(T->H);
  free(T);
}

static int next(HHash *T, uint h, int i) {
  uint H = T->H[h] & (~0 << i);
  if (H == 0) return -1;
  return ffs(H);
}

static int succ(HHash *T, uint h, uint i) {
  if (get(T->H[h], i)) return i;
  return next(T, h, i);
}

int hhashsucc(HHash *T, uint h, uint i) {
  h %= T->n;
  return succ(T, h, i);
}

static void move(HHash *T, uint h, uint i, uint j) {
  unset(T->H[h], i);
  set(T->H[h], j);
  i = mod(h + i, T->n);
  j = mod(h + j, T->n);
  uint v = T->V[i];
  T->V[i] = 0;
  T->V[j] = v;
}

void hhashdel(HHash *T, uint h, uint i) {
  h %= T->n;
  uint j = mod(h + i, T->n);
  if (T->V[j] == 0 || !get(T->H[h], i)) return;
  T->V[j] = 0;
  unset(T->H[h], i);
  --T->m;
}

static uint probe(HHash *T, uint h) {
  int i = 0;
  for (; h + i < T->n; ++i)
    if (T->V[h + i] == 0) return i;
  uint j = 0;
  for (; T->V[j] != 0; ++j)
    ;
  return i + j;
}

static uint seek(HHash *T, uint h) {
  for (uint i = T->k - 1; i > 0; --i) {
    uint hi = mod(T->n + h - i, T->n);
    if (T->H[hi] != 0 && ffs(T->H[hi]) < i) return i;
  }
  return 0;
}

int hhashput(HHash *T, uint h, uint v) {
  if (T->m == T->n || v == 0) return 0;
  h %= T->n;
  uint d = probe(T, h);
  while (d >= T->k) {
    uint hd = mod(h + d, T->n);
    uint z = seek(T, hd);
    if (z == 0) return 0;
    uint j = z;
    z = mod(T->n + hd - z, T->n);
    uint i = succ(T, z, 0);
    move(T, z, i, j);
    d = mod(T->n + z + i - h, T->n);
  }
  uint hd = mod(h + d, T->n);
  T->V[hd] = v;
  set(T->H[h], d);
  ++T->m;
  return 1;
}
