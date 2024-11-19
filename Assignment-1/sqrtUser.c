#include <stdio.h>

double calculatePrecision(int n) 
{ // This function uses n to calculate the precision to the decimal place
    double precision = 1.0;
    for (int i = 0; i < (n+1); i++) 
    {
        precision /= 10.0; // Divide by 10 for each decimal place plus an extra decimal place
    }
    return precision;
}


double sqrtUser (double number , int n )
{
    if (number==1 || number == 0)
    { //sqrt of these is itself
        printf("The square root of this number is %f\n", number); 
        return 0;   
    }

    if (number<0)
    { //can't calculate imaginary number sqrt
        printf("Can not calculate the square root of an negative number\n");
        return 0;
    }


    double left = 0;
    double right = number;
    double precision = calculatePrecision(n);

    if (number < 1)
    { //If it's between 0 and 1, just switch right to 1.0, left to 0
        right = 1.0;
    }

    while (right-left>=precision)
    {
        
        double middle = right-((right-left)/2); //middle between two values
        if(middle*middle<number)
        { //if middle value squared is smaller than num, move the left value up
            left=middle;
        }
        else
        {
            right=middle; // move the right value down if it's too big
        }
         
    }
   
   double answer = right;
   return answer;

}

int main() //main function that we can use to calculate sqrt to a certain decimal places
{
    double number = 65;
    int n = 10;
    double result = sqrtUser(number,n);
    if (result!=0){
        printf("The square root of %f, accurate to %d decimal places is %.*lf\n",number,n,n,result);
    }   
}


