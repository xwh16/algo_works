#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

unsigned int k, N;
vector<unsigned int> nums;

unsigned int select(vector<unsigned int> &a, unsigned int k);
unsigned int quickselect(vector<unsigned int> &a, unsigned int k);
unsigned int partition5(vector<unsigned int> &a, unsigned int l, unsigned int r);
unsigned int partition(vector<unsigned int> &a, unsigned int l, unsigned int r, unsigned int p);
unsigned int medianpivot(vector<unsigned int> &a, unsigned int l, unsigned int r);
unsigned int bfprtselect(vector<unsigned int> &a, unsigned int l, unsigned int r, unsigned int k);

unsigned int select(vector<unsigned int> &a, unsigned int k) {
	unsigned int i;
	unsigned int pivot = a.size() / 2;
	vector<unsigned int> sub1, sub2;	// sub1=A-, sub2=A+

	for (i = 0; i < a.size(); i++) {
		if (i == pivot) {
			continue;
		}
		else if (a[i] > a[pivot]) {
			sub2.push_back(a[i]);
		}
		else {
			sub1.push_back(a[i]);
		}
	}

	//cout << "Pivot=" << pivot << endl;
	//cout << "A- ";
	//for (i = 0; i < sub1.size(); i++) {
	//	cout << sub1[i] << " ";
	//}
	//cout << endl;
	//cout << "A+ ";
	//for (i = 0; i < sub2.size(); i++) {
	//	cout << sub2[i] << " ";
	//}
	//cout << endl;

	if (sub2.size() == k - 1) {
		return a[pivot];
	}
	else if (sub2.size() > k - 1) {
		return select(sub2, k);
	}
	else {
		return select(sub1, k - sub2.size() - 1);
	}
}

unsigned int quickselect(vector<unsigned int> &a, unsigned int k) {
	unsigned int i;
	unsigned int pivot = rand() % a.size();
	vector<unsigned int> sub1, sub2;	// sub1=A-, sub2=A+

	for (i = 0; i < a.size(); i++) {
		if (i == pivot) {
			continue;
		}
		else if (a[i] > a[pivot]) {
			sub2.push_back(a[i]);
		}
		else {
			sub1.push_back(a[i]);
		}
	}

	//cout << "Pivot=" << pivot << endl;
	//cout << "A- ";
	//for (i = 0; i < sub1.size(); i++) {
	//	cout << sub1[i] << " ";
	//}
	//cout << endl;
	//cout << "A+ ";
	//for (i = 0; i < sub2.size(); i++) {
	//	cout << sub2[i] << " ";
	//}
	//cout << endl;

	if (sub2.size() == k - 1) {
		return a[pivot];
	}
	else if (sub2.size() > k - 1) {
		return quickselect(sub2, k);
	}
	else {
		return quickselect(sub1, k - sub2.size() - 1);
	}
}

unsigned int partition5(vector<unsigned int> &a, unsigned int l, unsigned int r) {
	unsigned int temp, order[5] = {0, 1, 2, 3, 4};
	unsigned int i, j, k, n = r - l + 1;

	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - i - 1; j++) {
			if (a[l + order[j]] > a[l + order[j + 1]]) {
				temp = order[j];
				order[j] = order[j + 1];
				order[j + 1] = temp;
			}
		}
	}

	//printf("Group[%d-%d]: ", l, r);
	//for (i = 0; i < n; i++) {
	//	printf("%d ", a[l + i]);
	//}
	//printf("\nGroup Median: [%d] %d\n\n", l + order[n / 2], a[l + order[n / 2]]);

	return l + order[n / 2];
}

unsigned int partition(vector<unsigned int> &a, unsigned int l, unsigned int r, unsigned int p) {
	unsigned int i, j, k;
	unsigned int temp, pivot = a[p];
	
	// swap A[p] and A[r]
	// move pivot to the right end first
	temp = a[p];
	a[p] = a[r];
	a[r] = temp;

	// Lomuto's QuickSort-Partition
	i = l;
	for (j = l; j < r; j++) {
		if (a[j] < pivot) {
			// swap A[i] and A[j]
			temp = a[i];
			a[i] = a[j];
			a[j] = temp;
			i++;
		}
	}
	// swap A[r] and A[i]
	// get pivot back to its position
	temp = a[r];
	a[r] = a[i];
	a[i] = temp;

	//printf("Partition[%d-%d]p=%d: ", l, r, p);
	//for (j = 0; j < N; j++) {
	//	printf("%d ", a[j]);
	//}
	//printf("\n\n");

	return i;
}

unsigned int medianpivot(vector<unsigned int> &a, unsigned int l, unsigned int r) {
	unsigned int i, right, median;
	if (r - l < 5)	// 子问题凑不齐1组
		return partition5(a, l, r);
	for (i = l; i < r; i += 5) {
		unsigned int temp;
		right = i + 4;
		if (right > r)	// 最后一组不足5个元素
			right = r;
		median = partition5(a, i, right);
		temp = a[median];
		a[median] = a[l + (i - l) / 5];
		a[l + (i - l) / 5] = temp;
	}
	return bfprtselect(a, l, l + (r - l) / 5, (r - l) / 10 + 1);	// 寻找分组中位数的中位数
}

unsigned int bfprtselect(vector<unsigned int> &a, unsigned int l, unsigned int r, unsigned int k) {
	while (true) {
		if (l == r)
			return l;
		//unsigned int pivot = medianpivot(a, l, r);
		unsigned int pivot = (l + r) / 2;
		unsigned int pos = partition(a, l, r, pivot);
		if (pos == k - 1)
			return k - 1;
		else if (k - 1 < pos)
			r = pos - 1;
		else
			l = pos + 1;
	}
}

//int main() {
//	int i;
//
//	cin >> N >> k;
//	nums.resize(N);
//	for (i = 0; i<N; i++) {
//		scanf("%d", &nums[i]);
//	}
//
//	cout << quickselect(nums, k);
//
//	return 0;
//}