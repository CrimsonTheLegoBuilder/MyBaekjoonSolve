#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <vector>

void merge_sort(std::vector<int>& arr1, std::vector<int>& arr2, int len);

void merge_sortRecursion(std::vector<int>& arr1, std::vector<int>& arr2, int l, int r);

void merge(std::vector<int>& arr1, std::vector<int>& arr2, int l, int m, int r);

int main(void)
{
    int n, i = 0;

    scanf("%d", &n);

    std::vector<int> arrX(n), arrY(n);

    for (i = 0; i < n; i++)
    {
        scanf("%d %d", &arrX[i], &arrY[i]);
    }

    merge_sort(arrX, arrY, n);


    for (i = 0; i < n; i++)
    {
        printf("%d %d \n", arrX[i], arrY[i]);
    }
    return 0;
}

void merge_sort(std::vector<int>& arr1, std::vector<int>& arr2, int len)
{
    merge_sortRecursion(arr1, arr2, 0, len - 1);
}

void merge_sortRecursion(std::vector<int>& arr1, std::vector<int>& arr2, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        merge_sortRecursion(arr1, arr2, l, m);
        merge_sortRecursion(arr1, arr2, m + 1, r);

        merge(arr1, arr2, l, m, r);
    }
}

void merge(std::vector<int>& arr1, std::vector<int>& arr2, int l, int m, int r)
{
    int left_length = m - l + 1;
    int right_length = r - m;

    std::vector<int> left_temp1(left_length);
    std::vector<int> left_temp2(left_length);
    std::vector<int> right_temp1(right_length);
    std::vector<int> right_temp2(right_length);

    int i, j, k;

    for (i = 0; i < left_length; i++)
    {
        left_temp1[i] = arr1[l + i];
        left_temp2[i] = arr2[l + i];
    }

    for (i = 0; i < right_length; i++)
    {
        right_temp1[i] = arr1[m + 1 + i];
        right_temp2[i] = arr2[m + 1 + i];
    }

    for (i = 0, j = 0, k = l; k <= r; k++)
    {
        if ((i < left_length) && ((j >= right_length) || left_temp1[i] <= right_temp1[j]))
        {
            if (left_temp1[i] == right_temp1[j])
            {
                if (left_temp2[i] <= right_temp2[j])
                {
                    arr1[k] = left_temp1[i];
                    arr2[k] = left_temp2[i];
                    i++;
                }
                else if (left_temp2[i] > right_temp2[j])
                {
                    arr1[k] = right_temp1[j];
                    arr2[k] = right_temp2[j];
                    j++;
                }
            }
            else
            {
                arr1[k] = left_temp1[i];
                arr2[k] = left_temp2[i];
                i++;
            }
        }
        else
        {
            arr1[k] = right_temp1[j];
            arr2[k] = right_temp2[j];
            j++;
        }

    }
}