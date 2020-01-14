There are 3 Batch files attaced, in which Batch1 and Batch3 are successful operations, Batch2 is a failed operation.

Seed for srand():
Batch1 - 1
Batch2 - 2
Batch3 - 5

srand() is placed in int main().

Smart algorithm used in the program:
Erase-remove idiom
vector.erase(remove( vector.begin(), vector.end(), x),vector.end()), x is exact desired value to erase

Below is the pseudocode for remove:
template <class ForwardIterator, class T>
ForwardIterator remove (ForwardIterator first, ForwardIterator last, const T& val)
{
  ForwardIterator result = first;
  while (first!=last) {
    if (!(*first == val)) {
      *result = *first;
      ++result;
    }
    ++first;
  }
  return result;
}