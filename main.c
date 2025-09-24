/*
Description: This program reads in data feom four prodution lines and stores them in an appropriate structure array. Each line is then sorted based on weight lighest to heaves, Once sored the 4 lines are merged into one dispatch list sorted the same way. The user is then able to search for a specific product based on the products weight.


Author: Haseeb lateef



Date:
*/



#include <stdio.h>

#define SIZE 30
#define LINE_SIZE 10
#define DISPATCH_SIZE 40
#define TEMP_DISPATCH_SIZE 20



typedef struct 
{
    char name[SIZE];
    float weight;
    int lineCode;
    int batchCode;
    int batchTime;
    int productId;
    char targetEngine[SIZE];
    int binNo;
    float price;
    
}Product;


void swap(Product *, Product *);//funtion for swaping position of elements

int divide(Product [], int, int);//slits the array into different partitions using a pivid

void q_sort(Product [], int, int);

int get_data(const char *linex, Product parts[]);//gets the data from each line and places it the appropriate product array

int merge_sorted_arrays(Product [],Product [], Product [], int, int);//merges the lines to gether to form 1 array

int bin_search(Product[], float, int, int );//searches for tagetet weight

void count_line_product(Product [], int);//counts the amount of products each line has

void output_sorted_lines(Product [], Product [], Product [], Product []);

void output_dispatchlist_search(Product [], int);//outputs fully sorted dispatch list and allows the searching of a product by weight




int main()
{
    //count for the size of each line
    int line_count1 = 0;
    int line_count2 = 0;
    int line_count3 = 0;
    int line_count4 = 0;
    int temp_count1 = 0;
    int temp_count2 = 0;
    //count end
    int dis_count = 0;//count for the dipatch list

    //struct variables
    Product line1_data[LINE_SIZE];
    Product line2_data[LINE_SIZE];
    Product line3_data[LINE_SIZE];
    Product line4_data[LINE_SIZE];
    Product dispatch_list[DISPATCH_SIZE];//stores the fully merged lists from line 1-4
    Product temp_dispatch1[TEMP_DISPATCH_SIZE];//stores mergerd line 1 & 2
    Product temp_dispatch2[TEMP_DISPATCH_SIZE];//stores mergerd line 3 & 4
    //struct variables

    line_count1 = get_data("line1.txt",line1_data);

    line_count2 = get_data("line2.txt",line2_data);

    line_count3 = get_data("line3.txt",line3_data);

    line_count4 = get_data("line4.txt",line4_data);

    //sorting lines 1-4
    q_sort(line1_data, 0, line_count1 - 1);

    q_sort(line2_data, 0, line_count2 - 1);

    q_sort(line3_data, 0,  line_count3 - 1);
    
    q_sort(line4_data, 0, line_count4 - 1);
    //end sorting

    //mergers line 1 and 2 then 3 and 4. the two mergerg arrays are then merged together to form 1 array
    temp_count1 = merge_sorted_arrays(line1_data, line2_data, temp_dispatch1, line_count1, line_count2);
    temp_count2 = merge_sorted_arrays(line3_data, line4_data, temp_dispatch2, line_count3, line_count4);
    dis_count   = merge_sorted_arrays(temp_dispatch1, temp_dispatch2, dispatch_list, temp_count1, temp_count2);
    //end merging

    output_sorted_lines(line1_data, line2_data, line3_data, line4_data);//outputs sorted versions of line 1-4

    
    // output_dispatchlist_search(dispatch_list, dis_count);//outputs sorted dispatch list and allows the search of a product by a specified weight

    return 0;
}


void swap(Product *x, Product *y)
{
    Product temp = *x;
    *x = *y;
    *y = temp;

}


int divide(Product parts[], int low, int high)
{
    float pivit = parts[high].weight;//sets the last element as the pivit

    int i = low -1;//tracks location of where small sumber must be moved to

    
    for (int j = low; j < high; j++)
     {
        if (parts[j].weight < pivit)//check is current weight is less than pivit
        {
            i++;
            swap(&parts[i], &parts[j]);//move the smal number left
        }
    }

    swap(&parts[i + 1], &parts[high]);//puts piviot in correct space

    return i + 1;//returns index of the pivit


}

void q_sort(Product parts[], int low, int high)
{
    if (low < high)//checks if elements need to be sorted
     {
        int pivot_index = divide(parts, low, high); 

        q_sort(parts, low, pivot_index - 1);//sorts left half

        q_sort(parts, pivot_index + 1, high);//sorts right half       
    }

}


int get_data(const char *linex, Product parts[]) 
{
    FILE *fp;

    int count = 0;//track how many products are read in(needed for q sort)

    if ((fp = fopen(linex, "r")) == NULL)
    {
        printf(" ERROR COULNDT OPEN FILE: %s\n", linex);
        return 1;
    }
    else
    {
        //keeps reading from file as long as 9 values are sucefully read
        while (fscanf(fp, "%[^,],%f,%d,%d,%d,%d,%[^,],%d,%f\n",//[^,]=read string till comma and store it name
            parts[count].name,
            &parts[count].weight,
            &parts[count].lineCode,
            &parts[count].batchCode,
            &parts[count].batchTime,
            &parts[count].productId,
            parts[count].targetEngine,
            &parts[count].binNo,
            &parts[count].price) == 9)
        {
            count++;    
        }

    }
    
    fclose(fp);
    return count;   
}


int merge_sorted_arrays(Product line1[],Product line2[],Product temp[], int line_count1, int line_count2)
{
    int i = 0;//index for line 1
    int j = 0;//index for line 2
    int k = 0;//intex for temp(store merged array)

    while (i < line_count1 && j < line_count2) //sorts while there are still items in tha array
    {
        if (line1[i].weight <= line2[j].weight)//checks for the lighestet value in line 1 and line 2
        {
            temp[k] = line1[i];
            
            k++;
            i++;
        }
        else
        {
            temp[k] = line2[j];

            k++;

            j++;
        }     
    }

    //coppys any left overs for line 1
    while (i < line_count1)
    {

        temp[k] = line1[i];
        k++;
        i++;

    }

        //coppys any left overs for line 2
    while (j < line_count2)
    {
        temp[k] = line2[j];
        k++;
        j++;
    }


    return k;  // total number of products in temp
        
}


int bin_search(Product dispatch_list[] , float target, int l, int r)
{


    int mid = (l + r) / 2;// find the middle index between index l and index r
  
  
    if (l > r)
    {
        return -1;//return if no target is found

    }
  
    if (dispatch_list[mid].weight == target)//if the middle index is target return the middle index
    {
        return mid;
    }
    else if (target < dispatch_list[mid].weight)//if target is smaller than mid search the left 
    {
        return bin_search(dispatch_list, target, l, mid - 1);
    } 
    else
    {

        return bin_search(dispatch_list, target, mid + 1, r);//if target is larger than mid search the right

    }
    
  
}

void count_line_product(Product dispatch_list[], int size)
{

    int count_line1 = 0;
    int count_line2 = 0;
    int count_line3 = 0;
    int count_line4 = 0;

    for(int i = 0; i < size; i++)
    {
        if (dispatch_list[i].lineCode == 1)
        {
            count_line1++;
        }
        else if (dispatch_list[i].lineCode == 2)
        {
            count_line2++;
        }
        else if (dispatch_list[i].lineCode == 3)
        {
            count_line3++;
        }
        else if (dispatch_list[i].lineCode == 4)
        {
            count_line4++;
        }
    }

    printf("\nProduct Count per Delivery van:\n");
    printf("Van 1 contains: %d products\n", count_line1);
    printf("Van 2 conatins: %d products\n", count_line2);
    printf("Van 3 contains: %d products\n", count_line3);
    printf("Van 4 contains: %d products\n", count_line4);
}


void output_sorted_lines(Product line1_data[], Product line2_data[], Product line3_data[], Product line4_data[])
{
    printf("Line 1(sorted):\n");
    for (int i = 0; i < LINE_SIZE; i++)
    {
        printf("%s - %.2f\n", line1_data[i].name, line1_data[i].weight);
    }

    printf("\nLine 2(sorted):\n");
    for (int i = 0; i < LINE_SIZE; i++)
    {
        printf("%s - %.2f\n", line2_data[i].name, line2_data[i].weight);
        
    }

    printf("\nLine 3(sorted):\n");
    for (int i = 0; i < LINE_SIZE; i++)
    {
        printf("%s - %.2f\n", line3_data[i].name, line3_data[i].weight);
    }

    printf("\nLine 4(sorted):\n");
    for (int i = 0; i < LINE_SIZE; i++)
    {
        printf("%s - %.2f\n", line4_data[i].name, line4_data[i].weight);

    }

}


void output_dispatchlist_search(Product dispatch_list[], int dis_count)
{
    float target = 0;
    int target_index = 0;

    printf("\nDispatch List (Sorted by Weight-lighestet to heavest):\n");
    printf("-------------------------------------------------------------------------------\n");
    printf("%-20s %-8s %-6s %-6s %-10s %-8s %-10s %-6s %-8s\n",
            "Name", "Weight", "Line", "Batch", "Time", "ProdID", "Engine", "Bin", "Price");
    printf("-------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < dis_count; i++) {
        printf("%-20s %-8.2f %-6d %-6d %-10d %-8d %-10s %-6d EUR%.2f\n",
                dispatch_list[i].name,
                dispatch_list[i].weight,
                dispatch_list[i].lineCode,
                dispatch_list[i].batchCode,
                dispatch_list[i].batchTime,
                dispatch_list[i].productId,
                dispatch_list[i].targetEngine,
                dispatch_list[i].binNo,
                dispatch_list[i].price);
    }

    count_line_product(dispatch_list, dis_count);//counts how many products in each line


    printf("\nEnter weight to search for: ");
    scanf("%f", &target);

    target_index = bin_search(dispatch_list, target, 0, dis_count - 1);



    if (target_index == -1) 
    {
        printf("Sorry no product found with weight %.2f\n", target);
    } 
    else
    {
     
        printf("\n First matching product:\n");
        printf("%s | Weight: %.2f | ID: %d | Line: %d | Price: EUR%.2f\n",dispatch_list[target_index].name, dispatch_list[target_index].weight, dispatch_list[target_index].productId, dispatch_list[target_index].lineCode, dispatch_list[target_index].price);

    }

}
