#include <iostream>

void sort_e_and_o(std::vector<int> &arr) {

  if (arr.size() == 0)
    return;

  int left = 0;
  int right = arr.size() - 1;

  while (left < right) {

    if (arr[left] % 2 == 0) {
      left++;
      else if (arr[right] % 2 != 0) {
        right++;
      }
      else {
        swap(arr[right], arr[left]);
        left++;
        right--;
      }
    }
