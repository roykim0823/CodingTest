// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <vector>

using std::vector;

template <typename CoordType, typename HeightType>
class Skyline {
public:
	CoordType left, right;
	HeightType height;
};

// a.left <= b.left
template <typename CoordType, typename HeightType>
void merge_intersect_skylines(vector<Skyline<CoordType, HeightType>> &merged,
								Skyline<CoordType, HeightType> &a, int &a_idx,
								Skyline<CoordType, HeightType> &b, int &b_idx) {
  if (a.right <= b.right) {   	// b.right is the right line: a is between b's (left and right)
    if (a.height > b.height) {    // a.height is the skyline
      if (b.right != a.right) {
        merged.emplace_back(a), ++a_idx;
        b.left = a.right;         // a.right will be b->left
      } else {
        ++b_idx;                    // b.right is obstructed by a, so go to next b
      }
    } else if (a.height == b.height) {
      b.left = a.left, ++a_idx;
    } else {  // a.height < b.height.
      if (a.left != b.left) {
        merged.emplace_back(Skyline<CoordType, HeightType>{a.left, b.left, a.height});
      }
      ++a_idx;
    }
  } else {  // a.right > b.right -> b is between a's (left and right)
    if (a.height >= b.height) {
      ++b_idx;
    } else {
      if (a.left != b.left) {
        merged.emplace_back(Skyline<CoordType, HeightType>{a.left, b.left, a.height});
      }
      a.left = b.right;
      merged.emplace_back(b), ++b_idx;
    }
  }
}


template <typename CoordType, typename HeightType>
vector<Skyline<CoordType, HeightType>> merge_skylines(
	vector<Skyline<CoordType, HeightType>> &L,
	vector<Skyline<CoordType, HeightType>> &R) {
  int i = 0, j = 0;
  vector<Skyline<CoordType, HeightType>> merged;

  // check whether intersect exists
  while (i < (int)L.size() && j < (int)R.size()) {
    if (L[i].right < R[j].left) {			// no intersect
      merged.emplace_back(L[i++]);
    } else if (R[j].right < L[i].left) {   	// no intersect
      merged.emplace_back(R[j++]);
    } else if (L[i].left <= R[j].left) {
      merge_intersect_skylines(merged, L[i], i, R[j], j);
    } else {  // L[i].left > R[j].left.
      merge_intersect_skylines(merged, R[j], j, L[i], i);
    }
  }

  copy(L.cbegin() + i, L.cend(), back_inserter(merged));
  copy(R.cbegin() + j, R.cend(), back_inserter(merged));
  return merged;
}

template <typename CoordType, typename HeightType>
vector<Skyline<CoordType, HeightType>> drawing_skylines_helper(
	const vector<Skyline<CoordType, HeightType>>& skylines,
    const int &start, const int &end) {
  if (end - start <= 1) {  // 0 or 1 skyline, just copy it.
    return {skylines.cbegin() + start, skylines.cbegin() + end};
  }
  int mid = start + ((end - start) >> 1);
  auto L = drawing_skylines_helper(skylines, start, mid);
  auto R = drawing_skylines_helper(skylines, mid, end);
  return merge_skylines(L, R);
}

template <typename CoordType, typename HeightType>
vector<Skyline<CoordType, HeightType>> drawing_skylines(vector<Skyline<CoordType, HeightType>> skylines) {
  return drawing_skylines_helper(skylines, 0, skylines.size());
}
